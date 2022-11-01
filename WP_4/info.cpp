#include "info.h"

// 펜 스타일 지정함수,기존 myPen 삭제 myPen생성
void SetPen(HDC hdc, HPEN& myPen, HPEN& oldPen, int style, int width, COLORREF color) {
	myPen = CreatePen(style, width, color);
	oldPen = (HPEN)SelectObject(hdc, myPen);
}

//브러시
void SetBrush(HDC hdc, HBRUSH& myBrush, HBRUSH& oldBrush, int style, COLORREF color) {
	if (style > 5) {
		myBrush = CreateSolidBrush(color);
		oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
	}
	else {
		myBrush = CreateHatchBrush(style, color);
		oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
	}
}



//도구 선택
int SelectTool(int x, int y) {
	if (TOOL_Y1 < y && y < TOOL_Y2) {
		if (PEN_X1 < x && x < PEN_X2) return PEN;
		else if (LINE_X1 < x && x < LINE_X2) return LINE;
		else if (ELLIPSE_X1 < x && x < ELLIPSE_X2) return ELLIPSE;
		else if (RECT_X1 < x && x < RECT_X2) return RT;
		else if (ROUNDRECT_X1 < x && x < ROUNDRECT_X2) return RRT;
	}
	//지우개
	if (ERASER_Y1 < y && y < ERASER_Y2) {
		if (ERASER_X1 < x && x < ERASER_X2) return ERASER;
	}
	return NOT;
}
//펜 스타일 선택
int SelectPenStyle(int x, int y) {
	if (PENSTYLE_Y1 < y && y < PENSTYLE_Y2) {
		if (SOLID_X1 < x && x < SOLID_X2) return 0;
		else if (DAHS_X1 < x && x < DAHS_X2) return 1;
		else if (DOT_X1 < x && x < DOT_X2) return 2;
		else if (DASHDOT_X1 < x && x < DASHDOT_X2) return 3;
		else if (DASHDOTDOT_X1 < x && x < DASHDOTDOT_X2) return 4;
	}
	return NOT;
}
//브러쉬 스타일 선택
int SelectBrushStyle(int mode, int x, int y) {
	if (mode > LINE && BUSH_Y1 < y && y < BUSH_Y2) {
		if (PEN_X1 < x && x < PEN_X2) return 0;
		else if (LINE_X1 < x && x < LINE_X2) return 1;
		else if (ELLIPSE_X1 < x && x < ELLIPSE_X2) return 2;
		else if (RECT_X1 < x && x < RECT_X2) return 3;
		else if (ROUNDRECT_X1 < x && x < ROUNDRECT_X2) return 4;
		else if (ROUNDRECT_X1 + 60 < x && x < ROUNDRECT_X2 + 60) return 5;
		else if (ROUNDRECT_X1 + 120 < x && x < ROUNDRECT_X2 + 120) return 6;
	}
	return NOT;
}
// 텍스트 선택
int SelectText(int x, int y, int cyChar, int tW1, int tW2, int tW3) {
	if (TEXT_X < x && x < TEXT_X + tW1 && TEXT1_Y < y && y <TEXT1_Y + cyChar) {
		return NORMAL;
	}
	else if (TEXT_X < x && x < TEXT_X + tW2 && TEXT2_Y < y && y < TEXT2_Y + cyChar) {
		return TRANSP;
	}
	else if (TEXT_X < x && x < TEXT_X + tW3 && TEXT3_Y < y && y < TEXT3_Y + cyChar) {
		return OPACITY;
	}
	return NOT;
}
//전체 지우기
bool SelectFull(int x, int y) {
	if (ERASER_Y1 < y && y < ERASER_Y2) {
		if (FULL_X1 < x && x < FULL_X2) return true;
	}
	return false;
}




// 도형 그리기
void DrawFigure(HDC hdc, int mode, RECT r, bool bFill) {
	if(!bFill) SetROP2(hdc, R2_NOT);

	switch (mode) {
	case LINE:
		MoveToEx(hdc, r.left, r.top, NULL);
		LineTo(hdc, r.right, r.bottom);
		break;
	case ELLIPSE:
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		break;
	case RT:
		Rectangle(hdc, r.left, r.top, r.right, r.bottom);
		break;
	case RRT:
		RoundRect(hdc, r.left, r.top, r.right, r.bottom, 10, 10);
		break;
	}
}

// 미리보기 박스 초기화
void Box(HWND hWnd) {	//기존의 펜, 브러쉬에 영향을 주고싶지 않을 때 hWnd로 hdc생성
	HPEN mp, op;
	HBRUSH mb, ob;

	HDC hdc = GetDC(hWnd);

	mp = (HPEN)GetStockObject(WHITE_PEN);
	SelectObject(hdc, mp);
	mb = (HBRUSH)GetStockObject(WHITE_BRUSH);
	SelectObject(hdc, mb);
	Rectangle(hdc, 450, 500, 800, 700);

	ReleaseDC(hWnd, hdc);
}