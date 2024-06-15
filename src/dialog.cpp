#include "dialog.h"
#include "window.h"
#include <string>

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
