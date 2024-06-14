#include <windows.h>
#include <fstream>
#include <string>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddControls(HWND hwnd);
void LoadConfiguration();
void SaveConfiguration();
void ExecuteMacro(const std::string &command);
void RegisterHotKeys(HWND hwnd);

HWND hButton1, hButton2, hButtonSave;
HWND hEdit1, hEdit2;

std::string button1Macro;
std::string button2Macro;

#define ID_HOTKEY1 1
#define ID_HOTKEY2 2

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
    switch (wParam)
    {
    case 1:
      ExecuteMacro(button1Macro);
      break;
    case 2:
      ExecuteMacro(button2Macro);
      break;
    case 3:
      SaveConfiguration();
      MessageBox(hwnd, "Configuration saved!", "Info", MB_OK);
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

  hButton2 = CreateWindow(
      "BUTTON",
      "Button 2",
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
      50, 150, 150, 50,
      hwnd,
      (HMENU)2,
      (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
      NULL);

  hButtonSave = CreateWindow(
      "BUTTON",
      "Save Configuration",
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
      50, 250, 150, 50,
      hwnd,
      (HMENU)3,
      (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
      NULL);

  hEdit1 = CreateWindow(
      "EDIT",
      button1Macro.c_str(),
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
      250, 50, 200, 20,
      hwnd,
      NULL,
      (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
      NULL);

  hEdit2 = CreateWindow(
      "EDIT",
      button2Macro.c_str(),
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
      250, 150, 200, 20,
      hwnd,
      NULL,
      (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
      NULL);
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
  char buffer[256];

  GetWindowText(hEdit1, buffer, 256);
  button1Macro = buffer;

  GetWindowText(hEdit2, buffer, 256);
  button2Macro = buffer;

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
    // Show the command in a message box for debugging purposes
    MessageBox(NULL, command.c_str(), "Executing Command", MB_OK);

    // Execute the command
    system(command.c_str());
  }
  else
  {
    MessageBox(NULL, "No command entered.", "Error", MB_OK);
  }
}
