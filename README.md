# xv6-riscv with Symlinks

This repository contains a modified version of the [xv6-riscv](https://github.com/mit-pdos/xv6-riscv) operating system, which is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix Version 6 (v6) for the RISC-V architecture.

## New Feature: Symbolic Links (symlinks)

I have implemented a new system call `symlink(target, path)` that creates a symbolic link at `path` which points to `target`.

### Implementation Details:
- **System Call**: Added `SYS_symlink` to the kernel.
- **Inodes**: A new file type `T_SYMLINK` was introduced. The target path of the symlink is stored directly in the inode's data blocks.
- **Open Path Resolution**: The `open` system call was modified to follow symbolic links. If a file being opened is a symlink, the kernel recursively follows the link to its target.
- **Recursion Limit**: To prevent infinite loops caused by circular symlinks, a limit on the number of followed links is enforced (e.g., 10).
- **O_NOFOLLOW**: Support for the `O_NOFOLLOW` flag in `open` was added, which prevents following the symlink if the last component of the path is a symbolic link.

## Running with Docker

The easiest way to build and run xv6-riscv is using the provided Docker configuration. This ensures all necessary dependencies (RISC-V toolchain and QEMU) are correctly installed.

### Prerequisites:
- [Docker](https://www.docker.com/)
- [Docker Compose](https://docs.docker.com/compose/)

### Start the Container:
To build and start the xv6 environment:
```bash
docker compose up -d
```

### Access the Shell:
Once the container is running, you can attach to it to interact with xv6:
```bash
docker compose exec xv6 make qemu
```
This command will compile the kernel and user programs, and then launch xv6 within QEMU.

### Stopping the Container:
```bash
docker compose down
```

## Running Tests

To verify the new symlink feature, a test program `test_symlink` is included.

1. Start xv6 using `make qemu` (inside the container).
2. Inside the xv6 shell, run:
```bash
test_symlink
```
The output should indicate if all symlink tests (basic, chain, dangling, cycle, and nofollow) have passed.

## Acknowledgments
xv6 is inspired by John Lions's Commentary on UNIX 6th Edition. See the original repository for a full list of contributors.
