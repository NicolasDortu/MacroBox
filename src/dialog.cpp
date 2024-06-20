#include "dialog.h"
#include "window.h"
#include "utils.h"
#include <string>

// Macro Dialog Procedure
LRESULT CALLBACK MacroDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  std::string *currentMacro = reinterpret_cast<std::string *>(GetWindowLongPtr(hDlg, GWLP_USERDATA));

  switch (message)
  {
  case WM_INITDIALOG:
    currentMacro = reinterpret_cast<std::string *>(lParam);
    SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(currentMacro));
    SetWindowText(GetDlgItem(hDlg, IDC_MACRO_EDIT), currentMacro->c_str());
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    switch (LOWORD(wParam))
    {
    case IDC_BUTTON_SAVE:
    {
      char buffer[256];
      GetWindowText(GetDlgItem(hDlg, IDC_MACRO_EDIT), buffer, 256);
      *currentMacro = buffer;

      // Save the macro configuration without modifying the grid layout
      SaveConfigurationMacro(buttonMacros);
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

void ShowMacroInputDialog(HWND hwnd, int buttonId)
{
  HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

  if (buttonId < 1 || buttonId > buttonMacros.size())
  {
    MessageBox(hwnd, "Invalid button ID", "Error", MB_OK);
    return;
  }

  std::string *currentMacro = &buttonMacros[buttonId - 1];

  HWND hDlg = CreateWindowEx(
      WS_EX_DLGMODALFRAME,
      "MacroDialogClass",
      "Edit Macro",
      WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_POPUP | WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 300, 150,
      hwnd,
      NULL,
      hInstance,
      NULL); // Pass NULL here

  if (!hDlg)
  {
    MessageBox(hwnd, "Failed to create dialog", "Error", MB_OK);
    return;
  }

  SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(currentMacro)); // Set the user data here

  CreateWindow(
      "STATIC",
      "Macro:",
      WS_VISIBLE | WS_CHILD,
      10, 10, 60, 20,
      hDlg,
      NULL,
      hInstance,
      NULL);

  CreateWindow(
      "EDIT",
      currentMacro->c_str(),
      WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
      80, 10, 200, 20,
      hDlg,
      reinterpret_cast<HMENU>(IDC_MACRO_EDIT),
      hInstance,
      NULL);

  CreateWindow(
      "BUTTON",
      "Save",
      WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
      60, 50, 80, 25,
      hDlg,
      reinterpret_cast<HMENU>(IDC_BUTTON_SAVE),
      hInstance,
      NULL);

  CreateWindow(
      "BUTTON",
      "Cancel",
      WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
      160, 50, 80, 25,
      hDlg,
      reinterpret_cast<HMENU>(IDC_BUTTON_CANCEL),
      hInstance,
      NULL);

  ShowWindow(hDlg, SW_SHOW);
}

// Grid Layout Dialog Procedure
LRESULT CALLBACK GridLayoutDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
    switch (LOWORD(wParam))
    {
    case IDC_BUTTON_SAVE:
    {
      char rowsBuffer[10], colsBuffer[10];
      GetWindowText(GetDlgItem(hDlg, IDC_EDIT_ROWS), rowsBuffer, 10);
      GetWindowText(GetDlgItem(hDlg, IDC_EDIT_COLS), colsBuffer, 10);
      int rows = atoi(rowsBuffer);
      int cols = atoi(colsBuffer);
      if (rows > 0 && cols > 0)
      {
        std::string gridLayout = std::to_string(rows) + "x" + std::to_string(cols);

        // Save the grid layout configuration without modifying the macros
        SaveConfigurationGrid(gridLayout);
        MessageBox(hDlg, "Grid layout updated. Please restart the application to apply changes.", "Info", MB_OK);
        DestroyWindow(hDlg);
      }
      else
      {
        MessageBox(hDlg, "Please enter valid numbers for rows and columns.", "Error", MB_OK);
      }
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

void ShowGridLayoutDialog(HWND hwnd)
{
  HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

  HWND hDlg = CreateWindowEx(
      WS_EX_DLGMODALFRAME,
      "GridLayoutDialogClass",
      "Modify Grid Layout",
      WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_POPUP | WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
      hwnd,
      NULL,
      hInstance,
      NULL);

  if (!hDlg)
  {
    MessageBox(hwnd, "Failed to create dialog", "Error", MB_OK);
    return;
  }

  CreateWindow(
      "STATIC",
      "Rows:",
      WS_VISIBLE | WS_CHILD,
      10, 10, 60, 20,
      hDlg,
      NULL,
      hInstance,
      NULL);

  CreateWindow(
      "EDIT",
      "",
      WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
      80, 10, 200, 20,
      hDlg,
      reinterpret_cast<HMENU>(IDC_EDIT_ROWS),
      hInstance,
      NULL);

  CreateWindow(
      "STATIC",
      "Columns:",
      WS_VISIBLE | WS_CHILD,
      10, 50, 60, 20,
      hDlg,
      NULL,
      hInstance,
      NULL);

  CreateWindow(
      "EDIT",
      "",
      WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
      80, 50, 200, 20,
      hDlg,
      reinterpret_cast<HMENU>(IDC_EDIT_COLS),
      hInstance,
      NULL);

  CreateWindow(
      "BUTTON",
      "Save",
      WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
      60, 100, 80, 25,
      hDlg,
      reinterpret_cast<HMENU>(IDC_BUTTON_SAVE),
      hInstance,
      NULL);

  CreateWindow(
      "BUTTON",
      "Cancel",
      WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
      160, 100, 80, 25,
      hDlg,
      reinterpret_cast<HMENU>(IDC_BUTTON_CANCEL),
      hInstance,
      NULL);

  ShowWindow(hDlg, SW_SHOW);
}
