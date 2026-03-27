# Frequently Asked Questions (FAQ)

## General

### What is Fleen El-Guitar?

Fleen El-Guitar is a professional guitar processing VST3 plugin featuring a premium skeuomorphic interface, advanced DSP algorithms, and real-time visualizations. It's designed for guitarists, producers, and audio engineers who demand studio-quality tone with an intuitive workflow.

### Is Fleen El-Guitar free?

Yes! Fleen El-Guitar is open-source software released under the GPL-3.0-or-later license. You can download, use, and modify it for free.

### What platforms are supported?

Fleen El-Guitar supports:
- **macOS**: 10.15 (Catalina) and later (including Apple Silicon)
- **Windows**: 10 and later (64-bit)
- **Linux**: Ubuntu 20.04+ and compatible distributions

### What plugin formats are available?

Currently, Fleen El-Guitar is available as:
- **VST3** (primary format)
- **Standalone Application**

AU (Audio Units) and AAX (Avid) formats may be added in future releases.

---

## Installation

### How do I install Fleen El-Guitar?

See our [Getting Started Guide](GETTING_STARTED.md) for detailed installation instructions. Quick summary:

**macOS (Homebrew):**
```bash
brew tap tehuti01/audio
brew install fleen-el-guitar
```

**Manual:**
1. Download from [Releases](https://github.com/tehuti01/Fleen-el-guitar-vst/releases)
2. Copy `.vst3` file to your VST3 plugins directory
3. Restart your DAW

### Where are presets stored?

**macOS:** `~/Library/Application Support/Fleen/El-Guitar/Presets/`  
**Windows:** `%APPDATA%\Fleen\El-Guitar\Presets\`  
**Linux:** `~/.config/Fleen/El-Guitar/Presets/`

### The plugin doesn't show in my DAW

1. **Verify installation**: Check that the `.vst3` file is in the correct directory
2. **Rescan plugins**: Force your DAW to rescan VST3 plugins
3. **Check compatibility**: Ensure your DAW supports VST3 plugins
4. **Restart DAW**: Some DAWs require a full restart

Common VST3 directories:
- **macOS**: `~/Library/Audio/Plug-Ins/VST3/`
- **Windows**: `C:\Program Files\Common Files\VST3\`
- **Linux**: `~/.vst3/` or `/usr/lib/vst3/`

---

## Usage

### What are the system requirements?

**Minimum:**
- Intel Core i5 or equivalent (Apple Silicon supported)
- 4 GB RAM
- 500 MB disk space
- VST3-compatible DAW

**Recommended:**
- Intel Core i7 / Apple M1 or better
- 8 GB RAM
- 1920x1080 display

### Can I use Fleen El-Guitar with bass guitar?

Yes! While designed for electric guitar, Fleen El-Guitar works great with:
- Electric guitar
- Bass guitar
- Acoustic guitar (with pickup or microphone)
- Synthesizers
- Any audio source

### How do I create presets?

1. Dial in your desired sound
2. Click the preset dropdown (top-right)
3. Select "Save Preset..."
4. Enter a name and category
5. Click Save

### Can I share presets with others?

Absolutely! Presets are stored as JSON files. To share:

1. Locate the preset file in your presets directory
2. Send the `.json` file to your friend
3. They place it in their presets directory
4. Restart the plugin

### Does Fleen El-Guitar support MIDI control?

Yes! All parameters can be automated via your DAW's automation system. MIDI CC mapping is handled through your DAW's plugin automation interface.

---

## Technical

### What is the latency of Fleen El-Guitar?

Fleen El-Guitar is optimized for low-latency processing. Actual latency depends on:
- Your audio interface buffer size
- Sample rate
- DAW settings

Typical latency: 3-10 ms at 48 kHz with 256-sample buffer.

### Does it support oversampling?

Currently, Fleen El-Guitar operates at the host sample rate. Oversampling may be added in a future release.

### What sample rates are supported?

Fleen El-Guitar supports:
- 44.1 kHz
- 48 kHz
- 88.2 kHz
- 96 kHz
- 192 kHz

### Is Fleen El-Guitar CPU efficient?

Yes! The plugin is optimized for performance:
- Lock-free, allocation-free audio processing
- SIMD optimizations where applicable
- OpenGL-accelerated graphics
- Typically uses 2-5% CPU on modern systems

### Can I modify the source code?

Yes! Fleen El-Guitar is open-source under GPL-3.0-or-later. You can:
- Modify the code for personal use
- Distribute your modifications (must also be GPL-licensed)
- Submit pull requests to the main repository

See [CONTRIBUTING.md](../CONTRIBUTING.md) for guidelines.

---

## Troubleshooting

### The plugin crashes my DAW

1. **Update your DAW** to the latest version
2. **Update audio drivers** (especially on Windows)
3. **Try a different buffer size**
4. **Disable other plugins** to isolate the issue
5. **Report the bug** on GitHub with crash logs

### Audio crackling or popping

1. **Increase buffer size** in your DAW or audio interface settings
2. **Close other applications** using CPU
3. **Check sample rate** consistency across all devices
4. **Disable power saving** modes on your CPU

### The GUI looks blurry on high-DPI displays

Fleen El-Guitar supports high-DPI scaling. If you experience issues:

1. **Check DAW scaling settings**
2. **Update graphics drivers**
3. **Try standalone mode** to isolate the issue

### Presets aren't loading

1. **Verify file location**: Check that preset files are in the correct directory
2. **Check file format**: Ensure `.json` files are valid JSON
3. **Restart the plugin**: Close and reopen to refresh preset list

---

## Development

### How do I build from source?

See [GETTING_STARTED.md#building-from-source](GETTING_STARTED.md#building-from-source) for detailed build instructions.

Quick start:
```bash
git clone https://github.com/tehuti01/Fleen-el-guitar-vst.git
cd Fleen-el-guitar-vst
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### What dependencies are needed?

- CMake 3.24+
- C++20 compatible compiler
- JUCE 8 (automatically downloaded via CPM)
- Optional: Ninja build system

### How do I contribute?

See [CONTRIBUTING.md](../CONTRIBUTING.md) for contribution guidelines. Quick summary:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

### Is there an API documentation?

Yes! See [API.md](API.md) for code-level documentation.

---

## Licensing

### Can I use Fleen El-Guitar in commercial projects?

Yes! You can use Fleen El-Guitar in commercial music productions. The GPL license applies to the software itself, not the audio output.

### Can I sell modified versions?

Yes, under GPL-3.0-or-later:
- You can sell modified versions
- You must make the source code available
- Your modifications must also be GPL-licensed

### Do I need to credit Fleen El-Guitar in my music?

No, but it's appreciated! The license doesn't require attribution for audio output, only for derivative software works.

---

## Support

### How do I report a bug?

Create an issue on [GitHub Issues](https://github.com/tehuti01/Fleen-el-guitar-vst/issues) with:
- Description of the issue
- Steps to reproduce
- Your system information (OS, DAW, etc.)
- Any error messages or logs

### How do I request a feature?

Create a feature request on [GitHub Discussions](https://github.com/tehuti01/Fleen-el-guitar-vst/discussions) or [Issues](https://github.com/tehuti01/Fleen-el-guitar-vst/issues).

### Is there user support?

Yes! Several options:
- **GitHub Issues**: For bugs and feature requests
- **Discord**: Community support at [discord.gg/example](https://discord.gg/example)
- **Email**: support@fleenaudio.example.com

### How can I donate or support development?

Fleen El-Guitar is free and open-source. If you find it useful:
- ⭐ Star the repository on GitHub
- 🐛 Report bugs and suggest features
- 💻 Contribute code or documentation
- 📢 Share with other musicians

---

<div align="center">

**Still have questions?** [Open an issue](https://github.com/tehuti01/Fleen-el-guitar-vst/issues) or [join our Discord](https://discord.gg/example)

</div>
