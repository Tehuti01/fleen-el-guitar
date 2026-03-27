#!/usr/bin/env bash
# ============================================================================
# Fleen El-Guitar VST3 - Official Installer
# ============================================================================
# This script downloads and installs the VST3 plugin directly to your system
# Works with automatic fallback options if any step fails
#
# Usage:
#   curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/install.sh -o install.sh
#   chmod +x install.sh
#   ./install.sh
#
# Or one-liner:
#   curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/install.sh | bash
# ============================================================================

set -e

# ============================================================================
# Configuration
# ============================================================================

REPO_OWNER="Tehuti01"
REPO_NAME="fleen-el-guitar"
PLUGIN_NAME="FleenElGuitar"
GITHUB_URL="https://github.com/${REPO_OWNER}/${REPO_NAME}"
RELEASES_URL="https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}/releases/latest"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# Installation paths
DEFAULT_VST3_PATH="$HOME/Library/Audio/Plug-Ins/VST3"
SYSTEM_VST3_PATH="/Library/Audio/Plug-Ins/VST3"

# ============================================================================
# Helper Functions
# ============================================================================

print_header() {
    echo ""
    echo -e "${CYAN}╔════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║${NC}       ${BOLD}Fleen El-Guitar VST3 Installer${NC}                      ${CYAN}║${NC}"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════╝${NC}"
    echo ""
}

print_success() { echo -e "${GREEN}✓${NC} $1"; }
print_info() { echo -e "${BLUE}ℹ${NC} $1"; }
print_warning() { echo -e "${YELLOW}⚠${NC} $1"; }
print_error() { echo -e "${RED}✗${NC} $1"; }
print_step() { echo -e "${CYAN}▶${NC} $1"; }

# Check if running on macOS
check_macos() {
    if [[ "$(uname)" != "Darwin" ]]; then
        print_error "This installer is for macOS only."
        print_info "For Windows/Linux, please download manually from:"
        echo "  ${GITHUB_URL}/releases"
        exit 1
    fi
}

# Check internet connection
check_internet() {
    if ! ping -c 1 github.com &> /dev/null; then
        print_error "No internet connection. Please check your network."
        exit 1
    fi
}

# Find existing VST3 folder or create default
find_vst3_folder() {
    print_step "Looking for VST3 folder..."
    
    # Check common locations
    if [[ -d "$DEFAULT_VST3_PATH" ]]; then
        VST3_INSTALL_PATH="$DEFAULT_VST3_PATH"
        print_success "Found VST3 folder: $VST3_INSTALL_PATH"
        return 0
    fi
    
    if [[ -d "$SYSTEM_VST3_PATH" ]]; then
        VST3_INSTALL_PATH="$SYSTEM_VST3_PATH"
        print_success "Found system VST3 folder: $VST3_INSTALL_PATH"
        return 0
    fi
    
    # Create default folder
    print_warning "No VST3 folder found. Creating default folder..."
    mkdir -p "$DEFAULT_VST3_PATH"
    chmod 755 "$DEFAULT_VST3_PATH"
    VST3_INSTALL_PATH="$DEFAULT_VST3_PATH"
    print_success "Created: $VST3_INSTALL_PATH"
}

# ============================================================================
# Installation Methods
# ============================================================================

# Method 1: Download from GitHub Releases (FASTEST - Recommended)
install_from_release() {
    print_step "Method 1: Downloading pre-built release..."
    
    # Get latest release info
    local release_info
    release_info=$(curl -s "$RELEASES_URL" 2>/dev/null || echo "")
    
    if [[ -z "$release_info" ]] || echo "$release_info" | grep -q "Not Found"; then
        print_warning "Could not fetch release info"
        return 1
    fi
    
    # Extract download URL for macOS asset
    local download_url
    download_url=$(echo "$release_info" | grep -o '"browser_download_url": "[^"]*\.zip"' | head -1 | cut -d'"' -f4)
    
    if [[ -z "$download_url" ]]; then
        print_warning "No release asset found"
        return 1
    fi
    
    print_info "Downloading from: $download_url"
    
    # Create temp directory
    local temp_dir
    temp_dir=$(mktemp -d)
    
    # Download
    if ! curl -L "$download_url" -o "$temp_dir/plugin.zip" 2>/dev/null; then
        print_warning "Download failed"
        rm -rf "$temp_dir"
        return 1
    fi
    
    # Extract
    if ! unzip -q "$temp_dir/plugin.zip" -d "$temp_dir" 2>/dev/null; then
        print_warning "Extraction failed"
        rm -rf "$temp_dir"
        return 1
    fi
    
    # Find VST3 bundle
    local vst3_file
    vst3_file=$(find "$temp_dir" -name "*.vst3" -type d 2>/dev/null | head -1)
    
    if [[ -z "$vst3_file" ]] || [[ ! -d "$vst3_file" ]]; then
        print_warning "No VST3 bundle found in release"
        rm -rf "$temp_dir"
        return 1
    fi
    
    # Install
    print_info "Installing to: $VST3_INSTALL_PATH"
    if ! cp -R "$vst3_file" "$VST3_INSTALL_PATH/"; then
        print_warning "Copy failed"
        rm -rf "$temp_dir"
        return 1
    fi
    
    # Verify
    if [[ -d "$VST3_INSTALL_PATH/${PLUGIN_NAME}.vst3" ]]; then
        rm -rf "$temp_dir"
        print_success "Release installation successful!"
        return 0
    fi
    
    rm -rf "$temp_dir"
    return 1
}

