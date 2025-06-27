#include "HelloUnreal.h"

#include "iStd.h"
#include "Game.h"

bool runApp;
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    WCHAR szTitle[32] = L"Hello Unreal";
    WCHAR szWindowClass[32] = L"HU";

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HELLOUNREAL));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_HELLOUNREAL);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    runApp = false;
    RECT rt;
    setCurrentMonitor(NULL, rt);
    monitorSize.width = rt.right - rt.left;
    monitorSize.height = rt.bottom - rt.top;

    DWORD dwExStyle, dwStyle;
    setWindowStyle(false, dwExStyle, dwStyle);


    int x = 0;
    int y = 0;
    int w = monitorSize.width, h = monitorSize.height;
    // rt
    // 내가 사용할 윈도 사이즈의 영역을 구하는(예측하는)
    AdjustWindowRectEx(&rt, dwStyle, false, dwExStyle);
    borderSize.width = rt.right - rt.left - monitorSize.width;
    borderSize.height = rt.bottom - rt.top - monitorSize.height;

    HWND hWnd = CreateWindowEx(dwExStyle, szWindowClass, szTitle, 
        dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        rt.left, rt.top, 
        rt.right - rt.left, rt.bottom - rt.top,
        nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    loadApp(hWnd, loadGame, freeGame, drawGame, keyGame);

    h = monitorSize.height * 0.67f;
    // w : h = dw : dg
    w = h * devSize.width / devSize.height + borderSize.width;
    h += borderSize.height;

    MoveWindow(hWnd, (monitorSize.width - w) / 2,
        (monitorSize.height - h) / 2, w, h, true);

    MSG msg;
    runApp = true;
    while (runApp)
    {
        if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            drawApp(iFPS::instance()->update());
            iKeyboardUpdate(iFPS::instance()->delta);
            //iKeyboard::getInstance()->update(iFPS::instance()->delta);
            //keydown = keydown_none;
        }
    }

    freeApp();

    return (int) msg.wParam;
}

#include <stdio.h>

bool mouseMoving = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        printf("WM_CREATE\n");
        break;

    case WM_CLOSE:
        printf("WM_CLOSE\n");
        if (MessageBox(NULL, L"종료할까요?", L"정말?", MB_YESNO) == IDYES)
        {
            printf("예\n");
            runApp = false;
        }
        else
        {
            printf("아니오\n");
            return 0;
        }
        break;

    case WM_KEYDOWN:
        printf("WM_KEYDOWN %d\n", wParam);
        if (wParam == VK_ESCAPE)
        {
            goFullscreen();
            break;
        }
        iKeyboardAdd(true, wParam);
        break;
    case WM_KEYUP:
        printf("WM_KEYUP %d\n", wParam);
        iKeyboardAdd(false, wParam);
        break;

    case WM_LBUTTONDOWN:
        //printf("WM_LBUTTONDOWN %d (%d, %d)\n", wParam, LOWORD(lParam), HIWORD(lParam));
#if 0
        keyApp(iKeyStatBegan, convertCoord(LOWORD(lParam), HIWORD(lParam)));
#else
        // 키 값을 넣어놓음
        iQueueKeyAdd(iKeyStatBegan, convertCoord(LOWORD(lParam), HIWORD(lParam)));
       
#endif
        mouseMoving = true;
        break;
    case WM_LBUTTONUP:
        //printf("WM_LBUTTONDOWN %d (%d, %d)\n", wParam, LOWORD(lParam), HIWORD(lParam));
#if 0
        keyApp(iKeyStatEnded, convertCoord(LOWORD(lParam), HIWORD(lParam)));
#else
        // 키 값을 넣어놓음
        iQueueKeyAdd(iKeyStatEnded, convertCoord(LOWORD(lParam), HIWORD(lParam)));
#endif
        mouseMoving = false;
        break;
    case WM_MOUSEMOVE:
        //if(mouseMoving)
#if 0
        keyApp(iKeyStatMoved, convertCoord(LOWORD(lParam), HIWORD(lParam)));
