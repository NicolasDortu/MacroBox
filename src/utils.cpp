#include "utils.h"

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
