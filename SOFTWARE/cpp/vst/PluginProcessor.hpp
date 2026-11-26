#pragma once
#include "../core/SharedPluginFrame.hpp"
#include "../core/BallisticsCore.hpp"
#include <atomic>

namespace wbs::m466 {

class M466PluginProcessor : public engine::SharedPluginFrame {
public:
    M466PluginProcessor();
    ~M466PluginProcessor() override = default;

    void prepareModule(double hostRate, double oversampledRate, size_t maxBlockSize, size_t numChannels) override;
    void releaseModule() override;
    void updateParametersFromHost() noexcept override;
    void processModuleSlice(engine::AudioBlock& slice) noexcept override;
    void updateMeters(const engine::AudioBlock& audioBlock) noexcept override;

    // Parameter Setters
    void setSensDb(float db) noexcept { sensDb_ = db; }
    void setLimiterThreshold(float db) noexcept { limThld_ = db; }
    void setLimiterRelease(float sec) noexcept { limRls_ = sec; }
    void setCompThreshold(float db) noexcept { compThld_ = db; }
    void setCompSlope(float slope) noexcept { compSlope_ = slope; }
    void setCompRelease(float sec) noexcept { compRls_ = sec; }
    void setDeEssFreq(float freq) noexcept { dsFreq_ = freq; }

    void setLimiterIn(bool enabled) noexcept { limIn_ = enabled; }
    void setDeEssIn(bool enabled) noexcept { dsIn_ = enabled; }
    void setGrVuMode(bool isVu) noexcept { isVuMode_ = isVu; }
    void setCompIn(bool enabled) noexcept { compIn_ = enabled; }
    void setInsertIn(bool enabled) noexcept { insIn_ = enabled; }

    // Getters for UI display
    [[nodiscard]] bool getLed6Active() const noexcept { return led6Active_; }
    [[nodiscard]] bool getLed12Active() const noexcept { return led12Active_; }
    [[nodiscard]] float getCompGainReductionDb() const noexcept { return compGrDb_; }
    [[nodiscard]] float getLimGainReductionDb() const noexcept { return limGrDb_; }
    [[nodiscard]] float getTotalGainReductionDb() const noexcept { return totalGrDb_; }

    [[nodiscard]] bool isLimiterIn() const noexcept { return limIn_; }
    [[nodiscard]] bool isDeEssIn() const noexcept { return dsIn_; }
    [[nodiscard]] bool isGrVuMode() const noexcept { return isVuMode_; }
    [[nodiscard]] bool isCompIn() const noexcept { return compIn_; }
    [[nodiscard]] bool isInsertIn() const noexcept { return insIn_; }

private:
    double sampleRate_{ 48000.0 };

    // Continuous Parameters (matching R002 - M466.jpg)
    float sensDb_{ 0.0f };       // SENS: +8 to -10 dB
    float limThld_{ 12.0f };     // LIMIT THLD: 0 to +20 dB
    float limRls_{ 0.5f };       // RLS (Limiter): 0.1 to 5.0 s
    float compThld_{ -10.0f };   // COMP THLD: -20 to 0 dB
    float compSlope_{ 2.0f };    // SLOPE: 1.5, 2, 3, 5:1
    float compRls_{ 0.5f };      // RLS (Compressor): 0.2 to 5.0 s
    float dsFreq_{ 4000.0f };    // DS - MG: 1 kHz to 6 kHz

    // Button states
    bool limIn_{ true };
    bool dsIn_{ false };
    bool isVuMode_{ false };     // false = GR, true = VU
    bool compIn_{ true };
    bool insIn_{ true };

    // Detector envelopes
    float compEnv_{ 0.0f };
    float limEnv_{ 0.0f };

    // Metering telemetry
    bool led6Active_{ false };
    bool led12Active_{ false };
    float compGrDb_{ 0.0f };
    float limGrDb_{ 0.0f };
    float totalGrDb_{ 0.0f };

    // Sidechain De-Esser Bandpass Biquad
    float dsB0_{ 1.0f }, dsB1_{ 0.0f }, dsB2_{ 0.0f };
    float dsA1_{ 0.0f }, dsA2_{ 0.0f };
    float dsX1_{ 0.0f }, dsX2_{ 0.0f };
    float dsY1_{ 0.0f }, dsY2_{ 0.0f };

    void updateSidechainFilter() noexcept;
};

} // namespace wbs::m466
