#include "info.h"

// �� ��Ÿ�� �����Լ�,���� myPen ���� myPen����
void SetPen(HDC hdc, HPEN& myPen, HPEN& oldPen, int style, int width, COLORREF color) {
	myPen = CreatePen(style, width, color);
	oldPen = (HPEN)SelectObject(hdc, myPen);
}

//�귯��
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



//���� ����
int SelectTool(int x, int y) {
	if (TOOL_Y1 < y && y < TOOL_Y2) {
		if (PEN_X1 < x && x < PEN_X2) return PEN;
		else if (LINE_X1 < x && x < LINE_X2) return LINE;
		else if (ELLIPSE_X1 < x && x < ELLIPSE_X2) return ELLIPSE;
		else if (RECT_X1 < x && x < RECT_X2) return RT;
		else if (ROUNDRECT_X1 < x && x < ROUNDRECT_X2) return RRT;
	}
	//���찳
	if (ERASER_Y1 < y && y < ERASER_Y2) {
		if (ERASER_X1 < x && x < ERASER_X2) return ERASER;
	}
	return NOT;
}
//�� ��Ÿ�� ����
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
//�귯�� ��Ÿ�� ����
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
// �ؽ�Ʈ ����
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
//��ü �����
bool SelectFull(int x, int y) {
	if (ERASER_Y1 < y && y < ERASER_Y2) {
		if (FULL_X1 < x && x < FULL_X2) return true;
	}
	return false;
}




// ���� �׸���
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

// �̸����� �ڽ� �ʱ�ȭ
void Box(HWND hWnd) {	//������ ��, �귯���� ������ �ְ���� ���� �� hWnd�� hdc����
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