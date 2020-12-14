#ifndef UNICODE
#define UNICODE
#endif 
#ifndef _UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include <new>
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct StateInfo {
    int state;
    void* anything;
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.
    StateInfo* pStateInf = new (std::nothrow)StateInfo;
    if (pStateInf == NULL) {
        return 0;
    }
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        pStateInf        // Additional application data -> lparam->CreateStruct->lpCreateParams
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

inline StateInfo* GetAppState(HWND hwnd)
{
    //cast GWLP_USERDATA LONG_PTR to StateInfo*, so you can handle it with your struct/class
    LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
    StateInfo* pState = reinterpret_cast<StateInfo*>(ptr);
    return pState;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    StateInfo* pState;

    if (uMsg == WM_CREATE) {
        //get the additional Info from lParam
        CREATESTRUCT* pCs = reinterpret_cast<CREATESTRUCT*> (lParam);
        pState = reinterpret_cast<StateInfo*> (pCs->lpCreateParams);
        //put StateInfo pointer into GWLP_USERDATA
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);
    }
    else {
        pState = GetAppState(hwnd);
    }

    switch (uMsg)
    {
    case WM_CREATE:

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);



        FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(180,180,180)));

        EndPaint(hwnd, &ps);
    }
    break;
    case WM_CLOSE:
        if (MessageBox(hwnd, L"Really quit?", L"My Application", MB_OKCANCEL) == IDOK) {
            DestroyWindow(hwnd);
        }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}