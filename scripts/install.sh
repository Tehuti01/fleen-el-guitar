#!/usr/bin/env bash
# ============================================================================
# Fleen El-Guitar VST3 Installation Script
# ============================================================================
# Supports both public and private GitHub repositories
# 
# Usage:
#   # Public repo (no auth needed)
#   curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/scripts/install.sh | bash
#
#   # Private repo (with token)
#   curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/scripts/install.sh | bash -s -- --token YOUR_GITHUB_TOKEN
#
#   # Install specific version
#   curl -fsSL .../install.sh | bash -s -- --version v1.0.0
#
#   # Use pre-built release (recommended for private repos)
#   curl -fsSL .../install.sh | bash -s -- --release
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
NC='\033[0m'

# Configuration
GITHUB_USER="Tehuti01"
GITHUB_REPO="fleen-el-guitar"
GITHUB_TOKEN="${GITHUB_TOKEN:-}"
INSTALL_VERSION=""
USE_RELEASE=false
FORCE_BUILD=false

# ============================================================================
# Helper Functions
# ============================================================================

log_info() { echo -e "${BLUE}==> ${NC}$1"; }
log_success() { echo -e "${GREEN}==> ${NC}$1"; }
log_warning() { echo -e "${YELLOW}==> ${NC}$1"; }
log_error() { echo -e "${RED}==> ${NC}$1"; }
log_step() { echo -e "${CYAN}==> ${NC}$1"; }

show_help() {
    cat << EOF
Fleen El-Guitar VST3 Installer

Usage: install.sh [OPTIONS]

Options:
  --token TOKEN     GitHub Personal Access Token (for private repos)
  --version VER     Install specific version (e.g., v1.0.0)
  --release         Use pre-built release binary (recommended for private repos)
  --build           Force build from source
  --help            Show this help message

Environment Variables:
  GITHUB_TOKEN      GitHub Personal Access Token
  FLEEN_INSTALL_DIR Custom installation directory

Examples:
  # Public repository
  curl -fsSL .../install.sh | bash

  # Private repository with token
  curl -fsSL .../install.sh | bash -s -- --token ghp_xxxxx

  # Install specific version
  curl -fsSL .../install.sh | bash -s -- --version v1.0.0

  # Use pre-built release (fastest, works with private repos)
  curl -fsSL .../install.sh | bash -s -- --release

EOF
    exit 0
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --token)
            GITHUB_TOKEN="$2"
            shift 2
            ;;
        --version)
            INSTALL_VERSION="$2"
            shift 2
            ;;
        --release)
            USE_RELEASE=true
            shift
            ;;
        --build)
            FORCE_BUILD=true
            shift
            ;;
        --help)
            show_help
            ;;
        *)
            log_error "Unknown option: $1"
            show_help
            ;;
    esac
done

# ============================================================================
# System Checks
# ============================================================================

check_system() {
    log_info "Checking system requirements..."
    
    if [[ "$(uname)" != "Darwin" ]]; then
        log_warning "This installer is optimized for macOS."
        log_info "For Windows/Linux, please download from releases page."
    fi
    
    if command -v sw_vers &> /dev/null; then
        MACOS_VERSION=$(sw_vers -productVersion)
        log_info "macOS version: $MACOS_VERSION"
    fi
}

# ============================================================================
# GitHub API Helper (Works with Private Repos)
# ============================================================================

github_api_request() {
    local url="$1"
    local headers=("-H" "Accept: application/vnd.github.v3+json")
    
    if [[ -n "$GITHUB_TOKEN" ]]; then
        headers+=("-H" "Authorization: token $GITHUB_TOKEN")
    fi
    
    curl -s "${headers[@]}" "$url"
}

get_latest_release() {
    if [[ -n "$INSTALL_VERSION" ]]; then
        github_api_request "https://api.github.com/repos/$GITHUB_USER/$GITHUB_REPO/releases/tags/$INSTALL_VERSION"
    else
        github_api_request "https://api.github.com/repos/$GITHUB_USER/$GITHUB_REPO/releases/latest"
    fi
}

# ============================================================================
# VST3 Folder Detection
# ============================================================================

VST3_PATHS=(
    "$HOME/Library/Audio/Plug-Ins/VST3"
    "/Library/Audio/Plug-Ins/VST3"
    "$HOME/Music/Audio/Plug-Ins/VST3"
)

SELECTED_VST3_FOLDER=""

search_vst3_folders() {
    local found_paths=()
    log_step "Searching for VST3 plugin folders..."
    
    for path in "${VST3_PATHS[@]}"; do
        if [[ -d "$path" ]]; then
            found_paths+=("$path")
            echo -e "  ${GREEN}✓${NC} $path"
        fi
    done
    
    # Search with Spotlight
    if command -v mdfind &> /dev/null; then
        while IFS= read -r line; do
            if [[ -n "$line" && ! " ${found_paths[*]} " =~ " ${line} " ]]; then
                found_paths+=("$line")
                echo -e "  ${GREEN}✓${NC} $line"
            fi
        done < <(mdfind "kMDItemContentTypeTree == 'com.steinberg.vst3preset'" 2>/dev/null | head -10)
    fi
    
    printf '%s\n' "${found_paths[@]}"
}

