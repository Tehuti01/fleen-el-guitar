#!/usr/bin/env bash
# ============================================================================
# Fleen El-Guitar VST3 Installation Script
# ============================================================================
# This script installs the Fleen El-Guitar VST3 plugin on macOS
# 
# Usage:
#   curl -fsSL https://raw.githubusercontent.com/tehuti01/Fleen-el-guitar-vst/main/scripts/install.sh | bash
#
# Or using Homebrew:
#   brew install tehuti01/audio/fleen-el-guitar
# ============================================================================

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# ============================================================================
# Helper Functions
# ============================================================================

log_info() {
    echo -e "${BLUE}==> ${NC}$1"
}

log_success() {
    echo -e "${GREEN}==> ${NC}$1"
}

log_warning() {
    echo -e "${YELLOW}==> ${NC}$1"
}

log_error() {
    echo -e "${RED}==> ${NC}$1"
}

# ============================================================================
# System Checks
# ============================================================================

check_system() {
    log_info "Checking system requirements..."
    
    # Check if running on macOS
    if [[ "$(uname)" != "Darwin" ]]; then
        log_error "This installer is for macOS only."
        exit 1
    fi
    
    # Check macOS version
    MACOS_VERSION=$(sw_vers -productVersion)
    log_info "macOS version: $MACOS_VERSION"
}

# ============================================================================
# Installation Methods
# ============================================================================

install_via_homebrew() {
    log_info "Installing via Homebrew..."
    
    # Check if Homebrew is installed
    if ! command -v brew &> /dev/null; then
        log_warning "Homebrew not found. Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    
    # Add our tap
    log_info "Adding Fleen Homebrew tap..."
    brew tap tehuti01/audio
    
    # Install the plugin
    log_info "Installing Fleen El-Guitar VST3..."
    brew install fleen-el-guitar
    
    log_success "Installation complete!"
}

install_direct() {
    log_info "Installing directly from GitHub..."
    
    # Define installation directories
    VST3_DIR="$HOME/Library/Audio/Plug-Ins/VST3"
    APPLICATION_DIR="$HOME/Applications"
    
    # Create directories if they don't exist
    mkdir -p "$VST3_DIR"
    mkdir -p "$APPLICATION_DIR"
    
    # Get latest release from GitHub
    log_info "Fetching latest release from GitHub..."
    LATEST_RELEASE=$(curl -s https://api.github.com/repos/tehuti01/Fleen-el-guitar-vst/releases/latest)
    
    # Download the VST3 bundle
    DOWNLOAD_URL=$(echo "$LATEST_RELEASE" | grep -o '"browser_download_url": "[^"]*\.vst3\.zip"' | cut -d'"' -f4)
    
    if [[ -z "$DOWNLOAD_URL" ]]; then
        log_error "Could not find VST3 download URL"
        exit 1
    fi
    
    log_info "Downloading from: $DOWNLOAD_URL"
    
    # Download and extract
    TEMP_DIR=$(mktemp -d)
    curl -L "$DOWNLOAD_URL" -o "$TEMP_DIR/fleen-el-guitar.zip"
    unzip -q "$TEMP_DIR/fleen-el-guitar.zip" -d "$TEMP_DIR"
    
    # Install VST3
    log_info "Installing VST3 plugin..."
    cp -R "$TEMP_DIR"/*.vst3 "$VST3_DIR/"
    
    # Install standalone application
    if [[ -d "$TEMP_DIR/Fleen El-Guitar.app" ]]; then
        log_info "Installing standalone application..."
        cp -R "$TEMP_DIR/Fleen El-Guitar.app" "$APPLICATION_DIR/"
    fi
    
    # Cleanup
    rm -rf "$TEMP_DIR"
    
    log_success "Installation complete!"
}

# ============================================================================
# Post-Installation
# ============================================================================

post_install() {
    log_info "Running post-installation checks..."
    
    # Check if VST3 was installed correctly
    if [[ -d "$HOME/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3" ]]; then
        log_success "VST3 plugin installed successfully"
    else
        log_warning "VST3 plugin may not be installed correctly"
    fi
    
    # Inform user about DAW rescan
    echo ""
    log_info "=================================================="
    echo "Installation Complete!"
    echo ""
    echo "The Fleen El-Guitar VST3 plugin has been installed to:"
    echo "  $HOME/Library/Audio/Plug-Ins/VST3/"
    echo ""
    echo "Please restart your DAW to use the plugin."
    echo ""
    echo "For standalone mode, run:"
    echo "  /Applications/Fleen El-Guitar.app"
    echo "=================================================="
}

# ============================================================================
# Uninstall Function
# ============================================================================

uninstall() {
    log_info "Uninstalling Fleen El-Guitar VST3..."
    
    # Remove VST3
    if [[ -d "$HOME/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3" ]]; then
        rm -rf "$HOME/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3"
        log_success "Removed VST3 plugin"
    fi
    
    # Remove standalone app
    if [[ -d "$HOME/Applications/Fleen El-Guitar.app" ]]; then
        rm -rf "$HOME/Applications/Fleen El-Guitar.app"
        log_success "Removed standalone application"
    fi
    
    # Remove preset directory
    if [[ -d "$HOME/Library/Application Support/Fleen/El-Guitar" ]]; then
        rm -rf "$HOME/Library/Application Support/Fleen/El-Guitar"
        log_success "Removed user presets"
    fi
    
    log_success "Uninstallation complete!"
}

# ============================================================================
# Main
# ============================================================================

main() {
    echo ""
    echo "============================================"
    echo "  Fleen El-Guitar VST3 Installer"
    echo "============================================"
    echo ""
    
    case "${1:-install}" in
        install)
            check_system
            install_direct
            post_install
            ;;
        homebrew)
            check_system
            install_via_homebrew
            post_install
            ;;
        uninstall)
            uninstall
            ;;
        *)
            echo "Usage: $0 {install|homebrew|uninstall}"
            echo ""
            echo "Commands:"
            echo "  install    - Install directly from GitHub (default)"
            echo "  homebrew   - Install via Homebrew"
            echo "  uninstall  - Remove the plugin"
            exit 1
            ;;
    esac
}

main "$@"
