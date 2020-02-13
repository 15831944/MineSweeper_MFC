#define Alpha_Limit_0 0X50 //全透明与半透明界限
#define Alpha_Limit_1 0XB0 //半透明与不透明界限
#define Alpha_NULL_Value 0XFF //不透明 值
#define Alpha_HALF_Value 0X7F //半透明 值
#define Alpha_FULL_Value 0X00 //全透明 值
#include "E:/Library/VisualStudioHeadFiles/EasyXEffect.h"
class TouchImage {//易于检测的图像
private:
	IMAGE Img;				//图像类型
	DWORD* Pbuff;			//图像内存指针
	COLOR* Cbuff;			//图像分量指针
	WORD Width;				//图像宽度
	WORD Height;			//图像高度
	DWORD Len;				//图像总像素数
	WORD X;//当前输出横向偏移
	WORD Y;//当前输出纵向偏移

	DWORD D_BackColor;//背景颜色
	COLOR C_BackColor;//背景颜色

	static bool _IsBatchDraw;//当前为批量绘图模式

	static WORD WinWidth;	//窗口宽度
	static WORD WinHeight;	//窗口高度
	static DWORD* Pscreen;	//窗口内存指针
	static COLOR* Cscreen;	//窗口分量指针
	static HWND Hwnd;		//窗口句柄
public:
	TouchImage() {
		X = Y = 0;//初始化输出坐标
		C_BackColor.Color = D_BackColor = 0;//背景颜色
	};
	TouchImage(char* ResType,	//资源类型
		char* ResName		//资源名称
	) {
		X = Y = 0;//初始化输出坐标
		C_BackColor.Color = D_BackColor = 0;//背景颜色

		LoadImage(ResType, ResName);
	};
	TouchImage(char* ResType,	//资源类型
		DWORD ResID		//资源ID
	) {
		X = Y = 0;//初始化输出坐标
		C_BackColor.Color = D_BackColor = 0;//背景颜色

		char* ResName = MAKEINTRESOURCE(ResID);
		LoadImage(ResType, ResName);
	};
	inline static HWND Initgraph(WORD width = 640,	//绘图窗口宽度
		WORD height = 480,	//绘图窗口高度
		int flag = NULL		//绘图窗口样式
	)					//初始化绘图窗口
	{
		Hwnd = initgraph(WinWidth = width, WinHeight = height, flag);	//创建窗口
		Cscreen = (COLOR*)(Pscreen = GetImageBuffer(NULL));			//获取窗口内存及分量指针
		return Hwnd;											//返回句柄
	}
	inline static void Closegraph()	//关闭绘图窗口窗口
	{
		closegraph();			//关闭绘图窗口
	}
	void LoadImage(char* FileName,		//文件名称
		WORD width = 0,		//拉伸宽度
		WORD height = 0,		//拉伸高度
		bool bResize = false	//是否调整IMAGE的大小以适应图片
	)					//资源图片载入
	{
		if (width && height)											//指定高度宽度
			loadimage(&Img, FileName, width, height, bResize);	//读取图片
		else														//未指明高度宽度（之一）
			loadimage(&Img, FileName, bResize);				//读取图片
		Width = Img.getwidth();										//获取宽度
		Height = Img.getheight();										//获取高度
		Len = Width * Height;											//计算总像素数
		Cbuff = (COLOR*)(Pbuff = GetImageBuffer(&Img));					//获取内存及分量指针
	}
	void LoadImage(char* ResType,		//资源类型
		char* ResName,		//资源名称
		WORD width = 0,		//拉伸宽度
		WORD height = 0,		//拉伸高度
		bool bResize = false	//是否调整IMAGE的大小以适应图片
	)					//资源图片载入
	{
		if (width && height)											//指定高度宽度
			loadimage(&Img, ResType, ResName, width, height, bResize);	//读取图片
		else														//未指明高度宽度（之一）
			loadimage(&Img, ResType, ResName, bResize);				//读取图片
		Width = Img.getwidth();										//获取宽度
		Height = Img.getheight();										//获取高度
		Len = Width * Height;											//计算总像素数
		Cbuff = (COLOR*)(Pbuff = GetImageBuffer(&Img));					//获取内存及分量指针
	}
	void LoadImage(char* ResType,		//资源类型
		DWORD ResID,			//资源ID
		WORD width = 0,		//拉伸宽度
		WORD height = 0,		//拉伸高度
		bool bResize = false	//是否调整 IMAGE 的大小以适应图片
	)					//资源图片载入
	{
		char* ResName = MAKEINTRESOURCE(ResID);						//转换字符串
		if (width && height)//指定高度宽度
			loadimage(&Img, ResType, ResName, width, height, bResize);	//读取图片
		else														//未指明高度宽度（之一）
			loadimage(&Img, ResType, ResName, bResize);				//读取图片
		Width = Img.getwidth();										//获取宽度
		Height = Img.getheight();										//获取高度
		Len = Width * Height;											//计算总像素数
		Cbuff = (COLOR*)(Pbuff = GetImageBuffer(&Img));					//获取内存及分量指针
	}
	void SetAlpha(char* FileName)//设置透明度
	{
		IMAGE TmpMask;										//掩码图像
		loadimage(&TmpMask, FileName, Width, Height);	//读取图像
		COLOR* Ctmp = (COLOR*)GetImageBuffer(&TmpMask);		//获取分量指针
		DWORD p;											//循环变量
		for (p = 0; p < Len; p++)
			if (Ctmp[p].G < Alpha_Limit_0)						//全透明
				Cbuff[p].A = Alpha_FULL_Value;				//设置全透明
			else if (Ctmp[p].G < Alpha_Limit_1)				//半透明
				Cbuff[p].A = Alpha_HALF_Value;				//设置半透明
			else											//不透明
				Cbuff[p].A = Alpha_NULL_Value;				//设置不透明
	}
	void SetAlpha(char* ResType,	//资源类型
		char* ResName	//资源名称
	)				//设置透明度
	{
		IMAGE TmpMask;										//掩码图像
		loadimage(&TmpMask, ResType, ResName, Width, Height);	//读取图像
		COLOR* Ctmp = (COLOR*)GetImageBuffer(&TmpMask);		//获取分量指针
		DWORD p;											//循环变量
		for (p = 0; p < Len; p++)
			if (Ctmp[p].G < Alpha_Limit_0)						//全透明
				Cbuff[p].A = Alpha_FULL_Value;				//设置全透明
			else if (Ctmp[p].G < Alpha_Limit_1)				//半透明
				Cbuff[p].A = Alpha_HALF_Value;				//设置半透明
			else											//不透明
				Cbuff[p].A = Alpha_NULL_Value;				//设置不透明
	}
	void SetAlpha(char* ResType,	//资源类型
		DWORD ResID	//资源ID
	)				//设置透明度
	{
		char* ResName = MAKEINTRESOURCE(ResID);				//转换字符串
		IMAGE TmpMask;										//掩码图像
		loadimage(&TmpMask, ResType, ResName, Width, Height);	//读取图像
		COLOR* Ctmp = (COLOR*)GetImageBuffer(&TmpMask);		//获取分量指针
		DWORD p;											//循环变量
		for (p = 0; p < Len; p++)
			if (Ctmp[p].G < Alpha_Limit_0)						//全透明
				Cbuff[p].A = Alpha_FULL_Value;				//设置全透明
			else if (Ctmp[p].G < Alpha_Limit_1)				//半透明
				Cbuff[p].A = Alpha_HALF_Value;				//设置半透明
			else											//不透明
				Cbuff[p].A = Alpha_NULL_Value;				//设置不透明
	}
	void InitDefImage(char* ResType,			//资源类型
		char* ResName,			//资源名称
		WORD width = 0,			//拉伸宽度
		WORD height = 0,		//拉伸高度
		bool bResize = false	//是否调整IMAGE的大小以适应图片
	)
	{
		if (width && height)											//指定高度宽度
			loadimage(&Img, ResType, ResName, width, height, bResize);	//读取图片
		else														//未指明高度宽度（之一）
			loadimage(&Img, ResType, ResName, bResize);				//读取图片
		Width = Img.getwidth();										//获取宽度
		Height = Img.getheight();										//获取高度
		Len = Width * Height;											//计算总像素数
		Cbuff = (COLOR*)(Pbuff = GetImageBuffer(&Img));					//获取内存及分量指针

		static char ResMaskName[_MAX_PATH];
		char* Tmp = ResMaskName - 1;
		while (*++Tmp = *ResName++);
		*Tmp++ = '~';
		*Tmp = NULL;

		IMAGE TmpMask;										//掩码图像
		loadimage(&TmpMask, ResType, ResMaskName, Width, Height);//读取图像
		COLOR* Ctmp = (COLOR*)GetImageBuffer(&TmpMask);		//获取分量指针
		DWORD p;											//循环变量
		for (p = 0; p < Len; p++)
			if (Ctmp[p].G < Alpha_Limit_0)						//全透明
				Cbuff[p].A = Alpha_FULL_Value;				//设置全透明
			else if (Ctmp[p].G < Alpha_Limit_1)				//半透明
				Cbuff[p].A = Alpha_HALF_Value;				//设置半透明
			else											//不透明
				Cbuff[p].A = Alpha_NULL_Value;				//设置不透明
	};
	void Show(WORD x, WORD y, bool PosOnly = false)//显示图像
	{
		//设置输出位置
		X = x;
		Y = y;
		if (PosOnly)//只改变位置
			return;
		//输出宽度和高度
		WORD W = Width < WinWidth - X ? Width : WinWidth - X;//防止横向越界
		WORD H = Height < WinHeight - Y ? Height : WinHeight - Y;//防止纵向越界
		WORD i, j;//循环变量
		DWORD Pscr, Pimg;//为了加快输出速度，将偏移结果保存下来
		for (i = 0; i < W; i++)
			for (j = 0; j < H; j++)
			{
				Pscr = X + i + (Y + j) * WinWidth;//屏幕偏移地址
				Pimg = i + j * Width;//图像偏移地址
				if (Cbuff[Pimg].A >= Alpha_Limit_1)//不透明
					Pscreen[Pscr] = Pbuff[Pimg];
				else if (Cbuff[Pimg].A >= Alpha_Limit_0)//半透明
				{
					Cscreen[Pscr].R = (Cbuff[Pimg].R + Cscreen[Pscr].R) >> 1;
					Cscreen[Pscr].G = (Cbuff[Pimg].G + Cscreen[Pscr].G) >> 1;
					Cscreen[Pscr].B = (Cbuff[Pimg].B + Cscreen[Pscr].B) >> 1;
				}
				//全透明不输出
			}
	};
	void Show()//显示图像
	{
		//输出宽度和高度
		WORD W = Width < WinWidth - X ? Width : WinWidth - X;//防止横向越界
		WORD H = Height < WinHeight - Y ? Height : WinHeight - Y;//防止纵向越界
		WORD i, j;//循环变量
		DWORD Pscr, Pimg;//为了加快输出速度，将偏移结果保存下来
		for (i = 0; i < W; i++)
			for (j = 0; j < H; j++)
			{
				Pscr = X + i + (Y + j) * WinWidth;//屏幕偏移地址
				Pimg = i + j * Width;//图像偏移地址
				if (Cbuff[Pimg].A >= Alpha_Limit_1)//不透明
					Pscreen[Pscr] = Pbuff[Pimg];
				else if (Cbuff[Pimg].A >= Alpha_Limit_0)//半透明
				{
					Cscreen[Pscr].R = (Cbuff[Pimg].R + Cscreen[Pscr].R) >> 1;
					Cscreen[Pscr].G = (Cbuff[Pimg].G + Cscreen[Pscr].G) >> 1;
					Cscreen[Pscr].B = (Cbuff[Pimg].B + Cscreen[Pscr].B) >> 1;
				}
				//全透明不输出
			}
	};
	void TShow(WORD x, WORD y, BYTE Attrib)//不透明，半透明 显示图像
	{
		X = x;
		Y = y;

		//输出宽度和高度
		WORD W = Width < WinWidth - X ? Width : WinWidth - X;//防止横向越界
		WORD H = Height < WinHeight - Y ? Height : WinHeight - Y;//防止纵向越界
		WORD i, j;//循环变量
		DWORD Pscr, Pimg;//为了加快输出速度，将偏移结果保存下来
		if (Attrib == 0X00) {//全部不透明
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++)
					Pscreen[X + i + (Y + j) * WinWidth] = Pbuff[i + j * Width];
		}
		else if (Attrib == 0X01) {//全部半透明
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					Pscr = X + i + (Y + j) * WinWidth;//屏幕偏移地址
					Pimg = i + j * Width;//图像偏移地址
					Cscreen[Pscr].R = (Cbuff[Pimg].R + Cscreen[Pscr].R) >> 1;
					Cscreen[Pscr].G = (Cbuff[Pimg].G + Cscreen[Pscr].G) >> 1;
					Cscreen[Pscr].B = (Cbuff[Pimg].B + Cscreen[Pscr].B) >> 1;
				}
		}
		else if (Attrib == 0X02) {//局部不透明
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					if (Cbuff[i + j * Width].A > Alpha_Limit_0)//不是全透明
						Pscreen[X + i + (Y + j) * WinWidth] = Pbuff[i + j * Width];
				}
		}
		else if (Attrib == 0X03) {//局部半透明
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					Pscr = X + i + (Y + j) * WinWidth;//屏幕偏移地址
					Pimg = i + j * Width;//图像偏移地址
					if (Cbuff[Pimg].A >= Alpha_Limit_0)//不是全透明
					{
						Cscreen[Pscr].R = (Cbuff[Pimg].R + Cscreen[Pscr].R) >> 1;
						Cscreen[Pscr].G = (Cbuff[Pimg].G + Cscreen[Pscr].G) >> 1;
						Cscreen[Pscr].B = (Cbuff[Pimg].B + Cscreen[Pscr].B) >> 1;
					}
				}
		}
		else if (Attrib == 0X04) {//清除图像
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++)
					Pscreen[X + i + (Y + j) * WinWidth] = D_BackColor;//用背景色填充
		}
		else if (Attrib == 0X05) {//局部清除图像
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					Pscr = X + i + (Y + j) * WinWidth;//屏幕偏移地址
					Pimg = i + j * Width;//图像偏移地址
					if (Cbuff[Pimg].A >= Alpha_Limit_0)//不是全透明
						Pscreen[Pscr] = D_BackColor;//用背景色填充
				}
		}
	}
	void TShow(BYTE Attrib)//不透明，半透明 显示图像
	{
		//输出宽度和高度
		WORD W = Width < WinWidth - X ? Width : WinWidth - X;//防止横向越界
		WORD H = Height < WinHeight - Y ? Height : WinHeight - Y;//防止纵向越界
		WORD i, j;//循环变量
		DWORD Pscr, Pimg;//为了加快输出速度，将偏移结果保存下来
		if (Attrib == 0X00) {//全部不透明
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++)
					Pscreen[X + i + (Y + j) * WinWidth] = Pbuff[i + j * Width];
		}
		else if (Attrib == 0X01) {//全部半透明
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					Pscr = X + i + (Y + j) * WinWidth;//屏幕偏移地址
					Pimg = i + j * Width;//图像偏移地址
					Cscreen[Pscr].R = (Cbuff[Pimg].R + Cscreen[Pscr].R) >> 1;
					Cscreen[Pscr].G = (Cbuff[Pimg].G + Cscreen[Pscr].G) >> 1;
					Cscreen[Pscr].B = (Cbuff[Pimg].B + Cscreen[Pscr].B) >> 1;
				}
		}
		else if (Attrib == 0X02) {//局部不透明
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					if (Cbuff[i + j * Width].A > Alpha_Limit_0)//不是全透明
						Pscreen[X + i + (Y + j) * WinWidth] = Pbuff[i + j * Width];
				}
		}
		else if (Attrib == 0X03) {//局部半透明
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					Pscr = X + i + (Y + j) * WinWidth;//屏幕偏移地址
					Pimg = i + j * Width;//图像偏移地址
					if (Cbuff[Pimg].A >= Alpha_Limit_0)//不是全透明
					{
						Cscreen[Pscr].R = (Cbuff[Pimg].R + Cscreen[Pscr].R) >> 1;
						Cscreen[Pscr].G = (Cbuff[Pimg].G + Cscreen[Pscr].G) >> 1;
						Cscreen[Pscr].B = (Cbuff[Pimg].B + Cscreen[Pscr].B) >> 1;
					}
				}
		}
		else if (Attrib == 0X04) {//清除图像
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++)
					Pscreen[X + i + (Y + j) * WinWidth] = D_BackColor;//用背景色填充
		}
		else if (Attrib == 0X05) {//局部清除图像
			for (i = 0; i < W; i++)
				for (j = 0; j < H; j++) {
					Pscr = X + i + (Y + j) * WinWidth;//屏幕偏移地址
					Pimg = i + j * Width;//图像偏移地址
					if (Cbuff[Pimg].A >= Alpha_Limit_0)//不是全透明
						Pscreen[Pscr] = D_BackColor;//用背景色填充
				}
		}
	}
	static inline HWND GetHwnd() {//获得窗口句柄
		return Hwnd;
	}
	inline WORD width() {//获得图像宽度
		return Width;
	}
	inline WORD height() {//获得图像高度
		return Height;
	}
	static inline WORD WndWidth() {//获得窗口宽度
		return WinWidth;
	}
	static inline WORD WndHeight() {//获得窗口高度
		return WinHeight;
	}
	inline BYTE GetPointAlpha(WORD x, WORD y) {//获取图像指定点的透明度
		if (x < Width && y < Height)
			return Cbuff[x + y * Width].A;
		else
			return 0;
	}
	inline BYTE GetPointAlpha(DWORD pos) {//获取图像指定点的透明度
		if (pos < Len)
			return Cbuff[pos].A;
		else
			return 0;
	}
	inline BYTE At(WORD x, WORD y) {//检测指定点是否在图形内，返回透明度
		if (x > X&& y > Y&& x < X + Width && y < Y + Height)//在矩形内
			return Cbuff[x - X + (y - Y) * Width].A;
		else
			return false;
	}
	inline bool InRect(WORD x, WORD y) {//检测指定点是否矩形区域内
		return x > X&& y > Y&& x < X + Width && y < Y + Height;
	}
	inline bool IsAt(WORD x, WORD y) {//检测指定点是否在图形内
		if (x > X&& y > Y&& x < X + Width && y < Y + Height)//在矩形内
			return Cbuff[x - X + (y - Y) * Width].A > Alpha_Limit_0;
		else
			return 0;
	}
	inline void SetBKcolor(DWORD BKColor) {
		C_BackColor.Color = D_BackColor = BKColor;//背景颜色
	}
	inline void SetBKcolor(COLOR BKColor) {
		C_BackColor = BKColor;//背景颜色
		D_BackColor = C_BackColor.Color;
	}
	inline DWORD GetBKcolor() {
		return D_BackColor;
	}
	static inline void BatchDraw() {//开始批量绘图
		if (_IsBatchDraw = !_IsBatchDraw)//非批量绘图模式
			BeginBatchDraw();//开始批量绘图
		else//批量绘图模式
			EndBatchDraw();//开始批量绘图
	}
	static inline void FlushDraw() {//批量绘图
		FlushBatchDraw();//批量绘图
	}
	static inline void FlushDraw(WORD Left, WORD Top, WORD Right, WORD Bottom) {//批量绘图
		FlushBatchDraw(Left, Top, Right, Bottom);//开始批量绘图
	}
	inline void FlushLastDraw() {//批量绘图
		FlushBatchDraw(X, Y, X + Width, Y + Height);//开始批量绘图
	}
};
WORD	TouchImage::WinWidth = 640;	//设置窗口宽度初始值
WORD	TouchImage::WinHeight = 480;	//设置窗口高度初始值
HWND	TouchImage::Hwnd = NULL;	//设置窗口句柄初始值
DWORD* TouchImage::Pscreen = NULL;	//设置窗口内存指针初始值
COLOR* TouchImage::Cscreen = NULL;	//设置窗口分量指针初始值
bool	TouchImage::_IsBatchDraw = false;	//标记绘图模式

#undef Alpha_Limit_0
#undef Alpha_Limit_1
#undef Alpha_NULL_Value
#undef Alpha_HALF_Value
#undef Alpha_FULL_Value