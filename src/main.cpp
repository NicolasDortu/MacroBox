#include <windows.h>
#include "window.h"
#include "utils.h"
#include "dialog.h"
#include "icons.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  const char CLASS_NAME[] = "MacroBoxWindowClass";
  const char MACRO_DIALOG_CLASS_NAME[] = "MacroDialogClass";
  const char GRID_DIALOG_CLASS_NAME[] = "GridLayoutDialogClass";

  // Register the main window class
  WNDCLASS wc = {};
  wc.lpfnWndProc = WindowProc; // Set the window procedure
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

  RegisterClass(&wc);

  // Register the macro dialog class
  WNDCLASS macroDialogWc = {};
  macroDialogWc.lpfnWndProc = MacroDialogProc; // Set the dialog procedure
  macroDialogWc.hInstance = hInstance;
  macroDialogWc.lpszClassName = MACRO_DIALOG_CLASS_NAME;
  macroDialogWc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

  RegisterClass(&macroDialogWc);

  // Register the grid layout dialog class
  WNDCLASS gridDialogWc = {};
  gridDialogWc.lpfnWndProc = GridLayoutDialogProc; // Set the dialog procedure
  gridDialogWc.hInstance = hInstance;
  gridDialogWc.lpszClassName = GRID_DIALOG_CLASS_NAME;
  gridDialogWc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

  RegisterClass(&gridDialogWc);

  // Load icons from base64 strings
  hGearIcon = LoadIconFromBase64(gearIconBase64);
  HICON hLogoIcon = LoadIconFromBase64(logoIconBase64);
  hLogoIconTray = LoadIconFromBase64(logoIconBase64);

  // Load user configuration
  std::string gridLayout;
  LoadConfiguration(gridLayout, buttonMacros);
  int rows = 3, cols = 3; // Default to 3x3 grid if not specified in the config
  if (sscanf(gridLayout.c_str(), "%dx%d", &rows, &cols) != 2)
  {
    // Handle invalid or missing grid layout configuration
    rows = 3;
    cols = 3;
    SaveConfigurationGrid("3x3");
    MessageBox(NULL, "Invalid grid layout in configuration file. Using default 3x3 grid.", "Warning", MB_OK);
  }

  // Create the main window with adjusted size based on grid layout
  // Resizing is locked but maybe i will implement resizing of the buttons based on the window size instead of fixed size
  HWND hwnd = CreateWindowEx(
      0,
      CLASS_NAME,
      "Macro Box",
      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT, 100 + cols * 150, 100 + rows * 50,
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

  // Register hotkeys for the application
  RegisterHotKeys(hwnd);

  // Main message loop to keep the application running
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}
