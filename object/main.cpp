#include <graphics.h>//ͼ�ο�
#include <conio.h>//������Ӧ
#include<stdio.h>//��׼��
#include<stdlib.h>//��׼��
#include<windows.h>//window
#include<time.h>//ʱ��
#include <math.h>//��ѧ����
#pragma comment( lib, "MSIMG32.LIB") 
#pragma comment(lib,"Winmm.lib")//��Ƶ����Ҫ

IMAGE test,left,bk;

void transparentimage(IMAGE* dstimg, int xt, int yt, int wt, int ht, int xo, int yo, int wo, int ho, IMAGE* srcimg);
void getInput();
void load();//��������ͼƬ
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
struct _peo//��ɫ�ң�����
{
	float x, y;//λ��
	float w, h;//���
	float life;//����
	int sta;
	int fangx;//�������������������������������������������ж��Ƕ������Ļ�
	int isinner;//����Ļ���Ƿ�
	int boom_time = 0;//��ըʱ��
	int speed;
	int dir = 1;
	int box = 0, boy = 0;
	int shoujitime;
};
struct _peo play;

DWORD curTime = GetTickCount();
#define CHECK_INTERVAL 150 // ���ʱ���������룩
bool keyPressed = false; // ��¼����״̬ DWORD 
DWORD lastCheckTime = 0; // �ϴμ��ʱ��


void transparentimage(IMAGE* dstimg, int xt, int yt, int wt, int ht,
	int xo, int yo, int wo, int ho, IMAGE* srcimg) 
{
	//xt ytΪ�ڿ���̨�����ĸ�������ʾͼƬ,wt htΪ�ڿ���̨�����������ʾ��ͼƬ�Ŀ��,///
	//xo yo ���Ǽ��ؽ�ȥ��ͼƬ������㣬wo ho���Ǽ��ؽ�ȥ��ͼƬ���
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	//int w = srcimg->getwidth();
	//int h = srcimg->getheight(); 	// �ṹ��ĵ�������Ա��ʾ�����͸���ȣ�0 ��ʾȫ͸����255 ��ʾ��͸���� 	
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA }; 	// ʹ�� Windows GDI ����ʵ�ְ�͸��λͼ 	
	AlphaBlend(dstDC, xt, yt, wt, ht, srcDC, xo, yo, wo, ho, bf);
}
void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, 100, 300, srcDC, 700, 0, 100, h, bf);//�Լ��ص�ͼƬ��СΪ׼����ͼ�����ĸ��ط���ʾ����ʾ���ͼƬ��ȡ�Ĳ���
}
int main()
{
	load();
	int size = 56;
	int size_walk = 272;
	int size_walk_left = 755;
	int size_attack_shou_r[4] = { 65,100 }; int x_pos_shou=0; int p_shou_xiabiao = 0;//����/��ÿ��ͼ��С��ͼƬ��xλ�ã��ڼ����±�
	int size_attack_shou_r1[14] = { 53,98,60,57,122,50 }; int x_pos_shou1 = 167; int p_shou_xiabiao1 = 0;//����/��ÿ��ͼ��С��ͼƬ��xλ�ã��ڼ����±�
	int size_attack_shou_r2[14] = { 55,69,63,127,50,98,73,83 }; int x_pos_shou2 = 0; int p_shou_xiabiao2 = 0;


	initgraph(1200,600);// ��ͼ���ڳ�ʼ��
	
	
	BeginBatchDraw();
	while(1)
	{
		cleardevice();//����
		transparentimage(NULL, 0, 0, 1200, 600, 0, 0, 1220, 216, &bk);


		change();//ѡ��״̬

		switch (play.sta)
		{
			case STAND:
				transparentimage(NULL, play.x, play.y, 150, 240, 3, 8, 54, 80, &test);
				break;
			case REST:
				transparentimage(NULL, play.x, play.y, 150, 270, size, 0, 54, 90, &test);//����1
				size += 53;

				if (size == 268)
				{
					size = 56;
					play.sta = STAND;//����1�����������ȥ

				}
				break;
			case WALK:
				
				if (GetAsyncKeyState('D') & 0x8000)
				{
					play.x += 17;//����1
					transparentimage(NULL, play.x, play.y, 150, 240, size_walk, 4, 55, 80, &test);//����2
					size_walk += 56;
					if (size_walk == 776)
					{
						size_walk = 272;

					}
				}
				else if (GetAsyncKeyState('A') & 0x8000)
				{
					play.x -= 17;//����1
					transparentimage(NULL, play.x, play.y, 150, 240, size_walk_left, 4, 55, 80, &left);//����2
					size_walk_left -= 55;
					if (size_walk_left == 260)
					{
						size_walk_left = 755;

					}
				}
				
				play.sta = STAND;//����1û���ͳ�ȥ
				break;
			case ATTACK:
				/*transparentimage(NULL, play.x, play.y, 150, 270, 3, 0, 54, 90, &test);
				size_attack_shou_r += 53;

				if (size_attack_shou_r == 268)
				{
					size_attack_shou_r = 56;
					play.sta = STAND;//����1�����������ȥ
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
void getInput()//�ҷ�������ȡ
{
	////if (GetAsyncKeyState('D') & 0x8000 && plane.y > 0)//�ҷ��ɻ�����
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
/*void donghua(int* p_head, int* p_cur, int* x, int y, struct _peo* p_peo, IMAGE* p_img)//���ö��������˵���ʼ״̬
{
	transparentimage(NULL, p_peo->x, p_peo->y, 150, 270, *x, y, p_head[*p_cur], 90, p_img);//����2//�߶���ʱ����90
	*x += p_head[*p_cur];//ͼƬx�����
	*p_cur+=1;//�±��һ
	if (p_head[*p_cur] ==0)//��������
	{
		*p_cur = 0;
		*x = 0;//////
		p_peo->sta = STAND;
	}
}*/

//int a[] = {22,0 }, x_sum=a[0], no_xiabian=0;//��ÿ��ͼ��С��ͼƬ��xλ�ã��ڼ����±�
void donghua(struct _peo* p_peo,      int *p_xsum, int xo,int yo,   int *p_nowo,int *p_a,   int ho, IMAGE* srcimg)////////////////////////////
{
	//int wt, int ht,
	//3*p_a[*p_nowo], 3*ho
	transparentimage(NULL,p_peo->x, p_peo->y,  3*p_a[*p_nowo],  3*ho, *p_xsum, yo, p_a[*p_nowo], ho, srcimg);//����2//�߶���ʱ����90
	*p_xsum += p_a[*p_nowo];
	(* p_nowo)+=1;
	
	if (p_a[*p_nowo] == 0)
	{
		*p_nowo = 0;
		*p_xsum = xo;
		p_peo->sta = STAND;
	}
}

