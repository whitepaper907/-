#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <Windows.h> // 추가
#include <time.h> // 추가

//*********************************
//상수 선언
//*********************************

#define EXT_KEY			0xffffffe0	//확장키 인식값 
#define KEY_LEFT		0x4b
#define KEY_RIGHT		0x4d
#define KEY_UP			0x48
#define KEY_DOWN		0x50

//*********************************
//구조체 선언
//*********************************
struct STAGE{		//각 스테이지마다의 난이도 설정
	int	speed;	//숫자가 낮을수록 속도가 빠르다
	int stick_rate;		//막대가 나오는 확률 0~99 , 99면 막대기만 나옴
	int clear_line;
};

enum { 
		BLACK,      /*  0 : 까망 */ 
		DARK_BLUE,    /*  1 : 어두운 파랑 */ 
		DARK_GREEN,    /*  2 : 어두운 초록 */ 
		DARK_SKY_BLUE,  /*  3 : 어두운 하늘 */ 
		DARK_RED,    /*  4 : 어두운 빨강 */ 
		DARK_VOILET,  /*  5 : 어두운 보라 */ 
		DARK_YELLOW,  /*  6 : 어두운 노랑 */ 
		GRAY,      /*  7 : 회색 */ 
		DARK_GRAY,    /*  8 : 어두운 회색 */ 
		BLUE,      /*  9 : 파랑 */ 
		GREEN,      /* 10 : 초록 */ 
		SKY_BLUE,    /* 11 : 하늘 */ 
		RED,      /* 12 : 빨강 */ 
		VOILET,      /* 13 : 보라 */ 
		YELLOW,      /* 14 : 노랑 */ 	
		WHITE,      /* 15 : 하양 */ 	
}; 

//*********************************
//전역변수선언
//*********************************
int level;
int ab_x,ab_y;	//화면중 블럭이 나타나는 좌표의 절대위치
int block_shape,block_angle,block_x,block_y;
int next_block_shape;
int score;
int lines;
char total_block[21][14];		//화면에 표시되는 블럭들
struct STAGE stage_data[10];
char block[7][4][4][4]={
	//막대모양
	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,	1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,	1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,

	//네모모양
	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,

	//'ㅓ' 모양
	0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,	1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,	0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,

	//'ㄱ'모양
	1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,	1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,	0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,

	//'ㄴ' 모양
	1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,	1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,	0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,	1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,

	//'Z' 모양
	1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,	0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,	1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,	0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,

	//'S' 모양
	0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,	0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0

};
//*********************************
//함수 선언
//*********************************
int gotoxy(int x,int y);	//커서옮기기
void SetColor(int color);	//색표현
int init();					//각종변수 초기화
int show_cur_block(int shape,int angle,int x,int y);	//진행중인 블럭을 화면에 표시한다
int erase_cur_block(int shape,int angle,int x,int y);	//블럭 진행의 잔상을 지우기 위한 함수
int show_total_block();	//쌓여져있는 블럭을 화면에 표시한다.
int show_next_block(int shape);
int make_new_block();	//return값으로 block의 모양번호를 알려줌
int strike_check(int shape,int angle,int x,int y);	//블럭이 화면 맨 아래에 부닥쳤는지 검사 부닥치면 1을리턴 아니면 0리턴
int merge_block(int shape,int angle,int x,int y);	//블럭이 바닥에 닿았을때 진행중인 블럭과 쌓아진 블럭을 합침
int block_start(int shape,int* angle,int* x,int* y);	//블럭이 처음 나올때 위치와 모양을 알려줌
int move_block(int* shape,int* angle,int* x,int* y,int* next_shape);	//게임오버는 1을리턴 바닥에 블럭이 닿으면 2를 리턴
int rotate_block(int shape,int* angle,int* x,int* y);
int show_gameover();
int show_gamestat();
int show_logo();
int input_data();
int check_full_line();
char menuSelect(int cnt);
void setMenu(int cnt);
void setCursorView(bool visible);


