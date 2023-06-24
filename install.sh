#!/bin/bash

# Determine the operating system
OS=$(uname -s)

# Check the operating system and install dependencies accordingly
if [[ "$OS" == "Linux" ]]; then
    # Linux dependencies
    if command -v apt-get &>/dev/null; then
        # APT package manager (Debian, Ubuntu, etc.)
        echo "Installing Linux dependencies using APT..."
        sudo apt-get update
        sudo apt-get install -y libncursesw6 libnoise1 libssl3 libcrypto3 libsdl2-2.0-0 libsdl2-mixer-2.0-0 libstdc++6 libgcc1
        echo "Linux dependencies installed."
    elif command -v yum &>/dev/null; then
        # YUM package manager (CentOS, Fedora, RHEL, etc.)
        echo "Installing Linux dependencies using YUM..."
        sudo yum update
        sudo yum install -y ncurses libnoise openssl openssl-devel SDL2 SDL2_mixer libstdc++ libgcc
        echo "Linux dependencies installed."
    elif command -v dnf &>/dev/null; then
        # DNF package manager (Fedora, RHEL 8+, etc.)
        echo "Installing Linux dependencies using DNF..."
        sudo dnf update
        sudo dnf install -y ncurses libnoise openssl openssl-devel SDL2 SDL2_mixer libstdc++ libgcc
        echo "Linux dependencies installed."
    elif command -v pacman &>/dev/null; then
        # Pacman package manager (Arch, Manjaro, Garuda Linux, etc.)
        echo "Installing Linux dependencies using Pacman..."
        sudo pacman -Syu
        sudo pacman -S ncurses libnoise openssl libssl crypto++ sdl2 sdl2_mixer gcc-libs
        echo "Linux dependencies installed."
    else
        echo "Unsupported Linux package manager."
        exit 1
    fi
elif [[ "$OS" == "Darwin" ]]; then
    # macOS dependencies (Homebrew required)
    echo "Installing macOS dependencies..."
    brew update
    brew install ncurses libnoise sdl2 sdl2_mixer
    echo "macOS dependencies installed."
elif [[ "$OS" == "Windows" ]]; then
    # Windows dependencies
    echo "Please install the following dependencies on Windows manually:"
    echo "- libncursesw"
    echo "- libnoise"
    echo "- libssl"
    echo "- libcrypto"
    echo "- SDL2"
    echo "- SDL2_mixer"
    echo "- libstdc++"
    echo "- libgcc"
else
    echo "Unsupported operating system: $OS"
    exit 1
fi

# Run your game
echo "Running the game..."
./file_crawler
