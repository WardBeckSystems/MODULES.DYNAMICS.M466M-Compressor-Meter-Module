// M466M: its block of 2030.env/vst/core/ModuleGuiLayouts.hpp (176 x 770 = the faceplate).

        case 15: { // Ward-Beck M466 Compressor / Limiter with M466M Meter & Bus Tie
            m.title = "M466 Compressor / Limiter & M466M";
            m.subtitle = "Dual-Detector Broadcast Compressor / Limiter with M466M Meter & Stereo Tie";
            m.modelCode = "M466";
            m.m466ChannelCount = 2; // Default to stereo pair (2x M466 + M466M in center with LINK button)
            m.m466StereoLinked = true;
            updateM466Layout(m);
            break;
        }

inline void updateM466Layout(PluginGuiModel& m) {
    m.knobs.clear();
    m.buttons.clear();

    const bool hasTwo = (m.m466ChannelCount >= 2);
    m.width = hasTwo ? 528.0f : 352.0f;
    m.height = 770.0f;

    // --- Channel 1 (X: 0 to 176) ---
    // Knobs (exact 1:1 match to R002 - M466.jpg)
    m.knobs.push_back({ 0, "LIMIT THLD", "dB", 48.0f, 155.0f, 18.0f, 0.0, 20.0, 10.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaAmber });
    m.knobs.push_back({ 1, "COMP THLD", "dB", 48.0f, 432.0f, 18.0f, -20.0, 0.0, -10.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaYellow });
    m.knobs.push_back({ 2, "SENS", "dB", 126.0f, 104.0f, 18.0f, -10.0, 8.0, 0.0, false, KnobStyle::WbsElmaRegular, Palette::ElmaBlue });
    m.knobs.push_back({ 3, "LIM RLS", "s", 126.0f, 208.0f, 18.0f, 0.1, 5.0, 0.5, false, KnobStyle::WbsElmaPointer, Palette::ElmaAmber });
    m.knobs.push_back({ 4, "DS - MG", "Hz", 126.0f, 318.0f, 23.0f, 1000.0, 6000.0, 3500.0, false, KnobStyle::WbsElmaRegular, Palette::ElmaGreen });
    m.knobs.push_back({ 5, "SLOPE", ":1", 126.0f, 432.0f, 18.0f, 1.5, 5.0, 2.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaYellow });
    m.knobs.push_back({ 6, "COMP RLS", "s", 126.0f, 532.0f, 18.0f, 0.2, 5.0, 1.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaYellow });

    // Buttons
    m.buttons.push_back({ 7, "LIM", { 38.0f, 226.0f, 20.0f, 20.0f }, true, ButtonStyle::LatchingPush, Palette::WbsGreen });
    m.buttons.push_back({ 8, "DS", { 38.0f, 286.0f, 20.0f, 20.0f }, false, ButtonStyle::LatchingPush, Palette::ElmaGreen });
    m.buttons.push_back({ 9, "GR/VU", { 38.0f, 342.0f, 20.0f, 20.0f }, false, ButtonStyle::LatchingPush, Palette::ElmaBlue });
    m.buttons.push_back({ 10, "COMP", { 38.0f, 502.0f, 20.0f, 20.0f }, true, ButtonStyle::LatchingPush, Palette::ElmaYellow });
    m.buttons.push_back({ 11, "INS", { 38.0f, 564.0f, 20.0f, 20.0f }, true, ButtonStyle::LatchingPush, Palette::ElmaRed });

    // --- M466M Meter Module (X: 176 to 352) ---
    const float m466mX = 176.0f;
    m.m466CompTieRects[0] = { m466mX + 16.0f, 478.0f, 44.0f, 24.0f }; // Bus A
    m.m466CompTieRects[1] = { m466mX + 66.0f, 478.0f, 44.0f, 24.0f }; // Bus B
    m.m466CompTieRects[2] = { m466mX + 116.0f, 478.0f, 44.0f, 24.0f }; // Off
    m.m466MeterSourceRect = { m466mX + 25.0f, 520.0f, 126.0f, 24.0f };
    m.m466ZeroCalRect = { m466mX + 76.0f, 574.0f, 24.0f, 24.0f };
    m.m466PairToggleRect = { m466mX + 25.0f, 650.0f, 126.0f, 22.0f };

    // Link Button: Added ONLY when there are two M466s!
    if (hasTwo) {
        m.m466LinkButtonRect = { m466mX + 28.0f, 430.0f, 120.0f, 32.0f };
    } else {
        m.m466LinkButtonRect = { 0.0f, 0.0f, 0.0f, 0.0f };
    }

    // --- Channel 2 (X: 352 to 528, present only when 2x M466) ---
    if (hasTwo) {
        const float ch2X = 352.0f;
        // Knobs
        m.knobs.push_back({ 12, "LIMIT THLD", "dB", ch2X + 48.0f, 155.0f, 18.0f, 0.0, 20.0, 10.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaAmber });
        m.knobs.push_back({ 13, "COMP THLD", "dB", ch2X + 48.0f, 432.0f, 18.0f, -20.0, 0.0, -10.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaYellow });
        m.knobs.push_back({ 14, "SENS", "dB", ch2X + 126.0f, 104.0f, 18.0f, -10.0, 8.0, 0.0, false, KnobStyle::WbsElmaRegular, Palette::ElmaBlue });
        m.knobs.push_back({ 15, "LIM RLS", "s", ch2X + 126.0f, 208.0f, 18.0f, 0.1, 5.0, 0.5, false, KnobStyle::WbsElmaPointer, Palette::ElmaAmber });
        m.knobs.push_back({ 16, "DS - MG", "Hz", ch2X + 126.0f, 318.0f, 23.0f, 1000.0, 6000.0, 3500.0, false, KnobStyle::WbsElmaRegular, Palette::ElmaGreen });
        m.knobs.push_back({ 17, "SLOPE", ":1", ch2X + 126.0f, 432.0f, 18.0f, 1.5, 5.0, 2.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaYellow });
        m.knobs.push_back({ 18, "COMP RLS", "s", ch2X + 126.0f, 532.0f, 18.0f, 0.2, 5.0, 1.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaYellow });

        // Buttons
        m.buttons.push_back({ 19, "LIM", { ch2X + 38.0f, 226.0f, 20.0f, 20.0f }, true, ButtonStyle::LatchingPush, Palette::WbsGreen });
        m.buttons.push_back({ 20, "DS", { ch2X + 38.0f, 286.0f, 20.0f, 20.0f }, false, ButtonStyle::LatchingPush, Palette::ElmaGreen });
        m.buttons.push_back({ 21, "GR/VU", { ch2X + 38.0f, 342.0f, 20.0f, 20.0f }, false, ButtonStyle::LatchingPush, Palette::ElmaBlue });
        m.buttons.push_back({ 22, "COMP", { ch2X + 38.0f, 502.0f, 20.0f, 20.0f }, true, ButtonStyle::LatchingPush, Palette::ElmaYellow });
        m.buttons.push_back({ 23, "INS", { ch2X + 38.0f, 564.0f, 20.0f, 20.0f }, true, ButtonStyle::LatchingPush, Palette::ElmaRed });
    }

    // Common Bottom Badges
    m.themeToggleRect = { (m.width - 126.0f) * 0.5f, 726.0f, 126.0f, 22.0f };
    m.webRemoteRect = { (m.width - 180.0f) * 0.5f, 698.0f, 180.0f, 20.0f };
    m.bottomScrewRect = { m.width * 0.5f - 14.0f, 756.0f - 14.0f, 28.0f, 28.0f };
}