ask_vst3_folder() {
    local found_folders=("$@")
    
    echo ""
    log_step "VST3 Plugin Folder Selection"
    echo "========================================"
    
    if [[ ${#found_folders[@]} -eq 0 ]]; then
        log_warning "No VST3 folder found."
        echo ""
        read -p "$(echo -e ${YELLOW}Create default VST3 folder? [Y/n]: ${NC})" -n 1 -r
        echo ""
        
        if [[ $REPLY =~ ^[Yy]$ || -z $REPLY ]]; then
            create_default_vst3_folder
        else
            log_error "Installation cancelled."
            exit 1
        fi
    else
        echo ""
        echo "Found ${#found_folders[@]} VST3 folder(s):"
        for i in "${!found_folders[@]}"; do
            echo "  [$((i+1))] ${found_folders[$i]}"
        done
        echo "  [$(( ${#found_folders[@]} + 1 ))] Create new folder"
        echo "  [$(( ${#found_folders[@]} + 2 ))] Cancel"
        echo ""
        
        while true; do
            read -p "$(echo -e ${CYAN}Select folder [1-$(( ${#found_folders[@]} + 2 ))]: ${NC})" -n 1 -r choice
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
                    log_error "Installation cancelled."
                    exit 1
                fi
            fi
            log_warning "Invalid choice."
        done
    fi
}

create_default_vst3_folder() {
    local default_path="$HOME/Library/Audio/Plug-Ins/VST3"
    log_step "Creating VST3 folder: $default_path"
    
    mkdir -p "$default_path"
    chmod -R 755 "$HOME/Library/Audio/Plug-Ins"
    
    SELECTED_VST3_FOLDER="$default_path"
    log_success "Created default VST3 folder"
}

confirm_installation() {
    echo ""
    echo "========================================"
    log_step "Installation Summary"
    echo "========================================"
    echo "Plugin: Fleen El-Guitar VST3"
    echo "Version: ${INSTALL_VERSION:-Latest}"
    echo "Destination: $SELECTED_VST3_FOLDER"
    if [[ -n "$GITHUB_TOKEN" ]]; then
        echo "Auth: GitHub Token (private repo)"
    else
        echo "Auth: Public access"
    fi
    echo ""
    
    read -p "$(echo -e ${YELLOW}Continue installation? [Y/n]: ${NC})" -n 1 -r
    echo ""
    
    if [[ $REPLY =~ ^[Nn]$ ]]; then
        log_error "Installation cancelled."
        exit 1
    fi
}

# ============================================================================
# Installation Methods
# ============================================================================

install_from_release() {
    log_info "Installing from pre-built release..."
    
    # Get release info
    local release_json
    release_json=$(get_latest_release)
    
    if [[ -z "$release_json" ]] || echo "$release_json" | grep -q "Not Found"; then
        log_error "Could not access release. Check if:"
        echo "  1. Repository is public, OR"
        echo "  2. You provided a valid --token for private repo"
        exit 1
    fi
    
    # Find macOS asset
    local download_url
    download_url=$(echo "$release_json" | grep -o '"browser_download_url": "[^"]*macOS[^"]*"' | head -1 | cut -d'"' -f4)
    
    if [[ -z "$download_url" ]]; then
        # Try generic zip
        download_url=$(echo "$release_json" | grep -o '"browser_download_url": "[^"]*\.zip"' | head -1 | cut -d'"' -f4)
    fi
    
    if [[ -z "$download_url" ]]; then
        log_error "No macOS release asset found."
        log_info "Available assets:"
        echo "$release_json" | grep "browser_download_url"
        exit 1
    fi
    
    log_step "Downloading from release..."
    local temp_dir
    temp_dir=$(mktemp -d)
    
    # Download with auth if token provided
    if [[ -n "$GITHUB_TOKEN" ]]; then
        curl -L -H "Authorization: token $GITHUB_TOKEN" "$download_url" -o "$temp_dir/fleen-el-guitar.zip"
    else
        curl -L "$download_url" -o "$temp_dir/fleen-el-guitar.zip"
    fi
    
    unzip -q "$temp_dir/fleen-el-guitar.zip" -d "$temp_dir"
    
    # Find and install VST3
    local vst3_source
    vst3_source=$(find "$temp_dir" -name "*.vst3" -type d | head -1)
    
    if [[ -z "$vst3_source" ]]; then
        log_error "No VST3 bundle found in release."
        rm -rf "$temp_dir"
        exit 1
    fi
    
    log_step "Installing VST3 to: $SELECTED_VST3_FOLDER"
    cp -R "$vst3_source" "$SELECTED_VST3_FOLDER/"
    
    rm -rf "$temp_dir"
    log_success "VST3 installed successfully!"
}

install_from_source() {
    log_info "Building from source..."
    
    # Check for build tools
    if ! command -v cmake &> /dev/null; then
        log_warning "CMake not found. Installing..."
        if command -v brew &> /dev/null; then
            brew install cmake ninja
        else
            log_error "Please install CMake and Ninja, then run again."
            exit 1
        fi
    fi
    
    local temp_dir
    temp_dir=$(mktemp -d)
    log_step "Cloning repository..."
    
    # Clone with auth if private
    if [[ -n "$GITHUB_TOKEN" ]]; then
        git clone -b main "https://$GITHUB_TOKEN@github.com/$GITHUB_USER/$GITHUB_REPO.git" "$temp_dir/src"
    else
        # Try public clone first
        if ! git clone -b main "https://github.com/$GITHUB_USER/$GITHUB_REPO.git" "$temp_dir/src" 2>/dev/null; then
            log_error "Repository is private. Please use --token option."
            log_info "Generate token at: https://github.com/settings/tokens"
            rm -rf "$temp_dir"
            exit 1
        fi
    fi
    
    # Build
    cd "$temp_dir/src"
    mkdir build && cd build
    
    log_step "Configuring build..."
    cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release >/dev/null 2>&1
    
    log_step "Building (this may take 5-10 minutes)..."
    cmake --build . --config Release >/dev/null 2>&1
    
    # Install
    local vst3_source="FleenElGuitar_artefacts/Release/VST3/FleenElGuitar.vst3"
    if [[ -d "$vst3_source" ]]; then
        cp -R "$vst3_source" "$SELECTED_VST3_FOLDER/"
        log_success "VST3 installed successfully!"
    else
        log_error "Build completed but VST3 not found."
        rm -rf "$temp_dir"
        exit 1
    fi
    
    rm -rf "$temp_dir"
}

install_via_homebrew() {
    log_info "Installing via Homebrew..."
    
    if ! command -v brew &> /dev/null; then
        log_warning "Homebrew not found."
        read -p "$(echo -e ${YELLOW}Install Homebrew? [Y/n]: ${NC})" -n 1 -r
        echo ""
        
        if [[ $REPLY =~ ^[Yy]$ || -z $REPLY ]]; then
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        else
            log_error "Homebrew required."
            exit 1
        fi
    fi
    
    # For private repos, user needs to tap manually with token
    if [[ -n "$GITHUB_TOKEN" ]]; then
        log_warning "Private repo detected. Homebrew tap requires manual setup."
        log_info "Run these commands:"
        echo ""
        echo "  export GITHUB_TOKEN=your_token"
        echo "  brew tap $GITHUB_USER/audio"
        echo "  brew install fleen-el-guitar"
        echo ""
        return 1
    fi
    
    brew tap "$GITHUB_USER/audio" 2>/dev/null || log_warning "Tap may already exist"
    brew install fleen-el-guitar
    
    log_success "Installed via Homebrew!"
}

# ============================================================================
# Post-Installation
# ============================================================================

post_install() {
    echo ""
    log_info "Post-installation checks..."
    
    if [[ -d "$SELECTED_VST3_FOLDER/FleenElGuitar.vst3" ]]; then
        log_success "✓ VST3 installed successfully"
        echo "  Location: $SELECTED_VST3_FOLDER/FleenElGuitar.vst3"
    else
        log_warning "VST3 may not be installed correctly"
    fi
    
    echo ""
    echo "========================================"
    log_success "Installation Complete!"
    echo "========================================"
    echo ""
    echo "Next steps:"
    echo "  1. Restart your DAW"
    echo "  2. Rescan VST3 plugins"
    echo "  3. Find 'Fleen El-Guitar' in plugin list"
    echo ""
    echo "Documentation: https://github.com/$GITHUB_USER/$GITHUB_REPO/tree/main/docs"
    echo "Support: https://github.com/$GITHUB_USER/$GITHUB_REPO/issues"
    echo "========================================"
}

# ============================================================================
# Main
# ============================================================================

main() {
    echo ""
    echo "============================================"
    echo "  Fleen El-Guitar VST3 Installer"
    echo "  Version: ${INSTALL_VERSION:-Latest}"
    echo "============================================"
    echo ""
    
    check_system
    echo ""
    
    # Search for VST3 folders
    mapfile -t found_folders < <(search_vst3_folders)
    ask_vst3_folder "${found_folders[@]}"
    confirm_installation
    
    # Choose installation method
    if [[ "$USE_RELEASE" == true ]]; then
        install_from_release
    elif [[ "$FORCE_BUILD" == true ]]; then
        install_from_source
    else
        # Default: try release first, fallback to source
        log_info "Attempting release installation (fastest)..."
        if install_from_release; then
            log_success "Release installation successful!"
        else
            log_warning "Release installation failed."
            log_info "Falling back to source build..."
            install_from_source
        fi
    fi
    
    post_install
}

main "$@"
