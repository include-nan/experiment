#include <windows.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
typedef struct {
    int weight;
    int parent, lchild, rchild;
} HTNode, *HuffmanTree;
typedef struct {
    char* data;
    int* num;
    int length;
} TNode;
typedef struct {
    char* data;
    char** HM;
} Code;
typedef char** HuffmanCode;
bool IfEncode() {
    int choose;
    cout << "1.加密压缩     2.无密压缩" << endl;
    cout << "请输入选择：";
    cin >> choose;
    if (choose == 1)
        return true;
    else
        return false;
}
void Encode(vector<char>& v) {
    char ch[30];
    v.push_back('@');
    cout << "请输入压缩密码" << endl;
    cin >> ch;
    for (int i = 0; ch[i] != '\0'; i++) v.push_back(ch[i]);
    v.push_back('\0');
    v.push_back('@');
    cout << "开始压缩！" << endl;
}
void ReadTxt(vector<char>& v) {
    char ch;
    ifstream infile("test.txt", ios::in);
    if (!infile) {
        cerr << "open error" << endl;
        exit(1);
    }
    if (IfEncode()) Encode(v);
    while (infile.peek() != EOF) {
        infile.get(ch);
        v.push_back(ch);
    }
    infile.close();
}
void InitList(TNode& T) {
    T.data = new char[256];
    T.num = new int[256];
    if (!T.data || !T.num) exit(1);
    T.length = 0;
}
bool Find(TNode T, char ch) {
    int i;
    for (i = 0; i < T.length; i++)
        if (ch == T.data[i]) return true;
    return false;
}
void TCount(vector<char> v1, TNode& T) {
    int i, j = 0;
    char ch;
    int m = v1.size();
    for (i = 0; i < m; i++) {
        ch = v1[i];
        if (!Find(T, ch)) {
            T.data[j] = ch;
            T.num[j] = count(v1.begin(), v1.end(), ch);
            j++;
            T.length++;
        }
    }
}
void Select(HuffmanTree& HT, int m, int& s1, int& s2) {
    int k, j, n, min = 32767;
    for (k = 1; k <= m; k++) {
        if (HT[k].parent == 0)
            if (HT[k].weight <= min) {
                j = k;
                min = HT[k].weight;
            }
    }
    s1 = j;
    HT[j].parent = 1;
    min = 32767;
    for (k = 1; k <= m; k++) {
        if (HT[k].parent == 0)
            if (HT[k].weight <= min) {
                n = k;
                min = HT[k].weight;
            }
    }
    s2 = n;
}
void CreateHuffmanTree(HuffmanTree& HT, TNode T, int length) {
    int m, i, s1, s2;
    if (length <= 1) return;
    m = 2 * length - 1;
    HT = new HTNode[m + 1];
    for (i = 1; i <= m; ++i) {
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
    }
    for (i = 1; i <= length; ++i) HT[i].weight = T.num[i - 1];
    for (i = length + 1; i <= m; i++) {
        Select(HT, i - 1, s1, s2);
        HT[s1].parent = i;
        HT[s2].parent = i;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }
}
void CreatHuffmanCode(HuffmanTree HT, HuffmanCode& HC, int n) {
    int i, f, c, start;
    HC = new char*[n + 1];
    char* cd = new char[n];
    cd[n - 1] = '\0';
    for (i = 1; i <= n; i++) {
        start = n - 1;
        c = i;
        f = HT[i].parent;
        while (f != 0) {
            --start;
            if (HT[f].lchild == c)
                cd[start] = '0';
            else
                cd[start] = '1';
            c = f;
            f = HT[f].parent;
        }
        HC[i] = new char[n - start];
        strcpy(HC[i], &cd[start]);
    }
    delete cd;
}

