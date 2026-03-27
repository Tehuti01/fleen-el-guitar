#!/usr/bin/env bash
# ============================================================================
# Fleen El-Guitar VST3 Installation Script
# ============================================================================
# This script installs the Fleen El-Guitar VST3 plugin on macOS
# 
# Usage:
#   curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/scripts/install.sh | bash
#
# Or using Homebrew:
#   brew tap Tehuti01/audio
#   brew install fleen-el-guitar
# ============================================================================

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
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

log_step() {
    echo -e "${CYAN}==> ${NC}$1"
}

# ============================================================================
# System Checks
# ============================================================================

check_system() {
    log_info "Checking system requirements..."
    
    # Check if running on macOS
    if [[ "$(uname)" != "Darwin" ]]; then
        log_error "This installer is for macOS only."
        log_info "For Windows and Linux, please download manually from GitHub."
        exit 1
    fi
    
    # Check macOS version
    MACOS_VERSION=$(sw_vers -productVersion)
    log_info "macOS version: $MACOS_VERSION"
}

# ============================================================================
# VST3 Folder Detection
# ============================================================================

# Common VST3 installation paths
VST3_PATHS=(
    "$HOME/Library/Audio/Plug-Ins/VST3"
    "/Library/Audio/Plug-Ins/VST3"
    "$HOME/Music/Audio/Plug-Ins/VST3"
)

# Search for existing VST3 folders
search_vst3_folders() {
    local found_paths=()
    
    log_step "Searching for existing VST3 plugin folders..."
    
    # Check standard paths
    for path in "${VST3_PATHS[@]}"; do
        if [[ -d "$path" ]]; then
            found_paths+=("$path")
            echo -e "  ${GREEN}✓ Found:${NC} $path"
        fi
    done
    
    # Search for additional VST3 folders using mdfind (macOS Spotlight)
    log_step "Searching with Spotlight..."
    while IFS= read -r line; do
        if [[ -n "$line" && ! " ${found_paths[@]} " =~ " ${line} " ]]; then
            found_paths+=("$line")
            echo -e "  ${GREEN}✓ Found:${NC} $line"
        fi
    done < <(mdfind "kMDItemContentTypeTree == 'com.steinberg.vst3preset' 2>/dev/null | head -20" 2>/dev/null || true)
    
    # Also search common DAW plugin folders
    for daw_path in "$HOME/Documents/Audio" "$HOME/Music/DAW" "/Applications"; do
        if [[ -d "$daw_path" ]]; then
            while IFS= read -r line; do
                if [[ -n "$line" && ! " ${found_paths[@]} " =~ " ${line} " ]]; then
                    found_paths+=("$line")
                    echo -e "  ${GREEN}✓ Found:${NC} $line"
                fi
            done < <(find "$daw_path" -type d -name "VST3" 2>/dev/null | head -10 || true)
        fi
    done
    
    # Return found paths
    printf '%s\n' "${found_paths[@]}"
}

