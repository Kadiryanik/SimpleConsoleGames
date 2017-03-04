#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <fstream>
#include <windows.h>

#define SIZE_X 20
#define SIZE_Y 50

using namespace std;

fstream Dosya;


int area[SIZE_X][SIZE_Y] = { 0 };
char r = 'd';
int highest = 0;
int level = 2;

void CPos(int i, int j);
void end();
void goRight();
void goLeft();
void goUp();
void goDown();
void feed();
void generateFood();
void mainMenu();
void Start();
void snakeReset();

void CPos(int i, int j)
{
	COORD cPos;
	cPos.X = j;
	cPos.Y = i;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos);
}
class node{
public:
	node* prev;
	node* next;
	int node_i;
	int node_j;
	node();
	~node();
};

node::node(){
	next = NULL;
	prev = NULL;
	node_i = 0;
	node_j = 0;
}

node::~node(){
	area[node_i][node_j] = 0;
}

class snake{
public:
	node* head;
	node* queue;
	node* food;
	int lenght;
	snake();
	int rotation;
}snakeObj;

snake::snake(){
	// 4 birim

	lenght = 4;
	rotation = 3; // 1 sol 2 aþaðý 3 sað 5 yukarý

	food = new node;
	food->node_i = 5;
	food->node_j = 5;
	area[5][5] = 2;
	
	head = new node;
	queue = new node;
	node* a = new node;
	node* b = new node;
	a->node_i = 0;
	b->node_i = 0;
	a->node_j = 1;
	b->node_j = 2;

	snakeObj.head->next = NULL;
	snakeObj.head->node_i = 0;
	snakeObj.head->node_j = 3;
	snakeObj.head->prev = b;

	a->next = b;
	a->prev = snakeObj.queue;
	b->prev = a;
	b->next = snakeObj.head;

	snakeObj.queue->prev = NULL;
	snakeObj.queue->node_i = 0;
	snakeObj.queue->node_j = 0;
	snakeObj.queue->next = a;
	area[0][0] = 1;
	area[0][1] = 1;
	area[0][2] = 1;
	area[0][3] = 1;
}