#else
        // 키 값을 넣어놓음
        iQueueKeyAdd(iKeyStatMoved, convertCoord(LOWORD(lParam), HIWORD(lParam)));

#endif
        break;
    case WM_MOVE:
        //printf("WM_MOVE\n");
        if (runApp)
            drawApp(iFPS::instance()->update());
        break;
    case WM_SIZING: // 크기를 줄이거나 늘리고 있을 때
    {
        //printf("WM_SIZING\n");
        // RECT& rect = (RECT)(lParam); 과 똑같음
        RECT& rt = *reinterpret_cast<LPRECT>(lParam); // window area
        enforceSize(wParam, rt);
    }
        break;
    case WM_SIZE: // client area
        // window 창의 변경된 크기를 알 수 있음
        // 클라이언트의 영역을
        printf("WM_SIZE(%d, %d)\n", LOWORD(lParam), HIWORD(lParam));
        // viewport
        resize(LOWORD(lParam), HIWORD(lParam));
        if (runApp)
            drawApp(iFPS::instance()->update());
        break;

    //case WM_PAINT:
    //    {
    //        PAINTSTRUCT ps;
    //        HDC hdc = BeginPaint(hWnd, &ps);
    //        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
    //        EndPaint(hWnd, &ps);
    //    }
    //    break;
    case WM_DESTROY:
        printf("WM_DESTROY\n");
        PostQuitMessage(0);
        delete iFPS::instance();
        break;
    case WM_ERASEBKGND:
        return 1;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void resize(int width, int height)
{
    float& dw = devSize.width;
    float& dh = devSize.height;
    float& x = viewport.origin.x;
    float& y = viewport.origin.y;
    float& vw = viewport.size.width;
    float& vh = viewport.size.height;

    // 뷰포트 비율 설정
    float r0 = width / dw;
    float r1 = height / dh;
    if (r0 < r1)
    {
        // width 기준
        // 내가 디스플레이를 아래로 당겼다면
        // |ㅡㅡ|
        // |ㅡㅡ| 뷰포트가 양옆으로 채워져야함.
        x = 0;
        vw = width;

        // height 변경
        // vw : vh = dw : dh;
        vh = vw * dh / dw;
        y = (height - vh) / 2;
    }
    else //if (r0 >= r1)
    {
        // height 기준
        // 내가 디스플레이를 옆으로 당겼다면
        // | |  | | 뷰포트가 위쪽은 채워지고 양옆으로 공백이 생김
        y = 0;
        vh = height;

        // width 변경
        vw = dw * vh / dh;
        x = (width - vw) / 2;
    }

    // vWidth : vHieht = devSize.widht : devSize.height;
}

iPoint convertCoord(float x, float y)
{
    // x, y는 윈도우 클라이언트 영역에 해당하는 좌표
    iPoint p;
    p.x = (x - viewport.origin.x) / viewport.size.width * devSize.width;
    p.y = (y - viewport.origin.y) / viewport.size.height * devSize.height;
    //printf("(%.0f, %.0f) => (%.0f, %.0f)\n", x, y, p.x, p.y);
    return p;
}

void setCurrentMonitor(HWND hwnd, RECT& rt)
{
    if (hwnd == NULL)
    {
        rt.left = 0;
        rt.top = 0;
        rt.right = GetSystemMetrics(SM_CXSCREEN);
        rt.bottom = GetSystemMetrics(SM_CYSCREEN);
        return;
    }

    GetWindowRect(hwnd, &rt);
    HMONITOR hMonitor = MonitorFromRect(&rt, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hMonitor, &mi);
    if (mi.dwFlags == MONITORINFOF_PRIMARY)
    {
        rt.left = 0;
        rt.top = 0;
        rt.right = GetSystemMetrics(SM_CXSCREEN);
        rt.bottom = GetSystemMetrics(SM_CYSCREEN);
    }
    else
    {
        memcpy(&rt, &mi.rcWork, sizeof(RECT));
    }
}

