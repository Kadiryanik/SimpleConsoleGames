#include <iostream>
#include <conio.h>
#include <locale.h> // Başlık dosyamız
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#define SIZE_X 20
#define SIZE_Y 60

using namespace std;

char area[SIZE_X][SIZE_Y] = {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
							 "X                                                         X",
							 "X                                                         X",
							 "X                                                         X",
							 "X                                                         X",
							 "X                                                         X",
							 "X                                                         X",
							 "X                                                         X",
							 "X                                                         X",
							 "X             T                                           X",
							 "X                                                         X",
							 "X                                                         X",
							 "X                                                         X",
							 "X                                                         X",
							 "X                                                         X",
							 "X                                                         X",
							 "X                                                         X",
							 "X                                                         X",
							 "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" };

void CPos(int, int);
void DrawMap();

void areaPrint();

class node{
public:
	node* next;
	node* prev;
	node* down;
	node* up;
	int node_i;
	int node_j;
	node();
	~node();
};

node::node(){
	next = NULL;
	prev = NULL;
	down = NULL;
	up = NULL;
	node_i = 0;
	node_j = 0;
}
node::~node(){

}

class tank{
public:
	node* head;
	node* a;
	node* b;
	node* c;
	node* e;
	node* f;
	node* g;
	tank();
}tank;

tank::tank(){
	head = new node;
	a = new node;
	b = new node;
	c = new node;
	e = new node;
	f = new node;
	g = new node;

	head->prev = b;
	head->node_i = 2;
	head->node_j = 3;

	b->next = head;
	b->up = a;
	b->down = c;
	b->prev = f;
	b->node_i = 2;
	b->node_j = 2;

	a->prev = e;
	a->down = b;
	a->node_i = 1;
	a->node_j = 2;

	c->prev = g;
	c->up = b;
	c->node_i = 3;
	c->node_j = 2;
	
	e->next = a;
	e->down = f;
	e->node_i = 1;
	e->node_j = 1;

	f->next = b;
	f->up = e;
	f->down = g;
	f->node_i = 2;
	f->node_j = 1;

	g->next = c;
	g->up = f;
	g->node_i = 3;
	g->node_j = 1;

	area[1][1] = 1;
	area[1][2] = 1;
	area[2][1] = 1;
	area[2][2] = 1;
	area[2][3] = 1;
	area[3][1] = 1;
	area[3][2] = 1;
}

void goRight(){

	area[tank.e->node_i][tank.e->node_j] = 0;
	area[tank.f->node_i][tank.e->node_j] = 0;
	area[tank.g->node_i][tank.g->node_j] = 0;

	tank.head->node_j += 1;
	tank.a->node_j += 1;
	tank.b->node_j += 1;
	tank.c->node_j += 1;
	tank.e->node_j += 1;
	tank.f->node_j += 1;
	tank.g->node_j += 1;
	area[tank.head->node_i][tank.head->node_j] = 1;
	area[tank.a->node_i][tank.a->node_j] = 1;
	area[tank.b->node_i][tank.b->node_j] = 1;
	area[tank.c->node_i][tank.c->node_j] = 1;
	area[tank.e->node_i][tank.e->node_j] = 1;
	area[tank.f->node_i][tank.e->node_j] = 1;
	area[tank.g->node_i][tank.g->node_j] = 1;
}

void areaPrint(){
	system("cls");
	for (int i = 0; i < SIZE_X; i++){
		for (int j = 0; j < SIZE_Y; j++){
			if (area[i][j] == 'X'){
				CPos(i, j);
				cout << (char)219;
			}
			if (area[i][j] == 'T')
			{
				cout << "o";
			}
			/*else if (area[i][j] == 2){
				cout << "*";
			}
			else {
				cout << " ";
			}*/
		}
		cout << endl;
	}
}

int main()
{
	setlocale(LC_ALL, "Turkish");

	char c = 'd';
	while (true){
		c = 'o';
		if (_kbhit()){
			c = _getch();
		}

		switch (c){
		case 'd':
			goRight();
			break;
		default:
			break;
		}

		areaPrint();
		this_thread::sleep_for(chrono::milliseconds(500));
	}
	
	
	system("pause");
	return 0;
}

