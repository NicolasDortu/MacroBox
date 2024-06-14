#include <windows.h>

// Function declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddControls(HWND hwnd);
void OpenCalculator();

// Global variables
HWND hButton1, hButton2;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  const char CLASS_NAME[] = "MacroBoxWindowClass";

  // Register the window class
  WNDCLASS wc = {};

  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

  RegisterClass(&wc);

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

  // Run the message loop
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CREATE:
    AddControls(hwnd);
    return 0;

  case WM_COMMAND:
    switch (wParam)
    {
    case 1:
      OpenCalculator();
      break;
    case 2:
      MessageBox(hwnd, "Button 2 pressed!", "Info", MB_OK);
      break;
    }
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
      "Open Calculator",
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
      50, 50, 150, 50,
      hwnd,
      (HMENU)1,
      (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
      NULL);

  hButton2 = CreateWindow(
      "BUTTON",
      "Button 2",
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
      50, 150, 150, 50,
      hwnd,
      (HMENU)2,
      (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
      NULL);
}

void OpenCalculator()
{
  system("start calc.exe");
}