# Method 2: Download raw VST3 directly
install_direct_vst3() {
    print_step "Method 2: Downloading VST3 directly..."
    
    # Try to find direct VST3 download from releases
    local release_info
    release_info=$(curl -s "$RELEASES_URL" 2>/dev/null || echo "")
    
    if [[ -z "$release_info" ]]; then
        print_warning "Could not fetch release info"
        return 1
    fi
    
    # Look for .vst3.zip specifically
    local download_url
    download_url=$(echo "$release_info" | grep -o '"browser_download_url": "[^"]*\.vst3\.zip"' | head -1 | cut -d'"' -f4)
    
    if [[ -z "$download_url" ]]; then
        print_warning "No direct VST3 download found"
        return 1
    fi
    
    local temp_dir
    temp_dir=$(mktemp -d)
    
    print_info "Downloading: $download_url"
    
    if ! curl -L "$download_url" -o "$temp_dir/vst3.zip" 2>/dev/null; then
        print_warning "Download failed"
        rm -rf "$temp_dir"
        return 1
    fi
    
    if ! unzip -q "$temp_dir/vst3.zip" -d "$VST3_INSTALL_PATH" 2>/dev/null; then
        print_warning "Extraction failed"
        rm -rf "$temp_dir"
        return 1
    fi
    
    rm -rf "$temp_dir"
    
    if [[ -d "$VST3_INSTALL_PATH/${PLUGIN_NAME}.vst3" ]]; then
        print_success "Direct download successful!"
        return 0
    fi
    
    return 1
}

# Method 3: Manual download instructions
show_manual_instructions() {
    echo ""
    echo -e "${YELLOW}╔════════════════════════════════════════════════════════╗${NC}"
    echo -e "${YELLOW}║${NC}     ${BOLD}Manual Installation Instructions${NC}                   ${YELLOW}║${NC}"
    echo -e "${YELLOW}╚════════════════════════════════════════════════════════╝${NC}"
    echo ""
    print_info "Automatic installation could not complete. Please follow these steps:"
    echo ""
    echo -e "${BOLD}Step 1: Download the plugin${NC}"
    echo "  1. Open: ${GITHUB_URL}/releases/latest"
    echo "  2. Download the file named: ${PLUGIN_NAME}-*.zip"
    echo ""
    echo -e "${BOLD}Step 2: Extract and Install${NC}"
    echo "  3. Open the downloaded .zip file"
    echo "  4. Find the file ending in .vst3"
    echo "  5. Copy it to: $VST3_INSTALL_PATH"
    echo ""
    echo -e "${BOLD}Step 3: Verify Installation${NC}"
    echo "  6. Open Terminal and run:"
    echo -e "     ${CYAN}ls -la \"$VST3_INSTALL_PATH/${PLUGIN_NAME}.vst3\"${NC}"
    echo "  7. You should see the plugin folder listed"
    echo ""
    echo -e "${BOLD}Step 4: Use in Your DAW${NC}"
    echo "  8. Restart your DAW (Logic, Ableton, etc.)"
    echo "  9. Rescan VST3 plugins if needed"
    echo "  10. Find 'Fleen El-Guitar' in your plugin list"
    echo ""
    echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo ""
}

# ============================================================================
# Verification
# ============================================================================

verify_installation() {
    print_step "Verifying installation..."
    
    local plugin_path="$VST3_INSTALL_PATH/${PLUGIN_NAME}.vst3"
    
    if [[ -d "$plugin_path" ]]; then
        print_success "Plugin installed successfully!"
        echo ""
        echo -e "  ${GREEN}Location:${NC} $plugin_path"
        echo -e "  ${GREEN}Size:${NC} $(du -sh "$plugin_path" 2>/dev/null | cut -f1)"
        echo ""
        return 0
    else
        print_error "Plugin not found at: $plugin_path"
        return 1
    fi
}

show_success_message() {
    echo ""
    echo -e "${GREEN}╔════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║${NC}          ${BOLD}Installation Complete!${NC}                         ${GREEN}║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════╝${NC}"
    echo ""
    print_success "Fleen El-Guitar VST3 has been installed!"
    echo ""
    echo -e "${BOLD}Next Steps:${NC}"
    echo "  1. Restart your DAW (Logic Pro, Ableton Live, etc.)"
    echo "  2. Rescan VST3 plugins if your DAW requires it"
    echo "  3. Look for 'Fleen El-Guitar' in your plugin browser"
    echo ""
    echo -e "${BOLD}Plugin Location:${NC}"
    echo "  $VST3_INSTALL_PATH/${PLUGIN_NAME}.vst3"
    echo ""
    echo -e "${BOLD}Need Help?${NC}"
    echo "  Documentation: ${GITHUB_URL}/tree/main/docs"
    echo "  Issues: ${GITHUB_URL}/issues"
    echo ""
}

# ============================================================================
# Main Installation Flow
# ============================================================================

main() {
    print_header
    
    # Pre-flight checks
    check_macos
    check_internet
    find_vst3_folder
    
    echo ""
    print_info "Installing to: $VST3_INSTALL_PATH"
    echo ""
    
    # Try installation methods in order
    if install_from_release; then
        print_success "Installation completed using Method 1 (Release)"
    elif install_direct_vst3(); then
        print_success "Installation completed using Method 2 (Direct)"
    else
        print_warning "Automatic installation methods failed."
        show_manual_instructions
        exit 1
    fi
    
    # Verify
    if verify_installation; then
        show_success_message
        exit 0
    else
        print_error "Installation verification failed."
        show_manual_instructions
        exit 1
    fi
}

# Run main function
main "$@"
