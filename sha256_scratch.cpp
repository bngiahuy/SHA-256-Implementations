#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <chrono>

using namespace std;

// Right rotate
inline uint32_t rotr(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

// SHA-256 Constants (first 32 bits of the fractional parts of the cube roots of the first 64 primes)
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
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Initial hash values (first 32 bits of the fractional parts of the square roots of the first 8 primes)
const uint32_t h_init[8] = {
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19
};

// Padding function: returns vector of bytes after padding
vector<uint8_t> padMessage(const string& input) {
    vector<uint8_t> padded(input.begin(), input.end());

    uint64_t bitLen = static_cast<uint64_t>(padded.size()) * 8;
    padded.push_back(0x80); // Append 1 bit

    // Padding with zeros until message length ≡ 448 mod 512 (i.e., 56 bytes mod 64)
    while ((padded.size() % 64) != 56) {
        padded.push_back(0x00);
    }

    // Append original length as 64-bit big-endian
    for (int i = 7; i >= 0; --i) {
        padded.push_back((bitLen >> (8 * i)) & 0xFF);
    }

    return padded;
}

// Process one 512-bit block
void processBlock(const uint8_t* block, uint32_t* hash) {
    uint32_t w[64];

    // Copy block to first 16 words (big-endian)
    for (int i = 0; i < 16; ++i) {
        w[i] = (block[i * 4] << 24) | (block[i * 4 + 1] << 16) |
               (block[i * 4 + 2] << 8) | (block[i * 4 + 3]);
    }

    // Extend the 16 words into 64 words
    for (int i = 16; i < 64; ++i) {
        uint32_t s0 = rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
        uint32_t s1 = rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }

    // Initialize working variables
    uint32_t a = hash[0];
    uint32_t b = hash[1];
    uint32_t c = hash[2];
    uint32_t d = hash[3];
    uint32_t e = hash[4];
    uint32_t f = hash[5];
    uint32_t g = hash[6];
    uint32_t h = hash[7];

    // Compression function main loop
    for (int i = 0; i < 64; ++i) {
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

    // Add compressed chunk to current hash value
    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
    hash[4] += e;
    hash[5] += f;
    hash[6] += g;
    hash[7] += h;
}

// Main SHA-256 function
string sha256(const string& input) {
    vector<uint8_t> padded = padMessage(input);

    uint32_t hash[8];
    for (int i = 0; i < 8; ++i) {
        hash[i] = h_init[i];
    }

    for (size_t i = 0; i < padded.size(); i += 64) {
        processBlock(&padded[i], hash);
    }

    // Convert hash to hex string
    stringstream ss;
    for (int i = 0; i < 8; ++i) {
        ss << hex << setw(8) << setfill('0') << hash[i];
    }

    return ss.str();
}

#include <chrono>

int main() {
    string input;
    cout << "Enter message: ";
    getline(cin, input);

    double total_time = 0.0;
    string hash;
    for (int i = 0; i < 10; ++i) {
        auto start = chrono::high_resolution_clock::now();
        hash = sha256(input);
        chrono::duration<double, milli> duration = chrono::high_resolution_clock::now() - start;
        total_time += duration.count();
    }

    cout << "SHA-256: " << hash << endl;
    cout << "Average hash computation time over 10 runs: " << (total_time / 10.0) << " ms" << endl;

    return 0;
}
