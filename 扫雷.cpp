// ɨ��.cpp : Defines the entry point for the application.
//

//#define DIG_DUBUG
#define DIG_DUBUG_DELAY 50

#include "E:/Library/VisualStudioHeadFiles/TouchImage.h"
#include "E:/Library/VisualStudioHeadFiles/MEMMNG.H"
#include <time.h>
#include "stdio.h"
#include <mmsystem.h>//PlaySound������ͷ�ļ�
#pragma comment(lib,"winmm.lib")//���ӿ⣬PlaySound��������ʹ��

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

char MAP[X][Y];//���
char DigMAP[X][Y];//���(��ǰ)
TouchImage Num[9];//0~8
TouchImage Unknow;//δ֪
TouchImage Signed;//�ѱ��
TouchImage Bomb;//ը��

WORD SignNum;
WORD BombNum;

void InitResource() {//��ʼ����Դ
	Num[0].LoadImage("IMAGE", "NUM0", Size, Size);
	Num[0].SetAlpha("IMAGE", "NUM~");//����͸��
	Num[1].LoadImage("IMAGE", "NUM1", Size, Size);
	Num[1].SetAlpha("IMAGE", "NUM~");//����͸��
	Num[2].LoadImage("IMAGE", "NUM2", Size, Size);
	Num[2].SetAlpha("IMAGE", "NUM~");//����͸��
	Num[3].LoadImage("IMAGE", "NUM3", Size, Size);
	Num[3].SetAlpha("IMAGE", "NUM~");//����͸��
	Num[4].LoadImage("IMAGE", "NUM4", Size, Size);
	Num[4].SetAlpha("IMAGE", "NUM~");//����͸��
	Num[5].LoadImage("IMAGE", "NUM5", Size, Size);
	Num[5].SetAlpha("IMAGE", "NUM~");//����͸��
	Num[6].LoadImage("IMAGE", "NUM6", Size, Size);
	Num[6].SetAlpha("IMAGE", "NUM~");//����͸��
	Num[7].LoadImage("IMAGE", "NUM7", Size, Size);
	Num[7].SetAlpha("IMAGE", "NUM~");//����͸��
	Num[8].LoadImage("IMAGE", "NUM8", Size, Size);
	Num[8].SetAlpha("IMAGE", "NUM~");//����͸��
	Unknow.LoadImage("IMAGE", "NUM��", Size, Size);
	Unknow.SetAlpha("IMAGE", "NUM~");//����͸��
	Signed.LoadImage("IMAGE", "NUM_SIGNED", Size, Size);
	Signed.SetAlpha("IMAGE", "NUM~");//����͸��
	Bomb.LoadImage("IMAGE", "BOMB", Size, Size);
	Bomb.SetAlpha("IMAGE", "NUM~");//����͸��
}
WORD InitMAP(WORD MineNum) {
	BombNum = MineNum;
	SignNum = 0;
	if (MineNum >= X * Y)//�������и���
		return MineNum;
	++MineNum;
	srand(time(NULL));					//��ʼ�������
	WORD i, j, Len;
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
			MAP[i][j] = VOID;//���ÿ�
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
			DigMAP[i][j] = VOID;//����δ��
	Len = X * Y;
	CreateArray(char*, Sign, Len);//��־
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
			Sign[i + j * X] = &MAP[i][j];			//��ʼ����ַ
	while (--MineNum) {//������û�з�
		i = rand() % Len--;//������λ��
		*Sign[i] = BOMB;//���õ���
		Sign[i] = Sign[Len];//����հ�
	}
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
			if (MAP[i][j] != BOMB)//��ǰ������
				MAP[i][j] += (i > 0 && MAP[i - 1][j] == BOMB)//��ߵ���
				+ (i > 0 && j > 0 && MAP[i - 1][j - 1] == BOMB)//��ߵ���
				+ (i > 0 && j < Y - 1 && MAP[i - 1][j + 1] == BOMB)//��ߵ���

				+ (j > 0 && MAP[i][j - 1] == BOMB)//���µ���
				+ (j < Y - 1 && MAP[i][j + 1] == BOMB)//�·�����

				+ (i < X - 1 && MAP[i + 1][j] == BOMB)//�ұߵ���
				+ (i < X - 1 && j>0 && MAP[i + 1][j - 1] == BOMB)//�ұߵ���
				+ (i < X - 1 && j < Y - 1 && MAP[i + 1][j + 1] == BOMB);//�ұߵ���

	FreeMemery(Sign);

	return MineNum;
}
void DrawWholeMAP() {
	BYTE i, j;
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
			if (DigMAP[i][j] == VOID)//δ��
				Unknow.Show(i * Size, j * Size);
			else if (DigMAP[i][j] == SIGNED)//���
				Signed.Show(i * Size, j * Size);
			else if (DigMAP[i][j] == DIGED)
				if (MAP[i][j] >= 0)//����
					Num[MAP[i][j]].Show(i * Size, j * Size);
				else
					Bomb.Show(i * Size, j * Size);
}
char Dig(WORD x, WORD y) {//��
#define InQueue(a,b) Queue[ptail].x=a;Queue[ptail].y=b;ptail=(ptail+1)%Len;//���
#define OutQueue() P=Queue+phead;phead=(phead+1)%Len;//����
	static WORD Len = X < Y ? X * 4 : Y * 4, phead, ptail;
	//static CreateArray(POINT,Queue,Len);//�������
	POINT Queue[X < Y ? X * 4 : Y * 4];
	static POINT* P;
	phead = ptail = 0;

	if (MAP[x][y] == BOMB) {//��ը��
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

	InQueue(x, y);//������
#define DIG(a,b) DigMAP[a][b]=DIGED;
	while (phead != ptail) {//��δ������ȫ
		OutQueue();//����

#ifdef DIG_DUBUG
		DrawWholeMAP();
		TouchImage::FlushDraw();
		Sleep(DIG_DUBUG_DELAY);
#endif
		if (P->x > 0) {//�������
			if (P->y > 0 && DigMAP[P->x - 1][P->y - 1] != DIGED) {//��������
				if (MAP[P->x - 1][P->y - 1] == VOID) {//�գ�����
					DIG(P->x - 1, P->y - 1);//�ڿ�
					InQueue(P->x - 1, P->y - 1);
				}
				else if (MAP[P->x - 1][P->y - 1] > 0) {//������
					DIG(P->x - 1, P->y - 1);//�ڿ�
				}
				//���ײ�����
			}
			if (DigMAP[P->x - 1][P->y] != DIGED) {//�����м�
				if (MAP[P->x - 1][P->y] == VOID) {//�գ�����
					DIG(P->x - 1, P->y);//�ڿ�
					InQueue(P->x - 1, P->y);
				}
				else if (MAP[P->x - 1][P->y] > 0) {//������
					DIG(P->x - 1, P->y);//�ڿ�
				}
				//���ײ�����
			}
			if (P->y < Y - 1 && DigMAP[P->x - 1][P->y + 1] != DIGED) {//��������
				if (MAP[P->x - 1][P->y + 1] == VOID) {//�գ�����
					DIG(P->x - 1, P->y + 1);//�ڿ�
					InQueue(P->x - 1, P->y + 1);
				}
				else if (MAP[P->x - 1][P->y + 1] > 0) {//������
					DIG(P->x - 1, P->y + 1);//�ڿ�
				}
				//���ײ�����
			}
		}
		{//�����м�
			if (P->y > 0 && DigMAP[P->x][P->y - 1] != DIGED) {//��������
				if (MAP[P->x][P->y - 1] == VOID) {//�գ�����
					DIG(P->x, P->y - 1);//�ڿ�
					InQueue(P->x, P->y - 1);
				}
				else if (MAP[P->x][P->y - 1] > 0) {//������
					DIG(P->x, P->y - 1);//�ڿ�
				}
				//���ײ�����
			}
			if (DigMAP[P->x][P->y] != DIGED) {//�����м�
				if (MAP[P->x][P->y] == VOID) {//�գ�����
					DIG(P->x, P->y);//�ڿ�
					InQueue(P->x, P->y);
				}
				else if (MAP[P->x][P->y] > 0) {//������
					DIG(P->x, P->y);//�ڿ�
				}
				//���ײ�����
			}
			if (P->y < Y - 1 && DigMAP[P->x][P->y + 1] != DIGED) {//��������
				if (MAP[P->x][P->y + 1] == VOID) {//�գ�����
					DIG(P->x, P->y + 1);//�ڿ�
					InQueue(P->x, P->y + 1);
				}
				else if (MAP[P->x][P->y + 1] > 0) {//������
					DIG(P->x, P->y + 1);//�ڿ�
				}
				//���ײ�����
			}
		}
		if (P->x < X - 1) {//�����ұ�
			if (P->y > 0 && DigMAP[P->x + 1][P->y - 1] != DIGED) {//��������
				if (MAP[P->x + 1][P->y - 1] == VOID) {//�գ�����
					DIG(P->x + 1, P->y - 1);//�ڿ�
					InQueue(P->x + 1, P->y - 1);
				}
				else if (MAP[P->x + 1][P->y - 1] > 0) {//������
					DIG(P->x + 1, P->y - 1);//�ڿ�
				}
				//���ײ�����
			}
			if (DigMAP[P->x + 1][P->y] != DIGED) {//�����м�
				if (MAP[P->x + 1][P->y] == VOID) {//�գ�����
					DIG(P->x + 1, P->y);//�ڿ�
					InQueue(P->x + 1, P->y);
				}
				else if (MAP[P->x + 1][P->y] > 0) {//������
					DIG(P->x + 1, P->y);//�ڿ�
				}
				//���ײ�����
			}
			if (P->y < Y - 1 && DigMAP[P->x + 1][P->y + 1] != DIGED) {//��������
				if (MAP[P->x + 1][P->y + 1] == VOID) {//�գ�����
					DIG(P->x + 1, P->y + 1);//�ڿ�
					InQueue(P->x + 1, P->y + 1);
				}
				else if (MAP[P->x + 1][P->y + 1] > 0) {//������
					DIG(P->x + 1, P->y + 1);//�ڿ�
				}
				//���ײ�����
			}
		}
	}
#undef InQueue
#undef OutQueue
	return MAP[x][y];
}
BYTE Sign(WORD x, WORD y) {
	if (x >= X || y >= Y)//����
		return 0;
	if (DigMAP[x][y] == SIGNED) {//�ѱ��
		DigMAP[x][y] = VOID;//ȡ�����
		return --SignNum;
	}
	else if (DigMAP[x][y] == VOID) {//δ���
		if (SignNum >= BombNum)//����̫��
			return 0;
		else {//���Բ���
			DigMAP[x][y] = SIGNED;//����
			return ++SignNum;
		}
	}
	else if (DigMAP[x][y] == DIGED)//���ھ�
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
			if (MAP[i][j] >= 0 && DigMAP[i][j] == VOID)//����û�е㿪�ķ�����
				return 1;//δʤ��
	return 0;//ʤ��
}
BYTE SingleMain() {//������Ϸ
	MOUSEMSG Msg;//�����Ϣ

	DrawWholeMAP();
	TouchImage::FlushDraw();//ˢ��

	FlushMouseMsgBuffer();
	while (Check()) {
		if (MouseHit()) {//�������Ϣ
			Msg = GetMouseMsg();//��ȡ�����Ϣ
			if (Msg.uMsg == WM_LBUTTONUP)//���̧��
			{
				if (Dig(Msg.x / Size, Msg.y / Size) == BOMB) {//�ȵ�ը��
					GameOver();
					if (MessageBox(TouchImage::GetHwnd(), "�����ˣ��´�Ҫ����Ŷ��", "ʧ��", MB_RETRYCANCEL) == IDCANCEL)//ѡ����ȡ��;
						return 0;
					else
						return 1;//����
				}

				DrawWholeMAP();

				TouchImage::FlushDraw();//ˢ��
			}
			else if (Msg.uMsg == WM_RBUTTONUP)//�Ҽ�̧��
			{
				Sign(Msg.x / Size, Msg.y / Size);//�Ǻ�
				DrawWholeMAP();
				TouchImage::FlushDraw();//ˢ��
			}
			FlushMouseMsgBuffer();
		}
	}
	//ʤ��
	if (MessageBox(TouchImage::GetHwnd(), "��ϲ��ʤ���ˣ�Ҫ����һ����", "ʤ��", MB_RETRYCANCEL) == IDCANCEL)//ѡ����ȡ��;
		return 0;
	else
		return 1;//����
}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	InitResource();								//������Դ

	char StrBombNum[4];
	char SingleReturn;
	do {
		InputBox(StrBombNum, sizeof(StrBombNum), "����������(1~299)", "����", "30", 0, 0, true);
		sscanf(StrBombNum, "%u", &BombNum);
		if (BombNum < 1 || BombNum>299)
			continue;
		TouchImage::Initgraph(WinWidth, WinHeight);	//��������
		TouchImage::BatchDraw();					//������������
		InitMAP(BombNum);//��ʼ������
		SingleReturn = SingleMain();
		TouchImage::BatchDraw();
		TouchImage::Closegraph();
	} while (SingleReturn);

	return 0;
}