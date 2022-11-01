#pragma once
#include <Windows.h>
#include "pos.h"

//�� ����, ��Ÿ�� ����, �β� ���� ����
void SetPen(HDC hdc, HPEN& myPen, HPEN& oldPen, int style, int width = 1, COLORREF color = 0);
// solidBrush ����
void SetBrush(HDC hdc, HBRUSH& myBrush, HBRUSH& oldBrush, int style, COLORREF color = 0);


// ���� ����
int SelectTool(int x, int y);
// �� ��Ÿ�� ����
int SelectPenStyle(int x, int y);
// �귯�� ��Ÿ�� ����
int SelectBrushStyle(int mode, int x, int y);
// �ؽ�Ʈ ����
int SelectText(int x, int y, int cyChar, int tW1, int tW2, int tW3);
//��ü �����
bool SelectFull(int x, int y);



//���� �׸���
void DrawFigure(HDC hdc, int mode, RECT r, bool bFill);
//�̸����� �ڽ�
void Box(HWND hWnd);