# Ask user to confirm VST3 folder
ask_vst3_folder() {
    local found_folders=("$@")
    
    echo ""
    log_step "VST3 Plugin Folder Selection"
    echo "========================================"
    
    if [[ ${#found_folders[@]} -eq 0 ]]; then
        log_warning "No VST3 plugin folder found on your system."
        echo ""
        read -p "$(echo -e ${YELLOW}Would you like to create a default VST3 folder? [Y/n]: ${NC})" -n 1 -r
        echo ""
        
        if [[ $REPLY =~ ^[Yy]$ || -z $REPLY ]]; then
            create_default_vst3_folder
            return 0
        else
            log_error "Installation cancelled. No VST3 folder specified."
            exit 1
        fi
    else
        echo ""
        echo "Found ${#found_folders[@]} VST3 folder(s):"
        echo ""
        
        # Display options
        for i in "${!found_folders[@]}"; do
            echo "  [$((i+1))] ${found_folders[$i]}"
        done
        echo "  [${#found_folders[@]}+1] Create a new VST3 folder"
        echo "  [${#found_folders[@]}+2] Cancel installation"
        echo ""
        
        # Get user choice
        while true; do
            read -p "$(echo -e ${CYAN}Select VST3 folder number [1-${#found_folders[@]}+2]: ${NC})" -n 1 -r choice
            echo ""
            
            if [[ $choice =~ ^[0-9]+$ ]]; then
                if [[ $choice -ge 1 && $choice -le ${#found_folders[@]} ]]; then
                    SELECTED_VST3_FOLDER="${found_folders[$((choice-1))]}"
                    log_success "Selected: $SELECTED_VST3_FOLDER"
                    return 0
                elif [[ $choice -eq $((${#found_folders[@]}+1)) ]]; then
                    create_default_vst3_folder
                    return 0
                elif [[ $choice -eq $((${#found_folders[@]}+2)) ]]; then
                    log_error "Installation cancelled by user."
                    exit 1
                fi
            fi
            
            log_warning "Invalid choice. Please select a number between 1 and $((${#found_folders[@]}+2))."
        done
    fi
}

# Create default VST3 folder
create_default_vst3_folder() {
    local default_path="$HOME/Library/Audio/Plug-Ins/VST3"
    
    echo ""
    log_step "Creating default VST3 folder structure..."
    echo ""
    
    # Create directory structure
    if mkdir -p "$default_path"; then
        log_success "Created: $default_path"
        
        # Set proper permissions
        chmod -R 755 "$HOME/Library/Audio/Plug-Ins"
        log_success "Set permissions for VST3 folder"
        
        SELECTED_VST3_FOLDER="$default_path"
        log_success "Default VST3 folder created successfully!"
        return 0
    else
        log_error "Failed to create VST3 folder at: $default_path"
        exit 1
    fi
}

# Confirm installation location
confirm_installation() {
    echo ""
    echo "========================================"
    log_step "Installation Summary"
    echo "========================================"
    echo ""
    echo "Plugin: Fleen El-Guitar VST3"
    echo "Version: 1.0.0"
    echo "Destination: $SELECTED_VST3_FOLDER"
    echo ""
    
    read -p "$(echo -e ${YELLOW}Continue with installation? [Y/n]: ${NC})" -n 1 -r
    echo ""
    
    if [[ $REPLY =~ ^[Nn]$ ]]; then
        log_error "Installation cancelled by user."
        exit 1
    fi
}

# ============================================================================
# Installation Methods
# ============================================================================

install_direct() {
    log_info "Installing Fleen El-Guitar VST3..."
    echo ""
    
    # Check if VST3 bundle exists in current directory or download
    local vst3_source=""
    
    if [[ -d "FleenElGuitar.vst3" ]]; then
        vst3_source="FleenElGuitar.vst3"
        log_step "Found local VST3 bundle"
    elif [[ -d "build/FleenElGuitar_artefacts/Release/VST3/FleenElGuitar.vst3" ]]; then
        vst3_source="build/FleenElGuitar_artefacts/Release/VST3/FleenElGuitar.vst3"
        log_step "Found built VST3 bundle"
    else
        # Download from GitHub
        log_step "Downloading latest release from GitHub..."
        
        # Get latest release info
        LATEST_RELEASE=$(curl -s https://api.github.com/repos/Tehuti01/fleen-el-guitar/releases/latest)
        
        # Extract download URL for macOS
        DOWNLOAD_URL=$(echo "$LATEST_RELEASE" | grep -o '"browser_download_url": "[^"]*macOS[^"]*"' | head -1 | cut -d'"' -f4)
        
        if [[ -z "$DOWNLOAD_URL" ]]; then
            log_warning "Could not find pre-built macOS release. Downloading source..."
            
            # Download source and build
            TEMP_DIR=$(mktemp -d)
            curl -sL "https://github.com/Tehuti01/fleen-el-guitar/archive/refs/heads/main.zip" -o "$TEMP_DIR/source.zip"
            unzip -q "$TEMP_DIR/source.zip" -d "$TEMP_DIR"
            
            log_step "Building from source..."
            cd "$TEMP_DIR/fleen-el-guitar-main"
            mkdir build && cd build
            cmake .. -DCMAKE_BUILD_TYPE=Release >/dev/null 2>&1
            cmake --build . --config Release >/dev/null 2>&1
            
            if [[ -d "FleenElGuitar_artefacts/Release/VST3/FleenElGuitar.vst3" ]]; then
                vst3_source="FleenElGuitar_artefacts/Release/VST3/FleenElGuitar.vst3"
                cd ../..
            else
                log_error "Build failed. Please build manually following instructions on GitHub."
                rm -rf "$TEMP_DIR"
                exit 1
            fi
        else
            # Download pre-built release
            TEMP_DIR=$(mktemp -d)
            curl -L "$DOWNLOAD_URL" -o "$TEMP_DIR/fleen-el-guitar.zip"
            unzip -q "$TEMP_DIR/fleen-el-guitar.zip" -d "$TEMP_DIR"
            
            # Find VST3 bundle
            vst3_source=$(find "$TEMP_DIR" -name "*.vst3" -type d | head -1)
        fi
    fi
    
    if [[ -z "$vst3_source" || ! -d "$vst3_source" ]]; then
        log_error "Could not find VST3 bundle to install."
        exit 1
    fi
    
    # Install VST3
    log_step "Installing VST3 plugin to: $SELECTED_VST3_FOLDER"
    
    if cp -R "$vst3_source" "$SELECTED_VST3_FOLDER/"; then
        log_success "VST3 plugin installed successfully!"
    else
        log_error "Failed to install VST3 plugin."
        exit 1
    fi
    
    # Cleanup
    if [[ "$vst3_source" == *"/tmp/"* ]]; then
        rm -rf "${vst3_source%/*}"
    fi
    
    # Install standalone application (optional)
    echo ""
    read -p "$(echo -e ${CYAN}Would you like to install the standalone application? [y/N]: ${NC})" -n 1 -r
    echo ""
    
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        local app_dest="$HOME/Applications"
        mkdir -p "$app_dest"
        
        # Try to find or build standalone
        if [[ -d "Fleen El-Guitar.app" ]]; then
            cp -R "Fleen El-Guitar.app" "$app_dest/"
            log_success "Standalone application installed to: $app_dest/Fleen El-Guitar.app"
        elif [[ -d "build/FleenElGuitar_artefacts/Release/Standalone/Fleen El-Guitar.app" ]]; then
            cp -R "build/FleenElGuitar_artefacts/Release/Standalone/Fleen El-Guitar.app" "$app_dest/"
            log_success "Standalone application installed to: $app_dest/Fleen El-Guitar.app"
        else
            log_warning "Standalone application not available. You can build it manually."
        fi
    fi
}

install_via_homebrew() {
    log_info "Installing via Homebrew..."
    
    # Check if Homebrew is installed
    if ! command -v brew &> /dev/null; then
        log_warning "Homebrew not found."
        echo ""
        read -p "$(echo -e ${YELLOW}Would you like to install Homebrew? [Y/n]: ${NC})" -n 1 -r
        echo ""
        
        if [[ $REPLY =~ ^[Yy]$ || -z $REPLY ]]; then
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        else
            log_error "Homebrew required for this installation method."
            exit 1
        fi
    fi
    
    # Add our tap
    log_step "Adding Fleen Homebrew tap..."
    brew tap Tehuti01/audio 2>/dev/null || log_warning "Tap may already exist."
    
    # Install the plugin
    log_step "Installing Fleen El-Guitar VST3..."
    brew install fleen-el-guitar
    
    log_success "Installation complete via Homebrew!"
}

# ============================================================================
# Post-Installation
# ============================================================================

post_install() {
    echo ""
    log_info "Running post-installation checks..."
    echo ""
    
    # Check if VST3 was installed correctly
    if [[ -d "$SELECTED_VST3_FOLDER/FleenElGuitar.vst3" ]]; then
        log_success "✓ VST3 plugin installed successfully"
        echo "  Location: $SELECTED_VST3_FOLDER/FleenElGuitar.vst3"
    else
        log_warning "VST3 plugin may not be installed correctly"
    fi
    
    # Inform user about DAW rescan
    echo ""
    echo "========================================"
    log_success "Installation Complete!"
    echo "========================================"
    echo ""
    echo "The Fleen El-Guitar VST3 plugin has been installed to:"
    echo "  $SELECTED_VST3_FOLDER/FleenElGuitar.vst3"
    echo ""
    echo "Next steps:"
    echo "  1. Restart your DAW (if open)"
    echo "  2. Rescan VST3 plugins in your DAW"
    echo "  3. Look for 'Fleen El-Guitar' in your plugin list"
    echo ""
    echo "Common DAW plugin scan locations:"
    echo "  • Logic Pro: Automatic scan on launch"
    echo "  • Ableton Live: Preferences → Plug-Ins → Browse"
    echo "  • Cubase: Devices → Plug-in Information → Refresh"
    echo "  • Reaper: Options → Preferences → Plug-ins → VST"
    echo ""
    
    if [[ -d "$HOME/Applications/Fleen El-Guitar.app" ]]; then
        echo "Standalone application:"
        echo "  $HOME/Applications/Fleen El-Guitar.app"
        echo ""
    fi
    
    echo "Documentation: https://github.com/Tehuti01/fleen-el-guitar/tree/main/docs"
    echo "Support: https://github.com/Tehuti01/fleen-el-guitar/issues"
    echo "========================================"
}

# ============================================================================
# Uninstall Function
# ============================================================================

uninstall() {
    log_info "Uninstalling Fleen El-Guitar VST3..."
    echo ""
    
    # Search for installed VST3
    local found_folders=()
    while IFS= read -r line; do
        if [[ -n "$line" ]]; then
            found_folders+=("$line")
        fi
    done < <(search_vst3_folders)
    
    if [[ ${#found_folders[@]} -eq 0 ]]; then
        log_warning "No VST3 folders found. Nothing to uninstall."
        exit 0
    fi
    
    # Ask which folder to check
    for vst3_folder in "${found_folders[@]}"; do
        if [[ -d "$vst3_folder/FleenElGuitar.vst3" ]]; then
            echo -e "  ${GREEN}✓ Found:${NC} $vst3_folder/FleenElGuitar.vst3"
            
            read -p "$(echo -e ${YELLOW}Remove FleenElGuitar.vst3 from $vst3_folder? [Y/n]: ${NC})" -n 1 -r
            echo ""
            
            if [[ $REPLY =~ ^[Yy]$ || -z $REPLY ]]; then
                rm -rf "$vst3_folder/FleenElGuitar.vst3"
                log_success "Removed: $vst3_folder/FleenElGuitar.vst3"
            fi
        fi
    done
    
    # Remove standalone app
    if [[ -d "$HOME/Applications/Fleen El-Guitar.app" ]]; then
        read -p "$(echo -e ${YELLOW}Remove standalone application? [Y/n]: ${NC})" -n 1 -r
        echo ""
        
        if [[ $REPLY =~ ^[Yy]$ || -z $REPLY ]]; then
            rm -rf "$HOME/Applications/Fleen El-Guitar.app"
            log_success "Removed standalone application"
        fi
    fi
    
    # Remove preset directory (optional)
    if [[ -d "$HOME/Library/Application Support/Fleen/El-Guitar" ]]; then
        read -p "$(echo -e ${YELLOW}Remove user presets? [y/N]: ${NC})" -n 1 -r
        echo ""
        
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            rm -rf "$HOME/Library/Application Support/Fleen/El-Guitar"
            log_success "Removed user presets"
        fi
    fi
    
    echo ""
    log_success "Uninstallation complete!"
}

# ============================================================================
# Main
# ============================================================================

main() {
    echo ""
    echo "============================================"
    echo "  Fleen El-Guitar VST3 Installer"
    echo "  Version 1.0.0"
    echo "============================================"
    echo ""
    
    case "${1:-install}" in
        install)
            check_system
            echo ""
            
            # Search for VST3 folders
            mapfile -t found_folders < <(search_vst3_folders)
            
            # Ask user to select or create VST3 folder
            ask_vst3_folder "${found_folders[@]}"
            
            # Confirm installation
            confirm_installation
            
            # Install
            install_direct
            
            # Post-installation
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
            echo "  install    - Install with interactive VST3 folder selection (default)"
            echo "  homebrew   - Install via Homebrew (non-interactive)"
            echo "  uninstall  - Remove the plugin"
            echo ""
            echo "Examples:"
            echo "  $0 install"
            echo "  curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/scripts/install.sh | bash"
            exit 1
            ;;
    esac
}

main "$@"
