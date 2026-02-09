#!/bin/bash
# Build verification script for WaveRider

set -e

echo "========================================="
echo "WaveRider Build Verification Script"
echo "========================================="
echo ""

# Check if PICO_SDK_PATH is set
if [ -z "$PICO_SDK_PATH" ]; then
    echo "WARNING: PICO_SDK_PATH environment variable is not set"
    echo "You can either:"
    echo "  1. Set PICO_SDK_PATH to your Pico SDK installation"
    echo "  2. Let CMake auto-download the SDK (set PICO_SDK_FETCH_FROM_GIT=1)"
    echo ""
    
    read -p "Would you like to auto-download the Pico SDK? (y/n) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        export PICO_SDK_FETCH_FROM_GIT=1
        echo "Setting PICO_SDK_FETCH_FROM_GIT=1"
    else
        echo "Please set PICO_SDK_PATH and try again"
        exit 1
    fi
fi

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Run CMake
echo ""
echo "Running CMake configuration..."
cmake ..

# Build the project
echo ""
echo "Building project..."
# Detect number of cores in a portable way
if command -v nproc &> /dev/null; then
    CORES=$(nproc)
elif command -v sysctl &> /dev/null; then
    CORES=$(sysctl -n hw.ncpu 2>/dev/null || echo "4")
else
    CORES=4
fi
make -j${CORES}

# Check if build was successful
if [ -f waverider.uf2 ]; then
    echo ""
    echo "========================================="
    echo "Build successful!"
    echo "========================================="
    echo ""
    echo "Output files:"
    ls -lh waverider.*
    echo ""
    echo "To flash to your Pico:"
    echo "1. Hold BOOTSEL button while connecting Pico to computer"
    echo "2. Copy waverider.uf2 to the mounted Pico drive"
    echo ""
else
    echo ""
    echo "Build failed - waverider.uf2 not found"
    exit 1
fi