void areaPrint(){
	//system("cls");
	CPos(0, 0);
	for (int i = 0; i < SIZE_X; i++){
		for (int j = 0; j < SIZE_Y; j++){
			if (area[i][j] == 1)
			{
				cout << "*";
			}
			else if (area[i][j] == 2){
				cout << "o";
			}
			else {
				cout << " ";
			}
		}
		cout << (char)219 << endl;
	}
	CPos(SIZE_X, 0);
	int i = 0;
	while (i <= SIZE_Y){
		cout << (char)219;
		i++;
	}
	CPos(SIZE_X + 1, 0);
	
	cout << "Best Score in This PC : " << highest << endl;
	
	cout << "Score : " << snakeObj.lenght*10-40 << endl;
	
}
void goUp(){
	if (area[snakeObj.head->node_i - 1][snakeObj.head->node_j] == 1){//yukarda yýlan varsa çarpýcak
		end();
	}
	if (snakeObj.queue->node_i == 0 && snakeObj.queue->node_j == 0 && snakeObj.lenght == 4 && snakeObj.head->node_i == 0 && snakeObj.head->node_j == 3)
	{
		r = 'd';
		return;
	}
	snakeObj.rotation = 5;
	node* willDelete = snakeObj.queue;
	node* temp = snakeObj.queue;
	node* newHead = new node;
	
	if (area[snakeObj.head->node_i - 1][snakeObj.head->node_j] == 2 && snakeObj.head->node_i != 0){
		feed();
		return;
	}
	if (snakeObj.head->node_i == 0 && area[SIZE_X-1][snakeObj.head->node_j] == 2){
		newHead->node_i = SIZE_X-1;
		newHead->node_j = snakeObj.head->node_j;
		newHead->prev = snakeObj.head;
		snakeObj.head->next = newHead;
		snakeObj.head = newHead;

		area[snakeObj.head->node_i][snakeObj.head->node_j] = 1;
		generateFood();
		snakeObj.lenght++;
		return;
	}
	else if (snakeObj.head->node_i == 0){
		newHead->node_i = SIZE_X - 1;
		newHead->node_j = snakeObj.head->node_j;
		newHead->prev = snakeObj.head;
		snakeObj.head->next = newHead;
	}
	else{
		newHead->node_i = snakeObj.head->node_i - 1;
		newHead->node_j = snakeObj.head->node_j;
		newHead->prev = snakeObj.head;
		snakeObj.head->next = newHead;
	}

	area[newHead->node_i][newHead->node_j] = 1;

	while (temp->next != NULL){
		temp = temp->next;
	}

	snakeObj.head = newHead;
	snakeObj.queue = snakeObj.queue->next;
	snakeObj.queue->prev = NULL;
	delete willDelete;
}
void goDown(){
	if (area[snakeObj.head->node_i + 1][snakeObj.head->node_j] == 1){//aþaðýda yýlan varsa çarpýcak
		end();
	}

	if (snakeObj.queue->node_i == 0 && snakeObj.queue->node_j == 0 && snakeObj.lenght == 4 && snakeObj.head->node_i == 0 && snakeObj.head->node_j == 3)
	{
		r = 'd';
		return;
	}

	snakeObj.rotation = 2;
	

	node* willDelete = snakeObj.queue;
	node* temp = snakeObj.queue;
	node* newHead = new node;
	if (area[snakeObj.head->node_i + 1][snakeObj.head->node_j] == 2 && snakeObj.head->node_i != SIZE_X-1){
		feed();
		return;
	}

	if (snakeObj.head->node_i == SIZE_X - 1 && area[0][snakeObj.head->node_j] == 2){
		newHead->node_i = 0;
		newHead->node_j = snakeObj.head->node_j;
		newHead->prev = snakeObj.head;
		snakeObj.head->next = newHead;
		snakeObj.head = newHead;

		area[snakeObj.head->node_i][snakeObj.head->node_j] = 1;
		generateFood();
		snakeObj.lenght++;
		return;
	}
	else if (snakeObj.head->node_i == SIZE_X-1){
		newHead->node_i = 0;
		newHead->node_j = snakeObj.head->node_j;
		newHead->prev = snakeObj.head;
		snakeObj.head->next = newHead;
	}
	else{
		newHead->node_i = snakeObj.head->node_i + 1;
		newHead->node_j = snakeObj.head->node_j;
		newHead->prev = snakeObj.head;
		snakeObj.head->next = newHead;
	}
	

	area[newHead->node_i][newHead->node_j] = 1;

	while (temp->next != NULL){
		temp = temp->next;
	}

	snakeObj.head = newHead;
	snakeObj.queue = snakeObj.queue->next;
	snakeObj.queue->prev = NULL;
	delete willDelete;
}
void goLeft(){
	if (snakeObj.head->node_j == 0)
	{
		if (area[snakeObj.head->node_i][SIZE_Y-1] == 1){
			end();
		}

	}
	else{
		if (area[snakeObj.head->node_i][snakeObj.head->node_j - 1] == 1){
			end();
		}
	}
	if (snakeObj.queue->node_i == 0 && snakeObj.queue->node_j == 0 && snakeObj.lenght == 4 && snakeObj.head->node_i == 0 && snakeObj.head->node_j == 3)
	{
		r = 'd';
		return;
	}
	snakeObj.rotation = 1;
	
	node* willDelete = snakeObj.queue;
	node* temp = snakeObj.queue;
	node* newHead = new node;
	
	if (area[snakeObj.head->node_i][snakeObj.head->node_j - 1] == 2 && snakeObj.head->node_j != 0){
		feed();
		return;
	}
	

	if (snakeObj.head->node_j == 0 && area[snakeObj.head->node_i][SIZE_Y-1] == 2){
		newHead->node_i = snakeObj.head->node_i;
		newHead->node_j = SIZE_Y - 1;
		newHead->prev = snakeObj.head;
		snakeObj.head->next = newHead;
		snakeObj.head = newHead;

		area[snakeObj.head->node_i][snakeObj.head->node_j] = 1;
		generateFood();
		snakeObj.lenght++;
		return;
	}

	else if (snakeObj.head->node_j == 0){
		newHead->node_i = snakeObj.head->node_i;
		newHead->node_j = SIZE_Y-1;
		newHead->prev = snakeObj.head;
		snakeObj.head->next = newHead;
	}
	else{
		newHead->node_i = snakeObj.head->node_i;
		newHead->node_j = snakeObj.head->node_j - 1;
		newHead->prev = snakeObj.head;
		snakeObj.head->next = newHead;
	}

	area[newHead->node_i][newHead->node_j] = 1;

	while (temp->next != NULL){
		temp = temp->next;
	}

	snakeObj.head = newHead;
	snakeObj.queue = snakeObj.queue->next;
	snakeObj.queue->prev = NULL;
	delete willDelete;
}
void goRight(){
	if (snakeObj.head->node_j == SIZE_Y-1)
	{
		if (area[snakeObj.head->node_i][0] == 1){
			end();
		}
	}
	else{
		if (area[snakeObj.head->node_i][snakeObj.head->node_j + 1] == 1){
			end();
		}
	}


	snakeObj.rotation = 3;

	node* willDelete = snakeObj.queue;
	node* temp = snakeObj.queue;
	node* newHead = new node;
	if (area[snakeObj.head->node_i][snakeObj.head->node_j + 1] == 2 && snakeObj.head->node_j != SIZE_Y - 1){
		feed();
		return;
	}
	if (snakeObj.head->node_j == SIZE_Y - 1 && area[snakeObj.head->node_i][0] == 2){ // && node_i ye 0 da yem varsa
		newHead->node_i = snakeObj.head->node_i;
		newHead->node_j = 0;
		newHead->prev = snakeObj.head;
		snakeObj.head->next = newHead;
		snakeObj.head = newHead;

		area[snakeObj.head->node_i][snakeObj.head->node_j] = 1;
		generateFood();
		snakeObj.lenght++;
		return;
	}
	
	else if (snakeObj.head->node_j == SIZE_Y - 1){ // && node_i ye 0 da yem yoksa
		newHead->node_i = snakeObj.head->node_i;
		newHead->node_j = 0;
		newHead->prev = snakeObj.head;
		snakeObj.head->next = newHead;
	}
	else{
		newHead->node_i = snakeObj.head->node_i;
		newHead->node_j = snakeObj.head->node_j + 1;
		newHead->prev = snakeObj.head;
		snakeObj.head->next = newHead;
	}

	area[newHead->node_i][newHead->node_j] = 1;

	while (temp->next != NULL){
		temp = temp->next;
	}

	snakeObj.head = newHead;
	snakeObj.queue = snakeObj.queue->next;
	snakeObj.queue->prev = NULL;
	delete willDelete;
}

