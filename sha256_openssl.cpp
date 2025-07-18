#include <utility>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <openssl/sha.h>
#include <chrono>


std::string sha256(const std::string &str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

int main()
{
    std::string input;
    std::cout << "Enter message: ";
    std::getline(std::cin, input);
    double total_time = 0.0;
    for (int i = 0; i < 10; ++i)
    {
        // Measure time taken to compute SHA256 hash
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        std::string output = sha256(input);
        std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now() - start;
        total_time += duration.count();
    }
    std::cout << "SHA256 Hash: " << output << std::endl;
    std::cout << "Average hash computation time over 10 runs" << total_time / 10.0 << " ms" << std::endl;
    return 0;
}
