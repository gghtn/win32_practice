#include "info.h"
#include "framework.h"
#include "WP_4.h"
#pragma warning(disable:4996)

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
int g_nWidth, g_nHeight;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WP4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WP4));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg); //WM_CHAR 사용하기 위해서 필요
            DispatchMessage(&msg);
        }
        else {

        }
    }

    return (int)msg.wParam;
}

//윈도우 등록
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WP4));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
//윈도우 생성
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 1300, 750, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HPEN myPen, oldPen;
    HBRUSH myBrush, oldBrush;
    COLORREF penColor = 0, brushColor = 0;
    TEXTMETRIC tm;
    RECT textRect, posRect;

    char text1[] = "기본 텍스트", text2[] = "투명 텍스트", text3[] = "불투명 텍스트";
    //WM_PRINT의 x,y, 텍스트의 두께, 그리기모드, 두께
    static int x = 30, y = 500, tW1, tW2, tW3, cyChar = 0, drawMode = NOT, width = 1;

    static char string[201];       //문자열
    static int len = 0, reStrX;    //문자열 길이, 길이가 200을 넘을 경우 새로 그릴 x좌표

    static int penStyle = PS_SOLID, brushStyle = 6, textStyle = -1;
    static bool bDrawing = false; 
    static short x1, y1, x2, y2;

    switch (message)
    {
    case WM_CREATE:

        hdc = GetDC(hWnd);
        GetTextMetrics(hdc, &tm);
        cyChar = tm.tmHeight;     //폰트의 높이 static변수
        

        GetTextExtentPoint32(hdc, text1, strlen(text1), (LPSIZE)&tW1);
        GetTextExtentPoint32(hdc, text2, strlen(text2), (LPSIZE)&tW2);
        GetTextExtentPoint32(hdc, text3, strlen(text3), (LPSIZE)&tW3);

        ReleaseDC(hWnd, hdc);

        g_nWidth = LOWORD(lParam);
        g_nHeight = HIWORD(lParam);
        break;
    case WM_SIZE:
        g_nWidth = LOWORD(lParam);
        g_nHeight = HIWORD(lParam);
        break;

    case WM_PAINT:
    {
        int tmpX;
        HDC hdc = BeginPaint(hWnd, &ps);
        MoveToEx(hdc, 0, 490, NULL);
        LineTo(hdc, g_nWidth, 490);
        //펜, 직선, 원, 사각형, 라운드사각형 그리기
        for (int i = 0; i < 5; i++) {
            tmpX = x + i * 60;
            Rectangle(hdc, tmpX, y, tmpX + 50, y + 50);

            SetPen(hdc, myPen, oldPen, i);
            MoveToEx(hdc, tmpX + 5, y + 75, NULL);
            LineTo(hdc, tmpX + 45, y + 115);
            SelectObject(hdc, oldPen);
            DeleteObject(myPen);

            switch (i) {
            case 0:
                MoveToEx(hdc, tmpX + 5, y + 5, NULL);
                LineTo(hdc, tmpX + 30, y + 10);
                LineTo(hdc, tmpX + 10, y + 30);
                LineTo(hdc, tmpX + 45, y + 45);
                break;
            case 1:
                MoveToEx(hdc, tmpX + 5, y + 5, NULL);
                LineTo(hdc, tmpX + 45, y + 45);
                break;
            case 2:
                Ellipse(hdc, tmpX + 10, y + 10, tmpX + 40, y + 40);
                break;
            case 3:
                Rectangle(hdc, tmpX + 10, y + 10, tmpX + 40, y + 40);
                break;
            case 4:
                RoundRect(hdc, tmpX + 10, y + 10, tmpX + 40, y + 40, 10, 10);
                break;
            }
        }
        //해쉬 그리기
        for (int i = 0; i < 7; i++) {
            tmpX = x + i * 60;
            SetBrush(hdc, myBrush, oldBrush, i, 0);
            Ellipse(hdc, tmpX, 640, tmpX + 50, 690);
        }
        // 텍스트 그리기
        {
            textRect = { 350, y + 15, 350 + tW1, y + 15 + cyChar };
            DrawText(hdc, text1, strlen(text1), &textRect, DT_CENTER);

            textRect = { 350, y + 55, 350 + tW2, y + 55 + cyChar };
            DrawText(hdc, text2, strlen(text2), &textRect, DT_CENTER);

            textRect = { 350, y + 95, 350 + tW3, y + 95 + cyChar };
            DrawText(hdc, text3, strlen(text3), &textRect, DT_CENTER);
        }
        // 미리보기 박스
        if (drawMode) {
            Box(hWnd);      //미리보기 영역 흰색으로 초기화
            SetPen(hdc, myPen, oldPen, penStyle, width, penColor);
            SetBrush(hdc, myBrush, oldBrush, brushStyle, brushColor);
            switch (drawMode) {
            case PEN:
                MoveToEx(hdc, 550, 550, NULL);
                LineTo(hdc, 650, 600);
                LineTo(hdc, 600, 650);
                LineTo(hdc, 700, 650);
                break;
            case LINE:
                MoveToEx(hdc, 550, 550, NULL);
                LineTo(hdc, 700, 650);
                break;
            case ELLIPSE:
                Ellipse(hdc, 550, 520, 700, 670);
                break;
            case RT:
                Rectangle(hdc, 550, 520, 700, 670);
                break;
            case RRT:
                RoundRect(hdc, 550, 520, 700, 670, 10, 10);
                break;
            case TEXT:
                SetTextColor(hdc, penColor);
                switch (textStyle) {
                case NORMAL:
                    SetBkColor(hdc, RGB(255, 255, 255));
                    TextOut(hdc, 550, 550, text1, strlen(text1));
                    break;
                case TRANSP:
                    SetBkMode(hdc, TRANSPARENT);
                    TextOut(hdc, 550, 550, text2, strlen(text2));
                    break;
                case OPACITY:
                    SetBkColor(hdc, brushColor);
                    TextOut(hdc, 550, 550, text2, strlen(text2));
                    break;
                }
                break;
            }
            //?
            SelectObject(hdc, oldPen);
            SelectObject(hdc, oldBrush);
            DeleteObject(myPen);
            DeleteObject(myBrush);
        }
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN:
        x1 = x2 = LOWORD(lParam);
        y1 = y2 = HIWORD(lParam);

        if (y1 < 490) { //그리기 영역을 클릭한경우
            if (drawMode == PEN || drawMode == TEXT) {
                SetCursor(LoadCursor(NULL, IDC_ARROW));
            }
            else {
                SetCursor(LoadCursor(NULL, IDC_CROSS));
            }
            if (drawMode != TEXT) {
                SetCapture(hWnd);
                bDrawing = true;
            }
            else {
                memset(string, '\0', 200);  // TEXT모드일 때 그리기 영역 클릭시 string 초기화 
                len = 0;
            }
        }
        else {          //도구 영역을 클릭한 경우
            int tmp;

            while (1) {
                //도구
                tmp = SelectTool(x1, y1);
                if (tmp != NOT) {
                    drawMode = tmp;
                    break;
                }
                //펜 스타일
                tmp = SelectPenStyle(x1, y1);
                if (tmp != NOT) {
                    penStyle = tmp;
                    break;
                }
                //브러쉬 스타일
                tmp = SelectBrushStyle(drawMode, x1, y1);
                if (tmp != NOT) {
                    brushStyle = tmp;
                    break;
                }
                //텍스트 스타일
                tmp = SelectText(x1, y1, cyChar, tW1, tW2, tW3);
                if (tmp != NOT) {
                    drawMode = TEXT;
                    textStyle = tmp;
                    break;
                }
                break;
            }
            InvalidateRect(hWnd, NULL, false);  //무효화 WM_PAINT실행
        }
        break;

    case WM_MOUSEMOVE:
        if (bDrawing) {
            hdc = GetDC(hWnd);
            SetPen(hdc, myPen, oldPen, penStyle, width, penColor);
            SetBrush(hdc, myBrush, oldBrush, brushStyle, brushColor);

            if (drawMode == PEN) {    // 펜으로 그리기일 경우
                if(GetCapture() == hWnd){
                    MoveToEx(hdc, x1, y1, NULL);
                    x1 = LOWORD(lParam);
                    y1 = HIWORD(lParam);
                    if (y1 > 490) y1 = 490;
                    LineTo(hdc, x1, y1);
                }
            }
            else {
                posRect = {x1, y1, x2, y2};
                DrawFigure(hdc, drawMode, posRect, false);

                x2 = LOWORD(lParam);
                y2 = HIWORD(lParam);
                if (y2 > 490) y2 = 490;

                posRect = { x1, y1, x2, y2 };
                DrawFigure(hdc, drawMode, posRect, false);
            }
            ReleaseDC(hWnd, hdc);
        }
        
        break;

    case WM_LBUTTONUP:
        if (bDrawing) {
            hdc = GetDC(hWnd);
            SetPen(hdc, myPen, oldPen, penStyle, width, penColor);
            SetBrush(hdc, myBrush, oldBrush, brushStyle, brushColor);

            posRect = { x1, y1, x2, y2 };
            ReleaseCapture();
            bDrawing = false;               
            DrawFigure(hdc, drawMode, posRect, true);

            ReleaseDC(hWnd, hdc);
        }
        break;

    case WM_CHAR:
        if (drawMode == TEXT && y1 < 490) { //텍스트모드이고 클라이언트 영역을 클릭했을시
            hdc = GetDC(hWnd);

            SetTextColor(hdc, penColor);

            switch (textStyle) {
            case NORMAL:
                SetBkColor(hdc, RGB(255,255,255));
                break;
            case TRANSP:
                SetBkMode(hdc, TRANSPARENT);
                break;
            case OPACITY:
                SetBkColor(hdc, brushColor);
                break;
            }

            if (wParam == 8) {
                TextOut(hdc, x1, y1, string, len);
                string[len] = '\0';
                len--;
            }
            else if (wParam == 13) {

            }
            else string[len++] = (char)wParam;

            if (y1 > 490 - cyChar) y1 = 490 - cyChar;   //도구창 침범 방지         
            TextOut(hdc, x1, y1, string, len);
            
            if (len == 200) {
                GetTextExtentPoint32(hdc, string, 200, (LPSIZE)&reStrX);
                x1 = x1 + reStrX;
                memset(string, '\0', 200);
                len = 0;
            }
           

            ReleaseDC(hWnd, hdc);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

