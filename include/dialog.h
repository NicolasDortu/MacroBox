#ifndef DIALOG_H
#define DIALOG_H

#include <windows.h>

LRESULT CALLBACK MacroDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void ShowMacroInputDialog(HWND hwnd, int buttonId);

#endif // DIALOG_H
