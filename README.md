## SHA-256 Implementations

This repository contains several implementations and experiments with the SHA-256 cryptographic hash function in C/C++. It is intended for educational purposes, benchmarking, and comparison between a pure C implementation, a C++ implementation, and an OpenSSL-based implementation.

### Contents

- **sha256_c.c**: A standalone C implementation of SHA-256, including padding, block processing, and hash computation. It reads a message from standard input, computes its SHA-256 hash, and reports the average computation time over 10 runs.
- **sha256_scratch.cpp**: A C++ implementation of SHA-256, using STL containers and streams. It also benchmarks the hash computation over 10 runs.
- **sha256_openssl.cpp**: A C++ program that uses the OpenSSL library's SHA-256 routines to compute the hash, for comparison with the manual implementations. It also benchmarks the hash computation.

---

## Build Instructions

### Prerequisites
- **C Compiler** (e.g., GCC/MinGW `gcc`)
- **C++ Compiler** (e.g., `g++`)
- **OpenSSL development libraries** (for `sha256_openssl.cpp`)

### Windows (MSVC or MinGW)

#### Build `sha256_c.c` (C implementation)
```
cl sha256_c.c
```
or
```
gcc sha256_c.c -o sha256_c.o
```

#### Build `sha256_scratch.cpp` (C++ implementation)
```
cl sha256_scratch.cpp
```
or
```
g++ sha256_scratch.cpp -o sha256_scratch.o
```

#### Build `sha256_openssl.cpp` (OpenSSL implementation)
You must have OpenSSL installed and available to your compiler.

```
g++ sha256_openssl.cpp -o sha256_openssl.o -lssl -lcrypto
```

---

## Usage

Run any of the executables and enter a message when prompted. The program will output the SHA-256 hash and the average computation time over 10 runs.

Example:
```bash
$ ./sha256_c
Enter message: hello world
SHA-256: b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9
Average hash computation time over 10 runs: 0.0023000 ms
```

---

## Dependencies

- `sha256_c.c` and `sha256_scratch.cpp` have no external dependencies.
- `sha256_openssl.cpp` requires OpenSSL development libraries.