void feed(){//feedleme
	node* newHead = new node;

	newHead->prev = snakeObj.head;
	snakeObj.head->next = newHead;

	newHead->node_i = snakeObj.food->node_i;
	newHead->node_j = snakeObj.food->node_j;

	snakeObj.head = newHead;

	area[snakeObj.head->node_i][snakeObj.head->node_j] = 1;

	
	generateFood();
	snakeObj.lenght++;
	return;
}

void generateFood(){
	int x, y;
	srand(time(NULL));
	x = rand() % SIZE_X;
	y = rand() % SIZE_Y;
	while(area[x][y] == 1){
		x = rand() % SIZE_X;
		y = rand() % SIZE_Y;
	}
		
	snakeObj.food->node_i = x;
	snakeObj.food->node_j = y;
	area[x][y] = 2;
}

void end(){
	areaPrint();
	char c = 'o';

	int temp = 0;
	if (Dosya.eof() == 1){
		Dosya.open("score.txt", ios::in | ios::out);
		Dosya >> temp;
		Dosya.close();
	}
	if (temp < snakeObj.lenght * 10 - 40){
		Dosya.open("score.txt", ios::in | ios::out);
		Dosya << snakeObj.lenght * 10 - 40;
		Dosya.close();
	}
	else{

	}
	
	cout << "//***GAME OVER***\\\\" << endl;
	cout << "Cikmak icin q ya, " << endl;
	cout << "   Yeniden oynamak icin" << endl;
	cout << "     Herhangi bir tusaa basin!" << endl;
	c = _getch();
	if (c == 'q'){
		exit(0);
	}
	else {
		snakeReset();
		mainMenu();
	}
}