void Zip(HuffmanCode HC, vector<char> v, TNode T) {
    int i = 0, j = 0, k = 0;
    ofstream outfile("zip.txt", ios::out);
    if (!outfile) {
        cerr << "open error" << endl;
        exit(1);
    }
    for (i = 0; i < v.size(); i++) {
        for (j = 0; j < T.length; j++)
            if (T.data[j] == v[i]) break;
        for (k = 0; HC[j + 1][k] != '\0'; k++) outfile << HC[j + 1][k];
    }
    outfile.close();
    cout << "正在压缩..." << endl;
    cout << "压缩成功！可到zip.txt中查看压缩后文件" << endl;
}
bool REncode(char ch[]) {
    char code[30];
    cout << "请输入解压密码:" << endl;
    cin >> code;
    if (strcmp(code, ch) == 0)
        return true;
    else
        return false;
}
int in_HM(Code& c) {
    c.data = new char[256];
    c.HM = new char*[256];
    int length = 0, i;
    ifstream infile("code.dat", ios::in);
    if (!infile) {
        cerr << "open error" << endl;
        exit(1);
    }
    infile >> length;
    for (i = 0; i < length; i++) {
        infile >> c.data[i];
        c.HM[i] = new char[length + 1];
        infile >> c.HM[i];
    }
    infile.close();
    return length;
}
void out_HM(HuffmanCode HC, TNode T) {
    int i;
    ofstream outfile("code.dat", ios::out);
    if (!outfile) {
        cerr << "open error" << endl;
        exit(1);
    }
    outfile << T.length << endl;
    for (i = 0; i < T.length; i++) {
        outfile << T.data[i] << endl;
        outfile << HC[i + 1] << endl;
    }
    outfile.close();
}
void RZip(HuffmanCode HC, TNode T) {
    char ch;
    char ch2[30];
    int i, j, flag, flag2 = 0, m = 0;
    ofstream outfile("rzip.txt", ios::out);
    ifstream infile("zip.txt", ios::in);
    if (!outfile) {
        cerr << "open error" << endl;
        exit(1);
    }
    if (!infile) {
        cerr << "open error" << endl;
        exit(1);
    }
    while (infile.peek() != EOF) {
        flag = 0;
        char* cd = new char[T.length];
        for (i = 0;; i++) {
            infile >> ch;
            cd[i] = ch;
            cd[i + 1] = '\0';
            for (j = 1; j <= T.length; j++)
                if (strcmp(HC[j], cd) == 0) {
                    if (T.data[j - 1] == '@') {
                        if (flag2 == 0) {
                            flag = 1;
                            flag2 = 1;
                            delete cd;
                            break;
                        }
                        if (flag2 == 1) {
                            while (1) {
                                if (REncode(ch2)) {
                                    cout << "密码正确，正在解压！ " << endl;
                                    flag2 = 0;
                                    break;
                                } else
                                    cout << "密码错误，请重新输入" << endl;
                            }
                            flag = 1;
                            delete cd;
                            break;
                        }
                    }
                    if (flag2 == 1) {
                        ch2[m] = T.data[j - 1];
                        flag = 1;
                        m++;
                        delete cd;
                        break;
                    }
                    if (flag2 == 0) {
                        outfile << T.data[j - 1];
                        flag = 1;
                        delete cd;
                        break;
                    }
                }
            if (flag == 1) break;
        }
    }
    cout << "正在解压..." << endl;
    cout << "解压成功!请到rzip.txt中查看解压后文件" << endl;
}
void about() {
    cout << "\t默认压缩程序所在目录下的test.txt，生成压缩后的zip.txt" << endl
         << endl;
    cout << "\t默认解压序所在目录下的zip.txt，生成解压后的rzip.txt" << endl;
    system("pause");
}
void menu() {
    cout << "\t1. 压缩文件" << endl;
    cout << "\t2. 解压文件" << endl;
    cout << "\t3. 退出程序" << endl << endl;
}
int main() {
    vector<char> v;
    int n, choose;
    TNode T;
    InitList(T);
    HuffmanTree HT;
    HuffmanCode HC;
    about();
    while (1) {
        system("cls");
        menu();
        cout << "请输入所选择的序号:";
        cin >> choose;
        switch (choose) {
            case 1:
                ReadTxt(v);
                TCount(v, T);
                n = T.length;
                CreateHuffmanTree(HT, T, n);
                CreatHuffmanCode(HT, HC, n);
                Zip(HC, v, T);
                out_HM(HC, T);
                system("pause");
                break;
            case 2:
                RZip(HC, T);
                system("pause");
                break;
            case 3:
                return 0;
            default:
                cout << "输入错误！请重新输入";
                system("pause");
                break;
        }
    }
    return 0;
}
