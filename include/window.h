#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <vector>
#include <string>

// Constants for hotkeys and dialog controls
#define ID_HOTKEY1 1
#define IDC_MACRO_EDIT 1001
#define IDC_BUTTON_SAVE 1002
#define IDC_BUTTON_CANCEL 1003
#define IDC_BUTTON_MODIFY_GRID 1004
#define IDC_EDIT_ROWS 1005
#define IDC_EDIT_COLS 1006

// Global variables
extern HICON hGearIcon;
extern std::vector<HWND> buttonHandles;
extern std::vector<HWND> gearButtonHandles;
extern std::vector<std::string> buttonMacros;

// Function declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddControls(HWND hwnd, const std::string &gridLayout);
void LoadConfiguration(std::string &gridLayout, std::vector<std::string> &buttonMacros);
void SaveConfiguration(const std::string &gridLayout, const std::vector<std::string> &buttonMacros);
void ExecuteMacro(const std::string &command);
void RegisterHotKeys(HWND hwnd);
void ShowMacroInputDialog(HWND hwnd, int buttonId);
void ShowGridLayoutDialog(HWND hwnd);

#endif // WINDOW_H
