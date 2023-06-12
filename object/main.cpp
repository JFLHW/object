#include <graphics.h>//图形库
#include <conio.h>//按键响应
#include<stdio.h>//标准库
#include<stdlib.h>//标准库
#include<windows.h>//window
#include<time.h>//时间
#include <math.h>//数学计算
#pragma comment( lib, "MSIMG32.LIB") 
#pragma comment(lib,"Winmm.lib")//音频的需要

IMAGE test,left,bk;

void transparentimage(IMAGE* dstimg, int xt, int yt, int wt, int ht, int xo, int yo, int wo, int ho, IMAGE* srcimg);
void getInput();
void load();//载入所有图片
void change();
//void donghua(int* p_head, int* p_cur, int* x, int y, struct _peo* p_peo, IMAGE* p_img);
void donghua(struct _peo* p_peo,  int* p_xsum, int xo, int yo, int* p_nowo, int* p_a, int ho, IMAGE* srcimg);//int wt, int ht,

enum
{
	STAND=1,
	REST,
	WALK,
	RUN,
	ATTACK,
	ATTACK1,
	ATTACK2
};
struct _peo//角色我，敌人
{
	float x, y;//位置
	float w, h;//宽高
	float life;//生命
	int sta;
	int fangx;//方向左右两个。。。。。。。。。。。。。。所有都是二叉树的话
	int isinner;//在屏幕内是否
	int boom_time = 0;//爆炸时间
	int speed;
	int dir = 1;
	int box = 0, boy = 0;
	int shoujitime;
};
struct _peo play;

DWORD curTime = GetTickCount();
#define CHECK_INTERVAL 150 // 检测时间间隔（毫秒）
bool keyPressed = false; // 记录按键状态 DWORD 
DWORD lastCheckTime = 0; // 上次检测时间