int main(int argc, char* argv[])
{
	int i;
	int x;
	int is_gameover=0;
	char keytemp;
	init();
	show_logo();
	while(1)
	{
		
		input_data();
		show_total_block();
		block_shape = make_new_block();
		next_block_shape = make_new_block();
		show_next_block(next_block_shape);
		block_start(block_shape,&block_angle,&block_x,&block_y);
		show_gamestat();
		for(i=1;1;i++)
		{
			if(_kbhit())
			{
				keytemp = _getch();
				if(keytemp == EXT_KEY)
				{
					keytemp = _getch();
					switch(keytemp)
					{
					case KEY_UP:		//회전하기
						if (strike_check(block_shape, (block_angle + 1) % 4, block_x, block_y) == 0)
						{
							erase_cur_block(block_shape, block_angle, block_x, block_y);
							block_angle = (block_angle + 1) % 4;
							show_cur_block(block_shape, block_angle, block_x, block_y);
						}
						else {
							for (int k = 0; k < 4; k++) {
								if ((block_x + k == 13)) {									
									erase_cur_block(block_shape, block_angle, block_x, block_y);
									if (block_shape == 0) block_x -= 3;
									else block_x--;
									block_angle = (block_angle + 1) % 4;
									show_cur_block(block_shape, block_angle, block_x, block_y);
									break;
								}
							}
						}
						break;
					case KEY_LEFT:		//왼쪽으로 이동
						if(block_x>1)
						{
							erase_cur_block(block_shape,block_angle,block_x,block_y);
							block_x--;
							if(strike_check(block_shape,block_angle,block_x,block_y) == 1)
								block_x++;
							
							show_cur_block(block_shape,block_angle,block_x,block_y);
						}
						break;
					case KEY_RIGHT:		//오른쪽으로 이동
						if(block_x<14)
						{
							erase_cur_block(block_shape,block_angle,block_x,block_y);
							block_x++;
							if(strike_check(block_shape,block_angle,block_x,block_y) == 1)
								block_x--;
							show_cur_block(block_shape,block_angle,block_x,block_y);
						}
						break;
					case KEY_DOWN:		//아래로 이동
						is_gameover = move_block(&block_shape,&block_angle,&block_x,&block_y,&next_block_shape);
						show_cur_block(block_shape,block_angle,block_x,block_y);
						break;
					}
				}
				if(keytemp == 32 )	//스페이스바를 눌렀을때
				{
					while(is_gameover == 0)
					{
						is_gameover = move_block(&block_shape,&block_angle,&block_x,&block_y,&next_block_shape);
					}
					show_cur_block(block_shape,block_angle,block_x,block_y);
				}
			}

			if(i%stage_data[level].speed == 0)
			{
				is_gameover = move_block(&block_shape,&block_angle,&block_x,&block_y,&next_block_shape);
				show_cur_block(block_shape,block_angle,block_x,block_y);
			}
			

			if(is_gameover == 1)
			{
				show_gameover();
				SetColor(GRAY);
				is_gameover = 0;
				score = 0;
				break;
			}
			
			gotoxy(77,23);
			Sleep(15);
			gotoxy(77,23);
		}
		init();
	}
	return 0;
}

int gotoxy(int x,int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
	COORD pos; 
	pos.Y=y;
	pos.X=x;
	SetConsoleCursorPosition(hConsole, pos); 
	return 0;
}

void SetColor(int color) 

{ 
	static HANDLE std_output_handle=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(std_output_handle, color); 
	
} 

