#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <windows.h>

// Base64 decoding function declaration
std::vector<unsigned char> base64_decode(const std::string &in);

// Load icon from base64 function declaration
HICON LoadIconFromBase64(const std::string &base64);

// Configuration file handling function declarations
void LoadConfiguration(std::string &gridLayout, std::vector<std::string> &buttonMacros);
void SaveConfigurationMacro(const std::vector<std::string> &buttonMacros); // New function for saving macros
void SaveConfigurationGrid(const std::string &gridLayout);                 // New function for saving grid layout

#endif // UTILS_H
