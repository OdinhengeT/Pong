#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
    private:
        HINSTANCE hInstance;
        HWND hWnd;
    public:
        Window(std::wstring title, int width, int height);
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        ~Window();

        bool MsgProc();
        int getHeight();
        int getWidth();

        void drawFrame(BITMAPINFO* bitmap_info, void* frame);
};

class RGBColor {
    public:
        unsigned char Blue;
        unsigned char Green;
        unsigned char Red;
        unsigned char Padding;
};

#endif