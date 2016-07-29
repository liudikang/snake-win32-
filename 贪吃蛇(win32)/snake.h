#ifndef SNAKE_H 
#define SNAKE_H 1
#include<Windows.h>
#include<stdio.h>
#include<tchar.h>  

#include"resource.h"

#define STARTPOS 2  //��ʼƫ����
#define BITSIZE 30  //λͼ�߳�
#define GOUP 1       // R - D == U - L
#define GODOWN -1    // R - U == D - L
#define GOLEFT -2    // L - D == U - R
#define GORIGHT 2    // L - D == D - R      //֮������ô������ֵ��Ϊ�˷������ת��

#define RD 1
#define RU 3
#define LD -3
#define LU -1

#define DEAD 0
#define LIVE 1
#define PAUSE 2
typedef struct SNAKE{
	COORD position;	  //��{0,0}��ʼ
	short direction;
	struct SNAKE *pNext;
}Snake;


HBITMAP hBM_back,							  //   "she\\����.bmp"
		hBM_Apple,							  //   "she\\ƻ��.bmp"
	    hBM_bodyH,			                  //   "she\\����0.bmp"
		hBM_bodyV,			                  //   "she\\����1.bmp"
		hBM_headRight,						  //   "she\\��ͷ0.bmp"
		hBM_headUp,							  //   "she\\��ͷ1.bmp"
		hBM_headLeft,						  //   "she\\��ͷ2.bmp"
		hBM_headDown,						  //   "she\\��ͷ3.bmp"
		hBM_turnRU,							  //   "she\\ת��1.bmp"
		hBM_turnLU,							  //   "she\\ת��2.bmp"
	    hBM_turnLD,						      //   "she\\ת��3.bmp"
	    hBM_turnRD;							  //   "she\\ת��4.bmp"  


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