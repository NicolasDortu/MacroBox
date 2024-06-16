#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <windows.h>

// Base64 decoding function
std::vector<unsigned char> base64_decode(const std::string &in);

// Load icon from base64
HICON LoadIconFromBase64(const std::string &base64);

// Read configuration
void LoadConfiguration(std::string &gridLayout, std::vector<std::string> &buttonMacros);
void SaveConfiguration(const std::string &gridLayout, const std::vector<std::string> &buttonMacros);

#endif // UTILS_H
