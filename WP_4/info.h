#pragma once
#include <Windows.h>
#include "pos.h"

//펜 생성, 스타일 지정, 두께 색상 지정
void SetPen(HDC hdc, HPEN& myPen, HPEN& oldPen, int style, int width = 1, COLORREF color = 0);
// solidBrush 생성
void SetBrush(HDC hdc, HBRUSH& myBrush, HBRUSH& oldBrush, int style, COLORREF color = 0);


// 도구 선택
int SelectTool(int x, int y);
// 펜 스타일 선택
int SelectPenStyle(int x, int y);
// 브러시 스타일 선택
int SelectBrushStyle(int mode, int x, int y);
// 텍스트 선택
int SelectText(int x, int y, int cyChar, int tW1, int tW2, int tW3);
//전체 지우기
bool SelectFull(int x, int y);



//도형 그리기
void DrawFigure(HDC hdc, int mode, RECT r, bool bFill);
//미리보기 박스
void Box(HWND hWnd);