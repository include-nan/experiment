#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <malloc.h>
#include "cv.h"
#include "highgui.h"

using namespace std;

//输入大于等于min且小于等于max的整数给choice
void InputInt(int& choice, int min, int max)
{
    cin >> choice;

    while (1)
    {
        while (cin.fail())
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "输入错误，请输入大于等于" << min << "且小于等于" << max << "的整数！" << endl;
            cin >> choice;
        }

        while (choice < min || choice > max)
        {
            cout << "输入错误，请输入大于等于" << min << "且小于等于" << max << "的整数！" << endl;
            cin >> choice;
        }

        if (!cin.fail() && choice >= min && choice <= max)
            break;
    }
}

//构造结构体：图
struct Graph
{
    int vertexNumber;//顶点数目
    int arcNumber;//弧数目
    string* vertex;//顶点数组
    int** arc;//弧数组（矩阵）
    Graph(int vertexNumber, int arcNumber, string* vertex, int** arc);//构造函数（创建图）
    void ShortestPath(int vi, int vj);//求vi和vj之间的最短路径
    ~Graph();//析构函数（销毁图）
};

//构造函数（创建图）
Graph::Graph(int vertexNumber, int arcNumber, string* vertex, int** arc):vertexNumber(vertexNumber),arcNumber(arcNumber)
{
    this->vertex = new string[vertexNumber];//顶点数组
    if (!this->vertex)
    {
        exit(1);
    }
    this->arc = (int**)malloc(sizeof(int*) * vertexNumber);//弧数组（矩阵）
    if (!this->arc)
    {
        exit(1);
    }
    for (int i = 0; i < vertexNumber; i++)
    {
        this->arc[i] = (int*)malloc(sizeof(int) * vertexNumber);
        if (!this->arc[i])
        {
            exit(1);
        }
    }
    for (int i = 0; i < vertexNumber; i++)//初始化
    {
        this->vertex[i] = vertex[i];
        for (int j = 0; j < vertexNumber; j++)
        {
            this->arc[i][j] = *((int*)arc + vertexNumber * i + j);
        }
    }
}

//析构函数（销毁图）
Graph::~Graph()
{
    for (int i = 0; i < vertexNumber; i++)
    {
        free(arc[i]);
        arc[i] = NULL;
    }
    free(arc);
    arc = NULL;
    delete[] vertex;
    vertex = NULL;
}

//求vi和vj之间的最短路径
void Graph::ShortestPath(int vi, int vj)
{
    if (vi < 0 || vi >= vertexNumber || vj < 0 || vj >= vertexNumber)
    {
        exit(1);
    }
    int** length = (int**)malloc(sizeof(int*) * vertexNumber);//路径长度
    string** path = new string*[vertexNumber];//路径
    for (int i = 0; i < vertexNumber; i++)
    {
        length[i] = (int*)malloc(sizeof(int) * vertexNumber);
        path[i] = new string[vertexNumber];
    }
    for (int i = 0; i < vertexNumber; i++)//初始化
    {
        for (int j = 0; j < vertexNumber; j++)
        {
            length[i][j] = arc[i][j];
            if (length[i][j] < 0x3f3f3f3f)
            {
                path[i][j] = vertex[i] + " -> " + vertex[j];
            }
            else
            {
                path[i][j] = "";
            }
        }
    }
    for (int u = 0; u < vertexNumber; u++)
    {
        for (int i = 0; i < vertexNumber; i++)
        {
            for (int j = 0; j < vertexNumber; j++)
            {
                if (length[i][u] + length[u][j] < length[i][j])
                {
                    length[i][j] = length[i][u] + length[u][j];
                    path[i][j] = path[i][u] + "," + path[u][j];
                }
            }
        }
    }
    cout << "从" << vertex[vi] << "到" << vertex[vj] << ":" << endl;
    cout << "路径:" << path[vi][vj] << endl;
    cout << "路径长度:" << length[vi][vj] << "百米" <<endl;
    for (int i = 0; i < vertexNumber; i++)//释放length和path
    {
        free(length[i]);
        length[i] = NULL;
        delete[] path[i];
        path[i] = NULL;
    }
    free(length);
    length = NULL;
    delete[] path;
    path = NULL;
}

int main()
{
    string place[6] = { "图书馆", "教学楼", "操场", "宿舍楼", "食堂", "校医院" };
    int path[6][6] =
    {
        {0,6,3,0x3f3f3f3f,0x3f3f3f3f,0x3f3f3f3f},
        {6,0,2,5,0x3f3f3f3f,0x3f3f3f3f},
        {3,2,0,3,4,0x3f3f3f3f},
        {0x3f3f3f3f,5,3,0,2,3},
        {0x3f3f3f3f,0x3f3f3f3f,4,2,0,5},
        {0x3f3f3f3f,0x3f3f3f3f,0x3f3f3f3f,3,5,0}
    };
    Graph graph(6, 9, place, (int**)path);
    IplImage* img = cvLoadImage("Map.png", CV_LOAD_IMAGE_GRAYSCALE);
    cvNamedWindow("Map.png", CV_WINDOW_AUTOSIZE);//创建窗口
    cvShowImage("Map.png", img);//显示图像
    cvWaitKey(0);//等待按键
    int vi = 0;
    int vj = 0;
    while (1)
    {
        cout << "请根据图示输入出发地和目的地的序号(若输入“0 0”，则退出)：" << endl;
        InputInt(vi, 0, graph.vertexNumber);
        InputInt(vj, 0, graph.vertexNumber);
        if (vi == -1 || vj == -1)
        {
            break;
        }
        graph.ShortestPath(vi - 1, vj - 1);
        cout << endl;
    }
    cout << "请单击地图，然后按Enter键，退出程序" << endl;
    cvWaitKey(0);//等待按键
    cvDestroyWindow("Map.png");//销毁窗口
    cvReleaseImage(&img);//释放图像
}