void setWindowStyle(bool fullscreen, DWORD& dwExStyle, DWORD& dwStyle)
{
    if (fullscreen)
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP;
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle = WS_OVERLAPPEDWINDOW;
    }
}

void enforceSize(int edge, RECT& rect)
{
    switch (edge)
    {
    case WMSZ_BOTTOM:
    case WMSZ_TOP:
    {
        // w : h = devSize.width : devSize.height
        int h = (rect.bottom - rect.top) - borderSize.height; // client width
        int w = h * devSize.width / devSize.height + borderSize.width; // window width
        rect.left = (rect.left + rect.right) / 2 - w / 2;
        rect.right = rect.left + w;
        break;
    }
    case WMSZ_BOTTOMLEFT:
    {
        int w = (rect.right - rect.left) - borderSize.width; // client width
        int h = (rect.bottom - rect.top) - borderSize.height; // client height
        // if(w/h > devSize.width / devSize.height)
        if (w * devSize.height > devSize.width * h)
        {
            // w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + borderSize.width; // window width
            rect.left = rect.right - w;
        }
        else
        {
            // w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + borderSize.height; // window height
            rect.bottom = rect.top + h;
        }
        break;
    }
    case WMSZ_BOTTOMRIGHT:
    {
        int w = (rect.right - rect.left) - borderSize.width; // client width
        int h = (rect.bottom - rect.top) - borderSize.height; // client height
        // if(w/h > devSize.width / devSize.height)
        if (w * devSize.height > devSize.width * h)
        {
            // w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + borderSize.width; // window width
            rect.right = rect.left + w;
        }
        else
        {
            // w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + borderSize.height; // window height
            rect.bottom = rect.top + h;
        }
        break;
    }
    case WMSZ_LEFT:
    case WMSZ_RIGHT:
    {
        int w = (rect.right - rect.left) - borderSize.width; // client width
        int h = w * devSize.height / devSize.width + borderSize.height; // window height
        rect.top = (rect.top + rect.bottom) / 2 - h / 2;
        rect.bottom = rect.top + h;
        break;
    }
    case WMSZ_TOPLEFT:
    {
        int w = (rect.right - rect.left) - borderSize.width; // client width
        int h = (rect.bottom - rect.top) - borderSize.height; // client height
        if (w * devSize.height > devSize.width * h)
        {
            w = h * devSize.width / devSize.height + borderSize.width;
            rect.left = rect.right - w;
        }
        else
        {
            h = w * devSize.height / devSize.width + borderSize.height;
            rect.top = rect.bottom - h;
        }
        break;
    }
    case WMSZ_TOPRIGHT:
    {
        int w = (rect.right - rect.left) - borderSize.width; // client width
        int h = (rect.bottom - rect.top) - borderSize.height; // client height
        if (w * devSize.height > devSize.width * h)
        {
            w = h * devSize.width / devSize.height + borderSize.width;
            rect.right = rect.left + w;
        }
        else
        {
            h = w * devSize.height / devSize.width + borderSize.height;
            rect.top = rect.bottom - h;
        }
        break;
    }
    }
}

bool isFullscreen = false;
RECT rtPrev;

void goFullscreen()
{
    isFullscreen = !isFullscreen;
    DWORD dwExStyle, dwStyle;
    setWindowStyle(isFullscreen, dwExStyle, dwStyle);
    int x, y, w, h;
    if (isFullscreen)
    {
        GetWindowRect(hwnd, &rtPrev);
        RECT rt;
        setCurrentMonitor(hwnd, rt);
        x = rt.left, y = rt.top;
        w = rt.right - rt.left, h = rt.bottom - rt.top;
    }
    else
    {
        x = rtPrev.left, y = rtPrev.top;
        w = rtPrev.right - rtPrev.left, h = rtPrev.bottom - rtPrev.top;
    }
    // 속성
    SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle);
    SetWindowLong(hwnd, GWL_STYLE, dwStyle);
    // 좌표
    SetWindowPos(hwnd, HWND_TOP, x, y, w, h, SWP_SHOWWINDOW);
}