void mainMenu(){
	system("cls");
	cout << "istediginiz zorluk seviyesini seciniz" << endl;
	cout << "1-Cok Zor" << endl
		<< "2-Zor" << endl
		<< "3-Orta" << endl
		<< "4-Kolay" << endl;
	cout << " : ";
	cin >> level;
	system("cls");

	Dosya.open("C:\score.txt", ios::in | ios::out);
	Dosya >> highest;
	Dosya.close();
}

void Start(){
	cout << "*******************************" << endl;
	cout << "*******************************" << endl;
	cout << "** Yilan Oyununa Hosgeldiniz **" << endl;
	cout << "**    w,a,s,d yonlendirme    **" << endl;
	cout << "**       p duraklatma        **" << endl;
	cout << "**        q cikis            **" << endl;
	cout << "** Duvarlardan gecebilirsiniz**" << endl;
	cout << "** Baslamak icin bir tusa    **" << endl;
	cout << "**                  basiniz! **" << endl;
	cout << "*******************************" << endl;
	cout << "*******************************" << endl;
	char c = _getch();
	if (c == 'q'){
		exit(0);
		system("pause");
	}
	else{
		mainMenu();
	}
}

void snakeReset(){
	for (int i = 0; i < SIZE_X; i++){
		for (int j = 0; j < SIZE_Y; j++){
			area[i][j] = 0;
		}
	}

	snakeObj.lenght = 4;
	snakeObj.rotation = 3; // 1 sol 2 aþaðý 3 sað 5 yukarý

	snakeObj.food = new node;
	snakeObj.food->node_i = 5;
	snakeObj.food->node_j = 5;
	area[5][5] = 2;

	snakeObj.head = new node;
	snakeObj.queue = new node;
	node* a = new node;
	node* b = new node;
	a->node_i = 0;
	b->node_i = 0;
	a->node_j = 1;
	b->node_j = 2;

	snakeObj.head->next = NULL;
	snakeObj.head->node_i = 0;
	snakeObj.head->node_j = 3;
	snakeObj.head->prev = b;

	a->next = b;
	a->prev = snakeObj.queue;
	b->prev = a;
	b->next = snakeObj.head;

	snakeObj.queue->prev = NULL;
	snakeObj.queue->node_i = 0;
	snakeObj.queue->node_j = 0;
	snakeObj.queue->next = a;
	area[0][0] = 1;
	area[0][1] = 1;
	area[0][2] = 1;
	area[0][3] = 1;
}

int main()
{
	Dosya.open("score.txt", ios::in | ios::out|ios::app);
	Dosya.close();

	system("color 2");

	Start();

	

	while (r != 'q'){
		if (_kbhit()){
			r = _getch();
		}
		
		switch (r){
		case 'w':
			if (snakeObj.queue->node_i == 0 && snakeObj.queue->node_j == 0 && snakeObj.lenght == 4 && snakeObj.head->node_i == 0 && snakeObj.head->node_j == 3) goRight();
			else if(snakeObj.rotation != 2)goUp();
			else goDown();
			break;
		case 's':
			if (snakeObj.queue->node_i == 0 && snakeObj.queue->node_j == 0 && snakeObj.lenght == 4 && snakeObj.head->node_i == 0 && snakeObj.head->node_j == 3) goRight();
			else if (snakeObj.rotation != 5)goDown();
			else goUp();
			break;
		case 'a':
			if (snakeObj.queue->node_i == 0 && snakeObj.queue->node_j == 0 && snakeObj.lenght == 4 && snakeObj.head->node_i == 0 && snakeObj.head->node_j == 3) goRight();
			else if (snakeObj.rotation != 3)goLeft();
			else goRight();
			break;
		case 'd':
			if (snakeObj.rotation != 1)goRight();
			else goLeft();
			break;
		case 'p':
			break;
		default:
			if (snakeObj.rotation == 2){
				goDown();
			}
			else if (snakeObj.rotation == 1){
				goLeft();
			}
			else if (snakeObj.rotation == 3){
				goRight();
			}
			else if (snakeObj.rotation == 5){
				goUp();
			}
			break;
		}
		areaPrint();
		std::this_thread::sleep_for(std::chrono::milliseconds(25*level));	// 100 milli 0,1 saniye // 1000 mili 1 saniye // 1000 mikro 1 milli
	}
	cout << "Gule Gule" << endl;

	system("pause");
	return 0;
}