int init()
{
	int i,j;

	srand((unsigned)time(NULL));
	
	
	for(i=0;i<20;i++)
	{
		for(j=0;j<14;j++)
		{
			if((j==0) || (j==13))
			{
				total_block[i][j]=1;
			}else{
				total_block[i][j]=0;
			}
		}
	}

	for(j=0;j<14;j++)			//화면의 제일 밑의 줄은 1로 채운다.
		total_block[20][j]=1;
	
	//전역변수 초기화
	level=0;
	lines=0;
	ab_x = 5;
	ab_y = 1;

	stage_data[0].speed=40;
	stage_data[0].stick_rate=20;	
	stage_data[0].clear_line=2;
	stage_data[1].speed=38;
	stage_data[1].stick_rate=18;
	stage_data[1].clear_line=2;
	stage_data[2].speed=35;
	stage_data[2].stick_rate=18;
	stage_data[2].clear_line=2;
	stage_data[3].speed=30;
	stage_data[3].stick_rate=17;
	stage_data[3].clear_line=20;
	stage_data[4].speed=25;
	stage_data[4].stick_rate=16;
	stage_data[4].clear_line=20;
	stage_data[5].speed=20;
	stage_data[5].stick_rate=14;
	stage_data[5].clear_line=20;
	stage_data[6].speed=15;
	stage_data[6].stick_rate=14;
	stage_data[6].clear_line=20;
	stage_data[7].speed=10;
	stage_data[7].stick_rate=13;
	stage_data[7].clear_line=20;
	stage_data[8].speed=6;
	stage_data[8].stick_rate=12;
	stage_data[8].clear_line=20;
	stage_data[9].speed=4;
	stage_data[9].stick_rate=11;
	stage_data[9].clear_line=99999;
	return 0;
}

int show_cur_block(int shape,int angle,int x,int y)
{
	int i,j;
	
	switch(shape)
	{
	case 0:
		SetColor(RED);
		break;
	case 1:
		SetColor(BLUE);
		break;
	case 2:
		SetColor(SKY_BLUE);
		break;
	case 3:
		SetColor(WHITE);
		break;
	case 4:
		SetColor(YELLOW);
		break;
	case 5:
		SetColor(VOILET);
		break;
	case 6:
		SetColor(GREEN);
		break;
	}

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if( (j+y) <0)
				continue;

			if(block[shape][angle][j][i] == 1)
			{
				gotoxy((i+x)*2+ab_x,j+y+ab_y);
				printf("■");
			}
		}
	}
	SetColor(BLACK);
	gotoxy(77,23);
	return 0;
}

int erase_cur_block(int shape,int angle,int x,int y)
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(block[shape][angle][j][i] == 1)
			{
				gotoxy((i+x)*2+ab_x,j+y+ab_y);
				printf("  ");
				//break;
				
			}
		}
	}
	return 0;	
}

int show_total_block()
{
	int i,j;
	SetColor(DARK_GRAY);
	for(i=0;i<21;i++)
	{
		for(j=0;j<14;j++)
		{
			if(j==0 || j==13 || i==20)		//레벨에 따라 외벽 색이 변함
			{
				SetColor((level %6) +1);
				
			}else{
				SetColor(DARK_GRAY);
			}
			gotoxy( (j*2)+ab_x, i+ab_y );
			if(total_block[i][j] == 1)
			{	
				printf("■");
			}else{
				printf("  ");
			}
			
		}
	}
	SetColor(BLACK);
	gotoxy(77,23);
	return 0;
}

int make_new_block()
{
	int shape;
	int i;
	i=rand()%100;
	if(i<=stage_data[level].stick_rate)		//막대기 나올확률 계산
		return 0;							//막대기 모양으로 리턴

	shape = (rand()%6)+1;		//shape에는 1~6의 값이 들어감
	show_next_block(shape);
	return shape;
}


int strike_check(int shape,int angle,int x,int y)
{
	int i,j;
	int block_dat;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if (((x + j) == 0) || ((x + j) == 13)) {
				block_dat = 1;
			}
			else
				block_dat = total_block[y+i][x+j];
			
			
			if((block_dat == 1) && (block[shape][angle][i][j] == 1))																							//좌측벽의 좌표를 빼기위함
			{
				return 1;
			}
		}
	}
	return 0;
}

