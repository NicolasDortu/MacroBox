#include "window.h"
#include "dialog.h"
#include "utils.h"
#include <string>
#include <vector>

HICON hGearIcon;
std::vector<HWND> buttonHandles;
std::vector<HWND> gearButtonHandles;
std::vector<std::string> buttonMacros;

void RegisterHotKeys(HWND hwnd)
{
  for (size_t i = 0; i < buttonHandles.size(); ++i)
  {
    RegisterHotKey(hwnd, ID_HOTKEY1 + i, MOD_CONTROL, '1' + i);
  }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CREATE:
  {
    std::string gridLayout;
    LoadConfiguration(gridLayout, buttonMacros);
    AddControls(hwnd, gridLayout);
    return 0;
  }
  case WM_COMMAND:
  {
    int controlId = LOWORD(wParam);
    if (controlId >= 1 && controlId <= buttonHandles.size())
    {
      ExecuteMacro(buttonMacros[controlId - 1]);
    }
    else if (controlId >= 101 && controlId < 101 + buttonHandles.size())
    {
      ShowMacroInputDialog(hwnd, controlId - 100);
    }
    return 0;
  }
  case WM_HOTKEY:
  {
    int hotkeyId = wParam;
    if (hotkeyId >= ID_HOTKEY1 && hotkeyId < ID_HOTKEY1 + buttonHandles.size())
    {
      SendMessage(hwnd, WM_COMMAND, hotkeyId - ID_HOTKEY1 + 1, 0);
    }
    return 0;
  }
  case WM_CLOSE:
  {
    for (size_t i = 0; i < buttonHandles.size(); ++i)
    {
      UnregisterHotKey(hwnd, ID_HOTKEY1 + i);
    }
    DestroyWindow(hwnd);
    return 0;
  }
  case WM_DESTROY:
  {
    PostQuitMessage(0);
    return 0;
  }
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AddControls(HWND hwnd, const std::string &gridLayout)
{
  int rows = 3, cols = 3; // Default to 3x3 grid
  sscanf(gridLayout.c_str(), "%dx%d", &rows, &cols);

  int buttonId = 1;
  for (int row = 0; row < rows; ++row)
  {
    for (int col = 0; col < cols; ++col)
    {
      HWND hButton = CreateWindow(
          "BUTTON",
          ("Button " + std::to_string(buttonId)).c_str(),
          WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
          50 + col * 150, 50 + row * 50, 100, 50,
          hwnd,
          reinterpret_cast<HMENU>(buttonId),
          (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
          NULL);
      buttonHandles.push_back(hButton);

      HWND hGearButton = CreateWindow(
          "BUTTON",
          NULL,
          WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON,
          160 + col * 150, 50 + row * 50, 30, 30,
          hwnd,
          reinterpret_cast<HMENU>(100 + buttonId),
          (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
          NULL);
      SendMessage(hGearButton, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hGearIcon);
      gearButtonHandles.push_back(hGearButton);

      buttonId++;
    }
  }
}

void ExecuteMacro(const std::string &command)
{
  if (!command.empty())
  {
    system(command.c_str());
  }
  else
  {
    MessageBox(NULL, "No command entered.", "Error", MB_OK);
  }
}
