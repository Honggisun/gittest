#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

#include "../engine/engine2d.h"
#include "r_game.h"

char screen_buffer[8*8];

struct timespec work_timer;
double acc_tick,last_tick;

double rock_acc_tick;
int rock_pos_x,rock_pos_y;
int rock_pos_table[16] = {1,3,4,5,2,3,6,7,4};
int rock_cur_table_index = 0;
int bLoop = 1;

double star_acc_tick;
int star_pos_x,star_pos_y;
int star_pos_table[16] = {3,5,1,4,6,2,4,1,2};
int star_cur_table_index = 0;

int score = 0;

int car_posx,car_posy;

static int nFSM = 0;
static int nStep = 0;

void rg_PlayGame(double delta_tick)
{
	switch(nStep)
	{
		case 0:
			nStep++;
			system("clear");
			break;
		case 1:	
			//사용자 입력처리 
			if(kbhit() != 0) {
				char ch = getch();
				if(ch == 'q') {
					bLoop = 0;
				}
				else if(ch == 'a') {
					car_posx -= 1;
				}
				else if(ch == 'd') {
					car_posx += 1;
				}
				//printf("%d \r\n",ch);
			}

			//바위 움직이기 
		rock_acc_tick += delta_tick;

		if(rock_acc_tick > 0.2) {
			rock_acc_tick = 0;
			rock_pos_y += 1;
			//화면끝도달...
			if(rock_pos_y >= 8) {
				rock_pos_y = 0;
				rock_cur_table_index++;
				rock_cur_table_index %= 8;
				rock_pos_x = rock_pos_table[ rock_cur_table_index];
			}
		}
		star_acc_tick += delta_tick;

		if(star_acc_tick > 0.3) {
			star_acc_tick = 0;
			star_pos_y += 1;
			star_pos_x += 0;
			//화면끝도달...
			if(star_pos_y >= 8) {
				star_pos_y = 0;
				star_cur_table_index++;
				star_cur_table_index %= 8;
				star_pos_x = star_pos_table[ star_cur_table_index];


			}
		}
		//버퍼초기화 
			for(int i=0;i<64;i++) {
				screen_buffer[i] = 0;
			}

			//자동차 그리기 
			screen_buffer[ car_posy *8+ car_posx] = 2;
			//바위 그리기 
			screen_buffer[ rock_pos_y*8 + rock_pos_x ] = 1;
			//별 그리기
			screen_buffer[ star_pos_y*8 + star_pos_x ] = 3;

			//게임로직 (판정)
			if(rock_pos_y == car_posy &&
					rock_pos_x == car_posx)
			{
				nFSM = 0;

				//drawGame(screen_buffer);
				printf("game over\n");

			}   
			if(star_pos_y == car_posy &&
					star_pos_x == car_posx)
			{
				//drawGame(screen_buffer);
				printf("score : %d \r\n",score);

			} 

			acc_tick += delta_tick;
			if(acc_tick > 0.1 || bLoop == 0 ) {
				acc_tick = 0;
				drawGame(8,8,screen_buffer);
			}
			break;
	}

}

void rg_apply_mainTitle()
{

	
	switch(nStep) {
	
		case 0:
		puts("press to start");
		nStep = 1;
		break;
		case 1:
		//사용자 입력처리 
		if(kbhit() != 0) {
			
			char ch = getch();
			nFSM = 1;
			nStep = 0;
		}
		break;
	}
}

int main()
{
	//버퍼초기화 
	for(int i=0;i<64;i++) {
		screen_buffer[i] = 0;
	}

	set_conio_terminal_mode();
	
	acc_tick = last_tick = 0;
	system("clear");

	car_posy = 7;
	car_posx = 3;

	rock_pos_y = 0;
	rock_pos_x = rock_pos_table[ rock_cur_table_index ];

	star_pos_y = 0;
	star_pos_x = rock_pos_table[ rock_cur_table_index ];


	while(bLoop) {

		//타이밍처리 
		clock_gettime(CLOCK_MONOTONIC,&work_timer);
		double cur_tick = work_timer.tv_sec + 
			(double)(work_timer.tv_nsec * 1e-9);
		double delta_tick = cur_tick - last_tick;
		last_tick = cur_tick;

		if(nFSM == 0) { //타이틀 메뉴 상태 
			rg_apply_mainTitle(delta_tick);

		}
		else if(nFSM ==1 ) { //게임상태... 
			rg_PlayGame(delta_tick);
		}
		else {
		}

	}


	return 0;
}
