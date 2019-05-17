/* Dan Anthony A. Calixto  2014-23-25 */
/* Peter John G. Castillo */
/* Sokoban Game for ICS-OS */

#include "../../sdk/dexsdk.h"
#include "../../sdk/time.h"
#include <stdbool.h>

void clear(int startRow, int startColumn, int width, int height, char color);
void grid(int startRow, int startColumn, int width, int height, int divider, char color);
void set_coordinates(int x, int y);
void set_level(int lvl);
void print_tiles();
void changePos(char keyPress);
void set_player();
void game();

#define maxrow 10
#define maxcol 10
#define BOARD_SIZE 10
#define TILE_SIZE 18
#define BOARD_MAX 100
#define up_key 'w'
#define down_key 's'
#define left_key 'a'
#define right_key 'd'
#define quit 'q'
#define reset 'r'
#define yes 'y'
#define no 'n'
#define start '1'
#define controls '2'
#define quit_game '3'

#define YELLOW 54
#define RED 4
#define LIGHT_GRAY 7
#define PALE_YELLOW 62
#define ROYAL_BLUE 1
#define DARK_BLUE 8
#define GRAY 56
#define WHITE 63
#define BLACK 100

#define X_coord 85
#define Y_coord 35


int row, col, currentRow, currentColumn, previousRow, previousColumn;
int level=0, moves;
int field[maxrow][maxcol];
int field_x[maxrow][maxcol];
int field_y[maxrow][maxcol];
int prevVal;
bool win = false;
char keypress;
/*
	up is right, down is left, left is up, right is down
	0 = space
	1 = wall
	2 = player stepping on goal
	3 = box is in goal tile
	4 = player
	5 = goal
	8 = box
*/
int levels[5][10][10] = {
	{ //level 1
		{0, 0, 1, 1, 1, 1, 1, 0, 0, 0}, 
		{1, 1, 1, 0, 0, 0, 1, 0, 0, 0}, 
		{1, 5, 4, 8, 0, 0, 1, 0, 0, 0}, 
		{1, 1, 1, 0, 8, 5, 1, 0, 0, 0}, 
		{1, 5, 1, 1, 8, 0, 1, 0, 0, 0}, 
		{1, 0, 1, 0, 5, 0, 1, 1, 0, 0}, 
		{1, 8, 0, 3, 8, 8, 5, 1, 0, 0}, 
		{1, 0, 0, 0, 5, 0, 0, 1, 0, 0}, 
		{1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	},
	{ //level 2
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 1, 1, 1, 1, 0, 0, 0, 0}, 
		{0, 1, 1, 0, 0, 1, 0, 0, 0, 0}, 
		{0, 1, 0, 0, 8, 1, 0, 0, 0, 0}, 
		{0, 1, 1, 0, 0, 1, 1, 0, 0, 0}, 
		{0, 1, 1, 8, 0, 0, 1, 0, 0, 0}, 
		{0, 1, 5, 8, 8, 0, 1, 0, 0, 0}, 
		{0, 1, 3, 2, 5, 5, 1, 0, 0, 0}, 
		{0, 1, 1, 1, 1, 1, 1, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	},
	{ //level 3
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 1, 1, 1, 1, 0, 0, 0, 0}, 
		{0, 0, 1, 0, 0, 1, 1, 1, 0, 0}, 
		{0, 0, 1, 0, 0, 0, 0, 1, 0, 0}, 
		{0, 1, 1, 1, 8, 1, 0, 1, 1, 0}, 
		{0, 1, 3, 1, 0, 1, 0, 0, 1, 0}, 
		{0, 1, 5, 0, 0, 0, 1, 0, 1, 0}, 
		{0, 1, 5, 0, 8, 0, 4, 0, 1, 0}, 
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	},
	{ //level 4
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 1, 1, 1, 1, 1, 1, 0}, 
		{0, 0, 0, 1, 0, 0, 0, 0, 1, 0}, 
		{0, 1, 1, 1, 8, 8, 8, 0, 1, 0}, 
		{0, 1, 0, 0, 8, 5, 5, 0, 1, 0}, 
		{0, 1, 4, 8, 5, 5, 5, 1, 1, 0}, 
		{0, 1, 1, 1, 1, 0, 0, 1, 0, 0}, 
		{0, 0, 0, 0, 1, 1, 1, 1, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	},
	{ //level 5
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 1, 1, 1, 1, 1, 0, 0}, 
		{0, 1, 1, 1, 0, 0, 4, 1, 0, 0}, 
		{0, 1, 0, 0, 8, 5, 0, 1, 1, 0}, 
		{0, 1, 0, 0, 5, 8, 5, 0, 1, 0}, 
		{0, 1, 1, 1, 0, 3, 8, 0, 1, 0}, 
		{0, 0, 0, 1, 0, 0, 0, 1, 1, 0}, 
		{0, 0, 0, 1, 1, 1, 1, 1, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	},
	
};

