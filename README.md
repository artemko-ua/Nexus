# Nexux - A Minimal Unix-Like Operating System

Nexux is a custom-built Unix-like operating system designed for flexibility, simplicity, and full customization. It is developed from scratch, allowing users to configure everything, from the bootloader to the graphical environment. Nexux is ideal for those who want to understand the fundamentals of operating systems and explore low-level programming.

## Features
- **Custom Bootloader** – Written in Assembly for direct hardware interaction
- **Minimal Bash-Based Shell** – A lightweight command-line interface
- **Basic Filesystem** – In-memory file management
- **Text-based UI** – With cursor support and text navigation
- **Memory Management** – Simple heap allocation system
- **Keyboard Driver** – Support for special keys and key combinations
- **Modular and Configurable Architecture** – Users can build their system as needed
- **ISO Image Available** – Ready for testing and installation
- **Lightweight and Built from Scratch** – No unnecessary components
- **Designed for Learning and Experimentation**

## Getting Started
### Prerequisites
To build and run Nexux, you will need:
- A Linux-based development environment
- GCC and Binutils (for cross-compilation)
- NASM (for Assembly development)
- QEMU or VirtualBox (for testing the OS)

### Download the ISO
You can download the latest Nexux ISO from the [Releases](https://github.com/yourusername/nexux/releases) page.

### Running Nexux
To boot Nexux from the ISO, use QEMU:
```sh
qemu-system-x86_64 -fda nexux.img
```
Alternatively, you can create a bootable USB and test it on real hardware.

## Building Nexux
If you want to build Nexux from source:
```sh
# Clone the repository
git clone https://github.com/yourusername/nexux.git
cd nexux

# Build the OS
make

# Run the OS in QEMU
make run
```

## Shell Commands
Nexux includes a simple Unix-like shell with the following commands:

- `help` - Display a list of available commands
- `clear` - Clear the screen
- `echo [text]` - Print text to the screen
- `ls` - List files in the current directory
- `cat [filename]` - View the contents of a file
- `touch [filename]` - Create a new file
- `rm [filename]` - Remove a file
- `write [filename] [text]` - Write text to a file
- `pwd` - Print working directory

## File System
Nexux includes a simple in-memory filesystem that supports basic file operations. No persistence is provided in the current version; files exist only during the current session.

## Cursor Navigation
The cursor can be controlled using the arrow keys. This allows for more flexible text editing and navigation in the shell.

## Memory Management
Nexux features a simple memory management system that provides basic `malloc` and `free` functionality for dynamic memory allocation.

## Contributing
We welcome contributions! If you find bugs, have feature requests, or want to contribute code, feel free to submit an issue or pull request.

## Future Enhancements
- Persistent filesystem with disk I/O
- Multitasking support
- User accounts and permissions
- Network stack
- Package management
- GUI environment

## License
Nexux is released under the MIT License.