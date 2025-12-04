# SuperH Architecture Plugin for Binary Ninja
A Binary Ninja architecture plugin providing support for the SuperH family of instructions.

## Overview
This architecture plugin provides support for the disassembling and lifting of SuperH instructions.

### Features
- Full disassembly support for the SH-1, SH-2, and SH-2E instruction sets
- Partial lifting to Binary Ninja's low level IL
- Testing for proper info/text generation and opcode field extraction

## Installation
### Binary Download
The easiest way to use this plugin is to download and install a pre-compiled release binary.
1. Dowload the shared library corresponding to your OS/arch from the latest [GitHub Release](https://github.com/idaholab/bn-superh-arch/releases)
2. Copy the downloaded binary to your Binary Ninja plugin directory (e.g. `~/.binarynina/plygins/`)
3. Start Binary Ninja

### Manual Build and Installation
#### Prerequisites
- Binary Ninja (minimum version: `5.1.8104`)
- CMake 3.15 or higher
- C++20 compatible compiler
- Google Test (pulled in automatically via CMake during build)

#### Building from Source & Install
```bash
# Clone the binja API
git clone https://github.com/Vector35/binaryninja-api.git
cd binaryninja-api

# Set up CMake files
echo -e "\nadd_subdirectory(plugins)" >> CMakeLists.txt
echo -e "\nadd_subdirectory(superh)" >> plugins/CMakeLists.txt

# Download SuperH architecture source
cd plugins
git clone https://github.com/idaholab/bn-superh-arch.git
cd ..

# Build
cmake -DCMAKE_BUILD_TYPE=release -DHEADLESS=yes .
cmake --build . --target all -j

# Install
cp out/bin/libsuperh.so ~/.binaryninja/plugins/libsuperh.so
```

## Usage
### Opening Files
1. Open Binary Ninja and select "Open with Options..."
2. Under "Load Options", set your entry point offset and image base
3. Choose your desired architecture from the "Platform" dropdown menu
   - Options are: `superh-sh1` and `superh-sh2`
4. Open the binary file and wait for auto-analysis to complete

> Note that you may need to manually define functions or customize your binary view for the file to load properly

## Development
### Building for Development
To build with debug symbols, follow the instructions above to build from source but change the build type to debug:
```bash
cmake -DCMAKE_BUILD_TYPE=debug -DHEADLESS=yes .
```

## Testing
This architecture supports testing for some instruction info and text generation. See the `*_test.cpp` files for details.
Test binaries named `superh_architecture_test` and `superh_opcodes_test` are automatically created in the project 
directory when building. To run the tests, simply execute this binary. We welcome additional pull requests to further
enhance this test suite!

### Contributing
Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a [Pull Request](https://github.com/idaholab/bn-superh-arch/pulls)

## Limitations
While every effort was made to ensure the accuracy of this plugin, you may still encounter bugs such as:
- Improperly disassembled instructions
- Missing instructions
- Typos

If you encounter a bug, please consider opening an issue or pull request!

Additionally, not all instructions have been lifted to LLIL. We welcome pull requests to help complete this task!

## Resources
- [Binary Ninja C++ API Documentation](https://api.binary.ninja/cpp/index.html)
- [SuperH SH-1, SH-2, SH-DSP Instruction Set Reference](https://www.renesas.com/en/document/mah/sh-1sh-2sh-dsp-software-manual)
- [SuperH SH-2E Instruction Set Reference](https://www.renesas.com/en/document/mah/sh-2e-software-manual)

## License
Licensed under MIT.

See [LICENSE](LICENSE) file for details.

## Credits
Please see the [NOTICE](NOTICE.txt) file for details.

## Support
If you encounter issues with this repository, please create an [issue](https://github.com/idaholab/bn-st10-arch/issues).
