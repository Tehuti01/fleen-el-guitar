cmake_minimum_required(VERSION 3.24)

# ============================================================================
# Fleen El-Guitar VST3 Plugin - Professional Guitar Processing
# ============================================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS version")
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Project definition
project(FleenElGuitar 
    VERSION 1.0.0
    DESCRIPTION "Professional Guitar Processing VST3 Plugin"
    HOMEPAGE_URL "https://github.com/Tehuti01/fleen-el-guitar"
    LANGUAGES CXX C
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
# JUCE Framework
# ============================================================================

CPMAddPackage(
    NAME JUCE
    GITHUB_REPOSITORY juce-framework/JUCE
    GIT_TAG master
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    OPTIONS
        "JUCE_BUILD_EXAMPLES=OFF"
        "JUCE_BUILD_EXTRAS=OFF"
        "JUCE_ENABLE_MODULE_SOURCE_GROUPS=ON"
        "JUCE_ENABLE_ALL_SOURCE_GROUPS=ON"
        "JUCE_COPY_PLUGIN_AFTER_BUILD=ON"
        "JUCE_ENABLE_LTO=ON"
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
    BUNDLE_ID com.fleenaudio.fleenelguitar
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
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
        src/PluginEditor.h
        src/PluginEditor.cpp
        
        # Custom LookAndFeel
        src/lookandfeel/CustomLookAndFeel.h
        src/lookandfeel/CustomLookAndFeel.cpp
        
        # DSP Components
        src/dsp/GainStage.h
        src/dsp/ToneStack.h
        
        # GUI Components
        src/components/SkeuomorphicKnob.h
        src/components/SkeuomorphicKnob.cpp
        src/components/LedIndicator.h
        src/components/LedIndicator.cpp
        src/components/MeterDisplay.h
        src/components/MeterDisplay.cpp
        src/components/OpenGLVisualizer.h
        src/components/OpenGLVisualizer.cpp
        src/components/PresetSelector.h
        src/components/PresetSelector.cpp
        src/components/TestKeyboard.h
        src/components/TestKeyboard.cpp
        
        # State Management
        src/state/StateManager.h
        src/state/StateManager.cpp
        src/state/PresetManager.h
        src/state/PresetManager.cpp
        
        # Utilities
        src/utils/GoldenRatio.h
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
        ${CMAKE_CURRENT_BINARY_DIR}/FleenElGuitar_artefacts/JuceLibraryCode
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
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# ============================================================================
# Binary Resources
# ============================================================================

file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/presets")

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets/presets/default.json")
    file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/assets/presets/default.json" "{}")
endif()

juce_add_binary_data(BinaryData
    SOURCES
        assets/presets/default.json
)

target_link_libraries(FleenElGuitar PRIVATE BinaryData)

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
