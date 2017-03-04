#include<iostream>
#include<conio.h>
#include<Windows.h>
#include <time.h>
#include<wincon.h>//setconsoletitleA()
#include <limits>//struct console
#include <stdexcept>//struct console+
#pragma warning(disable:4996) // _CRT_SECURE_NO_WARNINGS

bool MultiPlayer = false;

using namespace std;

char map[20][60] = {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
					"X                                                         X",
					"X       XXX                                  XXX          X",
					"X       XXX                                  XXX          X",
					"X                           XXX                           X",
					"X       7777                XXX              7777         X",
					"X       7777        888             888      7777         X",
					"X         7777      888             888         7777      X",
					"X         7777      888             888         7777      X",
					"X         7777      888     777     888         7777      X",
					"XXX                 888    88888    888                 XXX",
					"X       7777        888   9999999   888      7777         X",
					"X       7777        888    88888    888      7777         X",
					"X         7777      888     777     888         7777      X",
					"X         7777      888             888         7777      X",
					"X                   888             888                   X",
					"X       XXX               999999             XXX          X",
					"X       XXX               98KK89             XXX          X",
					"X                         98KK89                          X",
					"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" };

class Tank{
	int x;
	int y;
	int rotation;
	int health;
	int enemyDestroyed;
	int color;
	char up;
	char down;
	char left;
	char right;
	char Union;
	bool live;
public:
	Tank(int a = 0,int b = 0,char c = 'E',int h = 1){
		x = a;
		y = b;
		Union = c;
		health = h;
		rotation = 3;
		color = 14;
		up = '5';
		down = '2';
		right = '6';
		left = '4';
		enemyDestroyed = 0;
		map[x][y] = c;
		live = false;
	}
	~Tank(){ 
		map[x][y] = ' ';
		x = 0;
		y = 0;
	}
	void Destroy(){
		map[x][y] = ' ';
		x = 0;
		y = 0;
		live = false;
	}
	void setColor(int i) { color = i; }
	void setChars(char r, char d, char l, char u){ right = r; down = d; left = l; up = u; }
	void setL(bool i){ live = i; }
	void seteD(int i){ enemyDestroyed = i; }
	void setX(int i){ x = i; }
	void setY(int i){ y = i; }
	void setR(int i){ rotation = i; }
	void setU(char c){ Union = c; }
	void downHealth(){ health--; }
	void upHealth(){ health++; }
	void fullHealth(){ health = 5; }
	void uPenemyDestroyed(){ enemyDestroyed++; }
	bool getL(){ return live; }
	int geteD(){ return enemyDestroyed; }
	int getX(){ return x; }
	int getY(){ return y; }
	int getR(){ return rotation; }
	int getH(){ return health; }
	char getU(){ return Union; }
	char getUp(){ return up; }
	char getDown(){ return down; }
	char getLeft() { return left; }
	char getRight(){ return right; }
	int getColor(){ return color; }
	
};
Tank tank1(5, 5, 72, 5);
Tank tank2(0, 0, 71, 5);//71 G 72 H
Tank enemyT[20];

int enemyS = 0;
int enemyHit = 0;
int P1Hit = 0;
int P2Hit = 0;
int sysColor = 9; // default renk
int holdSec = 0;

void CPos(int, int);
void DrawMap();
void FirstTimeDrawMap();
void goRight(Tank &tank);
void goLeft(Tank &tank);
void goUp(Tank &tank);
void goDown(Tank &tank);
void Fire(Tank &tank);
void EnemyMove();
void CreateEnemy();
void ShowConsoleCursor(bool showFlag);
int Start();
void GameOver(int i);//parametre kazanan kullanýcýnýn inti // 1 p1 win // 2 p2 win // 3 comp. win


int main()
{
	RECT r;
	HWND console = GetConsoleWindow();
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 1300, 800, TRUE);
	//SetConsoleOutputCP(1252); 1252 latin char destek
	SetConsoleTitleA("Tank Oyunu v1.0");
	
	//(e -> 6245 31nm) (p1->asdf qwer) (p2->klop tyvc) (sirasiyla sag asaðý sol yukarý)
	tank1.setChars('a', 's', 'd', 'f');
	tank1.setColor(11);
	tank2.setChars('k', 'l', 'o', 'p');
	tank2.setColor(10);

	srand(time(NULL));
	

	time_t currentTime;
	struct tm *localTime;
	int Sec;
	int eMove = 1;
	int eCreate = 1;
	int lating = 0;

	tank1.setL(true);
	tank2.setL(true);

	if (Start()){
		tank2.setX(17);
		tank2.setY(53);
	}

	system("color 7");
	ShowConsoleCursor(false);//imlec
	FirstTimeDrawMap();
	CPos(0,0);//baþta oluþan tankin H sini silmek için
	cout << (char)219;
	

	char c = 'p';
	while (c != 27){
		c = 'p';
		if (_kbhit()){
			c = _getch();
		}
		if (MultiPlayer){
			switch (c){
			case 'd':
				goRight(tank1);
				break;
			case 'a':
				goLeft(tank1);
				break;
			case 'w':
				goUp(tank1);
				break;
			case 's':
				goDown(tank1);
				break;
			case 'b':
				Fire(tank1);
				P1Hit++;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
				CPos(4, 73);
				cout << "  ";
				CPos(4, 73);
				cout << P1Hit;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				break;
			case 72:
				goUp(tank2);
				break;
			case 75:
				goLeft(tank2);
				break;
			case 77:
				goRight(tank2);
				break;
			case 80:
				goDown(tank2);
				break;
			case 'o':
				Fire(tank2);
				P2Hit++;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
				CPos(12, 73);
				cout << "  ";
				CPos(12, 73);
				cout << P2Hit;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				break;
			default:
				break;
			}
		}
		else{
			switch (c){
			case 'd':
				goRight(tank1);
				break;
			case 'a':
				goLeft(tank1);
				break;
			case 'w':
				goUp(tank1);
				break;
			case 's':
				goDown(tank1);
				break;
			case 'b':
				Fire(tank1);
				P1Hit++;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
				CPos(4, 73);
				cout << "  ";
				CPos(4, 73);
				cout << P1Hit;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				break;
			default:
				break;
			}
		}
		time(&currentTime);
		localTime = localtime(&currentTime);
		Sec = localTime->tm_sec;
		/*if (Sec % 2 == 0 && eMove == 1){//																														
			EnemyMove();
			eMove = 0;
		}
		if (Sec % 2 == 1){
			eMove = 1;
		}*/
		if (holdSec != Sec){
			holdSec = Sec;
			EnemyMove();
		}

		if (Sec % 6 == 0 && eCreate == 1){
			CreateEnemy();
			eCreate = 0;
		}
		if (Sec % 6 == 1){
			eCreate = 1;
		}
		DrawMap();
		//Sleep(25);
	}


	system("pause");
	return 0;
}


