#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h> // Them thu vien cho "srand(time(0))"
using namespace std;
#define H 20
#define W 15
char board[H][W] = {} ;
char blocks[][4][4] = {
        {{' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '}},
        {{' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'I','I','I','I'},
         {' ',' ',' ',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','T',' ',' '},
         {'T','T','T',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','S','S',' '},
         {'S','S',' ',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'Z','Z',' ',' '},
         {' ','Z','Z',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'J',' ',' ',' '},
         {'J','J','J',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ',' ','L',' '},
         {'L','L','L',' '},
         {' ',' ',' ',' '}}
};

char current_block[4][4]; //Khoi hien tai dang roi
int x=4,y=0,b=1;
void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
//Ham ho tro xoay
void copyBlock(char dest[4][4], const char src[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dest[i][j] = src[i][j];
        }
    }
}
//Cap nhat ham xoa block de ho tro xoay
void boardDelBlock(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (current_block[i][j] != ' ' && y+j < H)
                board[y+i][x+j] = ' ';
}
//Cap nhat ham ve khoi de xoay
void block2Board(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (current_block[i][j] != ' ' )
                board[y+i][x+j] = current_block[i][j];
}
void initBoard(){
    for (int i = 0 ; i < H ; i++)
        for (int j = 0 ; j < W ; j++)
            if ((i==H-1) || (j==0) || (j == W-1)) board[i][j] = '#';
            else board[i][j] = ' ';
}
void draw(){
    gotoxy(0,0);
    for (int i = 0 ; i < H ; i++, cout<<endl)
        for (int j = 0 ; j < W ; j++)
            cout<<board[i][j];
}
//Cap nhat ham kiem tra di chuyen
bool canMove(int dx, int dy){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (current_block[i][j] != ' '){
                int tx = x + j + dx;
                int ty = y + i + dy;
                if ( tx<1 || tx >= W-1 || ty >= H-1 || ty < 0) return false;
                if ( board[ty][tx] != ' ') return false;
            }
    return true;
}
//Them ham xoay phai
void rotate90(char block[4][4]) {
    char temp[4][4];
    copyBlock(temp, block);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            block[i][j] = temp[3 - j][i];
        }
    }
}
//Them ham xoay trai
void rotateNeg90(char block[4][4]) {
    char temp[4][4];
    copyBlock(temp, block);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            block[i][j] = temp[j][3 - i];
        }
    }
}
//Xoay voi Wall Kick
void doRotate(bool clockwise) {
    char temp_block[4][4];
    copyBlock(temp_block, current_block);
    int old_x = x;

    if (clockwise) {
        rotate90(current_block);
    } else {
        rotateNeg90(current_block);
    }

    //Kiem tra Wall Kick

    // Tai vi tri hien tai
    if (canMove(0, 0)) return;

    // Dich sang trai 1 o
    x--;
    if (canMove(0, 0)) return;

    // Quay lai vi tri cu roi dich sang phai 1 o
    x = old_x + 1;
    if (canMove(0, 0)) return;

    // Hoan tac
    x = old_x;
    copyBlock(current_block, temp_block);
}
void removeLine() {
    for (int i = H - 2; i > 0; i--) {
        bool full = true;
        for (int j = 1; j < W - 1; j++) {
            if (board[i][j] == ' ') {
                full = false;
                break;
            }
        }
        if (full) {
            for (int k = i; k > 0; k--) {
                for (int j = 1; j < W - 1; j++) {
                    board[k][j] = board[k - 1][j];
                }
            }
            i++;
        }
    }
}

int main()
{
    srand(time(0));
    b = rand() % 7;
    system("cls");
    initBoard();
    copyBlock(current_block, blocks[b]);
    while (1){
        boardDelBlock();
        if (kbhit()){
            char c = getch();
            if (c=='a' && canMove(-1,0)) x--;
            if (c=='d' && canMove(1,0) ) x++;
            if (c=='s' && canMove(0,1))  y++; //Dung phim s cho Sorf Drop
            if (c=='z') doRotate(false); //Xoay trai
            if (c=='x') doRotate(true); //Xoay phai
            if (c=='q') break;
        }
        if (canMove(0,1)) y++;
        else {
            block2Board();
            removeLine();
            x = 5; y = 0; b = rand() % 7;
            copyBlock(current_block, blocks[b]);
        }
        block2Board();
        draw();
        _sleep(200);
    }
    return 0;
}