int main(){
    keypress = start;
    int i,j;
    char str[15];
    set_graphics(VGA_320X200X256);
		set_coordinates(X_coord, Y_coord);
    do{
        clear(1,1,400,220,BLACK); 
        title();
        keypress=(char)getch();
        clear(1,1,400,220,BLACK); //erase menu
        if(keypress==start){
            do{
                write_text("Level",85,5,WHITE,0);
                set_level(0);
				print_tiles();
				do{
					game();
				} while(keypress != quit_game);
            } while (keypress != quit_game);
           	keypress=start;
        }else if(keypress==controls){
        	write_text("You can move player by using:",25,35,WHITE,0);
			write_text("[w] [s] [a] [d]",35,55,WHITE,0);
			write_text("You can also reset map using:",25,85,WHITE,0);
			write_text("[r]",35,105,WHITE,0);
			write_text("press any key to go back...",40,175,WHITE,0);
			keypress=(char)getch();
			keypress = start;
        }else{
        	write_text("Bye!",25,35,WHITE,0);	
        }
    }while(keypress != quit_game);
    set_graphics(VGA_TEXT80X25X16);
	clrscr();
}
void game(){
	int level=1;
	char _lvl[5][9] ={"Level 1", "Level 2", "Level 3", "Level 4", "Level 5"};
	do{
		keypress = start;
		write_text(_lvl[level-1],135,5,WHITE,0);
		set_level(level-1);
		print_tiles();
		do{
			keypress = (char)getch();
			changePos(keypress);
		}while(!win);
	}while(keypress!=reset);
}
void clear(int startRow, int startColumn, int width, int height, char color)
{
    int row, column;
	for (row = startRow; row < (startRow + width) + 1; row++)
		for (column = startColumn; column < (startColumn + height) + 1; column++)
			write_pixel(row, column, color);
}
void grid(int startRow, int startColumn, int width, int height, int divider, char color)
{
    int row, column;
	for (row = startRow; row < (startRow + width) + 1; row++)
		for (column = startColumn; column < (startColumn + height) + 1; column++)
			if (row % divider != 0 && column % divider != 0)
				write_pixel(row, column, color);
			else
				write_pixel(row, column, LIGHT_GRAY);
}
void set_coordinates(int x, int y){ //initialize bulb coordinates
	int i, j, a, b;
	
	a = x;
	b = y;
	
	for(i=0; i<maxrow; i++, b+=24){
		for(j=0; j<maxcol; j++, a+=31){
			field_x[i][j] = a;
			field_y[i][j] = b;
		}
		a=x;
	}
}
void print_tiles(){
	int i,j;
	int color;
	for(i=0;i<maxrow;i++){
		for(j=0;j<maxcol;j++){
			if(field[i][j]==0){
				color = GRAY;
			} 
			if(field[i][j]==1){
				color = DARK_BLUE;
			} 
			if(field[i][j]==2){
				color = LIGHTRED;
			} 
			if(field[i][j]==3){
				color = GREEN;
			} 
			if(field[i][j]==4){
				color = YELLOW;
			}
			if(field[i][j]==5){
				color = RED;
			} 
			if(field[i][j]==8){
				color = LIGHTCYAN;
			} 
			grid(TILE_SIZE * (i + 1), TILE_SIZE * (j + 1), TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
		}
	}
	write_text("w (Up)", 220, 35, WHITE, 0);
	write_text("a (Down)",220, 55, WHITE, 0);
	write_text("s (Left)", 220, 75, WHITE, 0);
	write_text("d (Righ)", 220, 95, WHITE, 0);
	write_text("r (Reset)", 220, 115, WHITE, 0);
	write_text("3 (Quit)", 220, 135, WHITE, 0);
}
void set_level(int lvl){
	int i,j;
	for(i=0;i<maxrow;i++){
		for(j=0;j<maxcol;j++){
			field[i][j] = levels[lvl][i][j];
			if(field[i][j]==4) {
				currentRow = i;
				currentColumn = j;
				previousRow = currentRow;
				previousColumn =currentColumn;
			}
		}
	}
}
void changePos(char keyPress) {
	if(keyPress == right_key && !(field[currentRow + 1][currentColumn] == 1)) {
		prevVal = field[currentRow+1][currentColumn];
		if(field[currentRow+1][currentColumn] == 8 || field[currentRow+1][currentColumn] == 3){
			if(field[currentRow+2][currentColumn] == 0){
				field[currentRow+2][currentColumn] = 8;
				previousRow = currentRow;
				previousColumn = currentColumn;
				currentRow++;
			}
			if(field[currentRow+2][currentColumn] == 5) {
				field[currentRow+2][currentColumn] = 3;
				previousRow = currentRow;
				previousColumn = currentColumn;
				currentRow++;
			}
		} else {
			previousRow = currentRow;
			previousColumn = currentColumn;
			currentRow++;
		}
	} else if (keyPress == left_key && !(field[currentRow-1][currentColumn] == 1))	{
		prevVal = field[currentRow-1][currentColumn];
		if(field[currentRow-1][currentColumn] == 8 || field[currentRow-1][currentColumn] == 3){
			if(field[currentRow-2][currentColumn] == 0){
				field[currentRow-2][currentColumn] = 8;
				previousRow = currentRow;
				previousColumn = currentColumn;
				currentRow--;
			}
			if(field[currentRow-2][currentColumn] == 5) {
				field[currentRow-2][currentColumn] = 3;
				previousRow = currentRow;
				previousColumn = currentColumn;
				currentRow--;
			}
		} else {
			previousRow = currentRow;
			previousColumn = currentColumn;
			currentRow--;
		}
	} else if (keyPress == up_key && !(field[currentRow][currentColumn-1] == 1)) {
		prevVal = field[currentRow][currentColumn-1];
		if(field[currentRow][currentColumn-1] == 8 || field[currentRow][currentColumn-1] == 3){
			if(field[currentRow][currentColumn-2] == 0){
				field[currentRow][currentColumn-2] = 8;
				previousRow = currentRow;
				previousColumn = currentColumn;
				currentColumn--;
			}
			if(field[currentRow][currentColumn-2] == 5) {
				field[currentRow][currentColumn-2] = 3;
				previousRow = currentRow;
				previousColumn = currentColumn;
				currentColumn--;
			}
		} else {
			previousRow = currentRow;
			previousColumn = currentColumn;
			currentColumn--;
		}
	} else if(keyPress == down_key && !(field[currentRow][currentColumn + 1] == 1)) {
		prevVal = field[currentRow][currentColumn+1];
		if(field[currentRow][currentColumn+1] == 8 || field[currentRow][currentColumn+1] == 3){
			if(field[currentRow][currentColumn+2] == 0){
				field[currentRow][currentColumn+2] = 8;
				previousRow = currentRow;
				previousColumn = currentColumn;
				currentColumn++;
			}
			if(field[currentRow][currentColumn+2] == 5) {
				field[currentRow][currentColumn+2] = 3;
				previousRow = currentRow;
				previousColumn = currentColumn;
				currentColumn++;
			}
		} else {
			previousRow = currentRow;
			previousColumn = currentColumn;
			currentColumn++;
		}
	}

	if(keyPress==reset){
			write_text("Reset? [y/n]",210,160,RED,0);
			keyPress = (char)getch(); 
			if(keyPress == yes){
				write_text("Reset? [y/n]",210,160,BLACK,0);
				set_level(level);
			} else{
				write_text("Reset? [y/n]",210,160,BLACK,0);
			}													
	}	
}
void set_player() {
	field[previousRow][previousColumn] = prevVal;
	print_tiles();
	field[currentRow][currentColumn] = 4;
	if(field[currentColumn][currentColumn] == 5) {
		field[currentRow][currentColumn] = 2;
	}
	clear(TILE_SIZE*maxrow+20,2,100,40,BLACK);
	clear(TILE_SIZE*maxrow+20,40,100,40,BLACK);
	textcolor(LIGHTCYAN);
	gotoxy(TILE_SIZE*maxrow+20,2);
  printf("x: %05d",currentRow);
	textcolor(LIGHTCYAN);
	gotoxy(TILE_SIZE*maxrow+20,40);
	printf("y: %05d",currentColumn);
	print_tiles();
}
title() {
  write_text("S O K O B A N",180,40,WHITE,1); //title

	//menu options
	write_text("Start (1)", 52, 113, WHITE, 1);
	write_text("Controls (2)", 178, 113, WHITE, 1);
	// write_text("Help (3)", 57, 153, WHITE, 1);
	write_text("Quit (3)", 126, 153, WHITE, 1);
}