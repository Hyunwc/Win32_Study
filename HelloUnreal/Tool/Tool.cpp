#include "Tool.h"

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
bool run;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    WCHAR szWindowClass[20] = L"Tool";           
    WCHAR szTitle[20] = L"Tool";                 
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOL));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TOOL);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TOOL));

    run = true;
    loadApp(hWnd);

    MSG msg;

    // 기본 메시지 루프입니다:
    while (run)
    {
        if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // draw
            drawApp(0.0f);
        }
    }

    freeApp();

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DROPFILES:
    {
        HDROP hDrop = (HDROP)wParam;

        wchar_t wPath[512];
        DragQueryFile(hDrop, 0, wPath, 512);

        char* path = utf16_to_utf8(wPath);
        printf("path = %s\n", path);
        delete path;

        break;
    }
    case WM_COMMAND:
        {
        updateCtrl(wParam);

        int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_CLOSE:
        if (MessageBox(NULL, L"할까요?", L"종료?", MB_YESNO) == IDYES)
        {
            run = false;
        }
        return 0; //break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
   //default:
   //    return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


HWND hBtn, hBtn2, hCheck, hCombo, hList;
void methodBtn(HWND hwnd);
void methodCombo(HWND hwnd);
void methodList(HWND hwnd);
HWND* hRadio;
void methodRadio(HWND hwnd);
void methodEdit(HWND hwnd);
void methodEditMulti(HWND hwnd);

void loadApp(HWND hwnd)
{
    hWnd = hwnd;
    DragAcceptFiles(hwnd, true);

    loadCtrl();

    createWndStatic(30, 30, 100, 25, methodBtn, "테스트");

    hBtn = createWndButton(30, 60, 100, 25, methodBtn, "푸시버튼");
    hBtn2 = createWndButton(170, 60, 100, 25, methodBtn, "푸시버튼");

    hCheck = createWndCheckBox(30, 90, 100, 25, methodBtn, "체크박스");

    const char* str[10] = { "임수아", "김성민", "조상현", "정우주",
    "송지호", "김재학", "함동희", "김태현", "진정우", "최병철" };
    hCombo = createWndComboBox(30, 120, 100, 300, methodCombo, str, 10);
    hList = createWndListBox(230, 120, 100, 100, methodList, str, 10);

    hRadio = new HWND[5];
    const char* strSatis[5] = { "아주만족", "만족", "보통", "불만족", "아주불만족"};
    for (int i = 0; i < 5; i++)
    {
        hRadio[i] = createWndRadio(30, 230 + 27 * i,
            120, 25, methodRadio, strSatis[i]);
    }

    createWndEdit(300, 60, 100, 22, WndEditInt, methodEdit, "1234");
    createWndEditMultiline(400, 60, 100, 150,  methodEditMulti, "1234");
}

void methodBtn(HWND hwnd)
{
    HWND h[3] = { hBtn, hBtn2, hCheck };
    for (int i = 0; i < 3; i++)
    {
        if(h[i] == hwnd)
            printf("hwnd[%d] = %d\n", i);
    }

    if (hwnd == hBtn)
    {
        const char* path = 
        openFileDlg(true, ITEM_FILTER);
        printf("path[%s]\n", path);
    }
    else if (hwnd == hBtn2)
    {
        const char* path =
        openFileDlg(false, ITEM_FILTER);

        printf("path[%s]\n", path);
    }
}

void methodCombo(HWND hwnd)
{
    printf("combo = %d\n", indexWndComboBox(hwnd));
}

void methodList(HWND hwnd)
{
    printf("list = %d\n", indexWndListBox(hwnd));
}

void methodRadio(HWND hwnd)
{
    for (int i = 0; i < 5; i++)
    {
        setWndRadio(hRadio[i], hRadio[i] == hwnd);
    }
}

void methodEdit(HWND hwnd)
{
    char* str = getWndText(hwnd);
    printf("edit[%s]\n", str);
    delete str;
}

void methodEditMulti(HWND hwnd)
{
    char* str = getWndText(hwnd);
    printf("edit[%s]\n", str);
    delete str;
}

void freeApp()
{
    freeCtrl();
}

void drawApp(float dt)
{

}