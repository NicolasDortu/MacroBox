#include "utils.h"
#include <fstream>
#include <sstream>
#include <iterator>

// Base64 decoding function
std::vector<unsigned char> base64_decode(const std::string &in)
{
  std::string base64_chars =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789+/";

  std::vector<unsigned char> out;
  std::vector<int> T(256, -1);
  for (int i = 0; i < 64; i++)
    T[base64_chars[i]] = i;

  int val = 0, valb = -8;
  for (unsigned char c : in)
  {
    if (T[c] == -1)
      break;
    val = (val << 6) + T[c];
    valb += 6;
    if (valb >= 0)
    {
      out.push_back((val >> valb) & 0xFF);
      valb -= 8;
    }
  }
  return out;
}

// Load icon from a base64 encoded string
HICON LoadIconFromBase64(const std::string &base64)
{
  std::vector<unsigned char> decodedData = base64_decode(base64);

  HICON hIcon = CreateIconFromResourceEx(
      decodedData.data(),
      decodedData.size(),
      TRUE,
      0x00030000,
      16, 16,
      LR_DEFAULTCOLOR);

  return hIcon;
}

// Load configuration from a file
void LoadConfiguration(std::string &gridLayout, std::vector<std::string> &buttonMacros)
{
  std::ifstream config("macros.cfg");
  buttonMacros.clear();   // Clear existing macros to prevent duplication
  int rows = 3, cols = 3; // Default to 3x3 grid
  if (config.is_open())
  {
    std::string line;
    while (std::getline(config, line))
    {
      if (line.find("grid_layout:") == 0)
      {
        gridLayout = line.substr(line.find(":") + 1);
        sscanf(gridLayout.c_str(), "%dx%d", &rows, &cols);
      }
      else if (line.find("button") == 0)
      {
        buttonMacros.push_back(line.substr(line.find(":") + 1));
      }
    }
    config.close();
  }
  buttonMacros.resize(rows * cols); // Ensure the vector is properly sized
}

// Save macro configuration to a file (preserving grid layout)
void SaveConfigurationMacro(const std::vector<std::string> &buttonMacros)
{
  std::ifstream config_in("macros.cfg");
  std::ofstream config_out("macros.tmp");
  std::string gridLayout;

  if (config_in.is_open() && config_out.is_open())
  {
    std::getline(config_in, gridLayout);   // Read the first line (grid layout)
    config_out << gridLayout << std::endl; // Write it to the temp file

    for (size_t i = 0; i < buttonMacros.size(); ++i)
    {
      config_out << "button" << (i + 1) << "_macro:" << buttonMacros[i] << std::endl;
    }

    config_in.close();
    config_out.close();

    std::remove("macros.cfg");
    std::rename("macros.tmp", "macros.cfg");
  }
}

// Save grid configuration to a file (preserving macros)
void SaveConfigurationGrid(const std::string &gridLayout)
{
  std::ifstream config_in("macros.cfg");
  std::ofstream config_out("macros.tmp");
  std::vector<std::string> buttonMacros;
  std::string line;

  if (config_in.is_open() && config_out.is_open())
  {
    std::getline(config_in, line);                           // Skip the old grid layout line
    config_out << "grid_layout:" << gridLayout << std::endl; // Write the new grid layout

    while (std::getline(config_in, line))
    {
      config_out << line << std::endl; // Copy remaining lines
    }

    config_in.close();
    config_out.close();

    std::remove("macros.cfg");
    std::rename("macros.tmp", "macros.cfg");
  }
}
