#include "window.h"
#include "dialog.h"
#include "utils.h"
#include <fstream>
#include <string>

HWND hButton1, hButton2, hGearButton1, hGearButton2;
HICON hGearIcon;
std::string button1Macro;
std::string button2Macro;

void RegisterHotKeys(HWND hwnd)
{
  RegisterHotKey(hwnd, ID_HOTKEY1, MOD_CONTROL, '1');
  RegisterHotKey(hwnd, ID_HOTKEY2, MOD_CONTROL, '2');
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CREATE:
    AddControls(hwnd);
    return 0;

  case WM_COMMAND:
    switch (LOWORD(wParam))
    {
    case 1:
      ExecuteMacro(button1Macro);
      break;
    case 2:
      ExecuteMacro(button2Macro);
      break;
    case 101:
      ShowMacroInputDialog(hwnd, 1);
      break;
    case 102:
      ShowMacroInputDialog(hwnd, 2);
      break;
    }
    return 0;

  case WM_HOTKEY:
    switch (wParam)
    {
    case ID_HOTKEY1:
      SendMessage(hwnd, WM_COMMAND, 1, 0);
      break;
    case ID_HOTKEY2:
      SendMessage(hwnd, WM_COMMAND, 2, 0);
      break;
    }
    return 0;

  case WM_CLOSE:
    UnregisterHotKey(hwnd, ID_HOTKEY1);
    UnregisterHotKey(hwnd, ID_HOTKEY2);
    DestroyWindow(hwnd);
    return 0;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AddControls(HWND hwnd)
{
  hButton1 = CreateWindow(
      "BUTTON",
      "Button 1",
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
      50, 50, 150, 50,
      hwnd,
      (HMENU)1,
      (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
      NULL);

  hGearButton1 = CreateWindow(
      "BUTTON",
      NULL,
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON,
      210, 50, 50, 50,
      hwnd,
      (HMENU)101,
      (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
      NULL);

  SendMessage(hGearButton1, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hGearIcon);

  hButton2 = CreateWindow(
      "BUTTON",
      "Button 2",
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
      50, 150, 150, 50,
      hwnd,
      (HMENU)2,
      (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
      NULL);

  hGearButton2 = CreateWindow(
      "BUTTON",
      NULL,
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON,
      210, 150, 50, 50,
      hwnd,
      (HMENU)102,
      (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
      NULL);

  SendMessage(hGearButton2, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hGearIcon);
}

void LoadConfiguration()
{
  std::ifstream config("macros.cfg");
  if (config.is_open())
  {
    std::getline(config, button1Macro);
    std::getline(config, button2Macro);
    config.close();
  }
}

void SaveConfiguration()
{
  std::ofstream config("macros.cfg");
  if (config.is_open())
  {
    config << button1Macro << std::endl;
    config << button2Macro << std::endl;
    config.close();
  }
}

void ExecuteMacro(const std::string &command)
{
  if (!command.empty())
  {
    // Execute the command
    system(command.c_str());
  }
  else
  {
    MessageBox(NULL, "No command entered.", "Error", MB_OK);
  }
}
