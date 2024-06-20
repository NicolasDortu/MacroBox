#ifndef DIALOG_H
#define DIALOG_H

#include <windows.h>

// Dialog procedure function declarations
LRESULT CALLBACK MacroDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GridLayoutDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Show dialog function declarations
void ShowMacroInputDialog(HWND hwnd, int buttonId);
void ShowGridLayoutDialog(HWND hwnd);

#endif // DIALOG_H