int merge_block(int shape,int angle,int x,int y)
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if (y + i >= 0) // 추가함
				total_block[y+i][x+j] |=  block[shape][angle][i][j];
		}
	}
	check_full_line();
	show_total_block();
	
	return 0;
}

int block_start(int shape,int* angle,int* x,int* y)
{
	
	*x = 5;
	*y = -3;
	*angle = 0;
	return 0;	
}

int show_gameover()
{
	SetColor(RED);
	gotoxy(15,8);
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(15,9);
	printf("┃**************************┃");
	gotoxy(15,10);
	printf("┃*        GAME OVER       *┃");
	gotoxy(15,11);
	printf("┃**************************┃");
	gotoxy(15,12);
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
	fflush(stdin);
	Sleep(1000);
	
	_getch();
	system("cls");

	return 0;
}

int move_block(int* shape,int* angle,int* x,int* y,int* next_shape)
{
	erase_cur_block(*shape,*angle,*x,*y);
	
	(*y)++;	//블럭을 한칸 아래로 내림
	if(strike_check(*shape,*angle,*x,*y) == 1)
	{
		(*y)--;
		merge_block(*shape,*angle,*x,*y);
		for (int k = 1; k < 13; k++) {
			if (total_block[0][k] == 1)
				return 1;
		}
		*shape = *next_shape;
		*next_shape = make_new_block();
		
		block_start(*shape,angle,x,y);	//angle,x,y는 포인터임
		show_next_block(*next_shape);
		return 2;
	}
	return 0;
}

int rotate_block(int shape,int* angle,int* x,int* y)
{
	return 0;
}

int check_full_line()
{
	int i,j,k;
	for(i=0;i<20;i++)
	{
		for(j=1;j<13;j++)
		{
			if(total_block[i][j] == 0)
				break;
		}
		if(j == 13)	//한줄이 다 채워졌음
		{
			lines++;
			show_total_block();
			SetColor(BLUE);
			gotoxy(1*2+ab_x,i+ab_y);
			for(j=1;j<13;j++)
			{
				printf("□");
				Sleep(10);
			}
			gotoxy(1*2+ab_x,i+ab_y);
			for(j=1;j<13;j++)
			{
				printf("  ");
				Sleep(10);
			}

			for(k=i;k>0;k--)
			{
				for(j=1;j<13;j++)
					total_block[k][j] = total_block[k-1][j];
			}
			for(j=1;j<13;j++)
				total_block[0][j] = 0;
			score+= 100+(level*10) + (rand()%10);
			show_gamestat();
			if (stage_data[level].clear_line <= lines)	//클리어 스테이지 // == 에서 <=로 바꿈
			{
				level++;
				lines = 0;
				show_gamestat(); // 추가해줌
				show_total_block();
			}
		}
		
	}
	return 0;
}

int show_next_block(int shape)
{
	int i,j;
	SetColor((level+1)%6+1);
	for(i=1;i<7;i++)
	{
		gotoxy(33,i);
		for(j=0;j<6;j++)
		{
			if(i==1 || i==6 || j==0 || j==5)
			{
				printf("■");				
			}else{
				printf("  ");
			}

		}
	}
	show_cur_block(shape,0,15,1);
	return 0;
}

int show_gamestat()
{
	static int printed_text=0;
	SetColor(GRAY);
	if(printed_text == 0)
	{
		gotoxy(35,7);
		printf("STAGE");

		gotoxy(35,9);
		printf("SCORE");

		gotoxy(35,12);
		printf("LINES");
		

	}
	gotoxy(41,7);
	printf("%d",level+1);
	gotoxy(35,10);
	printf("%10d",score);
	gotoxy(35,13);
	printf("%10d",stage_data[level].clear_line - lines);
	return 0;
}

