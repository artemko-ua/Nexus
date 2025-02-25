# Nexux

Nexux is a minimal Unix-like operating system built from scratch, following the philosophy of full customization and simplicity. Inspired by Arch Linux, it allows users to configure everything, from the bootloader to the graphical interface.

## Features
- **Custom bootloader** written in Assembly
- **Minimal Bash-based shell** for command-line interaction
- **Modular and configurable architecture**
- **Lightweight and built from scratch**
- **Designed for learning and flexibility**

## Getting Started
### Prerequisites
To build and run Nexux, you will need:
- A Linux-based development environment
- GCC and Binutils (for cross-compilation)
- NASM (for Assembly development)
- QEMU (for testing the OS in a virtual machine)

### Building Nexux
Clone the repository and follow these steps:
```sh
# Clone the repository
git clone https://github.com/yourusername/nexux.git
cd nexux

# Build the OS
make
```

### Running Nexux
You can run Nexux using QEMU:
```sh
make run
```

## Contributing
Contributions are welcome! Feel free to submit issues, feature requests, or pull requests.

## License
Nexux is released under the MIT License.
