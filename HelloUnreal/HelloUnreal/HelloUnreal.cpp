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
    int x = 0, y = 0, w = 640, h = 480;
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        x, y, 1920, 1080, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    loadApp(hWnd, loadGame, freeGame, drawGame, keyGame);

    MoveWindow(hWnd, x, y, w, h, true);

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
            keydown = keydown_none;
        }
    }

    freeApp();

    return (int) msg.wParam;
}

#include <stdio.h>

bool mouseMoving = false;

void ctrlKey(bool pressed, int& keydown, int key)
{
    int keys[][2] = {
        // 키를 추가 시키고싶다면 여기다 추가만 하면됨
        {87, keydown_w}, {65, keydown_a}, {83, keydown_s}, {68, keydown_d},
        {32, keydown_space},
    };
    int nKey = sizeof(keys) / sizeof(int) / 2;

    if (pressed)
    {
        for (int i = 0; i < nKey; i++)
        {
            if (key == keys[i][0])
            {
                keydown |= keys[i][1];
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < nKey; i++)
        {
            if (key == keys[i][0])
            {
                keydown &= ~keys[i][1];
                break;
            }
        }
    }
}

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
        ctrlKey(true, keystat, wParam);
        ctrlKey(true, keydown, wParam);
        break;
    case WM_KEYUP:
        printf("WM_KEYUP %d\n", wParam);
        ctrlKey(false, keystat, wParam);
        break;

    case WM_LBUTTONDOWN:
        //printf("WM_LBUTTONDOWN %d (%d, %d)\n", wParam, LOWORD(lParam), HIWORD(lParam));
        keyApp(iKeyStatBegan, convertCoord(LOWORD(lParam), HIWORD(lParam)));
        mouseMoving = true;
        break;
    case WM_LBUTTONUP:
        //printf("WM_LBUTTONDOWN %d (%d, %d)\n", wParam, LOWORD(lParam), HIWORD(lParam));
        keyApp(iKeyStatEnded, convertCoord(LOWORD(lParam), HIWORD(lParam)));
        mouseMoving = false;
        break;
    case WM_MOUSEMOVE:
        //if(mouseMoving)
        keyApp(iKeyStatMoved, convertCoord(LOWORD(lParam), HIWORD(lParam)));
        break;
    case WM_MOVE:
        //printf("WM_MOVE\n");
        if (runApp)
            drawApp(iFPS::instance()->update());
        break;
    case WM_SIZING:
    {
        //printf("WM_SIZING\n");
        // RECT& rect = (RECT)(lParam); 과 똑같음
        RECT* rect = reinterpret_cast<LPRECT>(lParam);
        // 윈도우의 영역을
        printf("WM_SIZING(%d, %d)\n", 
            rect->right - rect->left,
            rect->bottom - rect->top);
    }
        break;
    case WM_SIZE:
        // window 창의 변경된 크기를 알 수 있음
        // 클라이언트의 영역을
        printf("WM_SIZE(%d, %d)\n", LOWORD(lParam), HIWORD(lParam));
        // viewport
        resize(LOWORD(lParam), HIWORD(lParam));
        if (runApp)
            drawApp(iFPS::instance()->update());
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        printf("WM_DESTROY\n");
        PostQuitMessage(0);
        delete iFPS::instance();
        break;
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
