#include <string>
#include "database_engine.h"
#include <windows.h>
#define ID_PUT    1
#define ID_GET    2
#define ID_DELETE 3
#define ID_FLUSH  4
#define ID_STATS  5

HWND keyBox, valueBox, outputBox;
DatabaseEngine* db = NULL;
HWND addControl(HWND parent, const char* type, const char* text, DWORD style,
                int x, int y, int w, int h, int id) {

    HWND control = CreateWindowA(type, text, WS_CHILD | WS_VISIBLE | style,
                                 x, y, w, h, parent, (HMENU)(INT_PTR)id,
                                 GetModuleHandleA(NULL), NULL);

    SendMessageA(control, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
    return control;
}

std::string readText(HWND box) 
{
    int length = GetWindowTextLengthA(box);
    std::string text(length + 1, '\0');
    GetWindowTextA(box, &text[0], length + 1);
    text.resize(length);
    return text;
}

void showMessage(const std::string& message) 
{
    std::string all = message + "\r\n" + readText(outputBox);
    SetWindowTextA(outputBox, all.c_str());
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg) 
    {
    case WM_CREATE:
        addControl(hwnd, "STATIC", "Key:",   0, 20, 22, 60, 20, 0);
        keyBox   = addControl(hwnd, "EDIT", "", WS_BORDER | ES_AUTOHSCROLL, 90, 20, 260, 24, 0);
        addControl(hwnd, "STATIC", "Value:", 0, 20, 57, 60, 20, 0);
        valueBox = addControl(hwnd, "EDIT", "", WS_BORDER | ES_AUTOHSCROLL, 90, 55, 260, 24, 0);

        addControl(hwnd, "BUTTON", "Put",    BS_PUSHBUTTON, 20,  95, 60, 28, ID_PUT);
        addControl(hwnd, "BUTTON", "Get",    BS_PUSHBUTTON, 88,  95, 60, 28, ID_GET);
        addControl(hwnd, "BUTTON", "Delete", BS_PUSHBUTTON, 156, 95, 60, 28, ID_DELETE);
        addControl(hwnd, "BUTTON", "Flush",  BS_PUSHBUTTON, 224, 95, 60, 28, ID_FLUSH);
        addControl(hwnd, "BUTTON", "Stats",  BS_PUSHBUTTON, 292, 95, 58, 28, ID_STATS);

        outputBox = addControl(hwnd, "EDIT", "",
                               WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
                               20, 140, 330, 210, 0);

        return 0;

    case WM_COMMAND: 
    {
        int button = LOWORD(wParam);
        std::string key = readText(keyBox);
        std::string value = readText(valueBox);

        if (button == ID_PUT) 
        {
            if (key.empty() || value.empty()) 
            {
                showMessage("Please type both a key and a value.");

            } 
            else if (key.find('|') != std::string::npos || value.find('|') != std::string::npos) 
            {
                showMessage("The character | is not allowed.");
            } 
            else 
            {
                db->put(key, value);
                showMessage("Stored: " + key + " = " + value);
            }

        } 
        else if (button == ID_GET) 
        {
            std::string result;
            if (key.empty()) showMessage("Please type a key.");
            else if (db->get(key, result)) showMessage(key + " = " + result);
            else showMessage(key + " -> not found");
        } 
        else if (button == ID_DELETE) 
        {
            if (key.empty()) showMessage("Please type a key.");
            else { db->remove(key); showMessage("Deleted: " + key); }
        } 
        else if (button == ID_FLUSH) 
        {
            db->flush();
            showMessage("Saved to disk. Files on disk: " + std::to_string(db->tableCount()));
        } 
        else if (button == ID_STATS) 
        {
            showMessage("Files on disk: " + std::to_string(db->tableCount()) +
                        "   Skipped by Bloom Filter: " + std::to_string(db->bloomSkips) +
                        "   Files really read: " + std::to_string(db->diskReads));

        }
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) 
{
    DatabaseEngine engine("guidb", 1000); 
    db = &engine;
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MiniRocksGui";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursorA(NULL, MAKEINTRESOURCEA(32512));
    RegisterClassA(&wc);

    HWND window = CreateWindowA("MiniRocksGui", "Mini RocksDB",
                                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                                CW_USEDEFAULT, CW_USEDEFAULT, 385, 400,
                                NULL, NULL, hInstance, NULL);

    ShowWindow(window, nCmdShow);
    MSG message;
    while (GetMessageA(&message, NULL, 0, 0)) 
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    return 0;
}