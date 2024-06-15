#include <windows.h>
#include <fstream>
#include <string>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MacroDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void AddControls(HWND hwnd);
void LoadConfiguration();
void SaveConfiguration();
void ExecuteMacro(const std::string &command);
void RegisterHotKeys(HWND hwnd);
void ShowMacroInputDialog(HWND hwnd, int buttonId);

HWND hButton1, hButton2, hButtonEdit1, hButtonEdit2;

std::string button1Macro;
std::string button2Macro;

#define ID_HOTKEY1 1
#define ID_HOTKEY2 2
#define IDC_MACRO_EDIT 1001
#define IDC_BUTTON_SAVE 1002
#define IDC_BUTTON_CANCEL 1003

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

  // Create the window
  HWND hwnd = CreateWindowEx(
      0,
      CLASS_NAME,
      "Macro Box",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 350, 400,
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

  hButtonEdit1 = CreateWindow(
      "BUTTON",
      "Edit",
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
      210, 50, 50, 50,
      hwnd,
      (HMENU)101,
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

  hButtonEdit2 = CreateWindow(
      "BUTTON",
      "Edit",
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
      210, 150, 50, 50,
      hwnd,
      (HMENU)102,
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

void ShowMacroInputDialog(HWND hwnd, int buttonId)
{
  HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

  HWND hDlg = CreateWindowEx(
      WS_EX_DLGMODALFRAME,
      "MacroDialogClass",
      "Edit Macro",
      WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_POPUP | WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 250, 150,
      hwnd,
      NULL,
      hInstance,
      NULL);

  if (!hDlg)
  {
    MessageBox(hwnd, "Failed to create dialog", "Error", MB_OK);
    return;
  }

  std::string *currentMacro = (buttonId == 1) ? &button1Macro : &button2Macro;

  HWND hEditMacro = CreateWindow(
      "EDIT",
      currentMacro->c_str(),
      WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
      20, 20, 200, 20,
      hDlg,
      (HMENU)IDC_MACRO_EDIT,
      hInstance,
      NULL);

  HWND hButtonSave = CreateWindow(
      "BUTTON",
      "Save",
      WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
      50, 60, 60, 25,
      hDlg,
      (HMENU)IDC_BUTTON_SAVE,
      hInstance,
      NULL);

  HWND hButtonCancel = CreateWindow(
      "BUTTON",
      "Cancel",
      WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
      130, 60, 60, 25,
      hDlg,
      (HMENU)IDC_BUTTON_CANCEL,
      hInstance,
      NULL);

  SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)currentMacro);

  ShowWindow(hDlg, SW_SHOW);
}

LRESULT CALLBACK MacroDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  std::string *currentMacro = (std::string *)GetWindowLongPtr(hDlg, GWLP_USERDATA);

  switch (message)
  {
  case WM_COMMAND:
    switch (LOWORD(wParam))
    {
    case IDC_BUTTON_SAVE:
    {
      char buffer[256];
      GetWindowText(GetDlgItem(hDlg, IDC_MACRO_EDIT), buffer, 256);
      *currentMacro = buffer;
      SaveConfiguration();
      DestroyWindow(hDlg);
      return (INT_PTR)TRUE;
    }
    case IDC_BUTTON_CANCEL:
      DestroyWindow(hDlg);
      return (INT_PTR)TRUE;
    }
    break;
  case WM_CLOSE:
    DestroyWindow(hDlg);
    return (INT_PTR)TRUE;
  }
  return DefWindowProc(hDlg, message, wParam, lParam);
}
