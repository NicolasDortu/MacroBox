#include "window.h"
#include "dialog.h"
#include "utils.h"
#include <string>
#include <vector>
#include <shellapi.h>
#include <windows.h>

HICON hGearIcon;                       // Global icon handle
HICON hLogoIconTray;                   // Global logo icon handle
std::vector<HWND> buttonHandles;       // Handles to the macro buttons
std::vector<HWND> gearButtonHandles;   // Handles to the gear buttons
std::vector<std::string> buttonMacros; // List of macros for each button

NOTIFYICONDATA nid; // Notification icon data

// Global variable to track CTRL key state
bool ctrlPressed = false;

// Global variable to store deferred action
std::vector<int> digitsPressed;

void RegisterHotKeys(HWND hwnd)
{
  // Register CTRL+number hotkeys for each button
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
    // When the window is created, load the configuration and add controls
    std::string gridLayout;
    LoadConfiguration(gridLayout, buttonMacros);
    AddControls(hwnd, gridLayout);

    // Add button to modify grid layout
    HWND hModifyGridButton = CreateWindow(
        "BUTTON",
        "Modify Grid",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 10, 100, 30,
        hwnd,
        reinterpret_cast<HMENU>(IDC_BUTTON_MODIFY_GRID),
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);

    // Initialize NOTIFYICONDATA structure
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = 1100; // Unique ID for the icon
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_APP + 1; // Custom message for tray icon
    nid.hIcon = hLogoIconTray;         // Icon for the tray
    strcpy_s(nid.szTip, "Macro Box");

    Shell_NotifyIcon(NIM_ADD, &nid);

    return 0;
  }

  case WM_COMMAND:
  {
    if (LOWORD(wParam) == IDM_TRAY_EXIT)
    {
      Shell_NotifyIcon(NIM_DELETE, &nid);
      DestroyWindow(hwnd);
    }
    else
    {
      // Handle button click events
      int controlId = LOWORD(wParam);
      if (controlId >= 1 && controlId <= buttonHandles.size())
      {
        // Execute the macro associated with the button
        ExecuteMacro(buttonMacros[controlId - 1]);
      }
      else if (controlId >= 101 && controlId < 101 + buttonHandles.size())
      {
        // Show the macro input dialog for the gear button
        ShowMacroInputDialog(hwnd, controlId - 100);
      }
      else if (controlId == IDC_BUTTON_MODIFY_GRID)
      {
        // Show the grid layout modification dialog
        ShowGridLayoutDialog(hwnd);
      }
    }
    return 0;
  }

  case WM_KEYDOWN:
  {
    if (wParam == VK_CONTROL)
    {
      ctrlPressed = true;
    }
    return 0;
  }

  case WM_KEYUP:
  {
    if (wParam == VK_CONTROL)
    {
      ctrlPressed = false;
      if (!digitsPressed.empty())
      {
        // Concatenate the digits to form the hotkey string
        std::string hotkeyStr;
        for (int digit : digitsPressed)
        {
          hotkeyStr += std::to_string(digit);
        }

        // Convert the hotkey string to an integer
        int hotkeyId = std::stoi(hotkeyStr) - 1; // Adjust to zero-based index
        if (hotkeyId >= 0 && hotkeyId < buttonHandles.size())
        {
          // Execute the macro corresponding to the hotkey
          SendMessage(hwnd, WM_COMMAND, hotkeyId + 1, 0);
        }

        // Clear the digits vector
        digitsPressed.clear();
      }
    }
    return 0;
  }

  case WM_HOTKEY:
  {
    // Handle hotkey presses
    int hotkeyId = wParam;

    if (hotkeyId >= ID_HOTKEY1 && hotkeyId < ID_HOTKEY1 + buttonHandles.size())
    {
      // Bring the window to focus
      SetForegroundWindow(hwnd);

      if (!ctrlPressed)
      {
        // Launch the macro when CTRL is released
        int digit = hotkeyId - ID_HOTKEY1 + 1;
        digitsPressed.push_back(digit);
      }
    }
    return 0;
  }

  case WM_APP + 1:
  {
    if (LOWORD(lParam) == WM_RBUTTONUP)
    {
      // Show a context menu when right-clicking the tray icon
      HMENU hMenu = CreatePopupMenu();
      AppendMenu(hMenu, MF_STRING, IDM_TRAY_EXIT, "Exit");
      POINT pt;
      GetCursorPos(&pt);
      SetForegroundWindow(hwnd);
      TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
      DestroyMenu(hMenu);
    }
    else if (LOWORD(lParam) == WM_LBUTTONDBLCLK)
    {
      // Restore the window when double-clicking the tray icon
      ShowWindow(hwnd, SW_RESTORE);
      SetForegroundWindow(hwnd);
    }
    return 0;
  }

  case WM_CLOSE:
  {
    ShowWindow(hwnd, SW_HIDE);
    return 0;
  }
  case WM_DESTROY:
  {
    Shell_NotifyIcon(NIM_DELETE, &nid);
    for (size_t i = 0; i < buttonHandles.size(); ++i)
    {
      UnregisterHotKey(hwnd, ID_HOTKEY1 + i);
    }

    PostQuitMessage(0);
    return 0;
  }
  }
  // Default window procedure for unhandled messages
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AddControls(HWND hwnd, const std::string &gridLayout)
{
  int rows, cols;
  sscanf(gridLayout.c_str(), "%dx%d", &rows, &cols);

  int buttonId = 1;
  // Create buttons and gear buttons based on the grid layout
  for (int row = 0; row < rows; ++row)
  {
    for (int col = 0; col < cols; ++col)
    {
      // Create the macro button
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

      // Create the gear button for editing the macro
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
  // Execute the macro command using system call
  if (!command.empty())
  {
    system(command.c_str());
  }
  else
  {
    // Show error message if no command is set
    MessageBox(NULL, "No command entered.", "Error", MB_OK);
  }
}
