// Copyright (c) 2026 Ward-Beck Systems (Ward-Beck.com - Ward-Beck.ca). All rights reserved.
// M466M: its part of 2030.env/vst/clap_wrapper/ClapModuleRegistry.cpp (suite index 15).

// --- descriptor ---
static const clap_plugin_descriptor_t s_desc_m466 = {
    CLAP_VERSION,
    "ca.wbsps.m466_compressor_limiter",
    "Ward-Beck M466 Compressor / Limiter",
    "Ward-Beck Systems Preservation Society",
    "https://wbsps.ca",
    "",
    "https://wbsps.ca/contact",
    "1.0.0",
    "Dual-detector broadcast compressor and brickwall peak limiter with DS sibilance sidechain control",
    kCompressorFeatures
};

// --- parameters ---
std::vector<ParameterInfo> buildM466Params() {
    std::vector<ParameterInfo> p;
    p.push_back({
        0, "Limiter Threshold", "Limiter", 0.0, 20.0, 10.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m466::M466PluginProcessor*>(f)->setLimiterThreshold(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.1f dB", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        1, "Comp Threshold", "Compressor", -20.0, 0.0, -10.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m466::M466PluginProcessor*>(f)->setCompThreshold(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.1f dB", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        2, "Sensitivity", "Input", -10.0, 8.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m466::M466PluginProcessor*>(f)->setSensDb(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%+.1f dB", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        3, "Limiter Release", "Limiter", 0.1, 5.0, 0.5,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m466::M466PluginProcessor*>(f)->setLimiterRelease(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.2f s", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        4, "De-Ess Frequency", "Sidechain", 1000.0, 6000.0, 3500.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m466::M466PluginProcessor*>(f)->setDeEssFreq(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.0f Hz", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        5, "Comp Slope", "Compressor", 1.5, 5.0, 2.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m466::M466PluginProcessor*>(f)->setCompSlope(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.1f:1", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        6, "Comp Release", "Compressor", 0.2, 5.0, 1.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m466::M466PluginProcessor*>(f)->setCompRelease(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.2f s", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        7, "Limiter In", "Limiter", 0.0, 1.0, 1.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m466::M466PluginProcessor*>(f)->setLimiterIn(val >= 0.5); },
        nullptr,
        [](double v) { return (v >= 0.5) ? "LIM IN" : "LIM OUT"; },
        nullptr
    });
    p.push_back({
        8, "De-Esser In", "Sidechain", 0.0, 1.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m466::M466PluginProcessor*>(f)->setDeEssIn(val >= 0.5); },
        nullptr,
        [](double v) { return (v >= 0.5) ? "DS ENGAGED" : "DS BYPASS"; },
        nullptr
    });
    p.push_back({
        9, "Meter GR/VU", "Meter", 0.0, 1.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m466::M466PluginProcessor*>(f)->setGrVuMode(val >= 0.5); },
        nullptr,
        [](double v) { return (v >= 0.5) ? "VU MODE" : "GR MODE"; },
        nullptr
    });
    p.push_back({
        10, "Comp In", "Compressor", 0.0, 1.0, 1.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m466::M466PluginProcessor*>(f)->setCompIn(val >= 0.5); },
        nullptr,
        [](double v) { return (v >= 0.5) ? "COMP IN" : "COMP OUT"; },
        nullptr
    });
    p.push_back({
        11, "Insert In", "Master", 0.0, 1.0, 1.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m466::M466PluginProcessor*>(f)->setInsertIn(val >= 0.5); },
        nullptr,
        [](double v) { return (v >= 0.5) ? "INSERT ACTIVE" : "BYPASS"; },
        nullptr
    });
    return p;
}

// --- instance ---
        case 15:
            inst = new ClapPluginInstance(
                &s_desc_m466, host,
                []() { return std::make_unique<m466::M466PluginProcessor>(); },
                buildM466Params(),
                15
            );
            break;

