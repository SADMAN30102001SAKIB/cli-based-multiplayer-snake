#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <fstream>
using namespace std;

const int MAX_SNAKE_SIZE = 100;
const int SCREEN_WIDTH = 100;
const int SCREEN_HEIGHT = 30;
int snakeData[6] = {0};

HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoXY(int x, int y)
{
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

class Point
{
private:
    int x;
    int y;

public:
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }

    void setPoint(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    void moveUp()
    {
        y--;
    }

    void moveDown()
    {
        y++;
    }

    void moveLeft()
    {
        x--;
    }

    void moveRight()
    {
        x++;
    }

    void draw(int snakeColor)
    {
        gotoXY(x, y);
        SetConsoleTextAttribute(hconsole, snakeColor);
        cout << "#";
    }

    void drawFruit(int snakeColor)
    {
        gotoXY(x, y);
        SetConsoleTextAttribute(hconsole, snakeColor);
        cout << "@";
    }

    void drawBlank()
    {
        gotoXY(x, y);
        cout << " ";
    }

    void copyPosition(Point *p)
    {
        p->x = x;
        p->y = y;
    }
};

class Snake
{
private:
    Point *cell[MAX_SNAKE_SIZE + 4];
    int size;
    int speed;
    int score;
    int snakeColor;
    int fruitColor;
    int isDead = 0;
    char direction;
    Point fruit;

public:
    Snake(int x, int y, int c, int fc) : size(4), speed(100), score(0), snakeColor(c), fruitColor(fc)
    {
        cell[0] = new Point(x, y);
        cell[1] = new Point(x, y + 1);
        cell[2] = new Point(x, y + 2);
        cell[3] = new Point(x, y + 3);
        cell[4] = new Point(x, y + 4);
        for (int i = 5; i < MAX_SNAKE_SIZE + 4; i++)
        {
            cell[i] = nullptr;
        }
        generateFruit();
    }

    void move(int verticalSpeed, int horizontalSpeed)
    {
        for (int i = size; i > 0; i--)
        {
            cell[i - 1]->copyPosition(cell[i]);
        }

        switch (direction)
        {
        case 'u':
            cell[0]->moveUp();
            speed = verticalSpeed;
            break;
        case 'd':
            cell[0]->moveDown();
            speed = verticalSpeed;
            break;
        case 'l':
            cell[0]->moveLeft();
            speed = horizontalSpeed;
            break;
        case 'r':
            cell[0]->moveRight();
            speed = horizontalSpeed;
            break;
        }

        handleCollision();
        drawSnake();
        Sleep(speed);
    }

    void drawSnake()
    {
        for (int i = 0; i < size; i++)
        {
            cell[i]->draw(snakeColor);
        }
        cell[size]->drawBlank();
        fruit.drawFruit(fruitColor);
    }

    void clearSnake()
    {
        for (int i = 0; i < size; i++)
        {
            cell[i]->drawBlank();
        }
        fruit.drawBlank();
    }

    void drawSampleFruit()
    {
        fruit.drawFruit(fruitColor);
    }

    void addCell(int x, int y)
    {
        cell[++size] = new Point(x, y);
    }

    int getScore()
    {
        return score;
    }

    int getSize()
    {
        return size;
    }

    int getFlag()
    {
        return isDead;
    }

    void turnUp()
    {
        if (direction != 'd')
        {
            direction = 'u';
        }
    }

    void turnDown()
    {
        if (direction != 'u')
        {
            direction = 'd';
        }
    }

    void turnLeft()
    {
        if (direction != 'r')
        {
            direction = 'l';
        }
    }

    void turnRight()
    {
        if (direction != 'l')
        {
            direction = 'r';
        }
    }

    void generateFruit()
    {
        int X, Y;
        do
        {
            X = rand() % (SCREEN_WIDTH - 1);
            Y = rand() % (SCREEN_HEIGHT - 1);
        } while (X == 0 || Y == 0);

        fruit.setPoint(X, Y);
    }

    int cellX(int i)
    {
        return cell[i]->getX();
    }

    int cellY(int i)
    {
        return cell[i]->getY();
    }

    int fruitX()
    {
        return fruit.getX();
    }

    int fruitY()
    {
        return fruit.getY();
    }

    void handleCollision()
    {
        if (fruit.getX() == cell[0]->getX() && fruit.getY() == cell[0]->getY())
        {
            addCell(SCREEN_WIDTH, SCREEN_HEIGHT);
            generateFruit();
            score++;
        }
        if (cell[0]->getX() <= 0 || cell[0]->getX() >= SCREEN_WIDTH - 1 ||
            cell[0]->getY() <= 0 || cell[0]->getY() >= SCREEN_HEIGHT - 1)
        {
            isDead = 1;
        }

        for (int i = 1; i < size; i++)
        {
            if (cell[0]->getX() == cell[i]->getX() && cell[0]->getY() == cell[i]->getY())
            {
                isDead = 1;
                break;
            }
        }
    }

    void drawWall()
    {
        for (int i = 0; i < SCREEN_WIDTH; i++)
        {
            gotoXY(i, 0);
            SetConsoleTextAttribute(hconsole, 3);
            cout << "~";
        }

        for (int i = 0; i < SCREEN_WIDTH; i++)
        {
            gotoXY(i, SCREEN_HEIGHT - 1);
            SetConsoleTextAttribute(hconsole, 3);
            cout << "~";
        }

        for (int i = 1; i < SCREEN_HEIGHT - 1; i++)
        {
            gotoXY(0, i);
            SetConsoleTextAttribute(hconsole, 3);
            cout << ";";
            gotoXY(SCREEN_WIDTH - 1, i);
            SetConsoleTextAttribute(hconsole, 3);
            cout << ";";
        }
    }
};

void pauseGame()
{
    system("cls");
    gotoXY(SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 2);
    SetConsoleTextAttribute(hconsole, 12);
    cout << "Game Paused";
    gotoXY(SCREEN_WIDTH / 2 - 24, SCREEN_HEIGHT / 2 + 1);
    SetConsoleTextAttribute(hconsole, 6);
    cout << "Press Enter to continue or Esc to exit";
}

void play()
{
    system("cls");
    Snake snake(20, 20, 6, 12);
    char option;
    int snakeMoving = 0;
    bool paused = false;

    snake.drawWall();
    snake.drawSnake();

    gotoXY(35, 0);
    SetConsoleTextAttribute(hconsole, 6);
    cout << "Score: " << snake.getScore();
    gotoXY(50, 0);
    SetConsoleTextAttribute(hconsole, 10);
    cout << "HighScore: " << snakeData[0];

    do
    {
        if (_kbhit())
        {
            option = _getch();
            if (option == 27 && paused)
            {
                break;
            }
            if (option == 27 && !paused)
            {
                option = 0;
                paused = true;
                pauseGame();
            }
            if (option == 13 && paused)
            {
                system("cls");
                snake.drawWall();
                snake.drawSnake();
                gotoXY(35, 0);
                SetConsoleTextAttribute(hconsole, 6);
                cout << "Score: " << snake.getScore();
                gotoXY(50, 0);
                SetConsoleTextAttribute(hconsole, 10);
                cout << "HighScore: " << snakeData[0];
                paused = false;
            }
        }
        if (paused)
        {
            continue;
        }
        switch (option)
        {
        case 72:
            snake.turnUp();
            snakeMoving = 1;
            break;
        case 80:
            snake.turnDown();
            break;
        case 75:
            snake.turnLeft();
            snakeMoving = 1;
            break;
        case 77:
            snake.turnRight();
            snakeMoving = 1;
            break;
        }

        if (snakeMoving)
        {
            gotoXY(35, 0);
            SetConsoleTextAttribute(hconsole, 6);
            cout << "Score: " << snake.getScore();
            gotoXY(50, 0);
            SetConsoleTextAttribute(hconsole, 10);
            cout << "HighScore: " << snakeData[0];

            if (snake.getScore() > snakeData[0])
            {
                snakeData[0] = snake.getScore();
            }
            snake.move(85, 35);
        }
        if (snake.getFlag())
        {
            snakeData[1]++;
            break;
        }
    } while (option != 27);

    system("cls");
    if (option != 27)
    {
        gotoXY(30, 10);
        SetConsoleTextAttribute(hconsole, 13);
        cout << "Game Over 😆! Your Score is: " << snake.getScore();
        gotoXY(30, 12);
        SetConsoleTextAttribute(hconsole, 6);
        cout << "Please Press ENTER to Continue.";
        cin.ignore();
    }
}

void playMulti()
{
    system("cls");
    Snake snake1(90, 24, 12, 12), snake2(10, 24, 10, 10);
    char option;
    bool paused = false;
    int snake1Moving = 0, snake2Moving = 0, verticalSpeed = 85, horizontalSpeed = 35, snake1Alive = 1, snake2Alive = 1;

    snake1.drawWall();
    snake1.drawSnake();
    snake2.drawSnake();

    gotoXY(3, 0);
    SetConsoleTextAttribute(hconsole, 10);
    cout << "Score: " << snake2.getScore();
    gotoXY(15, 0);
    SetConsoleTextAttribute(hconsole, 10);
    cout << "HighScore: " << snakeData[4];
    gotoXY(73, 0);
    SetConsoleTextAttribute(hconsole, 12);
    cout << "Score: " << snake1.getScore();
    gotoXY(85, 0);
    SetConsoleTextAttribute(hconsole, 12);
    cout << "HighScore: " << snakeData[2];

    do
    {
        if (_kbhit())
        {
            option = _getch();
            if (option == 27 && paused)
            {
                break;
            }
            if (option == 27 && !paused)
            {
                option = 0;
                paused = true;
                pauseGame();
            }
            if (option == 13 && paused)
            {
                system("cls");
                snake1.drawWall();
                if (snake2Alive)
                {
                    snake2.drawSnake();
                    gotoXY(3, 0);
                    SetConsoleTextAttribute(hconsole, 10);
                    cout << "Score: " << snake2.getScore();
                    gotoXY(15, 0);
                    SetConsoleTextAttribute(hconsole, 10);
                    cout << "HighScore: " << snakeData[4];
                }
                if (snake1Alive)
                {
                    snake1.drawSnake();
                    gotoXY(73, 0);
                    SetConsoleTextAttribute(hconsole, 12);
                    cout << "Score: " << snake1.getScore();
                    gotoXY(85, 0);
                    SetConsoleTextAttribute(hconsole, 12);
                    cout << "HighScore: " << snakeData[2];
                }
                paused = false;
            }
        }
        if (paused)
        {
            continue;
        }
        switch (option)
        {
        case 72:
            snake1.turnUp();
            snake1Moving = 1;
            break;
        case 80:
            snake1.turnDown();
            break;
        case 75:
            snake1.turnLeft();
            snake1Moving = 1;
            break;
        case 77:
            snake1.turnRight();
            snake1Moving = 1;
            break;
        case 'w':
            snake2.turnUp();
            snake2Moving = 1;
            break;
        case 's':
            snake2.turnDown();
            break;
        case 'a':
            snake2.turnLeft();
            snake2Moving = 1;
            break;
        case 'd':
            snake2.turnRight();
            snake2Moving = 1;
            break;
        }

        if (snake1Moving && snake2Moving && snake1Alive && snake2Alive)
        {
            verticalSpeed = 30;
            horizontalSpeed = 15;
        }
        if (snake1Moving && snake1Alive)
        {
            gotoXY(73, 0);
            SetConsoleTextAttribute(hconsole, 12);
            cout << "Score: " << snake1.getScore();
            gotoXY(85, 0);
            SetConsoleTextAttribute(hconsole, 12);
            cout << "HighScore: " << snakeData[2];

            if (snake1.getScore() > snakeData[2])
            {
                snakeData[2] = snake1.getScore();
            }
            snake1.move(verticalSpeed, horizontalSpeed);
        }
        if (snake2Moving && snake2Alive)
        {
            gotoXY(3, 0);
            SetConsoleTextAttribute(hconsole, 10);
            cout << "Score: " << snake2.getScore();
            gotoXY(15, 0);
            SetConsoleTextAttribute(hconsole, 10);
            cout << "HighScore: " << snakeData[4];

            if (snake2.getScore() > snakeData[4])
            {
                snakeData[4] = snake2.getScore();
            }
            snake2.move(verticalSpeed, horizontalSpeed);
        }
        if (snake1.getFlag() && snake1Alive)
        {
            snake1Alive = 0;
            verticalSpeed = 85;
            horizontalSpeed = 35;
            snake1.clearSnake();
            snake2.drawWall();
            snakeData[3]++;

            gotoXY(45, 0);
            SetConsoleTextAttribute(hconsole, 12);
            cout << "Player1 died";
            gotoXY(3, 0);
            SetConsoleTextAttribute(hconsole, 10);
            cout << "Score: " << snake2.getScore();
            gotoXY(15, 0);
            SetConsoleTextAttribute(hconsole, 10);
            cout << "HighScore: " << snakeData[4];
        }
        if (snake2.getFlag() && snake2Alive)
        {
            snake2Alive = 0;
            verticalSpeed = 85;
            horizontalSpeed = 35;
            snake2.clearSnake();
            snake1.drawWall();
            snakeData[5]++;

            gotoXY(45, 0);
            SetConsoleTextAttribute(hconsole, 10);
            cout << "Player2 died";
            gotoXY(73, 0);
            SetConsoleTextAttribute(hconsole, 12);
            cout << "Score: " << snake1.getScore();
            gotoXY(85, 0);
            SetConsoleTextAttribute(hconsole, 12);
            cout << "HighScore: " << snakeData[2];
        }
        if (snake1.cellX(0) == snake2.cellX(0) && snake1.cellY(0) == snake2.cellY(0))
        {
            snake1Alive = 0;
            snake2Alive = 0;
        }

        for (int i = 1; i < snake2.getSize() && snake1Alive && snake2Alive; i++)
        {
            if (snake1.cellX(0) == snake2.cellX(i) && snake1.cellY(0) == snake2.cellY(i))
            {
                snake1Alive = 0;
                verticalSpeed = 85;
                horizontalSpeed = 35;
                snake1.clearSnake();
                snake2.drawSnake();
                snake2.drawWall();
                snakeData[3]++;

                gotoXY(45, 0);
                SetConsoleTextAttribute(hconsole, 12);
                cout << "Player1 died";
                gotoXY(3, 0);
                SetConsoleTextAttribute(hconsole, 10);
                cout << "Score: " << snake2.getScore();
                gotoXY(15, 0);
                SetConsoleTextAttribute(hconsole, 10);
                cout << "HighScore: " << snakeData[4];
            }
        }

        if (snake1.cellX(0) == snake2.fruitX() && snake1.cellY(0) == snake2.fruitY() && snake1Alive && snake2Alive)
        {
            gotoXY(35, 0);
            SetConsoleTextAttribute(hconsole, 12);
            cout << "Eat your own fruit Player1";
        }
        if (snake1.cellX(snake1.getSize()) == snake2.fruitX() && snake1.cellY(snake1.getSize()) == snake2.fruitY() && snake1Alive && snake2Alive)
        {
            snake2.drawSampleFruit();
        }

        for (int i = 1; i < snake1.getSize() && snake1Alive && snake2Alive; i++)
        {
            if (snake2.cellX(0) == snake1.cellX(i) && snake2.cellY(0) == snake1.cellY(i))
            {
                snake2Alive = 0;
                verticalSpeed = 85;
                horizontalSpeed = 35;
                snake2.clearSnake();
                snake1.drawSnake();
                snake1.drawWall();
                snakeData[5]++;

                gotoXY(45, 0);
                SetConsoleTextAttribute(hconsole, 10);
                cout << "Player2 died";
                gotoXY(73, 0);
                SetConsoleTextAttribute(hconsole, 12);
                cout << "Score: " << snake1.getScore();
                gotoXY(85, 0);
                SetConsoleTextAttribute(hconsole, 12);
                cout << "HighScore: " << snakeData[2];
            }
        }

        if (snake2.cellX(0) == snake1.fruitX() && snake2.cellY(0) == snake1.fruitY() && snake1Alive && snake2Alive)
        {
            gotoXY(35, 0);
            SetConsoleTextAttribute(hconsole, 10);
            cout << "Eat your own fruit Player2";
        }
        if (snake2.cellX(snake2.getSize()) == snake1.fruitX() && snake2.cellY(snake2.getSize()) == snake1.fruitY() && snake1Alive && snake2Alive)
        {
            snake1.drawSampleFruit();
        }
        if (!snake1Alive && !snake2Alive)
        {
            break;
        }
    } while (option != 27);

    system("cls");
    if (option != 27)
    {
        gotoXY(30, 10);
        SetConsoleTextAttribute(hconsole, 13);
        cout << "Game Over 😆!";
        gotoXY(30, 11);
        SetConsoleTextAttribute(hconsole, 12);
        cout << "Player1 Score: " << snake1.getScore();
        gotoXY(30, 12);
        SetConsoleTextAttribute(hconsole, 10);
        cout << "Player2 Score: " << snake2.getScore();
        gotoXY(30, 14);
        SetConsoleTextAttribute(hconsole, 6);
        cout << "Please Press ENTER to Continue.";
        cin.ignore();
    }
}

void instructions(int x, int y)
{
    system("cls");
    gotoXY(x, y);
    SetConsoleTextAttribute(hconsole, 3);
    cout << "Instructions:";
    gotoXY(x, y + 2);
    SetConsoleTextAttribute(hconsole, 13);
    cout << "Single-Player:";
    SetConsoleTextAttribute(hconsole, 3);
    gotoXY(x, y + 3);
    cout << "\tDon't hit the Wall.";
    gotoXY(x, y + 4);
    cout << "\tDon't hit your snake's own body";
    gotoXY(x, y + 5);
    cout << "\tEat the FRUIT '@' to get points with your SNAKE '#'";
    SetConsoleTextAttribute(hconsole, 6);
    gotoXY(x, y + 6);
    cout << "\tPress UP-ARROW to move up";
    gotoXY(x, y + 7);
    cout << "\tPress DOWN-ARROW to move down";
    gotoXY(x, y + 8);
    cout << "\tPress LEFT-ARROW to move left";
    gotoXY(x, y + 9);
    cout << "\tPress RIGHT-ARROW to move right";
    gotoXY(x, y + 11);
    SetConsoleTextAttribute(hconsole, 13);
    cout << "Multi-Player:";
    SetConsoleTextAttribute(hconsole, 3);
    gotoXY(x, y + 12);
    cout << "\tDon't hit the Wall.";
    gotoXY(x, y + 13);
    cout << "\tDon't hit your snake's own body";
    gotoXY(x, y + 14);
    cout << "\tEat your own colored FRUIT '@' to get points with your SNAKE '#'";
    gotoXY(x, y + 15);
    cout << "\tIf your SNAKE's head hits the other SNAKE's body, you'll die";
    gotoXY(x, y + 16);
    cout << "\tIf your both SNAKE's head hit each other, game will end";
    SetConsoleTextAttribute(hconsole, 13);
    gotoXY(x, y + 17);
    cout << "\tPlayer 1:";
    SetConsoleTextAttribute(hconsole, 12);
    gotoXY(x, y + 18);
    cout << "\t\tPress UP-ARROW to move up";
    gotoXY(x, y + 19);
    cout << "\t\tPress DOWN-ARROW to move down";
    gotoXY(x, y + 20);
    cout << "\t\tPress LEFT-ARROW to move left";
    gotoXY(x, y + 21);
    cout << "\t\tPress RIGHT-ARROW to move right";
    SetConsoleTextAttribute(hconsole, 13);
    gotoXY(x, y + 22);
    cout << "\tPlayer 2:";
    SetConsoleTextAttribute(hconsole, 10);
    gotoXY(x, y + 23);
    cout << "\t\tPress 'W' to move up";
    gotoXY(x, y + 24);
    cout << "\t\tPress 'S' to move down";
    gotoXY(x, y + 25);
    cout << "\t\tPress 'A' to move left";
    gotoXY(x, y + 26);
    cout << "\t\tPress 'D' to move right";
    gotoXY(x, y + 28);
    SetConsoleTextAttribute(hconsole, 9);
    cout << "Press 'escape' to exit";
    getch();
}

void scoreBoard()
{
    system("cls");
    gotoXY(10, 5);
    SetConsoleTextAttribute(hconsole, 3);
    cout << "Single-Player:";
    gotoXY(10, 6);
    SetConsoleTextAttribute(hconsole, 6);
    cout << "\tSingle-Player HighScore: " << snakeData[0]
         << " | Death Count: " << snakeData[1];
    gotoXY(10, 8);
    SetConsoleTextAttribute(hconsole, 3);
    cout << "Multiplayer:";
    gotoXY(10, 9);
    SetConsoleTextAttribute(hconsole, 12);
    cout << "\tPlayer1 HighScore: " << snakeData[2]
         << " | Death Count: " << snakeData[3];
    gotoXY(10, 10);
    SetConsoleTextAttribute(hconsole, 10);
    cout << "\tPlayer2 HighScore: " << snakeData[4]
         << " | Death Count: " << snakeData[5];
    getch();
}

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));
    SetConsoleOutputCP(CP_UTF8);
    fstream file("snakeData.txt", ios::in | ios::out);

    if (!file.is_open())
    {
        file.open("snakeData.txt", ios::out);
        if (!file.is_open())
        {
            cerr << "Failed to create the file!" << endl;
            return 1;
        }

        for (int i = 1; i <= 6; ++i)
        {
            file << i - i << ' ';
        }
        file.close();
    }

    file.close();
    file.open("snakeData.txt", ios::in | ios::out);

    for (int i = 0; i < 6; ++i)
    {
        file >> snakeData[i];
    }

    do
    {
        system("cls");
        gotoXY(10, 5);
        SetConsoleTextAttribute(hconsole, 6);
        cout << " ---------------------------- ";
        gotoXY(10, 6);
        SetConsoleTextAttribute(hconsole, 3);
        cout << " |     Snake 🐍 Game 💨     | ";
        gotoXY(10, 7);
        SetConsoleTextAttribute(hconsole, 6);
        cout << " ---------------------------- ";
        gotoXY(10, 9);
        SetConsoleTextAttribute(hconsole, 6);
        cout << "1. 🔥 Start Single-Player Game";
        gotoXY(10, 10);
        SetConsoleTextAttribute(hconsole, 12);
        cout << "2. ⚔️  Start Multi-player Game";
        gotoXY(10, 11);
        SetConsoleTextAttribute(hconsole, 13);
        cout << "3. 📜 Instructions";
        gotoXY(10, 12);
        SetConsoleTextAttribute(hconsole, 10);
        cout << "4. 💯 ScoreBoard";
        gotoXY(10, 13);
        SetConsoleTextAttribute(hconsole, 9);
        cout << "5. 🔴 Quit";
        gotoXY(10, 15);
        SetConsoleTextAttribute(hconsole, 3);
        cout << "➡️  Select option: ";
        char option = getche();

        if (option == '1')
        {
            play();
        }
        else if (option == '2')
        {
            playMulti();
        }
        else if (option == '3')
        {
            instructions(10, 0);
        }
        else if (option == '4')
        {
            scoreBoard();
        }
        else if (option == '5')
        {
            file.seekp(0);
            for (int i = 0; i < 6; ++i)
            {
                file << snakeData[i] << ' ';
            }
            file.close();
            exit(0);
        }
    } while (1);

    return 0;
}
