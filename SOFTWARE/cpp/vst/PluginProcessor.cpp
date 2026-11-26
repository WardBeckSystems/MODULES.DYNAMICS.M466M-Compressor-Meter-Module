#include "PluginProcessor.hpp"
#include <cmath>
#include <algorithm>

namespace wbs::m466 {

M466PluginProcessor::M466PluginProcessor()
    : engine::SharedPluginFrame("Ward-Beck M466 Broadcast Compressor / Limiter", "M466") {
    defaultOversampling_ = engine::OversamplingRate::None;
}

void M466PluginProcessor::prepareModule(double hostRate, double /*oversampledRate*/, size_t /*maxBlockSize*/, size_t /*numChannels*/) {
    sampleRate_ = hostRate;
    compEnv_ = 0.0f;
    limEnv_ = 0.0f;
    dsX1_ = dsX2_ = dsY1_ = dsY2_ = 0.0f;
    updateSidechainFilter();
}

void M466PluginProcessor::releaseModule() {
    compEnv_ = 0.0f;
    limEnv_ = 0.0f;
    dsX1_ = dsX2_ = dsY1_ = dsY2_ = 0.0f;
}

void M466PluginProcessor::updateParametersFromHost() noexcept {
    updateSidechainFilter();
}

void M466PluginProcessor::updateSidechainFilter() noexcept {
    // 2nd-order bandpass filter for de-esser sidechain (1kHz - 6kHz)
    const double omega = 2.0 * M_PI * std::clamp(static_cast<double>(dsFreq_), 800.0, 8000.0) / sampleRate_;
    const double q = 1.8;
    const double alpha = std::sin(omega) / (2.0 * q);
    const double cs = std::cos(omega);

    const double b0 = alpha;
    const double b1 = 0.0;
    const double b2 = -alpha;
    const double a0 = 1.0 + alpha;
    const double a1 = -2.0 * cs;
    const double a2 = 1.0 - alpha;

    dsB0_ = static_cast<float>(b0 / a0);
    dsB1_ = static_cast<float>(b1 / a0);
    dsB2_ = static_cast<float>(b2 / a0);
    dsA1_ = static_cast<float>(a1 / a0);
    dsA2_ = static_cast<float>(a2 / a0);
}

void M466PluginProcessor::processModuleSlice(engine::AudioBlock& slice) noexcept {
    if (slice.numChannels == 0 || slice.numSamples == 0) return;

    if (!insIn_) {
        // Insert Out: Hardware true bypass
        compGrDb_ = limGrDb_ = totalGrDb_ = 0.0f;
        led6Active_ = led12Active_ = false;
        return;
    }

    const float sensGain = std::pow(10.0f, sensDb_ / 20.0f);

    // Ballistics time constants
    const float compAttSec = 0.010f; // 10ms attack
    const float effCompRls = dsIn_ ? (compRls_ * 0.5f) : compRls_; // Halved release when DS engaged per WBS manual!
    const float compAttAlpha = 1.0f - std::exp(-1.0f / (compAttSec * static_cast<float>(sampleRate_)));
    const float compRlsAlpha = 1.0f - std::exp(-1.0f / (effCompRls * static_cast<float>(sampleRate_)));

    const float limAttSec = 0.000050f; // 50 microseconds brickwall attack
    const float limAttAlpha = 1.0f - std::exp(-1.0f / (limAttSec * static_cast<float>(sampleRate_)));
    const float limRlsAlpha = 1.0f - std::exp(-1.0f / (limRls_ * static_cast<float>(sampleRate_)));

    float maxCompGr = 0.0f;
    float maxLimGr = 0.0f;

    for (size_t i = 0; i < slice.numSamples; ++i) {
        // Compute peak detector input across all channels
        float peakSig = 0.0f;
        for (size_t ch = 0; ch < slice.numChannels; ++ch) {
            float s = slice.channelData[ch][i] * sensGain;
            peakSig = std::max(peakSig, std::abs(s));
        }

        // Sidechain filter if De-Esser engaged
        float sidechainSig = peakSig;
        if (dsIn_) {
            const float bp = dsB0_ * peakSig + dsB1_ * dsX1_ + dsB2_ * dsX2_ - dsA1_ * dsY1_ - dsA2_ * dsY2_;
            dsX2_ = dsX1_; dsX1_ = peakSig;
            dsY2_ = dsY1_; dsY1_ = bp;
            sidechainSig = std::abs(bp) * 2.5f; // Boost sibilance component to drive detector
        }

        const float inDb = (sidechainSig > 1e-5f) ? (20.0f * std::log10(sidechainSig) + 20.0f) : -40.0f;

        // 1. Compressor Path
        float targetCompGr = 0.0f;
        if (compIn_ && inDb > compThld_) {
            const float over = inDb - compThld_;
            targetCompGr = over * (1.0f - 1.0f / compSlope_);
        }
        if (targetCompGr > compEnv_) {
            compEnv_ += compAttAlpha * (targetCompGr - compEnv_);
        } else {
            compEnv_ += compRlsAlpha * (targetCompGr - compEnv_);
        }

        // 2. Limiter Path (50:1 slope)
        float targetLimGr = 0.0f;
        if (limIn_ && inDb > limThld_) {
            const float over = inDb - limThld_;
            targetLimGr = over * (1.0f - 1.0f / 50.0f);
        }
        if (targetLimGr > limEnv_) {
            limEnv_ += limAttAlpha * (targetLimGr - limEnv_);
        } else {
            limEnv_ += limRlsAlpha * (targetLimGr - limEnv_);
        }

        maxCompGr = std::max(maxCompGr, compEnv_);
        maxLimGr = std::max(maxLimGr, limEnv_);

        // Combined electronic attenuator gain
        const float totalGr = compEnv_ + limEnv_;
        const float modGain = std::pow(10.0f, -totalGr / 20.0f) * sensGain;

        // Apply to audio stream
        for (size_t ch = 0; ch < slice.numChannels; ++ch) {
            slice.channelData[ch][i] *= modGain;
        }
    }

    compGrDb_ = maxCompGr;
    limGrDb_ = maxLimGr;
    totalGrDb_ = maxCompGr + maxLimGr;

    // Dual Limiter Warning LEDs (matching R002 - M466.jpg)
    led6Active_ = (limGrDb_ >= 6.0f);
    led12Active_ = (limGrDb_ >= 12.0f);
}

void M466PluginProcessor::updateMeters(const engine::AudioBlock& /*audioBlock*/) noexcept {
    // Meters updated during block slice
}

} // namespace wbs::m466