void CPos(int i, int j)
{
	COORD cPos;
	cPos.X = j;
	cPos.Y = i;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos);
}
void DrawMap()
{
	for (int i = 1; i < 19; i++)
	{
		for (int j = 1; j < 59; j++)
		{
			if (map[i][j] == '3'){
				map[i][j] = '6';
			}
			if (map[i][j] == '1'){
				map[i][j] = '2';
			}
			if (map[i][j] == 'n'){
				map[i][j] = '5';
			}
			if (map[i][j] == 'm'){
				map[i][j] = '4';
			}
			if (map[i][j] == 'q'){
				map[i][j] = 'a';
			}
			if (map[i][j] == 'w'){
				map[i][j] = 's';
			}
			if (map[i][j] == 'e'){
				map[i][j] = 'd';
			}
			if (map[i][j] == 'r'){
				map[i][j] = 'f';
			}
			if (map[i][j] == 't'){
				map[i][j] = 'k';
			}
			if (map[i][j] == 'y'){
				map[i][j] = 'l';
			}
			if (map[i][j] == 'v'){
				map[i][j] = 'o';
			}
			if (map[i][j] == 'c'){
				map[i][j] = 'p';
			}

		}
	}


	for (int i = 1; i < 19; i++)
	{
		for (int j = 1; j < 59; j++)
		{
			switch (map[i][j])// H G E 7 8 9 K 6 7 8 9 3 4 5 m n (e -> 6245 31nm) (p1 -> asdf qwer) (p2 -> klop tyvc) (sirasiyla sag asaðý sol yukarý)
			{
			case 'H'://player1
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
				CPos(i, j);
				cout << 'O';
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				break;
			case 'G'://player2
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
				CPos(i, j);
				cout << 'O';
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				break;
			case 'E'://enemy
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), enemyT[0].getColor());
				CPos(i, j);
				cout << 'o';
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				break;
			case '7': // 1. seviye duvar
				CPos(i, j);
				cout << (char)176;
				break;
			case '8': // 2. seviye duvar
				CPos(i, j);
				cout << (char)177;
				break;
			case '9': // 3. seviye duvar kýrýlamaz
				CPos(i, j);
				cout << (char)178;
				break;
			case 'K':
				CPos(i, j);
				cout << (char)190;
				break;
			case ' ':
				CPos(i, j);
				cout << ' ';
				break;
																								//ENEMY FiRE
			case '6'://saga mermi
				if (map[i][j + 1] == 'X'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j + 1] == ' '){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = '3';//3 saga gidicek mermi
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), enemyT[0].getColor());
					cout << '*';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				else if (map[i][j + 1] == '3' || map[i][j + 1] == '2' || map[i][j + 1] == '1' || map[i][j + 1] == '4' || map[i][j + 1] == '5' || map[i][j + 1] == 'n' || map[i][j + 1] == 'm') {
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = ' ';
					cout << ' ';
				}
				else if(map[i][j + 1] == '9'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = '8';
					cout << (char)177;
				}
				else if (map[i][j + 1] == '8'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = '7';
					cout << (char)176;
				}
				else if (map[i][j + 1] == '7'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = ' ';
					cout << ' ';
				}
				else if (map[i][j + 1] == 'K'){
					GameOver(3); // 3 Bilgisayar kazandi
				}
				else if (map[i][j + 1] == 'G'){
					tank2.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
					CPos(11, 72);
					cout << tank2.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank2.getH() == 0){
						GameOver(1);//parametre kazanan kullanýcýnýn inti
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j + 1] == 'H'){
					tank1.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					CPos(3, 72);
					cout << tank1.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank1.getH() == 0){
						GameOver(2);
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j + 1] == 'E'){
					for (int k = 0; k < 21; k++){
						if (enemyT[k].getX() == i && enemyT[k].getY() == j+1){
							enemyT[k].Destroy();
						}
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					enemyHit++;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), enemyT[0].getColor());
					CPos(18, 76);
					cout << "   ";
					CPos(18, 76);
					cout << enemyHit;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				break;
			case '2'://asagi mermi
				if (map[i + 1][j] == 'X'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i + 1][j] == ' '){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i + 1, j);
					map[i + 1][j] = '1'; // asagi gidicek mermi
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), enemyT[0].getColor());
					cout << '*';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				else if (map[i + 1][j] == '3' || map[i + 1][j] == '1' || map[i + 1][j] == '4' || map[i + 1][j] == '5' || map[i + 1][j] == '6' || map[i + 1][j] == 'n' || map[i + 1][j] == 'm'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i+1, j);
					map[i+1][j] = ' ';
					cout << ' ';
				}
				else if (map[i+1][j] == '9'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i+1, j);
					map[i+1][j] = '8';
					cout << (char)177;
				}
				else if (map[i+1][j] == '8'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i+1, j);
					map[i+1][j] = '7';
					cout << (char)176;
				}
				else if (map[i+1][j] == '7'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i+1, j);
					map[i+1][j ] = ' ';
					cout << ' ';
				}
				else if (map[i+1][j] == 'K'){
					GameOver(3); // Bilgisayar kazandi
				}
				else if (map[i + 1][j] == 'G'){
					tank2.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
					CPos(11, 72);
					cout << tank2.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank2.getH() == 0){
						GameOver(1);//parametre kazanan kullanýcýnýn inti
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i + 1][j] == 'H'){
					tank1.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					CPos(3, 72);
					cout << tank1.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank1.getH() == 0){
						GameOver(2);
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i + 1][j] == 'E'){
					for (int k = 0; k < 21; k++){
						if (enemyT[k].getX() == i + 1 && enemyT[k].getY() == j){
							enemyT[k].Destroy();
						}
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					enemyHit++;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), enemyT[0].getColor());
					CPos(18, 76);
					cout << "   ";
					CPos(18, 76);
					cout << enemyHit;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				break;
			case '4'://sola mermi
				if (map[i][j - 1] == 'X'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == ' '){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = 'm'; // sola gidicek mermi
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), enemyT[0].getColor());
					cout << '*';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				else if (map[i][j - 1] == '3' || map[i][j - 1] == '2' || map[i][j - 1] == '1' || map[i][j - 1] == '5' || map[i][j - 1] == '6' || map[i][j - 1] == 'n' || map[i][j - 1] == 'm'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == '9'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = '8';
					cout << (char)177;
				}
				else if (map[i][j - 1] == '8'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = '7';
					cout << (char)176;
				}
				else if (map[i][j - 1] == '7'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == 'K'){
					GameOver(3); // Bilgisayar kazandi
				}
				else if (map[i][j - 1] == 'G'){
					tank2.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
					CPos(11, 72);
					cout << tank2.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank2.getH() == 0){
						GameOver(1);//parametre kazanan kullanýcýnýn inti
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == 'H'){
					tank1.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					CPos(3, 72);
					cout << tank1.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank1.getH() == 0){
						GameOver(2);
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == 'E'){
					for (int k = 0; k < 21; k++){
						if (enemyT[k].getX() == i && enemyT[k].getY() == j -1){
							enemyT[k].Destroy();
						}
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					enemyHit++;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), enemyT[0].getColor());
					CPos(18, 76);
					cout << "   ";
					CPos(18, 76);
					cout << enemyHit;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				break;
			case '5'://yukari mermi
				if (map[i - 1][j] == 'X'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == ' '){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = 'n'; // yukari gidicek mermi
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), enemyT[0].getColor());
					cout << '*';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				else if (map[i - 1][j] == '3' || map[i - 1][j] == '2' || map[i - 1][j] == '1' || map[i - 1][j] == '4' || map[i - 1][j] == '6' || map[i - 1][j] == 'n' || map[i - 1][j] == 'm'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == '9'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = '8';
					cout << (char)177;
				}
				else if (map[i - 1][j] == '8'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = '7';
					cout << (char)176;
				}
				else if (map[i - 1][j] == '7'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == 'K'){
					GameOver(3); // Bilgisayar kazandi
				}
				else if (map[i - 1][j] == 'G'){
					tank2.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
					CPos(11, 72);
					cout << tank2.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank2.getH() == 0){
						GameOver(1);//parametre kazanan kullanýcýnýn inti
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == 'H'){
					tank1.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					CPos(3, 72);
					cout << tank1.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank1.getH() == 0){
						GameOver(2);
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == 'E'){
					for (int k = 0; k < 21; k++){
						if (enemyT[k].getX() == i -1 && enemyT[k].getY() == j){
							enemyT[k].Destroy();
						}
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					enemyHit++;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), enemyT[0].getColor());
					CPos(18, 76);
					cout << "   ";
					CPos(18, 76);
					cout << enemyHit;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor); 
				}
				break;
																									//H  P1 FiRE	(p1 -> asdf qwer)   (sirasiyla sag asagi sol yukarý)
			case 'a'://saga mermi //a olucak																																				//
				if (map[i][j + 1] == 'X'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j + 1] == ' '){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = 'q';//3 saga gidicek mermi
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					cout << '*';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				else if (map[i][j + 1] == '3' || map[i][j + 1] == '2' || map[i][j + 1] == '1' || map[i][j + 1] == '4' || map[i][j + 1] == '5' || map[i][j + 1] == 'n' || map[i][j + 1] == 'm' ||
					map[i][j + 1] == 't' || map[i][j + 1] == 'l' || map[i][j + 1] == 'o' || map[i][j + 1] == 'p' || map[i][j + 1] == 'y' || map[i][j + 1] == 'v' || map[i][j + 1] == 'c' ||
					map[i][j + 1] == 'k' || map[i][j + 1] == 's' || map[i][j + 1] == 'd' || map[i][j + 1] == 'f' || map[i][j + 1] == 'q' || map[i][j + 1] == 'w' || map[i][j + 1] == 'e' || map[i][j + 1] == 'r') {
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = ' ';
					cout << ' ';
				}
				else if (map[i][j + 1] == '9'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = '8';
					cout << (char)177;
				}
				else if (map[i][j + 1] == '8'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = '7';
					cout << (char)176;
				}
				else if (map[i][j + 1] == '7'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = ' ';
					cout << ' ';
				}
				else if (map[i][j + 1] == 'K'){
					GameOver(3); // 3 Bilgisayar kazandi
				}
				else if (map[i][j + 1] == 'G'){
					tank2.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
					CPos(11, 72);
					cout << tank2.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank2.getH() == 0){
						tank1.uPenemyDestroyed();
						GameOver(1);//parametre kazanan kullanýcýnýn inti
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j + 1] == 'E'){
					for (int k = 0; k < 21; k++){
						if (enemyT[k].getX() == i && enemyT[k].getY() == j + 1){
							enemyT[k].Destroy();
							tank1.uPenemyDestroyed();
						}
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					enemyHit++;
				}
				break;
			case 's'://asagi mermi // s 'w'																																								//
				if (map[i + 1][j] == 'X'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i + 1][j] == ' '){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i + 1, j);
					map[i + 1][j] = 'w'; // asagi gidicek mermi
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					cout << '*';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				else if (map[i + 1][j] == '3' || map[i + 1][j] == '1' || map[i + 1][j] == '4' || map[i + 1][j] == '5' || map[i + 1][j] == '6' || map[i + 1][j] == 'n' || map[i + 1][j] == 'm' ||
					map[i + 1][j] == 'a' || map[i + 1][j] == 'd' || map[i + 1][j] == 'f' || map[i + 1][j] == 'q' || map[i + 1][j] == 'w' || map[i + 1][j] == 'e' || map[i + 1][j] == 'r' || 
					map[i + 1][j] == 'c' || map[i + 1][j] == 'v' || map[i + 1][j] == 'y' || map[i + 1][j] == 't' || map[i + 1][j] == 'p' || map[i + 1][j] == 'o' || map[i + 1][j] == 'l' || map[i + 1][j] == 'k'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i + 1, j);
					map[i + 1][j] = ' ';
					cout << ' ';
				}
				else if (map[i + 1][j] == '9'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i + 1, j);
					map[i + 1][j] = '8';
					cout << (char)177;
				}
				else if (map[i + 1][j] == '8'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i + 1, j);
					map[i + 1][j] = '7';
					cout << (char)176;
				}
				else if (map[i + 1][j] == '7'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i + 1, j);
					map[i + 1][j] = ' ';
					cout << ' ';
				}
				else if (map[i + 1][j] == 'K'){
					GameOver(3); // Bilgisayar kazandi
				}
				else if (map[i + 1][j] == 'G'){
					tank2.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
					CPos(11, 72);
					cout << tank2.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank2.getH() == 0){
						tank1.uPenemyDestroyed();
						GameOver(1);//parametre kazanan kullanýcýnýn inti
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i + 1][j] == 'E'){
					for (int k = 0; k < 21; k++){
						if (enemyT[k].getX() == i + 1 && enemyT[k].getY() == j){
							enemyT[k].Destroy();
							tank1.uPenemyDestroyed();
						}
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				break;
			case 'd'://sola mermi // d 'e'																																						//
				if (map[i][j - 1] == 'X'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == ' '){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = 'e'; // sola gidicek mermi
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					cout << '*';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				else if (map[i][j - 1] == '3' || map[i][j - 1] == '2' || map[i][j - 1] == '1' || map[i][j - 1] == '5' || map[i][j - 1] == '6' || map[i][j - 1] == 'n' || map[i][j - 1] == 'm' ||
					map[i][j - 1] == 'a' || map[i][j - 1] == 's' || map[i][j - 1] == 'f' || map[i][j - 1] == 'q' || map[i][j - 1] == 'w' || map[i][j - 1] == 'e' || map[i][j - 1] == 'r' ||
					map[i][j - 1] == 'c' || map[i][j - 1] == 'v' || map[i][j - 1] == 't' || map[i][j - 1] == 'y' || map[i][j - 1] == 'p' || map[i][j - 1] == 'o' || map[i][j - 1] == 'l' || map[i][j - 1] == 'k'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == '9'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = '8';
					cout << (char)177;
				}
				else if (map[i][j - 1] == '8'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = '7';
					cout << (char)176;
				}
				else if (map[i][j - 1] == '7'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == 'K'){
					GameOver(3); // Bilgisayar kazandi
				}
				else if (map[i ][j - 1] == 'G'){
					tank2.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
					CPos(11, 72);
					cout << tank2.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank2.getH() == 0){
						tank1.uPenemyDestroyed();
						GameOver(1);//parametre kazanan kullanýcýnýn inti
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == 'E'){
					for (int k = 0; k < 21; k++){
						if (enemyT[k].getX() == i && enemyT[k].getY() == j - 1){
							enemyT[k].Destroy();
							tank1.uPenemyDestroyed();
						}
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				break;
			case 'f'://yukari mermi // f 'r'																																				//
				if (map[i - 1][j] == 'X'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == ' '){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = 'r'; // yukari gidicek mermi
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					cout << '*';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				else if (map[i - 1][j] == '3' || map[i - 1][j] == '2' || map[i - 1][j] == '1' || map[i - 1][j] == '4' || map[i - 1][j] == '6' || map[i - 1][j] == 'n' || map[i - 1][j] == 'm' ||
					map[i - 1][j] == 'a' || map[i - 1][j] == 's' || map[i - 1][j] == 'd' || map[i - 1][j] == 'q' || map[i - 1][j] == 'w' || map[i - 1][j] == 'e' || map[i - 1][j] == 'r' ||
					map[i - 1][j] == 'k' || map[i - 1][j] == 'l' || map[i - 1][j] == 'o' || map[i - 1][j] == 'p' || map[i - 1][j] == 't' || map[i - 1][j] == 'y' || map[i - 1][j] == 'v' || map[i - 1][j] == 'c'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == '9'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = '8';
					cout << (char)177;
				}
				else if (map[i - 1][j] == '8'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = '7';
					cout << (char)176;
				}
				else if (map[i - 1][j] == '7'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == 'K'){
					GameOver(3); // Bilgisayar kazandi
				}
				else if (map[i - 1][j] == 'G'){
					tank2.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
					CPos(11, 72);
					cout << tank2.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank2.getH() == 0){
						tank1.uPenemyDestroyed();
						GameOver(1);//parametre kazanan kullanýcýnýn inti
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == 'E'){
					for (int k = 0; k < 21; k++){
						if (enemyT[k].getX() == i - 1 && enemyT[k].getY() == j){
							enemyT[k].Destroy();
							tank1.uPenemyDestroyed();
						}
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				break;
																											//G  P2 FiRE	(p2 -> klop tyvc)   (sirasiyla sag asagi sol yukarý)
			case 'k'://saga mermi // k olcak																																					//
				if (map[i][j + 1] == 'X'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j + 1] == ' '){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = 't';//3 saga gidicek mermi
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
					cout << '*';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				else if (map[i][j + 1] == '3' || map[i][j + 1] == '2' || map[i][j + 1] == '1' || map[i][j + 1] == '4' || map[i][j + 1] == '5' || map[i][j + 1] == 'n' || map[i][j + 1] == 'm' ||
						map[i][j + 1] == 't' || map[i][j + 1] == 'l' || map[i][j + 1] == 'o' || map[i][j + 1] == 'p' || map[i][j + 1] == 'y' || map[i][j + 1] == 'v' || map[i][j + 1] == 'c' ||
						map[i][j + 1] == 'a' || map[i][j + 1] == 's' || map[i][j + 1] == 'd' || map[i][j + 1] == 'f' || map[i][j + 1] == 'q' || map[i][j + 1] == 'w' || map[i][j + 1] == 'e' || map[i][j + 1] == 'r') {
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = ' ';
					cout << ' ';
				}
				else if (map[i][j + 1] == '9'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = '8';
					cout << (char)177;
				}
				else if (map[i][j + 1] == '8'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = '7';
					cout << (char)176;
				}
				else if (map[i][j + 1] == '7'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j + 1);
					map[i][j + 1] = ' ';
					cout << ' ';
				}
				else if (map[i][j + 1] == 'K'){
					GameOver(3); // 3 Bilgisayar kazandi
				}
				else if (map[i][j + 1] == 'H'){
					tank1.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					CPos(3, 72);
					cout << tank1.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank1.getH() == 0){
						tank2.uPenemyDestroyed();
						GameOver(2);
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j + 1] == 'E'){
					for (int k = 0; k < 21; k++){
						if (enemyT[k].getX() == i && enemyT[k].getY() == j + 1){
							enemyT[k].Destroy();
							tank2.uPenemyDestroyed();
						}
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				break;
			case 'l'://asagi mermi // l 'y'																																													//
				if (map[i + 1][j] == 'X'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i + 1][j] == ' '){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i + 1, j);
					map[i + 1][j] = 'y'; // asagi gidicek mermi
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
					cout << '*';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				else if (map[i + 1][j] == '3' || map[i + 1][j] == '1' || map[i + 1][j] == '4' || map[i + 1][j] == '5' || map[i + 1][j] == '6' || map[i + 1][j] == 'n' || map[i + 1][j] == 'm' ||
					map[i + 1][j] == 'a' || map[i + 1][j] == 'd' || map[i + 1][j] == 'f' || map[i + 1][j] == 'q' || map[i + 1][j] == 'w' || map[i + 1][j] == 'e' || map[i + 1][j] == 'r' ||
					map[i + 1][j] == 'c' || map[i + 1][j] == 'v' || map[i + 1][j] == 'y' || map[i + 1][j] == 't' || map[i + 1][j] == 'p' || map[i + 1][j] == 'o' || map[i + 1][j] == 's' || map[i + 1][j] == 'k'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i + 1, j);
					map[i + 1][j] = ' ';
					cout << ' ';
				}
				else if (map[i + 1][j] == '9'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i + 1, j);
					map[i + 1][j] = '8';
					cout << (char)177;
				}
				else if (map[i + 1][j] == '8'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i + 1, j);
					map[i + 1][j] = '7';
					cout << (char)176;
				}
				else if (map[i + 1][j] == '7'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i + 1, j);
					map[i + 1][j] = ' ';
					cout << ' ';
				}
				else if (map[i + 1][j] == 'K'){
					GameOver(3); // Bilgisayar kazandi
				}
				else if (map[i+1][j] == 'H'){
					tank1.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					CPos(3, 72);
					cout << tank1.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank1.getH() == 0){
						tank2.uPenemyDestroyed();
						GameOver(2);
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i + 1][j] == 'E'){
					for (int k = 0; k < 21; k++){
						if (enemyT[k].getX() == i + 1 && enemyT[k].getY() == j){
							enemyT[k].Destroy();
							tank2.uPenemyDestroyed();
						}
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				break;
			case 'o'://sola mermi // o 'v'																																									//
				if (map[i][j - 1] == 'X'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == ' '){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = 'v'; // sola gidicek mermi
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
					cout << '*';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				else if (map[i][j - 1] == '3' || map[i][j - 1] == '2' || map[i][j - 1] == '1' || map[i][j - 1] == '5' || map[i][j - 1] == '6' || map[i][j - 1] == 'n' || map[i][j - 1] == 'm' ||
					map[i][j - 1] == 'a' || map[i][j - 1] == 's' || map[i][j - 1] == 'f' || map[i][j - 1] == 'q' || map[i][j - 1] == 'w' || map[i][j - 1] == 'e' || map[i][j - 1] == 'r' ||
					map[i][j - 1] == 'c' || map[i][j - 1] == 'v' || map[i][j - 1] == 't' || map[i][j - 1] == 'y' || map[i][j - 1] == 'f' || map[i][j - 1] == 'd' || map[i][j - 1] == 'l' || map[i][j - 1] == 'k'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == '9'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = '8';
					cout << (char)177;
				}
				else if (map[i][j - 1] == '8'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = '7';
					cout << (char)176;
				}
				else if (map[i][j - 1] == '7'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i, j - 1);
					map[i][j - 1] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == 'K'){
					GameOver(3); // Bilgisayar kazandi
				}
				else if (map[i][j - 1] == 'H'){
					tank1.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					CPos(3, 72);
					cout << tank1.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank1.getH() == 0){
						tank2.uPenemyDestroyed();
						GameOver(2);
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i][j - 1] == 'E'){
					for (int k = 0; k < 21; k++){
						if (enemyT[k].getX() == i && enemyT[k].getY() == j - 1){
							enemyT[k].Destroy();
							tank2.uPenemyDestroyed();
						}
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				break;
			case 'p'://yukari mermi // p 'c'																																												//
				if (map[i - 1][j] == 'X'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == ' '){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = 'c'; // yukari gidicek mermi
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
					cout << '*';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
				}
				else if (map[i - 1][j] == '3' || map[i - 1][j] == '2' || map[i - 1][j] == '1' || map[i - 1][j] == '4' || map[i - 1][j] == '6' || map[i - 1][j] == 'n' || map[i - 1][j] == 'm' ||
					map[i - 1][j] == 'a' || map[i - 1][j] == 's' || map[i - 1][j] == 'd' || map[i - 1][j] == 'q' || map[i - 1][j] == 'w' || map[i - 1][j] == 'e' || map[i - 1][j] == 'r' ||
					map[i - 1][j] == 'k' || map[i - 1][j] == 'l' || map[i - 1][j] == 'o' || map[i - 1][j] == 'p' || map[i - 1][j] == 't' || map[i - 1][j] == 'y' || map[i - 1][j] == 'v' || map[i - 1][j] == 'c'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == '9'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = '8';
					cout << (char)177;
				}
				else if (map[i - 1][j] == '8'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = '7';
					cout << (char)176;
				}
				else if (map[i - 1][j] == '7'){
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
					CPos(i - 1, j);
					map[i - 1][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == 'K'){
					GameOver(3); // Bilgisayar kazandi
				}
				else if (map[i - 1][j] == 'H'){
					tank1.downHealth();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					CPos(3, 72);
					cout << tank1.getH();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					if (tank1.getH() == 0){
						tank2.uPenemyDestroyed();
						GameOver(2);
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				else if (map[i - 1][j] == 'E'){
					for (int k = 0; k < 21; k++){
						if (enemyT[k].getX() == i - 1 && enemyT[k].getY() == j){
							enemyT[k].Destroy();
							tank2.uPenemyDestroyed();
						}
					}
					CPos(i, j);
					map[i][j] = ' ';
					cout << ' ';
				}
				break;
			default:
				break;
			}
		}
		cout << endl;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
	CPos(5, 81);//renk
	cout << tank1.geteD();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
	CPos(13, 81);
	cout << tank2.geteD();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);

	CPos(20, 0);
	return;
}
void FirstTimeDrawMap(){
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
	if (MultiPlayer){
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 60; j++)
			{
				switch (map[i][j])
				{
				case 'X':
					CPos(i, j);
					cout << (char)219;
					break;
				case 'H':
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
					CPos(i, j);
					cout << 'O';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
					break;
				case 'G':
					CPos(i, j);
					cout << 'O';
					break;
				case '7': // 1. seviye duvar
					CPos(i, j);
					cout << (char)176;
					break;
				case '8': // 2. seviye duvar
					CPos(i, j);
					cout << (char)177;
					break;
				case '9': // 3. seviye duvar kýrýlamaz
					CPos(i, j);
					cout << (char)178;
					break;
				case 'K':
					CPos(i, j);
					cout << (char)190;
					break;
				}
			}
			cout << endl;
		}
		CPos(17, 53);
		map[17][53] = 'G';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
		cout << 'O';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
		CPos(1, 66);
		cout << "1. Oyuncu";
		CPos(3, 66);							//can 3 72
		cout << "Can = 5";
		CPos(4, 66);
		cout << "Ates = 0";
		CPos(5, 66);
		cout << "Vurulan Tank = 0";
		CPos(6, 66);
		cout << "W,A,S,D Yonlendirme";
		CPos(7, 66);
		cout << "B Ates";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
		CPos(9, 66);
		cout << "2. Oyuncu"; 
		CPos(11, 66);							//can2 11 72
		cout << "Can = 5";
		CPos(12, 66);
		cout << "Ates = 0";
		CPos(13, 66);
		cout << "Vurulan Tank = 0";
		CPos(14, 66);
		cout << "Yon Tuslari Yonlendirme";
		CPos(15, 66);
		cout << "O Ates";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), enemyT[0].getColor());
		CPos(17, 66);
		cout << "Uretilecek = 20";
		CPos(18, 66);
		cout << "Vurulan = 0";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		CPos(19, 66);
		cout << "Esc Cikis";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
	}
	else{
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 60; j++)
			{
				switch (map[i][j])
				{
				case 'X':
					CPos(i, j);
					cout << (char)219;
					break;
				case 'H':
					CPos(i, j);
					cout << 'H';
					break;
				case '7': // 1. seviye duvar
					CPos(i, j);
					cout << (char)176;
					break;
				case '8': // 2. seviye duvar
					CPos(i, j);
					cout << (char)177;
					break;
				case '9': // 3. seviye duvar kýrýlamaz
					CPos(i, j);
					cout << (char)178;
					break;
				case 'K':
					CPos(i, j);
					cout << (char)190;
					break;
				}
			}
			cout << endl;
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
		CPos(1, 66);
		cout << "Hos Geldiniz";
		CPos(3, 66);
		cout << "Can = 5";
		CPos(4, 66);
		cout << "Ates = 0";
		CPos(5, 66);
		cout << "Vurulan Tank = 0";
		CPos(6, 66);
		cout << "W,A,S,D Yonlendirme";
		CPos(7, 66);
		cout << "B Ates";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), enemyT[0].getColor());
		CPos(17, 66);
		cout << "Uretilecek = 20";
		CPos(18, 66);
		cout << "Vurulan = 0";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		CPos(19, 66);
		cout << "Esc Cikis";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
	}
	return;
}
void goRight(Tank &tank){
	if (map[tank.getX()][tank.getY() + 1] == 'X'){

	}
	else if (map[tank.getX()][tank.getY() + 1] == ' ' && tank.getY() != 58){
		map[tank.getX()][tank.getY()] = ' ';

		tank.setY(tank.getY() + 1);

		map[tank.getX()][tank.getY()] = tank.getU();
	}
	tank.setR(3);
}
void goLeft(Tank &tank){
	if (map[tank.getX()][tank.getY() - 1] == 'X'){

	}
	else if (map[tank.getX()][tank.getY() - 1] == ' '){
		map[tank.getX()][tank.getY()] = ' ';

		tank.setY(tank.getY() - 1);

		map[tank.getX()][tank.getY()] = tank.getU();
	}
	tank.setR(1);
}
void goUp(Tank &tank){
	if (map[tank.getX() - 1][tank.getY()] == 'X'){

	}
	else if (map[tank.getX() - 1][tank.getY()] == ' '){
		map[tank.getX()][tank.getY()] = ' ';

		tank.setX(tank.getX() - 1);

		map[tank.getX()][tank.getY()] = tank.getU();
	}

	tank.setR(5);//yukari
}
void goDown(Tank &tank){
	if (map[tank.getX() + 1][tank.getY()] == 'X'){

	}
	else if (map[tank.getX() + 1][tank.getY()] == ' '){
		map[tank.getX()][tank.getY()] = ' ';

		tank.setX(tank.getX() + 1);

		map[tank.getX()][tank.getY()] = tank.getU();
	}
	tank.setR(2);
}
void Fire(Tank &tank){
	if (tank.getR() == 3 && map[tank.getX()][tank.getY() + 1] == ' '){
		map[tank.getX()][tank.getY() + 1] = tank.getRight();
		CPos(tank.getX(), tank.getY() + 1);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank.getColor());
		cout << '*';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
	}
	else if (tank.getR() == 1 && map[tank.getX()][tank.getY() - 1] == ' '){
		map[tank.getX()][tank.getY() - 1] = tank.getLeft();
		CPos(tank.getX(), tank.getY() - 1);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank.getColor());
		cout << '*';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
	}
	else if (tank.getR() == 5 && map[tank.getX() - 1][tank.getY()] == ' '){
		map[tank.getX() - 1][tank.getY()] = tank.getUp();
		CPos(tank.getX() - 1, tank.getY());
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank.getColor());
		cout << '*';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
	}
	else if (tank.getR() == 2 && map[tank.getX() + 1][tank.getY()] == ' '){
		map[tank.getX() + 1][tank.getY()] = tank.getDown();
		CPos(tank.getX() + 1, tank.getY());
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank.getColor());
		cout << '*';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
	}
	else{
		if (tank.getR() == 3 && map[tank.getX()][tank.getY() + 1] == '9'){
			map[tank.getX()][tank.getY() + 1] = '8';
		}
		else if (tank.getR() == 1 && map[tank.getX()][tank.getY() - 1] == '9'){
			map[tank.getX()][tank.getY() - 1] = '8';
		}
		else if (tank.getR() == 5 && map[tank.getX() - 1][tank.getY()] == '9'){
			map[tank.getX() - 1][tank.getY()] = '8';
		}
		else if (tank.getR() == 2 && map[tank.getX() + 1][tank.getY()] == '9'){
			map[tank.getX() + 1][tank.getY()] = '8';
		}
		else if (tank.getR() == 3 && map[tank.getX()][tank.getY() + 1] == '8'){
			map[tank.getX()][tank.getY() + 1] = '7';
		}
		else if (tank.getR() == 1 && map[tank.getX()][tank.getY() - 1] == '8'){
			map[tank.getX()][tank.getY() - 1] = '7';
		}
		else if (tank.getR() == 5 && map[tank.getX() - 1][tank.getY()] == '8'){
			map[tank.getX() - 1][tank.getY()] = '7';
		}
		else if (tank.getR() == 2 && map[tank.getX() + 1][tank.getY()] == '8'){
			map[tank.getX() + 1][tank.getY()] = '7';
		}
		else if (tank.getR() == 3 && map[tank.getX()][tank.getY() + 1] == '7'){
			map[tank.getX()][tank.getY() + 1] = ' ';
		}
		else if (tank.getR() == 1 && map[tank.getX()][tank.getY() - 1] == '7'){
			map[tank.getX()][tank.getY() - 1] = ' ';
		}
		else if (tank.getR() == 5 && map[tank.getX() - 1][tank.getY()] == '7'){
			map[tank.getX() - 1][tank.getY()] = ' ';
		}
		else if (tank.getR() == 2 && map[tank.getX() + 1][tank.getY()] == '7'){
			map[tank.getX() + 1][tank.getY()] = ' ';
		}
		else if (tank.getR() == 3 && map[tank.getX()][tank.getY() + 1] == 'G'){
			tank2.downHealth();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
			CPos(11, 72);
			cout << tank2.getH();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
			if (tank2.getH() == 0){
				GameOver(1);//parametre kazanan kullanýcýnýn inti
			}
		}
		else if (tank.getR() == 1 && map[tank.getX()][tank.getY() - 1] == 'G'){
			tank2.downHealth();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
			CPos(11, 72);
			cout << tank2.getH();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
			if (tank2.getH() == 0){
				GameOver(1);//parametre kazanan kullanýcýnýn inti
			}
		}
		else if (tank.getR() == 5 && map[tank.getX() - 1][tank.getY()] == 'G'){
			tank2.downHealth();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
			CPos(11, 72);
			cout << tank2.getH();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
			if (tank2.getH() == 0){
				GameOver(1);//parametre kazanan kullanýcýnýn inti
			}
		}
		else if (tank.getR() == 2 && map[tank.getX() + 1][tank.getY()] == 'G'){
			tank2.downHealth();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank2.getColor());
			CPos(11, 72);
			cout << tank2.getH();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
			if (tank2.getH() == 0){
				GameOver(1);//parametre kazanan kullanýcýnýn inti
			}
		}
		else if (tank.getR() == 3 && map[tank.getX()][tank.getY() + 1] == 'H'){
			tank1.downHealth();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
			CPos(3, 72);
			cout << tank1.getH();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
			if (tank1.getH() == 0){
				GameOver(2);
			}
		}
		else if (tank.getR() == 1 && map[tank.getX()][tank.getY() - 1] == 'H'){
			tank1.downHealth();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
			CPos(3, 72);
			cout << tank1.getH();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
			if (tank1.getH() == 0){
				GameOver(2);
			}
		}
		else if (tank.getR() == 5 && map[tank.getX() - 1][tank.getY()] == 'H'){
			tank1.downHealth();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
			CPos(3, 72);
			cout << tank1.getH();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
			if (tank1.getH() == 0){
				GameOver(2);
			}
		}
		else if (tank.getR() == 2 && map[tank.getX() + 1][tank.getY()] == 'H'){
			tank1.downHealth();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tank1.getColor());
			CPos(3, 72);
			cout << tank1.getH();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
			if (tank1.getH() == 0){
				GameOver(2);
			}
		}
		else if (tank.getR() == 3 && map[tank.getX()][tank.getY() + 1] == 'E'){
			for (int i = 0; i < 20; i++){
				if (tank.getX() == enemyT[i].getX() || tank.getY() + 1 == enemyT[i].getY()){
					enemyT[i].Destroy();
				}
			}
		}
		else if (tank.getR() == 1 && map[tank.getX()][tank.getY() - 1] == 'E'){
			for (int i = 0; i < 20; i++){
				if (tank.getX() == enemyT[i].getX() || tank.getY() - 1 == enemyT[i].getY()){
					enemyT[i].Destroy();
				}
			}
		}
		else if (tank.getR() == 5 && map[tank.getX() - 1][tank.getY()] == 'E'){
			for (int i = 0; i < 20; i++){
				if (tank.getX() - 1 == enemyT[i].getX() || tank.getY() == enemyT[i].getY()){
					enemyT[i].Destroy();
				}
			}
		}
		else if (tank.getR() == 2 && map[tank.getX() + 1][tank.getY()] == 'E'){
			for (int i = 0; i < 20; i++){
				if (tank.getX() + 1 == enemyT[i].getX() || tank.getY() == enemyT[i].getY()){
					enemyT[i].Destroy();
				}
			}
		}
		else if (tank.getR() == 3 && map[tank.getX()][tank.getY() + 1] == 'K'){
			GameOver(3);
		}
		else if (tank.getR() == 1 && map[tank.getX()][tank.getY() - 1] == 'K'){
			GameOver(3);
		}
		else if (tank.getR() == 5 && map[tank.getX() - 1][tank.getY()] == 'K'){
			GameOver(3);
		}
		else if (tank.getR() == 2 && map[tank.getX() + 1][tank.getY()] == 'K'){
			GameOver(3);
		}

	}
	return;
}
void EnemyMove(){
	int x;
	
	for (int i = 0; i < 21; i++){
		x = rand() % 6;
		if (enemyT[i].getL() == true){
			if (x == 0){
				goRight(enemyT[i]);
			}
			else if (x == 1){
				goDown(enemyT[i]);
			}
			else if (x == 2){
				goUp(enemyT[i]);
			}
			else if (x == 3){
				goLeft(enemyT[i]);
			}
			else{
				Fire(enemyT[i]);
			}
		}
	}
	return;
}
void CreateEnemy(){
	srand(time(NULL));
	int x;
	int y;
	x = 1 + rand() % 16;
	y = 1 + rand() % 55;
	if (enemyS < 20){
		
		while (map[x][y] != ' '){
			x = 1 + rand() % 16;
			y = 1 + rand() % 55;
		}

		enemyT[enemyS].setL(true);
		enemyT[enemyS].setX(x);
		enemyT[enemyS].setY(y);
		map[x][y] = 'E';

		enemyS++;
	}
	else{

	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), enemyT[0].getColor());
	CPos(17, 79);
	cout << "  ";
	CPos(17, 79);
	cout << 20-enemyS;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);

}
// imlec goster/gizle (true,false)
void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
int Start(){
	int x = 0;
	ShowConsoleCursor(false);
	CPos(1, 4);
	system("color 4");
	cout << 'H';
	Sleep(200);
	system("color C");
	cout << 'O';
	Sleep(200);
	system("color 6");
	cout << 'S';
	Sleep(200);
	system("color 2");
	cout << 'G';
	system("color A");
	Sleep(200);
	system("color B");
	cout << 'E';
	Sleep(200);
	system("color 3");
	cout << 'L';
	Sleep(200);
	system("color 1");
	cout << 'D';
	Sleep(200);
	system("color 5");
	cout << 'I';
	Sleep(200);
	system("color 4");
	cout << 'N';
	Sleep(200);
	system("color C");
	cout << 'I';
	Sleep(200);
	system("color 2");
	cout << 'Z';
	Sleep(300);
	while (x!=3){
		CPos(1, 4);
		cout << "           ";
		Sleep(300);
		CPos(1, 4);
		cout << "HOSGELDINIZ";
		Sleep(300);
		x++;
	}
	system("color 3");
	system("cls");
	for (int i = 0; i < 33; i++){
		CPos(0, i);
		cout << (char)219;
		CPos(6, i);
		cout << (char)219;
	}
	for (int i = 0; i < 7; i++){
		CPos(i, 33);
		cout << (char)219;
		CPos(i, 0);
		cout << (char)219;
	}
	CPos(1, 5);
	cout << "1-Yeni Tek Kisilik Oyun";
	CPos(2, 5);
	cout << "2-Yeni Cift Kisilik Oyun";
	CPos(3, 5);
	cout << "3-Hakkinda";
	CPos(5, 5);
	cout << "Esc-Cikis";
	char cSw = 'p';
	while (cSw != 27){
		cSw = 'p';
		if (_kbhit()){
			cSw = _getch();
		}
		switch (cSw){
		case '1':
			cSw = 27;
			break;
		case '2':
			MultiPlayer = true;
			cSw = 27;
			return 1;
			break;
		case '3':
			
			for (int i = 33; i < 61; i++){
				CPos(0, i);
				cout << (char)219;
				CPos(6, i);
				cout << (char)219;
			}
			for (int i = 0; i < 7; i++){
				CPos(i, 33);
				cout << (char)219;
				CPos(i, 61);
				cout << (char)219;
			}
			CPos(1, 35);
			cout << "Tank Oyunu v1.0";
			CPos(2, 35);
			cout << "Iyi Eglenceler.";
			CPos(3, 35);
			cout << "Tum Haklari Saklidir 2016";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 48);
			CPos(5, 50);
			cout << "Kadir YANIK";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sysColor);
			break;
		case 27:
			exit(0);
			break;

		default:
			break;

		}
		
	}


	return 0;
}
void GameOver(int i){
	system("color C");
	if (i == 1){
		CPos(20, 20);
		cout << "1. Oyuncu Kazandi";
	}
	else if (i == 2){
		CPos(20, 20);
		cout << "2. Oyuncu Kazandi";
	}
	else{
		CPos(20, 20);
		cout << "Bilgisayar Kazandi";
	}
	CPos(21, 22);
	cout << '*';
	Sleep(200);
	CPos(21, 23);
	cout << '*';
	Sleep(200);
	CPos(21, 24);
	cout << 'G';
	Sleep(200);
	CPos(21, 25);
	cout << 'A';
	Sleep(200);
	CPos(21, 26);
	cout << 'M';
	Sleep(200);
	CPos(21, 27);
	cout << 'E';
	Sleep(200);
	CPos(21, 28);
	cout << ' ';
	Sleep(200);
	CPos(21, 29);
	cout << 'O';
	Sleep(200);
	CPos(21, 30);
	cout << 'V';
	Sleep(200);
	CPos(21, 31);
	cout << 'E';
	Sleep(200);
	CPos(21, 32);
	cout << 'R';
	Sleep(200);
	CPos(21, 33);
	cout << '*';
	Sleep(200);
	CPos(21, 34);
	cout << '*';
	Sleep(200);



	CPos(22, 0);

	system("pause");
	exit(0);
	return;
}
