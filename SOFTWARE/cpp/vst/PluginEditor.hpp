#pragma once
#include "PluginProcessor.hpp"

namespace wbs::m466 {

class M466PluginEditor {
public:
    explicit M466PluginEditor(M466PluginProcessor& processor);
    ~M466PluginEditor() = default;

private:
    M466PluginProcessor& processor_;
};

} // namespace wbs::m466
