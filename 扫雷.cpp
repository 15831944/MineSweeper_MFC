// 扫雷.cpp : Defines the entry point for the application.
//

//#define DIG_DUBUG
#define DIG_DUBUG_DELAY 50

#include "E:/Library/VisualStudioHeadFiles/TouchImage.h"
#include "E:/Library/VisualStudioHeadFiles/MEMMNG.H"
#include <time.h>
#include "stdio.h"
#include <mmsystem.h>//PlaySound函数的头文件
#pragma comment(lib,"winmm.lib")//链接库，PlaySound函数必须使用

#define Size 40
#define X 20
#define Y 15
#define WinWidth 800
#define WinHeight 600

#undef VOID
#define VOID		0
#define BOMB		-1

#define DIGED		-1
#define SIGNED		-2

char MAP[X][Y];//表格
char DigMAP[X][Y];//表格(当前)
TouchImage Num[9];//0~8
TouchImage Unknow;//未知
TouchImage Signed;//已标记
TouchImage Bomb;//炸弹

WORD SignNum;
WORD BombNum;

void InitResource() {//初始化资源
	Num[0].LoadImage("IMAGE", "NUM0", Size, Size);
	Num[0].SetAlpha("IMAGE", "NUM~");//设置透明
	Num[1].LoadImage("IMAGE", "NUM1", Size, Size);
	Num[1].SetAlpha("IMAGE", "NUM~");//设置透明
	Num[2].LoadImage("IMAGE", "NUM2", Size, Size);
	Num[2].SetAlpha("IMAGE", "NUM~");//设置透明
	Num[3].LoadImage("IMAGE", "NUM3", Size, Size);
	Num[3].SetAlpha("IMAGE", "NUM~");//设置透明
	Num[4].LoadImage("IMAGE", "NUM4", Size, Size);
	Num[4].SetAlpha("IMAGE", "NUM~");//设置透明
	Num[5].LoadImage("IMAGE", "NUM5", Size, Size);
	Num[5].SetAlpha("IMAGE", "NUM~");//设置透明
	Num[6].LoadImage("IMAGE", "NUM6", Size, Size);
	Num[6].SetAlpha("IMAGE", "NUM~");//设置透明
	Num[7].LoadImage("IMAGE", "NUM7", Size, Size);
	Num[7].SetAlpha("IMAGE", "NUM~");//设置透明
	Num[8].LoadImage("IMAGE", "NUM8", Size, Size);
	Num[8].SetAlpha("IMAGE", "NUM~");//设置透明
	Unknow.LoadImage("IMAGE", "NUM？", Size, Size);
	Unknow.SetAlpha("IMAGE", "NUM~");//设置透明
	Signed.LoadImage("IMAGE", "NUM_SIGNED", Size, Size);
	Signed.SetAlpha("IMAGE", "NUM~");//设置透明
	Bomb.LoadImage("IMAGE", "BOMB", Size, Size);
	Bomb.SetAlpha("IMAGE", "NUM~");//设置透明
}
WORD InitMAP(WORD MineNum) {
	BombNum = MineNum;
	SignNum = 0;
	if (MineNum >= X * Y)//超过所有格子
		return MineNum;
	++MineNum;
	srand(time(NULL));					//初始化随机数
	WORD i, j, Len;
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
			MAP[i][j] = VOID;//设置空
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
			DigMAP[i][j] = VOID;//设置未挖
	Len = X * Y;
	CreateArray(char*, Sign, Len);//标志
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
			Sign[i + j * X] = &MAP[i][j];			//初始化地址
	while (--MineNum) {//还有雷没有放
		i = rand() % Len--;//获得随机位置
		*Sign[i] = BOMB;//放置地雷
		Sign[i] = Sign[Len];//补充空白
	}
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
			if (MAP[i][j] != BOMB)//当前不是雷
				MAP[i][j] += (i > 0 && MAP[i - 1][j] == BOMB)//左边的雷
				+ (i > 0 && j > 0 && MAP[i - 1][j - 1] == BOMB)//左边的雷
				+ (i > 0 && j < Y - 1 && MAP[i - 1][j + 1] == BOMB)//左边的雷

				+ (j > 0 && MAP[i][j - 1] == BOMB)//上下的雷
				+ (j < Y - 1 && MAP[i][j + 1] == BOMB)//下方的雷

				+ (i < X - 1 && MAP[i + 1][j] == BOMB)//右边的雷
				+ (i < X - 1 && j>0 && MAP[i + 1][j - 1] == BOMB)//右边的雷
				+ (i < X - 1 && j < Y - 1 && MAP[i + 1][j + 1] == BOMB);//右边的雷

	FreeMemery(Sign);

	return MineNum;
}
void DrawWholeMAP() {
	BYTE i, j;
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
			if (DigMAP[i][j] == VOID)//未挖
				Unknow.Show(i * Size, j * Size);
			else if (DigMAP[i][j] == SIGNED)//标记
				Signed.Show(i * Size, j * Size);
			else if (DigMAP[i][j] == DIGED)
				if (MAP[i][j] >= 0)//数字
					Num[MAP[i][j]].Show(i * Size, j * Size);
				else
					Bomb.Show(i * Size, j * Size);
}
char Dig(WORD x, WORD y) {//挖
#define InQueue(a,b) Queue[ptail].x=a;Queue[ptail].y=b;ptail=(ptail+1)%Len;//入队
#define OutQueue() P=Queue+phead;phead=(phead+1)%Len;//出队
	static WORD Len = X < Y ? X * 4 : Y * 4, phead, ptail;
	//static CreateArray(POINT,Queue,Len);//申请队列
	POINT Queue[X < Y ? X * 4 : Y * 4];
	static POINT* P;
	phead = ptail = 0;

	if (MAP[x][y] == BOMB) {//是炸弹
		DigMAP[x][y] = DIGED;
		return BOMB;//
	}
	else if (DigMAP[x][y] == DIGED)
		return 0;
	else if (MAP[x][y] > 0)
	{
		if (DigMAP[x][y] == SIGNED)
			--SignNum;
		DigMAP[x][y] = DIGED;
		return 1;
	}

	InQueue(x, y);//起点入队
#define DIG(a,b) DigMAP[a][b]=DIGED;
	while (phead != ptail) {//尚未遍历完全
		OutQueue();//出队

#ifdef DIG_DUBUG
		DrawWholeMAP();
		TouchImage::FlushDraw();
		Sleep(DIG_DUBUG_DELAY);
#endif
		if (P->x > 0) {//处理左侧
			if (P->y > 0 && DigMAP[P->x - 1][P->y - 1] != DIGED) {//处理上面
				if (MAP[P->x - 1][P->y - 1] == VOID) {//空，入列
					DIG(P->x - 1, P->y - 1);//挖开
					InQueue(P->x - 1, P->y - 1);
				}
				else if (MAP[P->x - 1][P->y - 1] > 0) {//是数字
					DIG(P->x - 1, P->y - 1);//挖开
				}
				//地雷不处理
			}
			if (DigMAP[P->x - 1][P->y] != DIGED) {//处理中间
				if (MAP[P->x - 1][P->y] == VOID) {//空，入列
					DIG(P->x - 1, P->y);//挖开
					InQueue(P->x - 1, P->y);
				}
				else if (MAP[P->x - 1][P->y] > 0) {//是数字
					DIG(P->x - 1, P->y);//挖开
				}
				//地雷不处理
			}
			if (P->y < Y - 1 && DigMAP[P->x - 1][P->y + 1] != DIGED) {//处理下面
				if (MAP[P->x - 1][P->y + 1] == VOID) {//空，入列
					DIG(P->x - 1, P->y + 1);//挖开
					InQueue(P->x - 1, P->y + 1);
				}
				else if (MAP[P->x - 1][P->y + 1] > 0) {//是数字
					DIG(P->x - 1, P->y + 1);//挖开
				}
				//地雷不处理
			}
		}
		{//处理中间
			if (P->y > 0 && DigMAP[P->x][P->y - 1] != DIGED) {//处理上面
				if (MAP[P->x][P->y - 1] == VOID) {//空，入列
					DIG(P->x, P->y - 1);//挖开
					InQueue(P->x, P->y - 1);
				}
				else if (MAP[P->x][P->y - 1] > 0) {//是数字
					DIG(P->x, P->y - 1);//挖开
				}
				//地雷不处理
			}
			if (DigMAP[P->x][P->y] != DIGED) {//处理中间
				if (MAP[P->x][P->y] == VOID) {//空，入列
					DIG(P->x, P->y);//挖开
					InQueue(P->x, P->y);
				}
				else if (MAP[P->x][P->y] > 0) {//是数字
					DIG(P->x, P->y);//挖开
				}
				//地雷不处理
			}
			if (P->y < Y - 1 && DigMAP[P->x][P->y + 1] != DIGED) {//处理下面
				if (MAP[P->x][P->y + 1] == VOID) {//空，入列
					DIG(P->x, P->y + 1);//挖开
					InQueue(P->x, P->y + 1);
				}
				else if (MAP[P->x][P->y + 1] > 0) {//是数字
					DIG(P->x, P->y + 1);//挖开
				}
				//地雷不处理
			}
		}
		if (P->x < X - 1) {//处理右边
			if (P->y > 0 && DigMAP[P->x + 1][P->y - 1] != DIGED) {//处理上面
				if (MAP[P->x + 1][P->y - 1] == VOID) {//空，入列
					DIG(P->x + 1, P->y - 1);//挖开
					InQueue(P->x + 1, P->y - 1);
				}
				else if (MAP[P->x + 1][P->y - 1] > 0) {//是数字
					DIG(P->x + 1, P->y - 1);//挖开
				}
				//地雷不处理
			}
			if (DigMAP[P->x + 1][P->y] != DIGED) {//处理中间
				if (MAP[P->x + 1][P->y] == VOID) {//空，入列
					DIG(P->x + 1, P->y);//挖开
					InQueue(P->x + 1, P->y);
				}
				else if (MAP[P->x + 1][P->y] > 0) {//是数字
					DIG(P->x + 1, P->y);//挖开
				}
				//地雷不处理
			}
			if (P->y < Y - 1 && DigMAP[P->x + 1][P->y + 1] != DIGED) {//处理下面
				if (MAP[P->x + 1][P->y + 1] == VOID) {//空，入列
					DIG(P->x + 1, P->y + 1);//挖开
					InQueue(P->x + 1, P->y + 1);
				}
				else if (MAP[P->x + 1][P->y + 1] > 0) {//是数字
					DIG(P->x + 1, P->y + 1);//挖开
				}
				//地雷不处理
			}
		}
	}
#undef InQueue
#undef OutQueue
	return MAP[x][y];
}
BYTE Sign(WORD x, WORD y) {
	if (x >= X || y >= Y)//超界
		return 0;
	if (DigMAP[x][y] == SIGNED) {//已标记
		DigMAP[x][y] = VOID;//取消标记
		return --SignNum;
	}
	else if (DigMAP[x][y] == VOID) {//未标记
		if (SignNum >= BombNum)//旗子太多
			return 0;
		else {//可以插旗
			DigMAP[x][y] = SIGNED;//插旗
			return ++SignNum;
		}
	}
	else if (DigMAP[x][y] == DIGED)//已挖掘
		return 0;
	else
		return 0;
}
void GameOver() {
	WORD i, j;
	PlaySound("BOOM", NULL, SND_RESOURCE | SND_NODEFAULT | SND_ASYNC);
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
			if (MAP[i][j] == BOMB) {
				DigMAP[i][j] = DIGED;
				DrawWholeMAP();
				TouchImage::FlushDraw();
				//Sleep(500);
			}
}
char Check()
{
	WORD i, j;
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
			if (MAP[i][j] >= 0 && DigMAP[i][j] == VOID)//发现没有点开的非雷区
				return 1;//未胜利
	return 0;//胜利
}
BYTE SingleMain() {//单人游戏
	MOUSEMSG Msg;//鼠标消息

	DrawWholeMAP();
	TouchImage::FlushDraw();//刷新

	FlushMouseMsgBuffer();
	while (Check()) {
		if (MouseHit()) {//有鼠标消息
			Msg = GetMouseMsg();//获取鼠标消息
			if (Msg.uMsg == WM_LBUTTONUP)//左键抬起
			{
				if (Dig(Msg.x / Size, Msg.y / Size) == BOMB) {//踩到炸弹
					GameOver();
					if (MessageBox(TouchImage::GetHwnd(), "你输了，下次要加油哦！", "失败", MB_RETRYCANCEL) == IDCANCEL)//选择了取消;
						return 0;
					else
						return 1;//重玩
				}

				DrawWholeMAP();

				TouchImage::FlushDraw();//刷新
			}
			else if (Msg.uMsg == WM_RBUTTONUP)//右键抬起
			{
				Sign(Msg.x / Size, Msg.y / Size);//记号
				DrawWholeMAP();
				TouchImage::FlushDraw();//刷新
			}
			FlushMouseMsgBuffer();
		}
	}
	//胜利
	if (MessageBox(TouchImage::GetHwnd(), "恭喜你胜利了，要再来一次吗？", "胜利", MB_RETRYCANCEL) == IDCANCEL)//选择了取消;
		return 0;
	else
		return 1;//重玩
}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	InitResource();								//载入资源

	char StrBombNum[4];
	char SingleReturn;
	do {
		InputBox(StrBombNum, sizeof(StrBombNum), "请输入雷数(1~299)", "雷数", "30", 0, 0, true);
		sscanf(StrBombNum, "%u", &BombNum);
		if (BombNum < 1 || BombNum>299)
			continue;
		TouchImage::Initgraph(WinWidth, WinHeight);	//启动窗口
		TouchImage::BatchDraw();					//开启批量绘制
		InitMAP(BombNum);//初始化地雷
		SingleReturn = SingleMain();
		TouchImage::BatchDraw();
		TouchImage::Closegraph();
	} while (SingleReturn);

	return 0;
}