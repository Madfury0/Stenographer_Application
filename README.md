# Steganography File Embedding/Extraction Tool

A command-line application to embed and extract files using `steghide`, implemented in C and C++. Securely hide files within cover files (e.g., images) and retrieve them using a password.

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building the Application](#building-the-application)
- [Usage](#usage)
- [Notes](#notes)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Features

- **Embed Files**: Hide a secret file within a cover file (e.g., JPEG, BMP).
- **Extract Files**: Retrieve hidden files from steganographed files.
- **Password Protection**: Securely handle passwords for embedding/extraction.
- **Cross-Platform**: Works on Linux, macOS, and Windows (via WSL/Cygwin).

## Prerequisites

- `steghide` installed ([Download](http://steghide.sourceforge.net/))
- C/C++ Compiler (`gcc`/`g++` or `clang`)
- `CMake` (≥ 3.10)
- `make` or `ninja`

## Installation 
### Install Dependencies

**Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install steghide build-essential cmake
```

**Linux (Fedora):**
```bash
sudo dnf install steghide gcc g++ cmake make
```

**macOS (Homebrew):**
```bash
brew install steghide cmake
```

**Windows (WSL/Cygwin):**  
Follow Linux instructions in WSL or install Cygwin with required packages.

## Building the Application

### Directory Structure
```
steg_app/
├── c_version/       # C implementation
├── cpp_version/     # C++ implementation
└── README.md
```

### Build Instructions

**C Version:**
```bash
cd c_version
mkdir build && cd build
cmake ..             # Generate build files
make                 # Compile the executable
#Binary: ./steg_c
```

**C++ Version:**
```bash
cd cpp_version
mkdir build && cd build
cmake ..             # Generate build files
make                 # Compile the executable
#Binary: ./steg_cpp
```

## Usage
Run the compiled executable and follow the menu prompts.

### Example Workflow
1. **Embed a File**:
   ```
   $ ./steg_c
   1. Embed
   2. Extract
   3. Exit
   Choice: 1

   Enter cover file: cat.jpg
   Enter secret file: secret.txt
   Enter output file: output.jpg
   Enter password: [hidden input]
   Embedded successfully into output.jpg
   ```

2. **Extract a File**:
   ```
   $ ./steg_c
   1. Embed
   2. Extract
   3. Exit
   Choice: 2

   Enter embedded file: output.jpg
   Enter output file: retrieved_secret.txt
   Enter password: [hidden input]
   Extracted to retrieved_secret.txt
   ```

## Notes
- **Security**: Passwords are hidden during input but passed to `steghide` via command-line arguments (visible in process listings).
- **File Types**: Works best with JPEG, BMP, AU, and WAV files (supported by `steghide`).
- **File Paths**: Use absolute paths if files are not in the working directory.
- **Debugging**: Compile with `-g` in `CMakeLists.txt` to enable debug symbols.

## License
MIT License. See [LICENSE](LICENSE) for details.

## Acknowledgments
- `steghide` for the underlying steganography implementation.
