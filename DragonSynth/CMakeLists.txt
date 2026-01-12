cmake_minimum_required(VERSION 3.22)

project(DragonSynth VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch JUCE
include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.9
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(JUCE)

# Plugin target
juce_add_plugin(DragonSynth
    COMPANY_NAME "DragonAudio"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Drgn
    PLUGIN_CODE Drsn
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "Dragon Synth"
)

# Source files
target_sources(DragonSynth
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
        Source/SynthVoice.cpp
        Source/SynthVoice.h
        Source/SynthSound.h
        Source/Oscillator.cpp
        Source/Oscillator.h
        Source/ADSREnvelope.cpp
        Source/ADSREnvelope.h
        Source/Filter.cpp
        Source/Filter.h
        Source/LookAndFeel.cpp
        Source/LookAndFeel.h
        Source/KeyboardComponent.cpp
        Source/KeyboardComponent.h
        Source/WaveformDisplay.cpp
        Source/WaveformDisplay.h
)

juce_generate_juce_header(DragonSynth)

target_compile_definitions(DragonSynth
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(DragonSynth
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
