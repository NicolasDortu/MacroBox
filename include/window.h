#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <string>

#define ID_HOTKEY1 1
#define ID_HOTKEY2 2
#define IDC_MACRO_EDIT 1001
#define IDC_BUTTON_SAVE 1002
#define IDC_BUTTON_CANCEL 1003

extern HICON hGearIcon;
extern std::string button1Macro;
extern std::string button2Macro;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddControls(HWND hwnd);
void LoadConfiguration();
void SaveConfiguration();
void ExecuteMacro(const std::string &command);
void RegisterHotKeys(HWND hwnd);
void ShowMacroInputDialog(HWND hwnd, int buttonId);

#endif // WINDOW_H
