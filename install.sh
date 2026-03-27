#!/usr/bin/env bash
# ============================================================================
# Fleen El-Guitar VST3 - Official Installer
# ============================================================================
# Install, uninstall, or manage your Fleen El-Guitar VST3 plugin
#
# Usage:
#   # Install
#   curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/install.sh | bash
#
#   # Uninstall
#   curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/install.sh | bash -s -- --uninstall
#
#   # Check status
#   curl -fsSL .../install.sh | bash -s -- --status
#
#   # Show help
#   curl -fsSL .../install.sh | bash -s -- --help
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
NC='\033[0m'

# Installation paths
DEFAULT_VST3_PATH="$HOME/Library/Audio/Plug-Ins/VST3"
SYSTEM_VST3_PATH="/Library/Audio/Plug-Ins/VST3"
PRESET_PATH="$HOME/Library/Application Support/Fleen/El-Guitar/Presets"

# Action
ACTION="install"

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

show_help() {
    cat << EOF
${BOLD}Fleen El-Guitar VST3 Installer${NC}

${BOLD}USAGE:${NC}
    install.sh [OPTIONS]

${BOLD}OPTIONS:${NC}
    --install       Install the plugin (default)
    --uninstall     Remove the plugin completely
    --remove        Same as --uninstall
    --status        Check if plugin is installed
    --info          Show installation information
    --help          Show this help message

${BOLD}EXAMPLES:${NC}
    # Install plugin
    curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/install.sh | bash

    # Uninstall plugin
    curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/install.sh | bash -s -- --uninstall

    # Check status
    curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/install.sh | bash -s -- --status

${BOLD}MANUAL COMMANDS:${NC}
    # Install
    curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/install.sh | bash

    # Uninstall
    curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/install.sh | bash -s -- --uninstall

    # Check if installed
    ls -la ~/Library/Audio/Plug-Ins/VST3/${PLUGIN_NAME}.vst3

EOF
    exit 0
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --install)
            ACTION="install"
            shift
            ;;
        --uninstall|--remove)
            ACTION="uninstall"
            shift
            ;;
        --status|--check)
            ACTION="status"
            shift
            ;;
        --info)
            ACTION="info"
            shift
            ;;
        --help|-h)
            show_help
            ;;
        *)
            print_error "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# ============================================================================
# Check Functions
# ============================================================================

check_macos() {
    if [[ "$(uname)" != "Darwin" ]]; then
        print_error "This installer is for macOS only."
        print_info "For Windows/Linux, please download manually from:"
        echo "  ${GITHUB_URL}/releases"
        exit 1
    fi
}

check_internet() {
    if ! ping -c 1 github.com &> /dev/null; then
        print_error "No internet connection. Please check your network."
        exit 1
    fi
}

find_vst3_folder() {
    if [[ -d "$DEFAULT_VST3_PATH" ]]; then
        VST3_INSTALL_PATH="$DEFAULT_VST3_PATH"
        return 0
    fi
    
    if [[ -d "$SYSTEM_VST3_PATH" ]]; then
        VST3_INSTALL_PATH="$SYSTEM_VST3_PATH"
        return 0
    fi
    
    VST3_INSTALL_PATH="$DEFAULT_VST3_PATH"
}

# ============================================================================
# Status & Info Functions
# ============================================================================

check_status() {
    print_step "Checking installation status..."
    echo ""
    
    local plugin_path="$VST3_INSTALL_PATH/${PLUGIN_NAME}.vst3"
    
    if [[ -d "$plugin_path" ]]; then
        print_success "Plugin is INSTALLED"
        echo ""
        echo -e "  ${GREEN}Location:${NC} $plugin_path"
        echo -e "  ${GREEN}Size:${NC} $(du -sh "$plugin_path" 2>/dev/null | cut -f1)"
        echo -e "  ${GREEN}Modified:${NC} $(ls -ld "$plugin_path" | awk '{print $6, $7, $8}')"
        echo ""
        
        # Check presets
        if [[ -d "$PRESET_PATH" ]]; then
            local preset_count
            preset_count=$(find "$PRESET_PATH" -name "*.json" 2>/dev/null | wc -l | tr -d ' ')
            echo -e "  ${GREEN}User Presets:${NC} $preset_count found"
        else
            echo -e "  ${BLUE}User Presets:${NC} None"
        fi
        
        echo ""
        return 0
    else
        print_warning "Plugin is NOT INSTALLED"
        echo ""
        echo "  Expected location: $plugin_path"
        echo ""
        return 1
    fi
}

show_info() {
    echo ""
    echo -e "${BOLD}Fleen El-Guitar VST3 - Installation Information${NC}"
    echo ""
    echo -e "${BOLD}Plugin Details:${NC}"
    echo "  Name: Fleen El-Guitar"
    echo "  Type: VST3 Audio Plugin"
    echo "  Version: Latest from GitHub"
    echo "  Repository: ${GITHUB_URL}"
    echo ""
    echo -e "${BOLD}Installation Paths:${NC}"
    echo "  VST3 Plugin: $VST3_INSTALL_PATH/${PLUGIN_NAME}.vst3"
    echo "  User Presets: $PRESET_PATH"
    echo ""
    echo -e "${BOLD}System Information:${NC}"
    echo "  OS: $(sw_vers -productName) $(sw_vers -productVersion)"
    echo "  Architecture: $(uname -m)"
    echo ""
}

# ============================================================================
# Installation Functions
# ============================================================================

