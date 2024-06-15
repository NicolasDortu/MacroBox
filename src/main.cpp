#include <windows.h>
#include "window.h"
#include "utils.h"
#include "dialog.h"

const std::string gearIconBase64 = "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAM5JREFUOI1jZCAS8Cuo/IexPz64w0isPrhmz3kb/nvO2/Af2SCSNJuXtWDVzEKSaWgGMzAwMDCRokkjLIGBX0HlP7+Cyn+NsAQGBgYGBryBAbOFS1ySQc7eHUP+xqoFuF3Ar6Dyn0tckkEjLIHh28vnDI8O7sTQ/PHBHUasLoBpNkgvZnh34wpcAzKARSVGIGLTDLMdW/wzEqP528vnOBMPIyWa4QaQq5mBgYGBEaYZOZqI1czAgJSQTnbVkKwZxQUwQIpmBgakMIAJkJpVAdF9g4siKN0uAAAAAElFTkSuQmCC";

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

  // Create the window
  HWND hwnd = CreateWindowEx(
      0,
      CLASS_NAME,
      "Macro Box",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
      NULL,
      NULL,
      hInstance,
      NULL);

  if (hwnd == NULL)
  {
    return 0;
  }

  ShowWindow(hwnd, nCmdShow);

  // Load user configuration
  LoadConfiguration();

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
