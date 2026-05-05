# Use Ubuntu 24.04 as the base image
FROM ubuntu:24.04

# Disable interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install necessary tools for building and running xv6
# Includes RISC-V cross-compiler, QEMU, and build utilities
RUN apt-get update && apt-get install -y \
    gcc-riscv64-unknown-elf \
    qemu-system-misc \
    make \
    git \
    gcc \
    bc \
    binutils \
    && rm -rf /var/lib/apt/lists/*

# Create a non-root user 'xv6' and set the default shell to bash
RUN useradd -m -s /bin/bash xv6

# Switch to the 'xv6' user
USER xv6

# Set the working directory for subsequent commands
WORKDIR /home/xv6/xv6-riscv