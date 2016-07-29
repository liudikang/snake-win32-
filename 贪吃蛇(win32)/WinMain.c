#include"snake.h"
#include<time.h>
Snake *snake_head=NULL,*snake_tail=NULL;
int Pool[18][18]={0};  //记录身体（不包含头）的位置
COORD Apple_pos;
HWND Hwnd;
int state=1;
int score=0;

int CALLBACK WinMain( HINSTANCE hInstance,
					  HINSTANCE hPrevInstance,
					  LPSTR lpCmdLine,
					  int nCmdShow)
{
	srand((unsigned int)time(0));
	initWindows(hInstance,nCmdShow);
	return 0;
}

											
void initWindows(HINSTANCE hInstance,int nCmdShow)
{
	MSG msg;
	HWND windows;
	char *className="new_Windows";
	//设计
	WNDCLASSEX WND;
	WND.cbClsExtra = 0;
	WND.cbWndExtra = 0;
	WND.cbSize = sizeof(WNDCLASSEX);
	WND.style = CS_HREDRAW | CS_VREDRAW;
	WND.hInstance = hInstance;
	WND.hCursor = LoadCursor (NULL, IDC_ARROW);
	WND.hIcon = LoadIcon(NULL,IDI_INFORMATION);
	WND.hIconSm = NULL;
	WND.hbrBackground = CreateSolidBrush(RGB(171,171,226));//试试其他的几种
	WND.lpszMenuName = NULL;
	WND.lpszClassName = className;
	WND.lpfnWndProc = myPro;//回调函数
	//加载资源文件

	hBM_back	  = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1)) ;
	hBM_Apple	  = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2)) ;
	hBM_bodyH	  = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3)) ;
	hBM_bodyV	  = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4)) ;
	hBM_headRight = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP5)) ;
	hBM_headUp	  = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP6)) ;
	hBM_headLeft  = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP7)) ;
	hBM_headDown  = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP8)) ;
	hBM_turnRU	  = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP9)) ;
	hBM_turnLU	  = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP10)) ;
	hBM_turnLD	  = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP11)) ;
	hBM_turnRD    = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP12)) ;

	
	//注册		  
	RegisterClassEx(&WND);
	//创建窗口
	windows=CreateWindow(className, "Snake", 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX ,
		100, 100, 620, 642, NULL, NULL, hInstance, NULL); 
	 
	//显示窗口
	ShowWindow(windows,nCmdShow);
	//消息循环
	while(GetMessage(&msg,NULL,0,0))//当消息为0是  取到的是结束程序的消息
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK myPro(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HDC temp_hdc;
	PAINTSTRUCT ps;
	COORD p={1,1};
	static int time=1;
	//HBRUSH newBrush;
	//HPEN newPen = NULL;
	//HGDIOBJ oldBrush,oldPen;
	static int x,y;
	Hwnd=hwnd;
	switch (message)
	{
	case WM_CREATE:
		initGame(hwnd);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'w':
		case 'W':
		case VK_UP:
			if(snake_head->direction != GODOWN && state ==LIVE)
			{
				snake_head->direction = GOUP;
				GoAhead(hwnd);
			}

			break;
		case 'a':
		case 'A':
		case VK_LEFT:
			if(snake_head->direction != GORIGHT && state ==LIVE)
			{
				snake_head->direction = GOLEFT;
				GoAhead(hwnd);
			}
			break;
		case 's':
		case 'S':
		case VK_DOWN:
			if(snake_head->direction != GOUP && state ==LIVE)
			{
				snake_head->direction = GODOWN;
				GoAhead(hwnd);
			}
			break;
		case 'd':
		case 'D':
		case VK_RIGHT:
			if(snake_head->direction != GOLEFT && state ==LIVE)
			{
				snake_head->direction = GORIGHT;
				GoAhead(hwnd);
			}
			break;
		case VK_F2:
			initGame(hwnd);
			break;
		case VK_RETURN:
			if(state == LIVE)
			{
				state = PAUSE;
				KillTimer(hwnd,1);
			}else if(state == PAUSE)
			{
				state = LIVE;
				SetTimer(hwnd, 1, 1000, 0);
			}

		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint (hwnd, &ps) ;
		temp_hdc = CreateCompatibleDC(hdc);//创建之后一定要删除

		SelectObject(temp_hdc, hBM_back);
		BitBlt(hdc, STARTPOS , STARTPOS, 600, 600, temp_hdc, 0, 0, SRCCOPY);//设置背景

		DeleteObject(temp_hdc);
		if(state == LIVE)
		{
			drawSnake(hwnd, snake_head);
			drawApple(hwnd);
		}
		EndPaint (hwnd, &ps) ;
		break;
	
	case WM_TIMER:
		moveSnake(snake_head);
		if( isDead (snake_head->position) )
		{
			KillTimer(hwnd, 1); 
			MessageBoxPrintf(hwnd,"结束","    YOU ARE DEAD!\n      按F2重新开始   \n           分数=%d",score);
			state=DEAD;
		}

		hdc = GetDC (hwnd) ;
		temp_hdc = CreateCompatibleDC(hdc);

	    SelectObject(temp_hdc, hBM_back);
	    BitBlt(hdc, STARTPOS , STARTPOS, 600, 600, temp_hdc, 0, 0, SRCCOPY);//保持背景
		//KillTimer(hwnd,1);
		DeleteObject(temp_hdc);
	
		
		if(state == LIVE)
		{
			drawSnake(hwnd, snake_head);
			drawApple(hwnd);
		}
		ReleaseDC(hwnd,hdc);
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
void initGame(HWND hwnd)
{
	deleteSnake(&snake_head,&snake_tail);
	SetTimer(hwnd, 1, 1000, 0);   //定时器ID为1
	addBody(&snake_head, &snake_tail);
	addBody(&snake_head, &snake_tail);
	addBody(&snake_head, &snake_tail);
	giveApple();
	score = 0;
	state = LIVE;
}

void GoAhead(HWND hwnd)
{
	moveSnake(snake_head);
	if( isDead (snake_head->position))
	{
		KillTimer(hwnd, 1); 
		MessageBoxPrintf(hwnd,"结束","    YOU ARE DEAD!\n      按F2重新开始   \n           分数=%d",score);
		state = DEAD;
		return ;
	}
	InvalidateRect (hwnd,NULL,FALSE);    //重画

	KillTimer(hwnd, 1);                 //重置时间
	SetTimer(hwnd, 1, 1000, 0);
}

void moveSnake(Snake *head)
{
	int temp_dir1,temp_dir2;
	int isEat=0;
	Snake *current=head;
	memset(&Pool, 0, sizeof(Pool)); //重置游戏池
	movePosition(snake_head);
	while(current != NULL)
	{
		if(current == head)
		{
			temp_dir1 = current->direction;
			isEat=ifEat(snake_head->position);
		}else
		{
			temp_dir2 = current->direction;
			current->direction = temp_dir1;
			temp_dir1 = temp_dir2;
			Pool[current->position.Y][current->position.X]=-1;
		}
		current = current->pNext;
	}
	if(isEat)
	{
		giveApple();
	}
}

int ifEat(COORD pos)
{
	if(pos.X == Apple_pos.X && pos.Y == Apple_pos.Y)
	{
		addBody(&snake_head, &snake_tail);

		score++;
		return 1;
	}
	return 0;
}

int isDead(COORD pos)
{
	if(pos.X<0 || pos.X>17 || pos.Y<0 || pos.Y>17 || Pool[pos.Y][pos.X] == -1)
	{
		return 1;
	}
	return 0;
}

int CDECL MessageBoxPrintf (HWND hwnd,TCHAR * szCaption, TCHAR * szFormat, ...)
{
        
    TCHAR   szBuffer [1024] ;
    va_list pArgList ;

    va_start (pArgList, szFormat) ;
    _vsntprintf_s ( szBuffer, sizeof (szBuffer) / sizeof (TCHAR), sizeof (szBuffer) / sizeof (TCHAR),
                   szFormat, pArgList) ;

    va_end (pArgList) ;

    return MessageBox (hwnd, szBuffer, szCaption, 0) ;
     
}
void drawSnake(HWND hwnd,Snake *head)
{
	if(head != NULL)
	{
		HDC hdc = GetDC (hwnd) ;
		HDC temp_hdc = CreateCompatibleDC(hdc);
		//SelectObject(temp_hdc, )
		switch (head->direction)
		{
		case GOUP:
			PosBlt(hdc, temp_hdc, hBM_headUp,head->position);
			break;
		case GODOWN:
			PosBlt(hdc, temp_hdc, hBM_headDown,head->position);
			break;
		case GOLEFT:
			PosBlt(hdc, temp_hdc, hBM_headLeft,head->position);
			break;
		case GORIGHT:
			PosBlt(hdc, temp_hdc, hBM_headRight,head->position);
			break;
		default:
			break;
		}
		head = head->pNext;
		while(head != NULL)
		{
			if(head->pNext!=NULL && head->pNext->direction !=head->direction)
			{
				switch (head->direction - head->pNext->direction)
				{
				case RD:
					PosBlt(hdc, temp_hdc, hBM_turnRD,head->position);
					break;
				case RU:
					PosBlt(hdc, temp_hdc, hBM_turnRU,head->position);
					break;
				case LD:
					PosBlt(hdc, temp_hdc, hBM_turnLD,head->position);
					break;
				case LU:
					PosBlt(hdc, temp_hdc, hBM_turnLU,head->position);
					break;
				default:
					break;
				}
			}
			else
			{
				switch(head->direction)
				{
				case GODOWN:
				case GOUP:
					PosBlt(hdc, temp_hdc, hBM_bodyV,head->position);
					break;
				case GOLEFT:
				case GORIGHT:
					PosBlt(hdc, temp_hdc, hBM_bodyH,head->position);
					break;
				default:
					break;
				}
			}
			head = head->pNext;
		}
		DeleteObject(temp_hdc);
		ReleaseDC(hwnd,hdc);
	}
}

void  PosBlt(HDC hdcDest, HDC hdcSrc, HBITMAP body,COORD p)
{
	int a;
	SelectObject(hdcSrc, body);
	a=BitBlt(hdcDest, STARTPOS+BITSIZE*(p.X+1) , STARTPOS+BITSIZE*(p.Y+1), BITSIZE, BITSIZE, hdcSrc, 0, 0, SRCCOPY);
}

void addBody(Snake **head, Snake **tail)
{
	Snake *newBody = (Snake *) malloc(sizeof (Snake));
	if(*head == NULL)
	{
		
		while((newBody->direction = rand() % 3 - 2 ) == 0);
		
		newBody->position.X = rand() % 15 + 1;
		newBody->position.Y = rand() % 15 + 1;
		newBody->pNext = NULL;
		*head = newBody;
	}else
	{
		newBody->position = (*tail)->position;
		newBody->direction = (*tail)->direction;
		newBody->pNext = NULL;
		switch (newBody->direction)//调整新节点位置
		{
		case GOUP:
			newBody->position.Y++;
			break;
		case GODOWN:
			newBody->position.Y--;
			break;
		case GOLEFT:
			newBody->position.X++;
			break;
		case GORIGHT:
			newBody->position.X--;
			break;
		default:
			break;
		}
		(*tail)->pNext = newBody;
	}
	*tail=newBody;
}

void movePosition(Snake *head)
{
	while(head != NULL)
	{
		switch (head->direction)
		{
		case GOUP:
			head->position.Y--;
			break;
		case GODOWN:
			head->position.Y++;
			break;
		case GOLEFT:
			head->position.X--;
			break;
		case GORIGHT:
			head->position.X++;
			break;
		default:
			break;
		}
		head = head->pNext;
	}
}
void giveApple()
{
	do
	{
		Apple_pos.X = rand() % 17;
		Apple_pos.Y = rand() % 17;
	}while(Pool[Apple_pos.Y][Apple_pos.X] == -1 
		|| ( Apple_pos.X == snake_head->position.X
		     && Apple_pos.Y == snake_head->position.Y ));
}
void drawApple(HWND hwnd)
{
	HDC hdc = GetDC (hwnd) ;
	HDC temp_hdc = CreateCompatibleDC(hdc);
	PosBlt(hdc, temp_hdc, hBM_Apple, Apple_pos);
	DeleteObject(temp_hdc);
	ReleaseDC(hwnd,hdc);
}

void deleteSnake(Snake **head,Snake **tail)
{
	Snake *temp = NULL;
	while(*head != NULL)
	{
		temp=*head;
		*head = (*head)->pNext;
		free(temp);
	}
	*tail=NULL;
}
