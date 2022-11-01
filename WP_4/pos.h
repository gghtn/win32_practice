#pragma once

/*
	pen		    line	  ellipse		rect	   roundrect

	solid		dash	    dot		dashdot		dashdotdot

	horizontal	vertical  Cross		Dcross		 Bd    FD	solid
*/
//도구 좌표
enum {
	PEN_X1 = 30,
	LINE_X1 = 90,
	ELLIPSE_X1 = 150,
	RECT_X1 = 210,
	ROUNDRECT_X1 = 270,

	TOOL_Y1 = 500,


	PEN_X2 = 80,
	LINE_X2 = 140,
	ELLIPSE_X2 = 200,
	RECT_X2 = 260,
	ROUNDRECT_X2 = 320,

	TOOL_Y2 = 550,

};

enum {

	SOLID_X1 = 35,
	DAHS_X1 = 95,
	DOT_X1 = 155,
	DASHDOT_X1 = 215,
	DASHDOTDOT_X1 = 275,

	PENSTYLE_Y1 = 575,

	SOLID_X2 = 75,
	DAHS_X2 = 135,
	DOT_X2 = 195,
	DASHDOT_X2 = 255,
	DASHDOTDOT_X2 = 315,

	PENSTYLE_Y2 = 615
};

// x좌표 도구의 x좌표공유
enum {
	BUSH_Y1 = 640,
	BUSH_Y2 = 690
};

enum {
	TEXT_X = 350,

	TEXT1_Y = 515,
	TEXT2_Y = 555,
	TEXT3_Y = 595
};

//지우개 위치
enum {
	ERASER_X1 = 820,
	ERASER_X2 = 880,

	FULL_X1 = 920,
	FULL_X2 = 980,

	ERASER_Y1 = 640,
	ERASER_Y2 = 700
};

enum {
	//drawMode
	NOT = -1,
	PEN = 1,
	LINE,
	ELLIPSE,
	RT,
	RRT,
	TEXT,
	ERASER,

	//텍스트
	NORMAL = 1,
	TRANSP,
	OPACITY
};