void transparentimage(IMAGE* dstimg, int xt, int yt, int wt, int ht,
	int xo, int yo, int wo, int ho, IMAGE* srcimg) 
{
	//xt yt为在控制台窗口哪个坐标显示图片,wt ht为在控制台窗口坐标点显示的图片的宽高,///
	//xo yo 我们加载进去的图片的坐标点，wo ho我们加载进去的图片宽高
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	//int w = srcimg->getwidth();
	//int h = srcimg->getheight(); 	// 结构体的第三个成员表示额外的透明度，0 表示全透明，255 表示不透明。 	
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA }; 	// 使用 Windows GDI 函数实现半透明位图 	
	AlphaBlend(dstDC, xt, yt, wt, ht, srcDC, xo, yo, wo, ho, bf);
}
void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, 100, 300, srcDC, 700, 0, 100, h, bf);//以加载的图片大小为准。绘图串口哪个地方显示，显示多大。图片截取哪部分
}
int main()
{
	load();
	int size = 56;
	int size_walk = 272;
	int size_walk_left = 755;
	int size_attack_shou_r[4] = { 65,100 }; int x_pos_shou=0; int p_shou_xiabiao = 0;//攻击/存每张图大小，图片中x位置，第几个下标
	int size_attack_shou_r1[14] = { 53,98,60,57,122,50 }; int x_pos_shou1 = 167; int p_shou_xiabiao1 = 0;//攻击/存每张图大小，图片中x位置，第几个下标
	int size_attack_shou_r2[14] = { 55,69,63,127,50,98,73,83 }; int x_pos_shou2 = 0; int p_shou_xiabiao2 = 0;


	initgraph(1200,600);// 绘图窗口初始化
	
	
	BeginBatchDraw();
	while(1)
	{
		cleardevice();//清屏
		transparentimage(NULL, 0, 0, 1200, 600, 0, 0, 1220, 216, &bk);


		change();//选择状态

		switch (play.sta)
		{
			case STAND:
				transparentimage(NULL, play.x, play.y, 150, 240, 3, 8, 54, 80, &test);
				break;
			case REST:
				transparentimage(NULL, play.x, play.y, 150, 270, size, 0, 54, 90, &test);//动作1
				size += 53;

				if (size == 268)
				{
					size = 56;
					play.sta = STAND;//条件1动画播放完出去

				}
				break;
			case WALK:
				
				if (GetAsyncKeyState('D') & 0x8000)
				{
					play.x += 17;//动作1
					transparentimage(NULL, play.x, play.y, 150, 240, size_walk, 4, 55, 80, &test);//动作2
					size_walk += 56;
					if (size_walk == 776)
					{
						size_walk = 272;

					}
				}
				else if (GetAsyncKeyState('A') & 0x8000)
				{
					play.x -= 17;//动作1
					transparentimage(NULL, play.x, play.y, 150, 240, size_walk_left, 4, 55, 80, &left);//动作2
					size_walk_left -= 55;
					if (size_walk_left == 260)
					{
						size_walk_left = 755;

					}
				}
				
				play.sta = STAND;//条件1没按就出去
				break;
			case ATTACK:
				/*transparentimage(NULL, play.x, play.y, 150, 270, 3, 0, 54, 90, &test);
				size_attack_shou_r += 53;

				if (size_attack_shou_r == 268)
				{
					size_attack_shou_r = 56;
					play.sta = STAND;//条件1动画播放完出去
				}*/

				//donghua(size_attack_shou_r, &p_shou_xiabiao, &x_pos_shou,460, &play, &test);
				donghua(&play,  &x_pos_shou, 0,460, &p_shou_xiabiao,size_attack_shou_r, 80, &test);//150, 270,
				break;

			case ATTACK1:
				donghua(&play, &x_pos_shou1, 167, 460, &p_shou_xiabiao1, size_attack_shou_r1, 80, &test);//150, 270,
				break;
			case ATTACK2:
				donghua(&play, &x_pos_shou2, 0,285, &p_shou_xiabiao2, size_attack_shou_r2, 80, &test);//150, 270,
				break;
		}
		
		Sleep(150);
		FlushBatchDraw();
	}

	EndBatchDraw();
	_getch();
	closegraph();
	return 0;
}
void getInput()//我方按键获取
{
	////if (GetAsyncKeyState('D') & 0x8000 && plane.y > 0)//我方飞机按键
	//{
	//	play.x += 2;
	//}
	
}
void load()
{
	loadimage(&test, _T("./img./51.png"), 1086, 2478);
	loadimage(&left, _T("./img./left.png"), 1086, 2478);
	loadimage(&bk, _T("./img./bk.png"), 3662, 462);
	play.sta = 1;
	play.x = 0; play.y = 250;
}
void change()
{
	//play.sta = STAND;
	if (GetAsyncKeyState('H') & 0x8000)
	{
		play.sta = REST;
	}
	if (GetAsyncKeyState('D') & 0x8000|| GetAsyncKeyState('A') & 0x8000)
	{
		play.sta = WALK;
	}
	if (GetAsyncKeyState('K') & 0x8000)
	{
		play.sta = ATTACK;
	}
	if (GetAsyncKeyState('L') & 0x8000)
	{
		play.sta = ATTACK1;
	}
	if (GetAsyncKeyState('M') & 0x8000)
	{
		play.sta = ATTACK2;
	}
}
/*void donghua(int* p_head, int* p_cur, int* x, int y, struct _peo* p_peo, IMAGE* p_img)//适用动画结束退到初始状态
{
	transparentimage(NULL, p_peo->x, p_peo->y, 150, 270, *x, y, p_head[*p_cur], 90, p_img);//动作2//高度暂时都是90
	*x += p_head[*p_cur];//图片x坐标变
	*p_cur+=1;//下标加一
	if (p_head[*p_cur] ==0)//动画结束
	{
		*p_cur = 0;
		*x = 0;//////
		p_peo->sta = STAND;
	}
}*/

//int a[] = {22,0 }, x_sum=a[0], no_xiabian=0;//存每张图大小，图片中x位置，第几个下标
void donghua(struct _peo* p_peo,      int *p_xsum, int xo,int yo,   int *p_nowo,int *p_a,   int ho, IMAGE* srcimg)////////////////////////////
{
	//int wt, int ht,
	//3*p_a[*p_nowo], 3*ho
	transparentimage(NULL,p_peo->x, p_peo->y,  3*p_a[*p_nowo],  3*ho, *p_xsum, yo, p_a[*p_nowo], ho, srcimg);//动作2//高度暂时都是90
	*p_xsum += p_a[*p_nowo];
	(* p_nowo)+=1;
	
	if (p_a[*p_nowo] == 0)
	{
		*p_nowo = 0;
		*p_xsum = xo;
		p_peo->sta = STAND;
	}
}

