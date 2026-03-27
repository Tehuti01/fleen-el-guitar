# Contributing to Fleen El-Guitar

Thank you for your interest in contributing to Fleen El-Guitar! This document provides guidelines and instructions for contributing.

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
- [Code Style](#code-style)
- [Pull Request Process](#pull-request-process)
- [Testing](#testing)

---

## Code of Conduct

- Be respectful and inclusive
- Focus on constructive feedback
- Keep discussions professional and on-topic

---

## Getting Started

### 1. Fork the Repository

```bash
# Click "Fork" on GitHub, then clone your fork
git clone https://github.com/YOUR_USERNAME/Fleen-el-guitar-vst.git
cd Fleen-el-guitar-vst
```

### 2. Set Up Development Environment

Follow the [Building from Source](docs/GETTING_STARTED.md#building-from-source) guide.

### 3. Create a Branch

```bash
git checkout -b feature/your-feature-name
```

**Branch naming conventions:**
- `feature/` - New features
- `fix/` - Bug fixes
- `docs/` - Documentation updates
- `refactor/` - Code refactoring
- `test/` - Test additions/updates

---

## Development Workflow

### Making Changes

1. **Make small, focused commits**
2. **Write clear commit messages** (see below)
3. **Test your changes** thoroughly
4. **Update documentation** if needed

### Commit Message Format

```
type: short description

Longer description if needed.

Fixes #123
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation
- `style`: Formatting
- `refactor`: Code restructuring
- `test`: Tests
- `chore`: Maintenance

**Example:**
```
feat: add MIDI learn functionality

Implemented MIDI CC learning for all parameters.
Users can now right-click any control to assign a MIDI CC.

Fixes #45
```

---

## Code Style

### C++ Style Guide

We follow modern C++20 conventions:

#### Naming Conventions

```cpp
// Classes: PascalCase
class AudioProcessor { };

// Functions: camelCase
void processBlock (float* buffer, int numSamples);

// Variables: camelCase
float gainValue = 0.5f;

// Member variables: camelCase with m_ prefix (optional)
float m_sampleRate;

// Constants: PascalCase or UPPER_SNAKE_CASE
constexpr float MaxGain = 1.0f;
constexpr int SAMPLE_RATE = 44100;

// Namespaces: lowercase
namespace fleen { }
```

#### Formatting

```cpp
// Braces on new lines for functions/classes
void MyClass::myFunction()
{
    // Implementation
}

// Inline braces on same line
if (condition)
{
    // Code
}
else
{
    // Code
}

// Indentation: 4 spaces (no tabs)
void function()
{
    if (true)
    {
        doSomething();
    }
}

// Line length: ~100 characters maximum
auto veryLongVariableName = someFunctionCall (
    parameter1,
    parameter2,
    parameter3
);
```

#### Comments

```cpp
// Single line comment

/**
 * @brief Multi-line documentation
 * @param param1 Description of parameter 1
 * @return Description of return value
 */
float calculateGain (float param1);
```

#### Memory Management

```cpp
// Use smart pointers
std::unique_ptr<MyClass> ptr = std::make_unique<MyClass>();
std::shared_ptr<MyClass> shared = std::make_shared<MyClass>();

// Avoid raw new/delete
// ❌ Bad
MyClass* ptr = new MyClass();
delete ptr;

// ✅ Good
auto ptr = std::make_unique<MyClass>();
```

#### Audio Thread Safety

```cpp
// Use atomics for audio-to-GUI communication
std::atomic<float> parameterValue { 0.5f };

// No allocations in processBlock()
// ❌ Bad
void processBlock()
{
    auto buffer = new float[1024]; // NO!
    delete[] buffer;
}

// ✅ Good
void processBlock()
{
    // Use pre-allocated buffers
    // No allocations here
}
```

---

## Pull Request Process

### Before Submitting

1. **Rebase on latest main**
   ```bash
   git fetch origin
   git rebase origin/main
   ```

2. **Build and test**
   ```bash
   cmake --build build --config Release
   ctest --test-dir build
   ```

3. **Run clang-format**
   ```bash
   find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i
   ```

4. **Check for compiler warnings**
   ```bash
   cmake --build build --config Release 2>&1 | grep -i warning
   ```

### Creating the PR

1. **Push your branch**
   ```bash
   git push origin feature/your-feature
   ```

2. **Open a Pull Request** on GitHub

3. **Fill out the PR template:**
   - Description of changes
   - Related issues
   - Testing performed
   - Screenshots (if UI changes)

### PR Review

- Maintainers will review your code
- Address feedback by pushing new commits
- Once approved, your PR will be merged

---

## Testing

### Unit Tests

```bash
# Build tests
cmake --build build --config Release --target FleenElGuitar_Tests

# Run tests
ctest --test-dir build --verbose
```

### Plugin Validation

```bash
# Using pluginval (download from https://pluginval.com)
pluginval /path/to/FleenElGuitar.vst3 --level 2
```

### Manual Testing

Test in multiple DAWs:
- Logic Pro
- Ableton Live
- Cubase
- Reaper
- Pro Tools

---

## Documentation

When adding new features:

1. **Update user documentation** (`docs/USER_MANUAL.md`)
2. **Update developer documentation** (`docs/DEVELOPER_GUIDE.md`)
3. **Add code comments** for public APIs
4. **Update README.md** if necessary

---

## Questions?

- Check existing [issues](https://github.com/tehuti01/Fleen-el-guitar-vst/issues)
- Join our [Discord](https://discord.gg/example)
- Ask in [Discussions](https://github.com/tehuti01/Fleen-el-guitar-vst/discussions)

---

<div align="center">

**Thank you for contributing to Fleen El-Guitar! 🎸**

</div>
