#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime> // Them thu vien cho "srand(time(0))"
using namespace std;
#define H 20
#define W 15

// ===== Utility rotate (global) =====
static void rotate90CW(char a[4][4])
{
    char t[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            t[i][j] = a[i][j];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            a[i][j] = t[3 - j][i];
}
static void rotate90CCW(char a[4][4])
{
    char t[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            t[i][j] = a[i][j];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            a[i][j] = t[j][3 - i];
}

// ===== Polymorphism Piece =====
class Piece
{
protected:
    char shape[4][4]{};
    int state = 0;

    void copyFrom(const char src[4][4])
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = src[i][j];
    }

public:
    virtual ~Piece() {}
    const char (*getShape() const)[4] { return shape; }

    virtual void rotateCW() = 0;
    virtual void rotateCCW() = 0;
};

// ===== 7 pieces =====
class IPiece : public Piece
{
public:
    IPiece()
    {
        const char init[4][4] = {
            {' ', 'I', ' ', ' '},
            {' ', 'I', ' ', ' '},
            {' ', 'I', ' ', ' '},
            {' ', 'I', ' ', ' '}};
        copyFrom(init);
    }
    void rotateCW() override
    {
        rotate90CW(shape);
        state = (state + 1) % 2;
    }
    void rotateCCW() override
    {
        rotate90CCW(shape);
        state = (state + 1) % 2;
    }
};

class OPiece : public Piece
{
public:
    OPiece()
    {
        const char init[4][4] = {
            {' ', ' ', ' ', ' '},
            {' ', 'O', 'O', ' '},
            {' ', 'O', 'O', ' '},
            {' ', ' ', ' ', ' '}};
        copyFrom(init);
    }
    void rotateCW() override {}
    void rotateCCW() override {}
};

class TPiece : public Piece
{
public:
    TPiece()
    {
        const char init[4][4] = {
            {' ', ' ', ' ', ' '},
            {' ', 'T', ' ', ' '},
            {'T', 'T', 'T', ' '},
            {' ', ' ', ' ', ' '}};
        copyFrom(init);
    }
    void rotateCW() override
    {
        rotate90CW(shape);
        state = (state + 1) % 4;
    }
    void rotateCCW() override
    {
        rotate90CCW(shape);
        state = (state + 3) % 4;
    }
};

class SPiece : public Piece
{
public:
    SPiece()
    {
        const char init[4][4] = {
            {' ', ' ', ' ', ' '},
            {' ', 'S', 'S', ' '},
            {'S', 'S', ' ', ' '},
            {' ', ' ', ' ', ' '}};
        copyFrom(init);
    }
    void rotateCW() override
    {
        rotate90CW(shape);
        state = (state + 1) % 4;
    }
    void rotateCCW() override
    {
        rotate90CCW(shape);
        state = (state + 3) % 4;
    }
};

class ZPiece : public Piece
{
public:
    ZPiece()
    {
        const char init[4][4] = {
            {' ', ' ', ' ', ' '},
            {'Z', 'Z', ' ', ' '},
            {' ', 'Z', 'Z', ' '},
            {' ', ' ', ' ', ' '}};
        copyFrom(init);
    }
    void rotateCW() override
    {
        rotate90CW(shape);
        state = (state + 1) % 4;
    }
    void rotateCCW() override
    {
        rotate90CCW(shape);
        state = (state + 3) % 4;
    }
};

class JPiece : public Piece
{
public:
    JPiece()
    {
        const char init[4][4] = {
            {' ', ' ', ' ', ' '},
            {'J', ' ', ' ', ' '},
            {'J', 'J', 'J', ' '},
            {' ', ' ', ' ', ' '}};
        copyFrom(init);
    }
    void rotateCW() override
    {
        rotate90CW(shape);
        state = (state + 1) % 4;
    }
    void rotateCCW() override
    {
        rotate90CCW(shape);
        state = (state + 3) % 4;
    }
};

class LPiece : public Piece
{
public:
    LPiece()
    {
        const char init[4][4] = {
            {' ', ' ', ' ', ' '},
            {' ', ' ', 'L', ' '},
            {'L', 'L', 'L', ' '},
            {' ', ' ', ' ', ' '}};
        copyFrom(init);
    }
    void rotateCW() override
    {
        rotate90CW(shape);
        state = (state + 1) % 4;
    }
    void rotateCCW() override
    {
        rotate90CCW(shape);
        state = (state + 3) % 4;
    }
};

// Factory
static Piece *spawnRandomPiece()
{
    int r = rand() % 7;
    switch (r)
    {
    case 0:
        return new IPiece();
    case 1:
        return new OPiece();
    case 2:
        return new TPiece();
    case 3:
        return new SPiece();
    case 4:
        return new ZPiece();
    case 5:
        return new JPiece();
    default:
        return new LPiece();
    }
}

char board[H][W] = {};
Piece *curPiece = nullptr; // Current piece pointer
int x = 4, y = 0;
int speed = 200;           // tốc độ rơi (ms) - ban đầu
const int MIN_SPEED = 50;  // nhanh tối đa (ms)
const int SPEED_STEP = 15; // mỗi lần xóa dòng giảm bao nhiêu ms

void gotoxy(int x, int y)
{
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// Cap nhat ham xoa block de ho tro xoay
void boardDelBlock()
{
    auto s = curPiece->getShape();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (s[i][j] != ' ')
            {
                int by = y + i;
                int bx = x + j;
                if (by >= 0 && by < H && bx >= 0 && bx < W)
                    board[by][bx] = ' ';
            }
}

// Cap nhat ham ve khoi de xoay
void block2Board()
{
    auto s = curPiece->getShape();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (s[i][j] != ' ')
            {
                int by = y + i;
                int bx = x + j;
                if (by >= 0 && by < H && bx >= 0 && bx < W)
                    board[by][bx] = s[i][j];
            }
}

// Cap nhat ham initBoard
void initBoard()
{
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if ((i == H - 1) || (j == 0) || (j == W - 1))
                board[i][j] = '#';
            else
                board[i][j] = ' ';
}
// Cap nhat ham Draw
void draw()
{
    gotoxy(0, 0);
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            if (board[i][j] == '#')
            {
                cout << "##";
            }
            else if (board[i][j] != ' ')
            {
                cout << "[]";
            }
            else
            {
                cout << "  ";
            }
        }
        cout << endl;
    }
}
// Cap nhat ham kiem tra di chuyen
bool canMove(int dx, int dy)
{
    auto s = curPiece->getShape();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (s[i][j] != ' ')
            {
                int tx = x + j + dx;
                int ty = y + i + dy;
                if (tx < 1 || tx >= W - 1 || ty >= H - 1 || ty < 0)
                    return false;
                if (board[ty][tx] != ' ')
                    return false;
            }
    return true;
}
// Ham xoa dong
int removeLine()
{
    int cleared = 0;
    for (int i = H - 2; i > 0; i--)
    {
        bool full = true;
        for (int j = 1; j < W - 1; j++)
        {
            if (board[i][j] == ' ')
            {
                full = false;
                break;
            }
        }
        if (full)
        {
            cleared++;
            for (int k = i; k > 0; k--)
            {
                for (int j = 1; j < W - 1; j++)
                {
                    board[k][j] = board[k - 1][j];
                }
            }
            i++; // kiểm tra lại dòng vừa kéo xuống
        }
    }
    return cleared;
}

