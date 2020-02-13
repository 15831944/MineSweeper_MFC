#define Alpha_Limit_0 0X50 //ȫ͸�����͸������
#define Alpha_Limit_1 0XB0 //��͸���벻͸������
#define Alpha_NULL_Value 0XFF //��͸�� ֵ
#define Alpha_HALF_Value 0X7F //��͸�� ֵ
#define Alpha_FULL_Value 0X00 //ȫ͸�� ֵ
#include "E:/Library/VisualStudioHeadFiles/EasyXEffect.h"
class TouchImage {//���ڼ���ͼ��
private:
	IMAGE Img;				//ͼ������
	DWORD* Pbuff;			//ͼ���ڴ�ָ��
	COLOR* Cbuff;			//ͼ�����ָ��
	WORD Width;				//ͼ����
	WORD Height;			//ͼ��߶�
	DWORD Len;				//ͼ����������
	WORD X;//��ǰ�������ƫ��
	WORD Y;//��ǰ�������ƫ��

	DWORD D_BackColor;//������ɫ
	COLOR C_BackColor;//������ɫ

	static bool _IsBatchDraw;//��ǰΪ������ͼģʽ

	static WORD WinWidth;	//���ڿ��
	static WORD WinHeight;	//���ڸ߶�
	static DWORD* Pscreen;	//�����ڴ�ָ��
	static COLOR* Cscreen;	//���ڷ���ָ��
	static HWND Hwnd;		//���ھ��
public:
	TouchImage() {
		X = Y = 0;//��ʼ���������
		C_BackColor.Color = D_BackColor = 0;//������ɫ
	};
	TouchImage(char* ResType,	//��Դ����
		char* ResName		//��Դ����
	) {
		X = Y = 0;//��ʼ���������
		C_BackColor.Color = D_BackColor = 0;//������ɫ

		LoadImage(ResType, ResName);
	};
	TouchImage(char* ResType,	//��Դ����
		DWORD ResID		//��ԴID
	) {
		X = Y = 0;//��ʼ���������
		C_BackColor.Color = D_BackColor = 0;//������ɫ

		char* ResName = MAKEINTRESOURCE(ResID);
		LoadImage(ResType, ResName);
	};
	inline static HWND Initgraph(WORD width = 640,	//��ͼ���ڿ��
		WORD height = 480,	//��ͼ���ڸ߶�
		int flag = NULL		//��ͼ������ʽ
	)					//��ʼ����ͼ����
	{
		Hwnd = initgraph(WinWidth = width, WinHeight = height, flag);	//��������
		Cscreen = (COLOR*)(Pscreen = GetImageBuffer(NULL));			//��ȡ�����ڴ漰����ָ��
		return Hwnd;											//���ؾ��
	}
	inline static void Closegraph()	//�رջ�ͼ���ڴ���
	{
		closegraph();			//�رջ�ͼ����
	}
	void LoadImage(char* FileName,		//�ļ�����
		WORD width = 0,		//������
		WORD height = 0,		//����߶�
		bool bResize = false	//�Ƿ����IMAGE�Ĵ�С����ӦͼƬ
	)					//��ԴͼƬ����
	{
		if (width && height)											//ָ���߶ȿ��
			loadimage(&Img, FileName, width, height, bResize);	//��ȡͼƬ
		else														//δָ���߶ȿ�ȣ�֮һ��
			loadimage(&Img, FileName, bResize);				//��ȡͼƬ
		Width = Img.getwidth();										//��ȡ���
		Height = Img.getheight();										//��ȡ�߶�
		Len = Width * Height;											//������������
		Cbuff = (COLOR*)(Pbuff = GetImageBuffer(&Img));					//��ȡ�ڴ漰����ָ��
	}
	void LoadImage(char* ResType,		//��Դ����
		char* ResName,		//��Դ����
		WORD width = 0,		//������
		WORD height = 0,		//����߶�
		bool bResize = false	//�Ƿ����IMAGE�Ĵ�С����ӦͼƬ
	)					//��ԴͼƬ����
	{
		if (width && height)											//ָ���߶ȿ��
			loadimage(&Img, ResType, ResName, width, height, bResize);	//��ȡͼƬ
		else														//δָ���߶ȿ�ȣ�֮һ��
			loadimage(&Img, ResType, ResName, bResize);				//��ȡͼƬ
		Width = Img.getwidth();										//��ȡ���
		Height = Img.getheight();										//��ȡ�߶�
		Len = Width * Height;											//������������
		Cbuff = (COLOR*)(Pbuff = GetImageBuffer(&Img));					//��ȡ�ڴ漰����ָ��
	}
	void LoadImage(char* ResType,		//��Դ����
		DWORD ResID,			//��ԴID
		WORD width = 0,		//������
		WORD height = 0,		//����߶�
		bool bResize = false	//�Ƿ���� IMAGE �Ĵ�С����ӦͼƬ
	)					//��ԴͼƬ����
	{
		char* ResName = MAKEINTRESOURCE(ResID);						//ת���ַ���
		if (width && height)//ָ���߶ȿ��
			loadimage(&Img, ResType, ResName, width, height, bResize);	//��ȡͼƬ
		else														//δָ���߶ȿ�ȣ�֮һ��
			loadimage(&Img, ResType, ResName, bResize);				//��ȡͼƬ
		Width = Img.getwidth();										//��ȡ���
		Height = Img.getheight();										//��ȡ�߶�
		Len = Width * Height;											//������������
		Cbuff = (COLOR*)(Pbuff = GetImageBuffer(&Img));					//��ȡ�ڴ漰����ָ��
	}
	void SetAlpha(char* FileName)//����͸����
	{
		IMAGE TmpMask;										//����ͼ��
		loadimage(&TmpMask, FileName, Width, Height);	//��ȡͼ��
		COLOR* Ctmp = (COLOR*)GetImageBuffer(&TmpMask);		//��ȡ����ָ��
		DWORD p;											//ѭ������
		for (p = 0; p < Len; p++)
			if (Ctmp[p].G < Alpha_Limit_0)						//ȫ͸��
				Cbuff[p].A = Alpha_FULL_Value;				//����ȫ͸��
			else if (Ctmp[p].G < Alpha_Limit_1)				//��͸��
				Cbuff[p].A = Alpha_HALF_Value;				//���ð�͸��
			else											//��͸��
				Cbuff[p].A = Alpha_NULL_Value;				//���ò�͸��
	}
	void SetAlpha(char* ResType,	//��Դ����
		char* ResName	//��Դ����
	)				//����͸����
	{
		IMAGE TmpMask;										//����ͼ��
		loadimage(&TmpMask, ResType, ResName, Width, Height);	//��ȡͼ��
		COLOR* Ctmp = (COLOR*)GetImageBuffer(&TmpMask);		//��ȡ����ָ��
		DWORD p;											//ѭ������
		for (p = 0; p < Len; p++)
			if (Ctmp[p].G < Alpha_Limit_0)						//ȫ͸��
				Cbuff[p].A = Alpha_FULL_Value;				//����ȫ͸��
			else if (Ctmp[p].G < Alpha_Limit_1)				//��͸��
				Cbuff[p].A = Alpha_HALF_Value;				//���ð�͸��
			else											//��͸��
				Cbuff[p].A = Alpha_NULL_Value;				//���ò�͸��
	}
	void SetAlpha(char* ResType,	//��Դ����
		DWORD ResID	//��ԴID
	)				//����͸����
	{
		char* ResName = MAKEINTRESOURCE(ResID);				//ת���ַ���
		IMAGE TmpMask;										//����ͼ��
		loadimage(&TmpMask, ResType, ResName, Width, Height);	//��ȡͼ��
		COLOR* Ctmp = (COLOR*)GetImageBuffer(&TmpMask);		//��ȡ����ָ��
		DWORD p;											//ѭ������
		for (p = 0; p < Len; p++)
			if (Ctmp[p].G < Alpha_Limit_0)						//ȫ͸��
				Cbuff[p].A = Alpha_FULL_Value;				//����ȫ͸��
			else if (Ctmp[p].G < Alpha_Limit_1)				//��͸��
				Cbuff[p].A = Alpha_HALF_Value;				//���ð�͸��
			else											//��͸��
				Cbuff[p].A = Alpha_NULL_Value;				//���ò�͸��
	}
	void InitDefImage(char* ResType,			//��Դ����
		char* ResName,			//��Դ����
		WORD width = 0,			//������
		WORD height = 0,		//����߶�
		bool bResize = false	//�Ƿ����IMAGE�Ĵ�С����ӦͼƬ
	)
	{
		if (width && height)											//ָ���߶ȿ��
			loadimage(&Img, ResType, ResName, width, height, bResize);	//��ȡͼƬ
		else														//δָ���߶ȿ�ȣ�֮һ��
			loadimage(&Img, ResType, ResName, bResize);				//��ȡͼƬ
		Width = Img.getwidth();										//��ȡ���
		Height = Img.getheight();										//��ȡ�߶�
		Len = Width * Height;											//������������
		Cbuff = (COLOR*)(Pbuff = GetImageBuffer(&Img));					//��ȡ�ڴ漰����ָ��

		static char ResMaskName[_MAX_PATH];
		char* Tmp = ResMaskName - 1;
		while (*++Tmp = *ResName++);
		*Tmp++ = '~';
		*Tmp = NULL;

		IMAGE TmpMask;										//����ͼ��
		loadimage(&TmpMask, ResType, ResMaskName, Width, Height);//��ȡͼ��
		COLOR* Ctmp = (COLOR*)GetImageBuffer(&TmpMask);		//��ȡ����ָ��
		DWORD p;											//ѭ������
		for (p = 0; p < Len; p++)
			if (Ctmp[p].G < Alpha_Limit_0)						//ȫ͸��
				Cbuff[p].A = Alpha_FULL_Value;				//����ȫ͸��
			else if (Ctmp[p].G < Alpha_Limit_1)				//��͸��
				Cbuff[p].A = Alpha_HALF_Value;				//���ð�͸��
			else											//��͸��
				Cbuff[p].A = Alpha_NULL_Value;				//���ò�͸��
	};
	void Show(WORD x, WORD y, bool PosOnly = false)//��ʾͼ��
	{
		//�������λ��
		X = x;
		Y = y;
		if (PosOnly)//ֻ�ı�λ��
			return;
		//�����Ⱥ͸߶�
		WORD W = Width < WinWidth - X ? Width : WinWidth - X;//��ֹ����Խ��
		WORD H = Height < WinHeight - Y ? Height : WinHeight - Y;//��ֹ����Խ��
		WORD i, j;//ѭ������
		DWORD Pscr, Pimg;//Ϊ�˼ӿ�����ٶȣ���ƫ�ƽ����������
		for (i = 0; i < W; i++)
			for (j = 0; j < H; j++)
			{
				Pscr = X + i + (Y + j) * WinWidth;//��Ļƫ�Ƶ�ַ
				Pimg = i + j * Width;//ͼ��ƫ�Ƶ�ַ
				if (Cbuff[Pimg].A >= Alpha_Limit_1)//��͸��
					Pscreen[Pscr] = Pbuff[Pimg];
				else if (Cbuff[Pimg].A >= Alpha_Limit_0)//��͸��
				{
					Cscreen[Pscr].R = (Cbuff[Pimg].R + Cscreen[Pscr].R) >> 1;
					Cscreen[Pscr].G = (Cbuff[Pimg].G + Cscreen[Pscr].G) >> 1;
					Cscreen[Pscr].B = (Cbuff[Pimg].B + Cscreen[Pscr].B) >> 1;
				}
				//ȫ͸�������
			}
	};
	void Show()//��ʾͼ��
	{
		//�����Ⱥ͸߶�
		WORD W = Width < WinWidth - X ? Width : WinWidth - X;//��ֹ����Խ��
		WORD H = Height < WinHeight - Y ? Height : WinHeight - Y;//��ֹ����Խ��
		WORD i, j;//ѭ������
		DWORD Pscr, Pimg;//Ϊ�˼ӿ�����ٶȣ���ƫ�ƽ����������
		for (i = 0; i < W; i++)
			for (j = 0; j < H; j++)
			{
				Pscr = X + i + (Y + j) * WinWidth;//��Ļƫ�Ƶ�ַ
				Pimg = i + j * Width;//ͼ��ƫ�Ƶ�ַ
				if (Cbuff[Pimg].A >= Alpha_Limit_1)//��͸��
					Pscreen[Pscr] = Pbuff[Pimg];
				else if (Cbuff[Pimg].A >= Alpha_Limit_0)//��͸��
				{
					Cscreen[Pscr].R = (Cbuff[Pimg].R + Cscreen[Pscr].R) >> 1;
					Cscreen[Pscr].G = (Cbuff[Pimg].G + Cscreen[Pscr].G) >> 1;
					Cscreen[Pscr].B = (Cbuff[Pimg].B + Cscreen[Pscr].B) >> 1;
				}
				//ȫ͸�������
			}
	};
	void TShow(WORD x, WORD y, BYTE Attrib)//��͸������͸�� ��ʾͼ��
	{
		X = x;
		Y = y;

		//�����Ⱥ͸߶�
		WORD W = Width < WinWidth - X ? Width : WinWidth - X;//��ֹ����Խ��
		WORD H = Height < WinHeight - Y ? Height : WinHeight - Y;//��ֹ����Խ��
		WORD i, j;//ѭ������
		DWORD Pscr, Pimg;//Ϊ�˼ӿ�����ٶȣ���ƫ�ƽ����������
		if (Attrib == 0X00) {//ȫ����͸��
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++)
					Pscreen[X + i + (Y + j) * WinWidth] = Pbuff[i + j * Width];
		}
		else if (Attrib == 0X01) {//ȫ����͸��
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					Pscr = X + i + (Y + j) * WinWidth;//��Ļƫ�Ƶ�ַ
					Pimg = i + j * Width;//ͼ��ƫ�Ƶ�ַ
					Cscreen[Pscr].R = (Cbuff[Pimg].R + Cscreen[Pscr].R) >> 1;
					Cscreen[Pscr].G = (Cbuff[Pimg].G + Cscreen[Pscr].G) >> 1;
					Cscreen[Pscr].B = (Cbuff[Pimg].B + Cscreen[Pscr].B) >> 1;
				}
		}
		else if (Attrib == 0X02) {//�ֲ���͸��
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					if (Cbuff[i + j * Width].A > Alpha_Limit_0)//����ȫ͸��
						Pscreen[X + i + (Y + j) * WinWidth] = Pbuff[i + j * Width];
				}
		}
		else if (Attrib == 0X03) {//�ֲ���͸��
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					Pscr = X + i + (Y + j) * WinWidth;//��Ļƫ�Ƶ�ַ
					Pimg = i + j * Width;//ͼ��ƫ�Ƶ�ַ
					if (Cbuff[Pimg].A >= Alpha_Limit_0)//����ȫ͸��
					{
						Cscreen[Pscr].R = (Cbuff[Pimg].R + Cscreen[Pscr].R) >> 1;
						Cscreen[Pscr].G = (Cbuff[Pimg].G + Cscreen[Pscr].G) >> 1;
						Cscreen[Pscr].B = (Cbuff[Pimg].B + Cscreen[Pscr].B) >> 1;
					}
				}
		}
		else if (Attrib == 0X04) {//���ͼ��
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++)
					Pscreen[X + i + (Y + j) * WinWidth] = D_BackColor;//�ñ���ɫ���
		}
		else if (Attrib == 0X05) {//�ֲ����ͼ��
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					Pscr = X + i + (Y + j) * WinWidth;//��Ļƫ�Ƶ�ַ
					Pimg = i + j * Width;//ͼ��ƫ�Ƶ�ַ
					if (Cbuff[Pimg].A >= Alpha_Limit_0)//����ȫ͸��
						Pscreen[Pscr] = D_BackColor;//�ñ���ɫ���
				}
		}
	}
	void TShow(BYTE Attrib)//��͸������͸�� ��ʾͼ��
	{
		//�����Ⱥ͸߶�
		WORD W = Width < WinWidth - X ? Width : WinWidth - X;//��ֹ����Խ��
		WORD H = Height < WinHeight - Y ? Height : WinHeight - Y;//��ֹ����Խ��
		WORD i, j;//ѭ������
		DWORD Pscr, Pimg;//Ϊ�˼ӿ�����ٶȣ���ƫ�ƽ����������
		if (Attrib == 0X00) {//ȫ����͸��
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++)
					Pscreen[X + i + (Y + j) * WinWidth] = Pbuff[i + j * Width];
		}
		else if (Attrib == 0X01) {//ȫ����͸��
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					Pscr = X + i + (Y + j) * WinWidth;//��Ļƫ�Ƶ�ַ
					Pimg = i + j * Width;//ͼ��ƫ�Ƶ�ַ
					Cscreen[Pscr].R = (Cbuff[Pimg].R + Cscreen[Pscr].R) >> 1;
					Cscreen[Pscr].G = (Cbuff[Pimg].G + Cscreen[Pscr].G) >> 1;
					Cscreen[Pscr].B = (Cbuff[Pimg].B + Cscreen[Pscr].B) >> 1;
				}
		}
		else if (Attrib == 0X02) {//�ֲ���͸��
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					if (Cbuff[i + j * Width].A > Alpha_Limit_0)//����ȫ͸��
						Pscreen[X + i + (Y + j) * WinWidth] = Pbuff[i + j * Width];
				}
		}
		else if (Attrib == 0X03) {//�ֲ���͸��
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					Pscr = X + i + (Y + j) * WinWidth;//��Ļƫ�Ƶ�ַ
					Pimg = i + j * Width;//ͼ��ƫ�Ƶ�ַ
					if (Cbuff[Pimg].A >= Alpha_Limit_0)//����ȫ͸��
					{
						Cscreen[Pscr].R = (Cbuff[Pimg].R + Cscreen[Pscr].R) >> 1;
						Cscreen[Pscr].G = (Cbuff[Pimg].G + Cscreen[Pscr].G) >> 1;
						Cscreen[Pscr].B = (Cbuff[Pimg].B + Cscreen[Pscr].B) >> 1;
					}
				}
		}
		else if (Attrib == 0X04) {//���ͼ��
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++)
					Pscreen[X + i + (Y + j) * WinWidth] = D_BackColor;//�ñ���ɫ���
		}
		else if (Attrib == 0X05) {//�ֲ����ͼ��
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					Pscr = X + i + (Y + j) * WinWidth;//��Ļƫ�Ƶ�ַ
					Pimg = i + j * Width;//ͼ��ƫ�Ƶ�ַ
					if (Cbuff[Pimg].A >= Alpha_Limit_0)//����ȫ͸��
						Pscreen[Pscr] = D_BackColor;//�ñ���ɫ���
				}
		}
	}
	static inline HWND GetHwnd() {//��ô��ھ��
		return Hwnd;
	}
	inline WORD width() {//���ͼ����
		return Width;
	}
	inline WORD height() {//���ͼ��߶�
		return Height;
	}
	static inline WORD WndWidth() {//��ô��ڿ��
		return WinWidth;
	}
	static inline WORD WndHeight() {//��ô��ڸ߶�
		return WinHeight;
	}
	inline BYTE GetPointAlpha(WORD x, WORD y) {//��ȡͼ��ָ�����͸����
		if (x < Width && y < Height)
			return Cbuff[x + y * Width].A;
		else
			return 0;
	}
	inline BYTE GetPointAlpha(DWORD pos) {//��ȡͼ��ָ�����͸����
		if (pos < Len)
			return Cbuff[pos].A;
		else
			return 0;
	}
	inline BYTE At(WORD x, WORD y) {//���ָ�����Ƿ���ͼ���ڣ�����͸����
		if (x > X&& y > Y&& x < X + Width && y < Y + Height)//�ھ�����
			return Cbuff[x - X + (y - Y) * Width].A;
		else
			return false;
	}
	inline bool InRect(WORD x, WORD y) {//���ָ�����Ƿ����������
		return x > X&& y > Y&& x < X + Width && y < Y + Height;
	}
	inline bool IsAt(WORD x, WORD y) {//���ָ�����Ƿ���ͼ����
		if (x > X&& y > Y&& x < X + Width && y < Y + Height)//�ھ�����
			return Cbuff[x - X + (y - Y) * Width].A > Alpha_Limit_0;
		else
			return 0;
	}
	inline void SetBKcolor(DWORD BKColor) {
		C_BackColor.Color = D_BackColor = BKColor;//������ɫ
	}
	inline void SetBKcolor(COLOR BKColor) {
		C_BackColor = BKColor;//������ɫ
		D_BackColor = C_BackColor.Color;
	}
	inline DWORD GetBKcolor() {
		return D_BackColor;
	}
	static inline void BatchDraw() {//��ʼ������ͼ
		if (_IsBatchDraw = !_IsBatchDraw)//��������ͼģʽ
			BeginBatchDraw();//��ʼ������ͼ
		else//������ͼģʽ
			EndBatchDraw();//��ʼ������ͼ
	}
	static inline void FlushDraw() {//������ͼ
		FlushBatchDraw();//������ͼ
	}
	static inline void FlushDraw(WORD Left, WORD Top, WORD Right, WORD Bottom) {//������ͼ
		FlushBatchDraw(Left, Top, Right, Bottom);//��ʼ������ͼ
	}
	inline void FlushLastDraw() {//������ͼ
		FlushBatchDraw(X, Y, X + Width, Y + Height);//��ʼ������ͼ
	}
};
WORD	TouchImage::WinWidth = 640;	//���ô��ڿ�ȳ�ʼֵ
WORD	TouchImage::WinHeight = 480;	//���ô��ڸ߶ȳ�ʼֵ
HWND	TouchImage::Hwnd = NULL;	//���ô��ھ����ʼֵ
DWORD* TouchImage::Pscreen = NULL;	//���ô����ڴ�ָ���ʼֵ
COLOR* TouchImage::Cscreen = NULL;	//���ô��ڷ���ָ���ʼֵ
bool	TouchImage::_IsBatchDraw = false;	//��ǻ�ͼģʽ

#undef Alpha_Limit_0
#undef Alpha_Limit_1
#undef Alpha_NULL_Value
#undef Alpha_HALF_Value
#undef Alpha_FULL_Value