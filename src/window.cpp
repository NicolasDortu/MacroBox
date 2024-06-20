#include "window.h"
#include "dialog.h"
#include "utils.h"
#include <string>
#include <vector>

HICON hGearIcon;                       // Global icon handle
std::vector<HWND> buttonHandles;       // Handles to the macro buttons
std::vector<HWND> gearButtonHandles;   // Handles to the gear buttons
std::vector<std::string> buttonMacros; // List of macros for each button

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

    return 0;
  }
  case WM_COMMAND:
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
    return 0;
  }
  case WM_HOTKEY:
  {
    // Handle hotkey presses
    int hotkeyId = wParam;
    if (hotkeyId >= ID_HOTKEY1 && hotkeyId < ID_HOTKEY1 + buttonHandles.size())
    {
      // Simulate button click when hotkey is pressed
      SendMessage(hwnd, WM_COMMAND, hotkeyId - ID_HOTKEY1 + 1, 0);
    }
    return 0;
  }
  case WM_CLOSE:
  {
    // When the window is closed, unregister hotkeys and destroy the window
    for (size_t i = 0; i < buttonHandles.size(); ++i)
    {
      UnregisterHotKey(hwnd, ID_HOTKEY1 + i);
    }
    DestroyWindow(hwnd);
    return 0;
  }
  case WM_DESTROY:
  {
    // Post a quit message to end the message loop
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