int main()
{
    srand(time(0));
    system("cls");
    initBoard();

    // tạo khối đầu tiên
    x = 5;
    y = 0;
    curPiece = spawnRandomPiece();

    while (1)
    {
        // xóa dấu vết khối khỏi board trước khi xử lý (rất quan trọng)
        boardDelBlock();

        if (kbhit())
        {
            char c = getch();

            if (c == 'a' && canMove(-1, 0))
                x--;
            if (c == 'd' && canMove(1, 0))
                x++;
            if (c == 's' && canMove(0, 1))
                y++; // soft drop
            if (c == 'q')
                break;

            // đa hình xoay + rollback nếu xoay bị đụng tường/đụng block
            if (c == 'z')
            {
                boardDelBlock();
                curPiece->rotateCCW();
                if (!canMove(0, 0))
                    curPiece->rotateCW();
            }
            if (c == 'x')
            {
                boardDelBlock();
                curPiece->rotateCW();
                if (!canMove(0, 0))
                    curPiece->rotateCCW();
            }
        }

        // rơi tự động
        if (canMove(0, 1))
        {
            y++;
        }
        else
        {
            // khóa khối lại
            block2Board();

            // SV5: xóa dòng -> tăng tốc
            int cleared = removeLine();
            if (cleared > 0)
            {
                speed -= cleared * SPEED_STEP;
                if (speed < MIN_SPEED)
                    speed = MIN_SPEED;
            }

            // sinh khối mới
            x = 5;
            y = 0;

            delete curPiece;
            curPiece = spawnRandomPiece();

            // (tùy chọn) nếu spawn xong mà không đặt được => game over
            if (!canMove(0, 0))
            {
                gotoxy(0, H + 1);
                cout << "GAME OVER!\n";
                break;
            }
        }

        // vẽ lại
        block2Board();
        draw();
        Sleep(speed);
    }

    delete curPiece;
    return 0;
}
