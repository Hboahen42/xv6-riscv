FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    gcc-riscv64-unknown-elf \
    qemu-system-misc \
    make \
    git \
    gcc \
    bc \
    binutils \
    && rm -rf /var/lib/apt/lists/*

RUN useradd -m -s /bin/bash xv6
USER xv6
WORKDIR /home/xv6/xv6-riscv