install_from_release() {
    print_step "Method 1: Downloading pre-built release..."
    
    local release_info
    release_info=$(curl -s "$RELEASES_URL" 2>/dev/null || echo "")
    
    if [[ -z "$release_info" ]] || echo "$release_info" | grep -q "Not Found"; then
        print_warning "Could not fetch release info"
        return 1
    fi
    
    local download_url
    download_url=$(echo "$release_info" | grep -o '"browser_download_url": "[^"]*\.zip"' | head -1 | cut -d'"' -f4)
    
    if [[ -z "$download_url" ]]; then
        print_warning "No release asset found"
        return 1
    fi
    
    print_info "Downloading from: $download_url"
    
    local temp_dir
    temp_dir=$(mktemp -d)
    
    if ! curl -L "$download_url" -o "$temp_dir/plugin.zip" 2>/dev/null; then
        print_warning "Download failed"
        rm -rf "$temp_dir"
        return 1
    fi
    
    if ! unzip -q "$temp_dir/plugin.zip" -d "$temp_dir" 2>/dev/null; then
        print_warning "Extraction failed"
        rm -rf "$temp_dir"
        return 1
    fi
    
    local vst3_file
    vst3_file=$(find "$temp_dir" -name "*.vst3" -type d 2>/dev/null | head -1)
    
    if [[ -z "$vst3_file" ]] || [[ ! -d "$vst3_file" ]]; then
        print_warning "No VST3 bundle found in release"
        rm -rf "$temp_dir"
        return 1
    fi
    
    print_info "Installing to: $VST3_INSTALL_PATH"
    if ! cp -R "$vst3_file" "$VST3_INSTALL_PATH/"; then
        print_warning "Copy failed"
        rm -rf "$temp_dir"
        return 1
    fi
    
    rm -rf "$temp_dir"
    
    if [[ -d "$VST3_INSTALL_PATH/${PLUGIN_NAME}.vst3" ]]; then
        print_success "Release installation successful!"
        return 0
    fi
    
    rm -rf "$temp_dir"
    return 1
}

install_direct_vst3() {
    print_step "Method 2: Downloading VST3 directly..."
    
    local release_info
    release_info=$(curl -s "$RELEASES_URL" 2>/dev/null || echo "")
    
    if [[ -z "$release_info" ]]; then
        print_warning "Could not fetch release info"
        return 1
    fi
    
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
# Uninstall Functions
# ============================================================================

uninstall_plugin() {
    print_header
    print_step "Uninstalling Fleen El-Guitar VST3..."
    echo ""
    
    local plugin_path="$VST3_INSTALL_PATH/${PLUGIN_NAME}.vst3"
    local uninstall_failed=false
    
    # Check if plugin is installed
    if [[ ! -d "$plugin_path" ]]; then
        print_warning "Plugin is not installed at: $plugin_path"
        echo ""
        print_info "Nothing to uninstall."
        return 0
    fi
    
    # Confirm uninstall
    echo -e "${YELLOW}⚠ WARNING: This will permanently remove the plugin.${NC}"
    echo ""
    read -p "Are you sure you want to uninstall? [y/N]: " -n 1 -r
    echo ""
    
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        print_info "Uninstall cancelled."
        return 0
    fi
    
    # Remove VST3 plugin
    print_step "Removing VST3 plugin..."
    if rm -rf "$plugin_path"; then
        print_success "Removed: $plugin_path"
    else
        print_error "Failed to remove: $plugin_path"
        uninstall_failed=true
    fi
    
    # Ask about presets
    echo ""
    if [[ -d "$PRESET_PATH" ]]; then
        local preset_count
        preset_count=$(find "$PRESET_PATH" -name "*.json" 2>/dev/null | wc -l | tr -d ' ')
        
        if [[ $preset_count -gt 0 ]]; then
            echo -e "${YELLOW}Found $preset_count user preset(s)${NC}"
            read -p "Remove user presets as well? [y/N]: " -n 1 -r
            echo ""
            
            if [[ $REPLY =~ ^[Yy]$ ]]; then
                print_step "Removing user presets..."
                if rm -rf "$PRESET_PATH"; then
                    print_success "Removed: $PRESET_PATH"
                else
                    print_error "Failed to remove presets"
                    uninstall_failed=true
                fi
            else
                print_info "Keeping user presets"
            fi
        fi
    fi
    
    # Summary
    echo ""
    if [[ "$uninstall_failed" == true ]]; then
        print_warning "Uninstall completed with errors"
        return 1
    else
        print_success "Uninstall completed successfully!"
        echo ""
        echo -e "${BOLD}Note:${NC}"
        echo "  If you had the plugin open in a DAW, please restart it."
        echo "  The plugin will no longer appear in your plugin list."
        echo ""
        echo -e "${BOLD}Want to reinstall later?${NC}"
        echo "  Run: curl -fsSL ${GITHUB_URL}/main/install.sh | bash"
        echo ""
        return 0
    fi
}

# ============================================================================
# Main Installation Flow
# ============================================================================

do_install() {
    print_header
    
    check_macos
    check_internet
    find_vst3_folder
    
    echo ""
    print_info "Installing to: $VST3_INSTALL_PATH"
    echo ""
    
    if install_from_release; then
        print_success "Installation completed using Method 1 (Release)"
    elif install_direct_vst3; then
        print_success "Installation completed using Method 2 (Direct)"
    else
        print_warning "Automatic installation methods failed."
        show_manual_instructions
        exit 1
    fi
    
    if verify_installation; then
        show_success_message
        exit 0
    else
        print_error "Installation verification failed."
        show_manual_instructions
        exit 1
    fi
}

# ============================================================================
# Main Entry Point
# ============================================================================

main() {
    check_macos
    find_vst3_folder
    
    case $ACTION in
        install)
            do_install
            ;;
        uninstall|remove)
            uninstall_plugin
            ;;
        status|check)
            check_status
            exit $?
            ;;
        info)
            show_info
            check_status
            exit $?
            ;;
        *)
            print_error "Unknown action: $ACTION"
            show_help
            ;;
    esac
}

main "$@"