int input_data()      //*********
{
	int i = 0;
	SetColor(GRAY);
	gotoxy(10, 7);
	printf("┏━━━━━━━━━<GAME KEY>━━━━━━━━━┓");
	Sleep(10);
	gotoxy(10, 8);
	printf("┃ UP   : Rotate Block        ┃");
	Sleep(10);
	gotoxy(10, 9);
	printf("┃ DOWN : Move One-Step Down  ┃");
	Sleep(10);
	gotoxy(10, 10);
	printf("┃ SPACE: Move Bottom Down    ┃");
	Sleep(10);
	gotoxy(10, 11);
	printf("┃ LEFT : Move Left           ┃");
	Sleep(10);
	gotoxy(10, 12);
	printf("┃ RIGHT: Move Right          ┃");
	Sleep(10);
	gotoxy(10, 13);
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");


	gotoxy(10, 3);
	printf("Select Start level[1-10]:       \b\b\b\b\b\b\b");

	setCursorView(false);

	setMenu(10);
	i = menuSelect(10);
	Sleep(30);
	Sleep(30);


	level = i;
	system("cls");
	return 0;
}

int show_logo()
{
	int i,j;
	gotoxy(13,3);
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
	Sleep(100);
	gotoxy(13,4);
	printf("┃ ◆◆◆  ◆◆◆  ◆◆◆   ◆◆     ◆     ◆◆┃");
	Sleep(100);
	gotoxy(13,5);
	printf("┃   ◆    ◆        ◆     ◆ ◆    ◆    ◆   ┃");
	Sleep(100);
	gotoxy(13,6);
	printf("┃   ◆    ◆◆◆    ◆     ◆◆     ◆     ◆  ┃");
	Sleep(100);
	gotoxy(13,7);
	printf("┃   ◆    ◆        ◆     ◆ ◆    ◆      ◆ ┃");
	Sleep(100);
	gotoxy(13,8);
	printf("┃   ◆    ◆◆◆    ◆     ◆  ◆   ◆   ◆◆  ┃");
	Sleep(100);
	gotoxy(13,9);
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

	gotoxy(28,20);
	printf("Please Press Any Key~!");

	for(i=0;i>=0;i++)
	{
		if(i%40==0)
		{
			for(j=0;j<5;j++)
			{
				gotoxy(18,14+j);
				printf("                                                          ");
				
				
			}
			show_cur_block(rand()%7,rand()%4,6,14);
			show_cur_block(rand()%7,rand()%4,12,14);
			show_cur_block(rand()%7,rand()%4,19,14);
			show_cur_block(rand()%7,rand()%4,24,14);
		}
		if(_kbhit())
			break;
		Sleep(30);
	}
	
	_getch();
	system("cls");

	return 0;
}
void setMenu(int cnt)
{
	int x = 51, y = 3;
	char i;

	for (i = 0; i < cnt; i++)
	{
		gotoxy(x, y);
		printf("%d단계 ", i + 1);


		y += 2;
	}
}

char menuSelect(int cnt)
{
	int g_xMargin = 3;
	gotoxy(10, 5);

	int x = 49, y = 3;
	char current = 0;
	int input;

	gotoxy(x, y + current * 2);
	printf("*");

	while (1)
	{
		input = _getch();
		gotoxy(x, y + current * 2);
		printf(" ");
		if (input == 224)
		{
			input = _getch();
			switch (input)
			{
			case 72:   // ARROW_UP
			case 75:   // ARROW_LEFT
				current = (current + cnt - 1) % cnt;
				break;
			case 77:   // ARROW_RIGHT
			case 80:   // ARROW_DOWN
				current = (current + 1) % cnt;
				break;
			}
		}
		else if ((input >= 9 && input <= 13) || input == ' ')
		{
			gotoxy(x, y + current * 2);
			printf(">>");
			break;
		}
		gotoxy(x, y + current * 2);
		printf("*");
	}
	return current;
}

void setCursorView(bool visible)
{
	CONSOLE_CURSOR_INFO cursor = { 1, visible };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}