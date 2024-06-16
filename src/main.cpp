#include <windows.h>
#include "window.h"
#include "utils.h"
#include "dialog.h"
#include "icons.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  const char CLASS_NAME[] = "MacroBoxWindowClass";
  const char DIALOG_CLASS_NAME[] = "MacroDialogClass";

  // Register the window class
  WNDCLASS wc = {};

  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

  RegisterClass(&wc);

  // Register the dialog class
  WNDCLASS dialogWc = {};

  dialogWc.lpfnWndProc = MacroDialogProc;
  dialogWc.hInstance = hInstance;
  dialogWc.lpszClassName = DIALOG_CLASS_NAME;
  dialogWc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

  RegisterClass(&dialogWc);

  // Load the gear icon from base64
  hGearIcon = LoadIconFromBase64(gearIconBase64);
  HICON hLogoIcon = LoadIconFromBase64(logoIconBase64);

  // Create the window
  HWND hwnd = CreateWindowEx(
      0,
      CLASS_NAME,
      "Macro Box",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 550, 400,
      NULL,
      NULL,
      hInstance,
      NULL);

  if (hwnd == NULL)
  {
    return 0;
  }

  // Set the window icon
  SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hLogoIcon);
  SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hLogoIcon);

  ShowWindow(hwnd, nCmdShow);

  // Load user configuration
  std::string gridLayout;
  LoadConfiguration(gridLayout, buttonMacros);

  // Register hotkeys
  RegisterHotKeys(hwnd);

  // Run the message loop
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}
