#include <iostream>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <cstdlib>

//variables

using namespace std;
bool gameOver; //constant
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX [100], tailY[100];
int nTail;
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN}; //track direction of snake
eDirection dir;


#ifdef _WIN32
#include <windows.h>
void sleep_for_ms (int num_MS) {
   Sleep(num_MS);
}
#elif defined (__APPLE__)
#include <unistd.h>
void sleep_for_ms (int num_MS) {
   cout << "Sleeping for " << num_MS << " milliseconds " << endl;
   usleep (1000 * num_MS);
}

#elif defined (__linux__)
#include <unistd>
void sleep_for_ms (int num_MS) {
   cout << "Sleeping for " << num_MS << " milliseconds " << endl;
   usleep (1000 * num_MS);
}
#endif

const int STDIN = 0;

bool _kbhit() {
   static bool initialized = false;

   if (! initialized) {
      // Use termios to turn off line buffering
      termios term;
      tcgetattr(STDIN, &term);
      term.c_lflag &= ~ICANON;
      tcsetattr(STDIN, TCSANOW, &term);
      setbuf(stdin, NULL);
      initialized = true;
   }

   int bytesWaiting;
   ioctl(STDIN, FIONREAD, &bytesWaiting);
   return bytesWaiting;
}

void restoreKeyBuf () {
   termios term;
   tcgetattr(STDIN, &term);
   term.c_lflag |= ICANON;
   tcsetattr(STDIN, TCSANOW, &term);
   setbuf(stdin, NULL);
}
const int waitForKey = 5;

//start code for game here ***

void Setup()
{
  gameOver = false;
  dir = STOP;
  x = width / 2;
  y = height /2;
  fruitX = rand () % width; //random placement of fruit
  fruitY = rand () % height;
  score = 0;
}
void Draw()
{
system("clear");
for (int i = 0; i < width + 2; i++) //top border of map
    cout <<"*";
cout <<endl;

for (int i = 0; i < height; i++)
{
  for (int j = 0; j < width; j++)
  {
    if (j == 0)
    cout <<"*";
if (i == y && j == x)
cout << "O";
else if (i == fruitY && j == fruitX)
cout << "@";
else
{
  bool print = false;
  for (int k = 0; k < nTail; k++)
  {

    if(tailX[k] == j && tailY[k] == i)
    {
      cout <<"o";
      print = true;
    }

  }
  if(!print)
  cout<< " ";
}


    if (j == width-1) //wall
    cout << "*";

  }
  cout <<endl;
}
for (int i = 0; i < width + 2; i++)
cout <<"*";
cout << endl;
cout <<"Score:" << score << endl;
}
void Input()
{
_kbhit();
{

switch (getch())

{
  case'a':
  dir = LEFT;
  break;
  case 'd' :
  dir = RIGHT;
  break;
  case 'w' :
  dir = UP;
  break;
  case 's':
  dir = DOWN;
  break;
  case 'x':
  gameOver = true;
  break;
}
}
}
void Logic()
{
  int prevX = tailX[0];
  int prevY = tailY[0];
  int prev2X, prev2Y;
  tailX[0] = x;
  tailY[0] = y; //tells the snake tail to follow the head
  for (int i = 1; i <nTail; i++)
{
  prev2X = tailX[i];
  prev2Y = tailY [i];
  tailX[i] = prevX;
  tailY[i] = prevY;
  prevX = prev2X;
  prevY = prev2Y;
}
switch(dir)
{
  case LEFT:
  x--;
  break;

  case RIGHT:
  x++;
  break;

  case UP:
  y--;
  break;

  case DOWN:
    y++;
  break;

  default:
  break;
}
 if ( x > width || x < 0 || y > height || y < 0)
 gameOver = true; //terminates game if snake touches a border
 for (int i = 0; i <nTail; i++)
 if(tailX[i]== x && tailY [i] == y)//if you hit the tail, the game terminates
 gameOver = true;
if (x == fruitX && y == fruitY)
{
  score += 10;
  fruitX = rand () % width;
  fruitY = rand () % height;
  nTail++;
}
}

int main ()
{
  Setup();
  while (!gameOver)
  {
    Draw();
    Input();
    Logic();
    //sleep(10) slow the game down
  }
  return 0;
}
