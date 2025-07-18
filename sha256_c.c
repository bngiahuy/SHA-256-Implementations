#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

// Right rotate
static inline uint32_t rotr(uint32_t x, uint32_t n)
{
    return (x >> n) | (x << (32 - n));
}

// SHA-256 Constants
const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

const uint32_t h_init[8] = {
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19};

// Padding function: returns pointer to padded message and sets new length
uint8_t *padMessage(const char *input, size_t len, size_t *newLen)
{
    uint64_t bitLen = (uint64_t)len * 8;
    size_t padLen = len + 1;
    while ((padLen % 64) != 56)
        padLen++;
    *newLen = padLen + 8;
    uint8_t *padded = (uint8_t *)calloc(*newLen, 1);
    memcpy(padded, input, len);
    padded[len] = 0x80;
    for (int i = 0; i < 8; ++i)
    {
        padded[padLen + i] = (bitLen >> (56 - 8 * i)) & 0xFF;
    }
    return padded;
}

void processBlock(const uint8_t *block, uint32_t *hash)
{
    uint32_t w[64];
    for (int i = 0; i < 16; ++i)
    {
        w[i] = (block[i * 4] << 24) | (block[i * 4 + 1] << 16) |
               (block[i * 4 + 2] << 8) | (block[i * 4 + 3]);
    }
    for (int i = 16; i < 64; ++i)
    {
        uint32_t s0 = rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
        uint32_t s1 = rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    uint32_t a = hash[0];
    uint32_t b = hash[1];
    uint32_t c = hash[2];
    uint32_t d = hash[3];
    uint32_t e = hash[4];
    uint32_t f = hash[5];
    uint32_t g = hash[6];
    uint32_t h = hash[7];
    for (int i = 0; i < 64; ++i)
    {
        uint32_t S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
        uint32_t ch = (e & f) ^ ((~e) & g);
        uint32_t temp1 = h + S1 + ch + k[i] + w[i];
        uint32_t S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
        uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        uint32_t temp2 = S0 + maj;
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }
    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
    hash[4] += e;
    hash[5] += f;
    hash[6] += g;
    hash[7] += h;
}

void sha256(const char *input, char *output)
{
    size_t len = strlen(input);
    size_t paddedLen;
    uint8_t *padded = padMessage(input, len, &paddedLen);
    uint32_t hash[8];
    for (int i = 0; i < 8; ++i)
        hash[i] = h_init[i];
    for (size_t i = 0; i < paddedLen; i += 64)
    {
        processBlock(padded + i, hash);
    }
    for (int i = 0; i < 8; ++i)
    {
        sprintf(output + i * 8, "%08x", hash[i]);
    }
    free(padded);
}

int main()
{
    char input[1024];
    printf("Enter message: ");
    if (!fgets(input, sizeof(input), stdin))
        return 1;
    size_t l = strlen(input);
    if (l > 0 && input[l - 1] == '\n')
        input[l - 1] = 0;
    double total_time = 0.0;
    char hash[65] = {0};
    for (int i = 0; i < 10; ++i)
    {
        clock_t start = clock();
        sha256(input, hash);
        clock_t end = clock();
        total_time += (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
    }
    printf("SHA-256: %s\n", hash);
    printf("Average hash computation time over 10 runs: %.7f ms\n", total_time / 10.0);
    return 0;
}
