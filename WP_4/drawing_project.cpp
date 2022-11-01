#include "info.h"
#include "framework.h"
#include "drawing_project.h"
#pragma warning(disable:4996)

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

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


HWND hScroll[7];                        //펜RGB, 브러시RGB, 두께
HINSTANCE g_hInst;                      //스크롤바 윈도우 인스턴트핸들
int nScroll[7];                         //펜RGB 브러시RGB 좌표, 두께 nScroll[6] => width
int g_nWidth, g_nHeight;                //클라이언트 창 너비, 높이

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HPEN myPen, oldPen;
    HBRUSH myBrush, oldBrush;
    static COLORREF penColor = 0, brushColor = 0;   //정적변수가 아닐시 루프를 돌면서 계속 실행됨 -> 0으로 값이 고정
    TEXTMETRIC tm;
    RECT tmpRect, posRect;

    char text1[] = "기본 텍스트", text2[] = "투명 텍스트", text3[] = "불투명 텍스트";
    //WM_PRINT의 x,y, 텍스트의 두께, 그리기모드, 두께
    static int x = 30, y = 500, tW1, tW2, tW3, cyChar = 0, drawMode = NOT;

    static char string[201];       //문자열
    static int len = 0, reStrX;    //문자열 길이, 길이가 200을 넘을 경우 새로 그릴 x좌표

    static int penStyle = PS_SOLID, brushStyle = 6, textStyle = -1, width;
    static bool bDrawing = false, bWidth = false, bFull = false;  //드로잉 영역을 클릭했는지, 두께 스크롤 창을 클릭했는지, 전체지우기인지
    static short x1, y1, x2, y2;
    int tmpPos; //

    switch (message)
    {
    case WM_CREATE:
        for (int i = 0; i < 3; i++) {   //펜 스크롤
            hScroll[i] = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 800, 500 + i*40, 200, 20, hWnd, NULL, g_hInst, NULL);
            SetScrollRange(hScroll[i], SB_CTL, 0, 255, TRUE);
            SetScrollPos(hScroll[i], SB_CTL, 0, TRUE);
        }
        for (int i = 0; i < 3; i++) {   //브러쉬 스크롤
            hScroll[i+3] = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 1050, 500 + i * 40, 200, 20, hWnd, NULL, g_hInst, NULL);
            SetScrollRange(hScroll[i+3], SB_CTL, 0, 255, TRUE);
            SetScrollPos(hScroll[i+3], SB_CTL, 0, TRUE);
        }
        //두께 스크롤
        hScroll[6] = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 1050, 640, 200, 20, hWnd, NULL, g_hInst, NULL);
        SetScrollRange(hScroll[6], SB_CTL, 1, 30, TRUE);
        SetScrollPos(hScroll[6], SB_CTL, 1, TRUE);

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
        //스크롤바 텍스트, 지우개 그리기
        {
            tmpRect = { 800, 610, 1000, 630 };
            DrawText(hdc, "펜RGB", 5, &tmpRect, DT_CENTER);
            tmpRect = { 1050, 610, 1250, 630 };
            DrawText(hdc, "브러쉬RGB", 9, &tmpRect, DT_CENTER);
            tmpRect = { 1050, 670, 1250, 690 };
            DrawText(hdc, "펜 굵기", 7, &tmpRect, DT_CENTER);

            tmpRect = { ERASER_X1, ERASER_Y1, ERASER_X2, ERASER_Y2 };
            Rectangle(hdc, tmpRect.left, tmpRect.top, tmpRect.right, tmpRect.bottom);
            DrawText(hdc, "\n\n지우개", 8, &tmpRect, DT_CENTER | DT_WORDBREAK);  //센터정렬, 오른쪽 끝에서 개행

            tmpRect = { FULL_X1, ERASER_Y1, FULL_X2, ERASER_Y2 };
            Rectangle(hdc, tmpRect.left, tmpRect.top, tmpRect.right, tmpRect.bottom);
            DrawText(hdc, "\n전체\n지우개", 12, &tmpRect, DT_CENTER | DT_WORDBREAK);

        }
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
            SelectObject(hdc, oldBrush);
            DeleteObject(myBrush);
        }
        // 텍스트 그리기
        {
            tmpRect = { 350, y + 15, 350 + tW1, y + 15 + cyChar };
            DrawText(hdc, text1, strlen(text1), &tmpRect, DT_CENTER);

            tmpRect = { 350, y + 55, 350 + tW2, y + 55 + cyChar };
            DrawText(hdc, text2, strlen(text2), &tmpRect, DT_CENTER);

            tmpRect = { 350, y + 95, 350 + tW3, y + 95 + cyChar };
            DrawText(hdc, text3, strlen(text3), &tmpRect, DT_CENTER);
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
            default:
                break;
            }
            //당장에 문제가 없어보여도 복구와 삭제는 반드시 한다. -> 계속 생성만되면 문제발생
            SelectObject(hdc, oldPen);
            SelectObject(hdc, oldBrush);
            DeleteObject(myPen);
            DeleteObject(myBrush);
        }

        //전체지우기 클릭시
        if (bFull) {    
            bFull = false;
            SetPen(hdc, myPen, oldPen, penStyle, 1, RGB(255, 255, 255));
            SetBrush(hdc, myBrush, oldBrush, 5, RGB(255, 255, 255));
            Rectangle(hdc, 0, 0, g_nWidth, 490);
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
            if (drawMode == PEN || drawMode == TEXT || drawMode == ERASER) {
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

                bFull = SelectFull(x1, y1);
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
                    if (y1 > 490 - width/2) y1 = 490 - width / 2;
                    LineTo(hdc, x1, y1);
                }
            }
            else if (drawMode == ERASER) {
                SelectObject(hdc, oldPen);
                DeleteObject(myPen);
                SetPen(hdc, myPen, oldPen, penStyle, width, RGB(255,255,255));
                if (GetCapture() == hWnd) {
                    MoveToEx(hdc, x1, y1, NULL);
                    x1 = LOWORD(lParam);
                    y1 = HIWORD(lParam);
                    if (y1 > 490 - width / 2) y1 = 490 - width / 2;
                    LineTo(hdc, x1, y1);
                }
            }
            else {
                posRect = {x1, y1, x2, y2};
                DrawFigure(hdc, drawMode, posRect, false);

                x2 = LOWORD(lParam);
                y2 = HIWORD(lParam);
                if (y2 > 490 - width / 2) y2 = 490 - width / 2;

                posRect = { x1, y1, x2, y2 };
                DrawFigure(hdc, drawMode, posRect, false);
            }
            SelectObject(hdc, oldPen);
            SelectObject(hdc, oldBrush);
            DeleteObject(myPen);
            DeleteObject(myBrush);

            ReleaseDC(hWnd, hdc);
        }
        
        break;

    case WM_LBUTTONUP:
        if (bDrawing) {
            hdc = GetDC(hWnd);
            if (drawMode == ERASER) {
                ReleaseCapture();
            }
            else {
                SetPen(hdc, myPen, oldPen, penStyle, width, penColor);
                SetBrush(hdc, myBrush, oldBrush, brushStyle, brushColor);

                posRect = { x1, y1, x2, y2 };
                ReleaseCapture();
                bDrawing = false;
                DrawFigure(hdc, drawMode, posRect, true);

                SelectObject(hdc, oldPen);
                SelectObject(hdc, oldBrush);
                DeleteObject(myPen);
                DeleteObject(myBrush);
            }
            
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

    case WM_HSCROLL:
        for (int i = 0; i < 7; i++) {
            if ((HWND)lParam == hScroll[i]) {       //클릭한 창이 스크롤 창일 경우 스크롤 창의 좌표를 구함
                tmpPos = nScroll[i];
                if (i == 6) bWidth = true;
                break;
            }
        }
        switch (LOWORD(wParam)) {           //wParam 하위 2바이트 = 눌린버튼
        case SB_LINELEFT:
            tmpPos = max(0, tmpPos - 1);
            break;
        case SB_LINERIGHT:
            if(bWidth)  tmpPos = min(30, tmpPos + 1);   //두께 스크롤 창을 클릭한 경우 최대크기가 50을 넘지 않음
            else tmpPos = min(255, tmpPos + 1);
            break;
        case SB_PAGELEFT:
            tmpPos = max(0, tmpPos - 10);
            break;
        case SB_PAGERIGHT:
            if (bWidth)  tmpPos = min(30, tmpPos + 10);
            else tmpPos = min(255, tmpPos + 10);
            break;
        case SB_THUMBTRACK:
            tmpPos = HIWORD(wParam);        //wParam 상위 2바이트 = thumb위치
            break;
        }
        for (int i = 0; i < 7; i++) {
            if ((HWND)lParam == hScroll[i]) {       //다시 클릭한 창을 구해 좌표 갱신
                nScroll[i] = tmpPos;
                break;
            }
        }

        penColor = RGB(nScroll[0], nScroll[1], nScroll[2]);
        brushColor = RGB(nScroll[3], nScroll[4], nScroll[5]);
        width = nScroll[6];
        bWidth = false;

        SetScrollPos((HWND)lParam, SB_CTL, tmpPos, TRUE);  //thumb의 위치 재설정
        InvalidateRect(hWnd, NULL, false);

        break;
   
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

