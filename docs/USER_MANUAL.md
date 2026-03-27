# User Manual - Fleen El-Guitar

## Interface Overview

<div align="center">
  <img src="images/interface-annotated.png" alt="Annotated Interface" width="800"/>
</div>

The Fleen El-Guitar interface is divided into three main sections:

1. **Main Control Panel** (Left) - Primary tone shaping controls
2. **Visual Display** (Right) - Real-time visualization and meters
3. **Preset Browser** (Top Right) - Preset selection and management

---

## Controls Reference

### Gain Stage Section

#### GAIN
**Type:** Rotary Knob  
**Range:** 0% - 100%  
**Default:** 50%

Controls the input gain level. Higher values increase the signal level hitting the distortion stage.

- **Low values (0-30%)**: Clean, transparent boost
- **Medium values (30-70%)**: Balanced input level
- **High values (70-100%)**: Hot signal for aggressive tones

#### DRIVE
**Type:** Rotary Knob  
**Range:** 0% - 100%  
**Default:** 50%

Controls the amount of soft-clipping distortion.

- **Low values (0-30%)**: Clean to light breakup
- **Medium values (30-70%)**: Classic rock crunch
- **High values (70-100%)**: Heavy distortion and saturation

---

### Tone Stack Section

#### BASS
**Type:** Rotary Knob  
**Range:** 0% - 100%  
**Default:** 50%  
**Frequency:** 200 Hz

Low-frequency tone control using a low-shelf EQ.

- **Cut (0-40%)**: Tighten low end, reduce mud
- **Boost (60-100%)**: Add warmth and fullness

#### MID
**Type:** Rotary Knob  
**Range:** 0% - 100%  
**Default:** 50%  
**Frequency:** 1000 Hz

Mid-frequency tone control using a peaking EQ.

- **Cut (0-40%)**: Scooped sound, modern metal
- **Boost (60-100%)**: Cut through the mix, classic rock

#### TREBLE
**Type:** Rotary Knob  
**Range:** 0% - 100%  
**Default:** 50%  
**Frequency:** 3000 Hz

High-frequency tone control using a high-shelf EQ.

- **Cut (0-40%)**: Darker, smoother tone
- **Boost (60-100%)**: Brighter, more presence

#### PRESENCE
**Type:** Rotary Knob  
**Range:** 0% - 100%  
**Default:** 50%  
**Frequency:** 5000 Hz

Ultra-high frequency boost for clarity and articulation.

- **Low values**: Smooth, vintage character
- **High values**: Cutting, modern clarity

---

### Effects Section

#### REVERB
**Type:** Rotary Knob  
**Range:** 0% - 100%  
**Default:** 30%

Controls the mix level of the built-in reverb.

- **0-20%**: Subtle room ambience
- **20-50%**: Natural room/hall reverb
- **50-100%**: Large spaces, atmospheric effects

#### COMPRESSION
**Type:** Rotary Knob  
**Range:** 0% - 100%  
**Default:** 50%

Controls the amount of dynamic compression.

- **0-30%**: Light compression, natural dynamics
- **30-70%**: Sustained notes, even levels
- **70-100%**: Heavy compression, maximum sustain

---

### Utility Controls

#### BYPASS Button
**Type:** Toggle Button

Engages or bypasses the entire effect chain.

- **LED Off**: Effect active
- **LED On (Blue)**: Effect bypassed

#### Power LED
**Type:** Status Indicator (Red)

Indicates plugin is active and processing audio.

---

## Visual Display

### Waveform Display

Real-time visualization of the input and output waveforms.

- **Blue trace**: Input signal
- **Pink trace**: Output signal (processed)
- **Glow intensity**: Signal level indicator

### Spectrum Analyzer

Frequency spectrum display showing the tonal balance.

- **Left side**: Low frequencies
- **Right side**: High frequencies
- **Height**: Amplitude at each frequency

### Level Meters

VU-style meters showing input and output levels.

- **Green**: Normal operating level (-60 to -12 dB)
- **Yellow**: Hot signal (-12 to -3 dB)
- **Red**: Clipping warning (-3 to 0 dB)

---

## Presets

### Using Presets

1. Click the preset dropdown in the top-right corner
2. Select a preset from the list
3. Adjust controls to taste
4. Save modified presets as needed

### Factory Presets

| Preset | Description | Best For |
|--------|-------------|----------|
| **Default** | Balanced starting point | General purpose |
| **Clean** | Crystal clear tone | Jazz, clean funk |
| **Crunch** | Light overdrive | Classic rock, blues |
| **High Gain** | Heavy distortion | Metal, hard rock |
| **Lead** | Singing sustain | Guitar solos |

### Creating Custom Presets

1. Dial in your desired sound
2. Click the preset dropdown
3. Select "Save Preset..."
4. Enter a name and category
5. Click Save

Presets are stored as JSON files and can be shared:
- **macOS**: `~/Library/Application Support/Fleen/El-Guitar/Presets/`
- **Windows**: `%APPDATA%\Fleen\El-Guitar\Presets\`

---

## Tips and Tricks

### Getting Great Tones

#### Clean Tones
1. Set DRIVE to 0-20%
2. Adjust GAIN for desired level
3. Boost BASS slightly (55-60%)
4. Add subtle REVERB (20-30%)

#### Classic Rock Crunch
1. Set DRIVE to 40-60%
2. Boost MID (65-75%)
3. Keep TREBLE moderate (50-60%)
4. Add COMPRESSION (40-50%)

#### Modern High Gain
1. Set DRIVE to 70-90%
2. Scoop MID slightly (40-50%)
3. Boost BASS (65-75%)
4. Use heavy COMPRESSION (60-80%)

### Performance Tips

1. **Use the bypass** to A/B your sound with the dry signal
2. **Watch the meters** to avoid clipping
3. **Start with presets** as a foundation for custom tones
4. **Use PRESENCE** sparingly for cutting lead tones

### Recording Tips

1. **Track with moderate compression** (30-50%)
2. **Leave headroom** - don't push GAIN too high
3. **Use REVERB** in moderation during tracking
4. **Save multiple presets** for different song sections

---

## MIDI Control

Fleen El-Guitar supports MIDI CC automation through your DAW's automation system. All parameters can be automated:

1. Enable automation in your DAW
2. Select the parameter to automate
3. Record or draw automation curves

---

<div align="center">

**Previous:** [Getting Started](GETTING_STARTED.md) | **Next:** [Developer Guide](DEVELOPER_GUIDE.md)

</div>
