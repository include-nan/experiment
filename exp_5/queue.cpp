#include <malloc.h>

#include <ctime>
#include <iostream>
using namespace std;

struct node {
    node* pre;
    node* next;
    int x;
    int y;
    int data;
};
struct Maze {
    node* head;
    int rom;  //行
    int col;  //列
};
struct Queue {
    node* front;  //队首
    node* next;
    node* rear;   //队尾
};
//初始化迷宫
void InitMaze(Maze& maze, int rom, int col) {
    maze.rom = rom;
    maze.col = col;
    for (int i = 0; i < rom; i++) {
        for (int j = 0; j < col; j++) {
            maze.head = (node*)malloc(sizeof(node) * rom * col);
            if (!maze.head) exit(-1);
            node* p = maze.head;
            for (int i = 1; i <= rom * col; i++) {
                if (i % maze.col == 0) {
                    p->x = i / maze.col;
                    p->y = maze.col;
                } else {
                    p->x = i / maze.col + 1;
                    p->y = i % maze.col;
                }
                p->pre = NULL;
                int flag = rand() % 2;
                if (flag == 0)
                    p->data = 0;
                else
                    p->data = 1;
                p++;
            }
        }
    }
}
//打印迷宫
void PrintMaze(Maze& maze) {
    cout << "迷宫随机生成，最右边一列为行数，最下边一行为列数" << endl;
    node* p = maze.head;
    for (int i = 1; i <= maze.rom * maze.col; i++) {
        cout << p->data << " ";
        if (p->y == maze.col) {
            cout << "  " << p->x << endl;
        }
        p++;
    }
    cout << endl;
    for (int i = 1; i <= maze.col; i++) {
        cout << i << " ";
    }
    cout << endl << endl;
}
//销毁迷宫
void DestroyMaze(Maze& maze) {
    free(maze.head);
    maze.head = NULL;
}
//根据坐标来求序号
int Location(Maze& maze, int x, int y) {
    if (x < 1 || x > maze.rom || y < 1 || y > maze.col) exit(-1);
    int order;
    if (y == maze.col) {
        order = x * y;
    } else {
        order = maze.col * (x - 1) + y;
    }
    return order;
}
//判断是否为终点
bool End(Maze& maze, node *p) {
    if (p->data == 0) {
        return false;
    }
    if (p->x == 1 || p->x == maze.rom || p->y == 1 || p->y == maze.col) {
        return true;
    }
    return false;
}
//打通道路
void GetThrough(Maze& maze, int x, int y) {
    node* p = maze.head;
    (p + Location(maze, x, y))->data = 1;
}

//初始化队列
void InitQueue(Queue& Q) {
    Q.front = (node*)malloc(sizeof(node));
    if (!Q.front) exit(-1);
    Q.rear = Q.front;
    Q.front->pre = NULL;
    Q.rear->pre = NULL;
}
//入队
void EnQueue(Queue& Q, node* e) {
    node* p = (node*)malloc(sizeof(node));
    if (!p) exit(-1);
    p->x = e->x;
    p->y = e->y;
    p->data = e->data;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = p;
}
//出队
void DeQueue(Queue& Q) {
    if (Q.front == Q.rear) exit(-1);
    node* p = Q.front->next;
    Q.front->next = p->next;
    if (Q.rear == p) {
        Q.rear = Q.front;
    }
    free(p);
}
//销毁队列
void DestroyQueue(Queue& Q) {
    while (Q.front) {
        Q.rear = Q.front->next;
        free(Q.front);
        Q.front = NULL;
        Q.front = Q.rear;
    }
}
//打印队列
void PrintQueue(Queue Q) {
    node* p = Q.rear;
    while (Q.rear->pre) {
        cout << "(" << Q.rear->x << " " << Q.rear->y << ")" << endl;
        Q.rear = Q.rear->pre;
        if(Q.front == Q.rear)   return;
    }
}
//判断队列是否为空
bool EmptyQueue(Queue& Q) {
    if (Q.front == Q.rear) return true;
    return false;
}

//求路径
void bfsSearch(Maze &maze,int x,int y){
    node* p = maze.head+Location(maze,x,y)-1;
    p->pre = NULL;
    p->data = (maze.head+Location(maze,x,y)-1)->data;
    p->x = (maze.head+Location(maze,x,y)-1)->x;
    p->y = (maze.head+Location(maze,x,y)-1)->y;
    p->next = NULL;
    if(!p)  exit(-1);
    Queue route;
    InitQueue(route);
    EnQueue(route,p);
    PrintQueue(route);
    while(!EmptyQueue(route)){
        if(End(maze,route.rear)){
            PrintQueue(route);
            DestroyQueue(route);
            return;
        }
        if((p+1)->data == 1){   //右边
            EnQueue(route,(p+1));
            (p+1)->pre = route.front;
        }
        if((p+maze.col)->data == 1){   //下边
            EnQueue(route,(p+maze.col));
            (p+maze.col)->pre = route.front;
        }
        if((p-maze.col)->data == 1){   //上边
            EnQueue(route,(p-maze.col));
            (p-maze.col)->pre = route.front;
        }
        if((p-1)->data == 1){   //左边
            EnQueue(route,(p-1));
            (p-1)->pre = route.front;
        }
        DeQueue(route);
    }
}

int main(){
    Maze maze;
    maze.head = NULL;
    maze.rom = 0;
    maze.col = 0;
    cout << "请输入迷宫的行数和列数" << endl;
    int rom;
    int col;
    cin >> rom >> col;
    InitMaze(maze, rom, col);
    cout << "请输入起点的行数和列数" << endl;
    int x = 0;
    int y = 0;
    cin >> x >> y;
    while (x < 1 || x > rom || y < 1 || y > col) {
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