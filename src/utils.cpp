#include "utils.h"
#include <fstream>
#include <sstream>
#include <iterator>

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

void LoadConfiguration(std::string &gridLayout, std::vector<std::string> &buttonMacros)
{
  std::ifstream config("macros.cfg");
  buttonMacros.clear();   // Clear any existing macros to prevent duplication
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

void SaveConfiguration(const std::string &gridLayout, const std::vector<std::string> &buttonMacros)
{
  std::ofstream config("macros.cfg");
  if (config.is_open())
  {
    config << "grid_layout:" << gridLayout << std::endl;
    for (size_t i = 0; i < buttonMacros.size(); ++i)
    {
      config << "button" << (i + 1) << "_macro:" << buttonMacros[i] << std::endl;
    }
    config.close();
  }
}
