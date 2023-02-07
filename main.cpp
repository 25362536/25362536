#define _CRT_SECURE_NO_WARNINGS 1
#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#define HEIGHT 30
#define WIDTH 40
#define BLOCK_SIZE 20

int Blocks[HEIGHT][WIDTH] = { 0 };//全局变量
char moveDirection;//
int food_i, food_j;//
int isFailure = 0;//

void startup()
{
	int i;
	Blocks[HEIGHT / 2][WIDTH / 2] = 1;
	for (i = 1; i <= 4; i++)
		Blocks[HEIGHT / 2][WIDTH / 2 - i] = i + 1;
	moveDirection = 'd';
	food_i = rand() % (HEIGHT - 5) + 2;
	food_j = rand() % (WIDTH - 5) + 2;
	initgraph(WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE);
	setlinecolor(RGB(200, 200, 200));
	BeginBatchDraw();
}


void moveSnake()
{
	int i, j;
	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++)
			if (Blocks[i][j] > 0)
				Blocks[i][j]++;
	int oldTail_i, oldTail_j, oldHead_i, oldHead_j;//定义蛇头和蛇尾
	int max = 0;//用于记录最大值
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (max < Blocks[i][j])
			{
				max = Blocks[i][j];
				oldTail_i = i;//
				oldTail_j = j;
			}
			if (Blocks[i][j] == 2)
			{
				oldHead_i = i;
				oldHead_j = j;
			}
		}
	}
	int newHead_i = oldHead_i;
	int newHead_j = oldHead_j;//记录新的蛇头位置并储存
	//控制上下左右移动
	if (moveDirection == 'w')
		newHead_i = oldHead_i - 1;
	else if (moveDirection == 's')
		newHead_i = oldHead_i + 1;
	else if (moveDirection == 'a')
		newHead_j = oldHead_j - 1;
	else if (moveDirection == 'd')
		newHead_j = oldHead_j + 1;
	if (newHead_i >= HEIGHT || newHead_i < 0
		|| newHead_j >= WIDTH || newHead_j < 0
		|| Blocks[newHead_i][newHead_j] > 0)
	{
		isFailure = 1;
		return;
	}
	Blocks[newHead_i][newHead_j] = 1;
	if (newHead_i == food_i && newHead_j == food_j)
	{
		food_i = rand() % (HEIGHT - 5) + 2;
		food_j = rand() % (WIDTH - 5) + 2;
	}
	else
		Blocks[oldTail_i][oldTail_j] = 0;
}

void show()
{
	cleardevice();//清屏
	int i = 0;
	int j = 0;
	//制作二维数组
	for (i= 0;i<HEIGHT;i++)
	{
		for ( j = 0; j < WIDTH; j++)
		{
			if (Blocks[i][j] > 0)//元素大于0表示蛇，
				setfillcolor(HSVtoRGB(Blocks[i][j] * 10, 0.9, 1)); //
			else
				setfillcolor(RGB(150, 150, 150));//
			//在对应位置绘制不同颜色的小方格
			fillrectangle(j * BLOCK_SIZE, i * BLOCK_SIZE,
				(j + 1) * BLOCK_SIZE, (i + 1) * BLOCK_SIZE);
		}
	}
	setfillcolor(RGB(0, 255, 0));//食物为绿色
	//绘制食物小方格
	fillrectangle(food_j * BLOCK_SIZE, food_i * BLOCK_SIZE,
		(food_j + 1) * BLOCK_SIZE, (food_i + 1) * BLOCK_SIZE);
	if (isFailure) //
	{
		setbkmode(TRANSPARENT);//文字透明
		settextcolor(RGB(255, 0, 0));//设置文字颜色
		settextstyle(80, 0, _T("宋体"));//设置文字的样式
		outtextxy(240, 220, _T("game over"));//输出内容
	}
	FlushBatchDraw();
}
void updateWithoutInput()
{
	if (isFailure)//如果游戏失败，直接返回
		return;
	static int waitIndex = 1;//静态局部变量，初始化为1
	waitIndex++;//每一帧+1
	if (waitIndex == 10)//小蛇每隔10帧猜移动一次
	{
		moveSnake();//
		waitIndex = 1;
	}
}

void updateWithIput()
{
	if(kbhit() && isFailure == 0)//如果有按键输入，执行相对的指令
	{
		char input = getch();//获得按键输入
		if (input == 'a' || input == 's'
			|| input == 'd' || input == 'w')//asdw控制小蛇移动
		{
			moveDirection = input;//设定移动方向
			moveSnake();//调用移动小蛇函数
		}
	}
}

int main()
{

		startup();
		
		while (1)
		{
			show();//绘制地图
			updateWithoutInput();//输入无关变量的处理
			updateWithIput();//输入有关变量的处理
		}

	return 0;
}