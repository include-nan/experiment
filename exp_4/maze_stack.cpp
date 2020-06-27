#include <windows.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

//定义结构体：步
typedef struct Element {
    int order;       //次序
    int x;           //行
    int y;           //列
    bool pass;       //是否可走
    bool footPrint;  //是否走过
} Step;

//定义结构体：路线
typedef struct Stack {
    Element* base;  //栈底指针
    Element* top;   //栈顶指针
    int stacksize;  //栈的容量
} Route;

//定义结构体：迷宫
struct Maze {
    Step* head;  //头指针
    int rom;     //行数
    int column;  //列数
};

//构造栈：路线
void InitStack(Route& route, int stacksize) {
    route.base = (Step*)malloc(sizeof(Step) * stacksize);
    if (!route.base) {
        exit(0);
    }
    route.top = route.base;
    route.stacksize = stacksize;
}

//入栈
void Push(Route& route, Step step) {
    if (route.top - route.base == route.stacksize) {
        Step* p =
            (Step*)realloc(route.base, sizeof(Step) * (route.stacksize + 10));
        if (!p) {
            exit(0);
        }
        route.base = p;
        route.top = route.base + route.stacksize;
        route.stacksize = route.stacksize + 10;
    }
    *(route.top) = step;
    route.top++;
}

//出栈
void Pop(Route& route, Step& step) {
    if (route.top == route.base) {
        exit(0);
    }
    route.top--;
    step = *route.top;
}

//打印路线
void PrintRoute(Route route) {
    Step* p = route.base;
    cout << "通路为：" << endl;
    while (p != route.top) {
        cout << "(" << p->x << "," << p->y << ")" << endl;
        p++;
    }
}

//销毁栈
void DestroyStack(Route& route) {
    free(route.base);
    route.base = NULL;
    route.top = NULL;
    route.stacksize = 0;
}

//构造迷宫
void InitMaze(Maze& maze, int rom, int column) {
    maze.rom = rom;
    maze.column = column;
    int count = maze.rom * maze.column;
    maze.head = (Step*)malloc(sizeof(Step) * count);
    if (!maze.head) {
        exit(0);
    }
    Step* p = maze.head;
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
        p->footPrint = false;
        p++;
    }
}

//打印迷宫
void PrintMaze(Maze maze) {
    cout << "迷宫随机生成，最右边一列为行数，最下边一行为列数" << endl;
    Step* p = maze.head;
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
Step* Location(Maze maze, int order) {
    if (order < 1 || order > maze.rom * maze.column) {
        exit(0);
    }
    Step* p = maze.head;
    for (int i = 0; i < order - 1; i++) {
        p++;
    }
    return p;
}

//已知坐标求地址
Step* Location(Maze maze, int x, int y) {
    if (x < 1 || x > maze.rom || y < 1 || y > maze.column) {
        exit(0);
    }
    int order = 0;
    if (y == maze.column) {
        order = x * y;
    } else {
        order = maze.column * (x - 1) + y;
    }
    Step* p = maze.head;
    for (int i = 0; i < order - 1; i++) {
        p++;
    }
    return p;
}

//判断是否是终点
bool End(Maze maze, Step step) {
    if (step.pass == false) {
        return false;
    }
    if (step.x == 1 || step.x == maze.rom || step.y == 1 ||
        step.y == maze.column) {
        return true;
    }
    return false;
}

//判断是否可通
bool Pass(Step step) {
    if (step.pass == true && step.footPrint == false) {
        return true;
    }
    return false;
}

//打通道路
void GetThrough(Maze& maze, int x, int y) {
    Step* p = Location(maze, x, y);
    p->pass = true;
}

//求路线
void MazeRoute(Maze maze, int x0, int y0) {
    if (x0 < 1 || x0 > maze.rom || y0 < 1 || y0 > maze.column) {
        exit(0);
    }
    Step* p = Location(maze, x0, y0);
    if (p->pass == false) {
        exit(0);
    }
    Route route;
    route.base = NULL;
    route.top = NULL;
    route.stacksize = 0;
    InitStack(route, 10);
    Push(route, *p);
    p->footPrint = true;
    while (route.base != route.top) {
        if (End(maze, *(route.top - 1))) {
            PrintRoute(route);
            DestroyStack(route);
            return;
        } else if (Pass(*(p + 1))) {
            Push(route, *(p + 1));
            p++;
            p->footPrint = true;
        } else if (Pass(*(p + maze.column))) {
            Push(route, *(p + maze.column));
            p = p + maze.column;
            p->footPrint = true;
        } else if (Pass(*(p - maze.column))) {
            Push(route, *(p - maze.column));
            p = p - maze.column;
            p->footPrint = true;
        } else if (Pass(*(p - 1))) {
            Push(route, *(p - 1));
            p--;
            p->footPrint = true;
        } else {
            Pop(route, *p);
            p->pass = false;
            p = Location(maze, (route.top - 1)->order);
        }
    }
    cout << "没有通路" << endl;
    DestroyStack(route);
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
    MazeRoute(maze, x, y);
    DestroyMaze(maze);
    return 0;
}
