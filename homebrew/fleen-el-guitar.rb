# ============================================================================
# Homebrew Formula for Fleen El-Guitar VST3
# ============================================================================
# Repository: tehuti01/homebrew-audio
# 
# Installation:
#   brew tap tehuti01/audio
#   brew install fleen-el-guitar
#
# Uninstall:
#   brew uninstall fleen-el-guitar
# ============================================================================

class FleenElGuitar < Formula
  desc "Professional guitar processing VST3 plugin with premium skeuomorphic interface"
  homepage "https://github.com/tehuti01/Fleen-el-guitar-vst"
  url "https://github.com/tehuti01/Fleen-el-guitar-vst/archive/refs/tags/v1.0.0.tar.gz"
  sha256 :no_check # Will be calculated during release
  license "GPL-3.0-or-later"
  head "https://github.com/tehuti01/Fleen-el-guitar-vst.git", branch: "main"

  depends_on "cmake" => :build
  depends_on "ninja" => :build
  
  # Audio plugin dependencies
  depends_on "juce" => :build
  
  def install
    # Create build directory
    mkdir "build" do
      # Configure with CMake
      system "cmake", "..",
        "-G", "Ninja",
        "-DCMAKE_BUILD_TYPE=Release",
        "-DCMAKE_INSTALL_PREFIX=#{prefix}",
        "-DCMAKE_OSX_DEPLOYMENT_TARGET=10.15"
      
      # Build
      system "ninja"
    end
    
    # Install VST3 bundle to user's Audio plug-ins directory
    vst3_dir = ENV["HOME"] + "/Library/Audio/Plug-Ins/VST3"
    mkdir_p vst3_dir
    
    # Copy VST3 bundle
    cp_r "build/FleenElGuitar_artefacts/Release/VST3/FleenElGuitar.vst3", vst3_dir
    
    # Install standalone application
    app_dir = ENV["HOME"] + "/Applications"
    mkdir_p app_dir
    cp_r "build/FleenElGuitar_artefacts/Release/Standalone/Fleen El-Guitar.app", app_dir
  end

  def caveats
    <<~EOS
      Fleen El-Guitar VST3 has been installed to:
        ~/Library/Audio/Plug-Ins/VST3/
      
      The standalone application has been installed to:
        ~/Applications/
      
      Please restart your DAW to use the VST3 plugin.
    EOS
  end

  test do
    # Verify VST3 bundle exists
    assert_predicate File.directory?("#{ENV["HOME"]}/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3"), 
      "VST3 bundle not found"
    
    # Verify standalone app exists
    assert_predicate File.directory?("#{ENV["HOME"]}/Applications/Fleen El-Guitar.app"),
      "Standalone application not found"
  end
end
