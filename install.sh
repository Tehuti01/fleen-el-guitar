#!/usr/bin/env bash
# ============================================================================
# Fleen El-Guitar VST3 - Simple Installer
# ============================================================================
# This script downloads the plugin and shows you where to put it
#
# Usage:
#   curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/install.sh | bash
# ============================================================================

set -e

# Colors
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

# Download URL - Update this when you create a release
DOWNLOAD_URL="https://github.com/Tehuti01/fleen-el-guitar/releases/latest/download/FleenElGuitar-macOS.zip"

print_header() {
    echo ""
    echo -e "${CYAN}╔════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║${NC}       ${BOLD}Fleen El-Guitar VST3 - Simple Installer${NC}         ${CYAN}║${NC}"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════╝${NC}"
    echo ""
}

print_success() { echo -e "${GREEN}✓${NC} $1"; }
print_info() { echo -e "${BLUE}ℹ${NC} $1"; }
print_step() { echo -e "${CYAN}▶${NC} $1"; }

print_header

# Check macOS
if [[ "$(uname)" != "Darwin" ]]; then
    echo -e "${YELLOW}⚠ This installer is for macOS only.${NC}"
    echo ""
    echo "For Windows/Linux, please download manually from:"
    echo "  https://github.com/Tehuti01/fleen-el-guitar/releases"
    exit 1
fi

# Set download location
DOWNLOAD_DIR="$HOME/Downloads"
ZIP_FILE="$DOWNLOAD_DIR/FleenElGuitar-macOS.zip"

# Download the plugin
print_step "Downloading Fleen El-Guitar..."
echo ""

if curl -L "$DOWNLOAD_URL" -o "$ZIP_FILE" 2>/dev/null; then
    print_success "Downloaded to: $ZIP_FILE"
else
    echo -e "${YELLOW}⚠ Could not download automatically.${NC}"
    echo ""
    echo "Please download manually from:"
    echo "  https://github.com/Tehuti01/fleen-el-guitar/releases/latest"
    echo ""
    exit 1
fi

# Extract to Downloads
print_step "Extracting..."
echo ""

if unzip -q "$ZIP_FILE" -d "$DOWNLOAD_DIR" 2>/dev/null; then
    print_success "Extracted to: $DOWNLOAD_DIR"
else
    print_info "You can extract the zip file manually"
fi

echo ""
echo -e "${GREEN}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║${NC}          ${BOLD}Download Complete!${NC}                             ${GREEN}║${NC}"
echo -e "${GREEN}╚════════════════════════════════════════════════════════╝${NC}"
echo ""
echo -e "${BOLD}📁 Your Files:${NC}"
echo ""
echo "  Downloaded: $ZIP_FILE"
echo "  Extracted:  $DOWNLOAD_DIR/FleenElGuitar.vst3"
echo ""
echo -e "${BOLD}📦 INSTALLATION INSTRUCTIONS:${NC}"
echo ""
echo "  Copy the FleenElGuitar.vst3 folder to ONE of these locations:"
echo ""
echo -e "    ${CYAN}Option 1 (Recommended):${NC}"
echo "    cp -R ~/Downloads/FleenElGuitar.vst3 ~/Library/Audio/Plug-Ins/VST3/"
echo ""
echo -e "    ${CYAN}Option 2 (System-wide):${NC}"
echo "    sudo cp -R ~/Downloads/FleenElGuitar.vst3 /Library/Audio/Plug-Ins/VST3/"
echo ""
echo -e "${BOLD}🎹 USING IN YOUR DAW:${NC}"
echo ""
echo "  1. Copy the .vst3 file to one of the locations above"
echo "  2. Restart your DAW (Logic, Ableton, GarageBand, etc.)"
echo "  3. Find 'Fleen El-Guitar' in your plugin browser"
echo "  4. Load it on an audio track and start playing!"
echo ""
echo -e "${BOLD}📖 DOCUMENTATION:${NC}"
echo ""
echo "  Full documentation: https://github.com/Tehuti01/fleen-el-guitar/tree/main/docs"
echo "  Report issues:      https://github.com/Tehuti01/fleen-el-guitar/issues"
echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""
print_info "The .vst3 file is ready in your Downloads folder!"
echo ""
