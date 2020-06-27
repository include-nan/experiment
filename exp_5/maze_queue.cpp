#include <malloc.h>
#include <ctime>
#include <iostream>
using namespace std;

//点
typedef struct node {
    int x;
    int y;
    int order;
    bool pass;
    node* next;
    node* pre;
} Node;

//迷宫
struct Maze {
    Node* head;
    int rom;     //行数
    int column;  //列数
};

//队列
typedef struct Queue {
    Node* front;  //队首
    Node* rear;   //队尾
} Queue;

//初始化队列
void InitQueue(Queue& route) {
    route.front = (Node*)malloc(sizeof(Node));
    if (!route.front) exit(-1);
    route.rear = route.front;
    route.front->next = NULL;
    route.front->pre = NULL;
}

//入队
void EnQueue(Queue& route, Node e) {
    Node* p = (Node*)malloc(sizeof(Node));
    if (!p) exit(-1);
    p->x = e.x;
    p->y = e.y;
    p->order = e.order;
    p->pass = e.pass;
    p->next = NULL;
    route.rear->next = p;
    route.rear = p;
}

//出队
void DeQueue(Queue& route) {
    if (route.rear == route.front) exit(-1);
    Node* p = route.front->next;
    route.front->next = p->next;
    if (route.rear == p) {
        route.rear = route.front;
    }
    free(p);
}

//销毁队列
void DestroyQUeue(Queue& route) {
    while (route.front) {
        route.rear = route.front->next;
        free(route.front);
        route.front = NULL;
        route.front = route.rear;
    }
}

void PrintRoute(Queue route) {
    Node* p = route.rear;
    cout << "通路为：" << endl;
    while(route.rear->pre){
        cout<<"("<<route.rear->x<<" "<<route.rear->y<<")"<<endl;
        route.rear = route.rear->pre;
    }
}

//构造迷宫
void InitMaze(Maze& maze, int rom, int column) {
    maze.rom = rom;
    maze.column = column;
    int count = maze.rom * maze.column;
    maze.head = (Node*)malloc(sizeof(Node) * count);
    if (!maze.head) exit(0);
    Node* p = maze.head;
    unsigned seed = time(0);
    srand(seed);
    int number = 0;
    for (int i = 1; i <= count; i++) {
        p->order = i;
        if (i % maze.column == 0) {
            p->x = i / maze.column;
            p->y = maze.column;
        } else {
            p->x = i / maze.column + 1;
            p->y = i % maze.column;
        }
        number = rand() % 2;
        if (number == 0) {
            p->pass = false;
        } else {
            p->pass = true;
        }
        p++;
    }
}

//打印迷宫
void PrintMaze(Maze maze) {
    cout << "迷宫随机生成，最右边一列为行数，最下边一行为列数" << endl;
    Node* p = maze.head;
    for (int i = 1; i <= maze.rom * maze.column; i++) {
        if (p->pass == true) {
            cout << 1 << " ";
        } else {
            cout << 0 << " ";
        }
        if (p->y == maze.column) {
            cout << "  " << p->x << endl;
        }
        p++;
    }
    cout << endl;
    for (int i = 1; i <= maze.column; i++) {
        cout << i << " ";
    }
    cout << endl << endl;
}

//销毁迷宫
void DestroyMaze(Maze& maze) {
    free(maze.head);
    maze.head = NULL;
    maze.rom = 0;
    maze.column = 0;
}

//已知次序求地址
Node* Location(Maze maze, int order) {
    if (order < 1 || order > maze.rom * maze.column) {
        exit(0);
    }
    Node* p = maze.head;
    for (int i = 0; i < order - 1; i++) {
        p++;
    }
    return p;
}

//已知坐标求地址
Node* Location(Maze maze, int x, int y) {
    if (x < 1 || x > maze.rom || y < 1 || y > maze.column) {
        exit(0);
    }
    int order = 0;
    if (y == maze.column) {
        order = x * y;
    } else {
        order = maze.column * (x - 1) + y;
    }
    Node* p = maze.head;
    for (int i = 0; i < order - 1; i++) {
        p++;
    }
    return p;
}

//判断是否是终点
bool End(Maze maze, Node node) {
    if (node.pass == false) {
        return false;
    }
    if (node.x == 1 || node.x == maze.rom || node.y == 1 ||
        node.y == maze.column) {
        return true;
    }
    return false;
}

//判断是否可通
bool Pass(Node p){
    if(p.pass && p.pre == NULL){
        return true;
    }
    return false;
}

//判断队列是否为空
bool EmptyQueue(Queue route) {
    if (route.front == route.rear) {
        return true;
    }
    return false;
}

//打通道路
void GetThrough(Maze& maze, int x, int y) {
    Node* p = Location(maze, x, y);
    p->pass = true;
}

//求路径
void bfsSearch(Maze maze, int x, int y) {
    if (x < 1 || x > maze.rom || y < 1 || y > maze.column) {
        exit(0);
    }
    Node* p = Location(maze, x, y);
    if (!p->pass) {
        exit(0);
    }
    Queue route;
    InitQueue(route);
    EnQueue(route, *p);
    while (!EmptyQueue(route)) {
        if(End(maze,*(route.rear))){
            PrintRoute(route);
            DestroyQUeue(route);
            return;
        }
        if(Pass(*(p+1))){   //右边
            EnQueue(route,*(p+1));
        }
        if(Pass(*(p+maze.column))){  //下边
            EnQueue(route,*(p+maze.column));
        }
        if(Pass(*(p-maze.column))){  //上边
            EnQueue(route,*(p-maze.column));
        }
        if(Pass(*(p-1))){  //左边
            EnQueue(route,*(p-1));
        }
    }
}

int main() {
    Maze maze;
    maze.head = NULL;
    maze.rom = 0;
    maze.column = 0;
    cout << "请输入迷宫的行数和列数" << endl;
    int rom = 0;
    int column = 0;
    cin >> rom >> column;
    InitMaze(maze, rom, column);
    cout << "请输入起点的行数和列数" << endl;
    int x = 0;
    int y = 0;
    cin >> x >> y;
    while (x < 1 || x > rom || y < 1 || y > column) {
        cout << "错误，请重新输入" << endl;
        cin >> x >> y;
    }
    GetThrough(maze, x, y);
    cout << endl;
    PrintMaze(maze);
    bfsSearch(maze, x, y);
    DestroyMaze(maze);
    return 0;
}
