cmake_minimum_required(VERSION 3.24)

# ============================================================================
# Fleen El-Guitar VST3 Plugin
# Professional Audio Plugin with Premium Front-End and Robust Back-End
# ============================================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS version")

# Project definition
project(FleenElGuitar 
    VERSION 1.0.0
    DESCRIPTION "Professional Guitar Processing VST3 Plugin"
    HOMEPAGE_URL "https://github.com/tehuti01/Fleen-el-guitar-vst"
    LANGUAGES CXX
)

# ============================================================================
# CPM.cmake - Dependency Management
# ============================================================================

set(CPM_DOWNLOAD_VERSION 0.40.2)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD 
        "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake"
        ${CPM_DOWNLOAD_LOCATION}
    )
endif()

include(${CPM_DOWNLOAD_LOCATION})

# ============================================================================
# JUCE Framework (v8)
# ============================================================================

CPMAddPackage(
    NAME JUCE
    GITHUB_REPOSITORY juce-framework/JUCE
    VERSION 8.0.4
    OPTIONS
        "JUCE_BUILD_EXAMPLES=OFF"
        "JUCE_BUILD_EXTRAS=OFF"
        "JUCE_ENABLE_MODULE_SOURCE_GROUPS=ON"
        "JUCE_ENABLE_ALL_SOURCE_GROUPS=ON"
        "JUCE_COPY_PLUGIN_AFTER_BUILD=ON"
        "JUCE_ENABLE_LTO=ON"
)

# ============================================================================
# Additional Modules
# ============================================================================

# Melatonin Blur - High-performance shadows for neumorphism
CPMAddPackage(
    NAME melatonin_blur
    GITHUB_REPOSITORY sudara/melatonin_blur
    GIT_TAG main
)

# Melatonin Inspector - Component hierarchy auditing
CPMAddPackage(
    NAME melatonin_inspector
    GITHUB_REPOSITORY sudara/melatonin_inspector
    GIT_TAG main
)

# JIVE - Declarative UI layouts
CPMAddPackage(
    NAME JIVE
    GITHUB_REPOSITORY ImJimmi/JIVE
    GIT_TAG main
)

# Foleys GUI Magic - Runtime GUI designer
CPMAddPackage(
    NAME foleys_gui_magic
    GITHUB_REPOSITORY ffAudio/foleys_gui_magic
    GIT_TAG main
)

# ============================================================================
# Plugin Configuration
# ============================================================================

set(PLUGIN_CODE FlnG)
set(PLUGIN_NAME "Fleen El-Guitar")
set(PLUGIN_COMPANY "Fleen Audio")
set(PLUGIN_DESCRIPTION "Professional guitar processing with premium skeuomorphic interface")
set(PLUGIN_MANUFACTURER_CODE FlnA)

# ============================================================================
# Audio Processor Target
# ============================================================================

juce_add_plugin(FleenElGuitar
    COMPANY_NAME "${PLUGIN_COMPANY}"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE "${PLUGIN_MANUFACTURER_CODE}"
    PLUGIN_CODE "${PLUGIN_CODE}"
    FORMATS VST3 Standalone
    PRODUCT_NAME "${PLUGIN_NAME}"
    VST3_CATEGORIES "Instrument" "Guitar" "Effect"
    VST3_SUBCATEGORY "Instrument|Synth"
)

# ============================================================================
# Source Files
# ============================================================================

target_sources(FleenElGuitar
    PRIVATE
        # Core Plugin
        src/PluginProcessor.h
        src/PluginProcessor.cpp
        
        # GUI Components
        src/PluginEditor.h
        src/PluginEditor.cpp
        
        # Custom LookAndFeel
        src/lookandfeel/CustomLookAndFeel.h
        src/lookandfeel/CustomLookAndFeel.cpp
        
        # DSP Components
        src/dsp/GainStage.h
        src/dsp/ToneStack.h
        src/dsp/Distortion.h
        src/dsp/Reverb.h
        src/dsp/Compressor.h
        
        # GUI Components
        src/components/SkeuomorphicKnob.h
        src/components/SkeuomorphicKnob.cpp
        src/components/LedIndicator.h
        src/components/LedIndicator.cpp
        src/components/MeterDisplay.h
        src/components/MeterDisplay.cpp
        src/components/OpenGLVisualizer.h
        src/components/OpenGLVisualizer.cpp
        
        # State Management
        src/state/StateManager.h
        src/state/StateManager.cpp
        src/state/PresetManager.h
        src/state/PresetManager.cpp
        
        # Utilities
        src/utils/GoldenRatio.h
        src/utils/Assets.h
)

# ============================================================================
# Include Directories
# ============================================================================

target_include_directories(FleenElGuitar
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lookandfeel
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dsp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/components
        ${CMAKE_CURRENT_SOURCE_DIR}/src/state
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
)

# ============================================================================
# Compile Definitions
# ============================================================================

target_compile_definitions(FleenElGuitar
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_PRODUCT_USAGE=0
        FLEEN_VERSION="${PROJECT_VERSION}"
)

# ============================================================================
# Link Libraries
# ============================================================================

target_link_libraries(FleenElGuitar
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_audio_formats
        juce::juce_audio_devices
        juce::juce_dsp
        juce::juce_gui_extra
        juce::juce_opengl
        melatonin::melatonin_blur
        melatonin::melatonin_inspector
        JIVE::JIVE
        foleys::foleys_gui_magic
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# ============================================================================
# Binary Resources
# ============================================================================

juce_add_binary_data(BinaryData
    SOURCES
        assets/fonts/Inter-Regular.ttf
        assets/fonts/Inter-Bold.ttf
        assets/images/knob_metal_01.png
        assets/images/knob_metal_02.png
        assets/images/knob_indicator.png
        assets/images/led_red_off.png
        assets/images/led_red_on.png
        assets/images/led_blue_off.png
        assets/images/led_blue_on.png
        assets/images/faceplate_brush.png
        assets/images/background_dark.png
        assets/presets/default.json
        assets/presets/clean.json
        assets/presets/crunch.json
        assets/presets/high_gain.json
        assets/presets/lead.json
)

target_compile_definitions(BinaryData
    PUBLIC
        JUCE_USE_CUSTOM_PLUGIN_STANDALONE_NAME=1
)

target_link_libraries(FleenElGuitar
    PRIVATE
        BinaryData
)

# ============================================================================
# Installation
# ============================================================================

include(GNUInstallDirs)

install(TARGETS FleenElGuitar
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/VST3"
    BUNDLE DESTINATION "${CMAKE_INSTALL_LIBDIR}/VST3"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# ============================================================================
# Print Configuration
# ============================================================================

message(STATUS "")
message(STATUS "=== Fleen El-Guitar Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "JUCE Version: ${JUCE_VERSION}")
message(STATUS "Formats: VST3, Standalone")
message(STATUS "=====================================")
message(STATUS "")
