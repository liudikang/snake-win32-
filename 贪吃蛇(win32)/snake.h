#ifndef SNAKE_H 
#define SNAKE_H 1
#include<Windows.h>
#include<stdio.h>
#include<tchar.h>  

#include"resource.h"

#define STARTPOS 2  //初始偏移量
#define BITSIZE 30  //位图边长
#define GOUP 1       // R - D == U - L
#define GODOWN -1    // R - U == D - L
#define GOLEFT -2    // L - D == U - R
#define GORIGHT 2    // L - D == D - R      //之所以这么设置数值是为了方便计算转弯

#define RD 1
#define RU 3
#define LD -3
#define LU -1

#define DEAD 0
#define LIVE 1
#define PAUSE 2
typedef struct SNAKE{
	COORD position;	  //从{0,0}开始
	short direction;
	struct SNAKE *pNext;
}Snake;


HBITMAP hBM_back,							  //   "she\\背景.bmp"
		hBM_Apple,							  //   "she\\苹果.bmp"
	    hBM_bodyH,			                  //   "she\\蛇身0.bmp"
		hBM_bodyV,			                  //   "she\\蛇身1.bmp"
		hBM_headRight,						  //   "she\\蛇头0.bmp"
		hBM_headUp,							  //   "she\\蛇头1.bmp"
		hBM_headLeft,						  //   "she\\蛇头2.bmp"
		hBM_headDown,						  //   "she\\蛇头3.bmp"
		hBM_turnRU,							  //   "she\\转弯1.bmp"
		hBM_turnLU,							  //   "she\\转弯2.bmp"
	    hBM_turnLD,						      //   "she\\转弯3.bmp"
	    hBM_turnRD;							  //   "she\\转弯4.bmp"  


LRESULT CALLBACK myPro(HWND, UINT, WPARAM, LPARAM);
void initWindows(HINSTANCE hInstance,int nCmdShow);
int CDECL MessageBoxPrintf (HWND hwnd,TCHAR * szCaption, TCHAR * szFormat, ...);
void  PosBlt(HDC hdcDest, HDC hdcSrc, HBITMAP body,COORD p);
void drawSnake(HWND hwnd,Snake *head);

void initGame(HWND hwnd);

void addBody(Snake **head, Snake **tail);
void movePosition(Snake *head);
void moveSnake(Snake *head);
void deleteSnake(Snake **head,Snake **tail);

int isDead(COORD pos);
void GoAhead(HWND hwnd);
void giveApple();
void drawApple(HWND hwnd);
int ifEat(COORD pos);
#endif