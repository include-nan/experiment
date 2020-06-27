#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

int* Next(string s) {
    int* next = new int[s.length()];
    next[0] = -1;
    int k = -1;
    int j = 0;
    while (j < s.length() - 1) {
        if (k == -1 || s[j] == s[k]) {
            ++k;
            ++j;
            next[j] = k;
        } else {
            k = next[k];
        }
    }
    return next;
}

int KMP(string filelocation,string T) {
    string S;
    ifstream file;
    int i = 0, j = 0, head = 1;
    //打开文件，获得数据
    file.open(filelocation,ios::in);
    if (!file.is_open()) {
        return -1;
    }
    getline(file, S);
    int* next = Next(T);    //根据模式串T,初始化next指针
    while (j < T.length()) {
        if (S[i] == T[j]) {  //对应位置字符相等
            i++;
            j++;
        } else {
            if (next[j + 1] == 0) {
                i++;
                j = 0;
                head = i;
            } else {
                i += j - next[j];
                j = j - next[j] - 1;
                head = i;
            }
        }
    }
    return head;
}

int main() {
    int location;
    string T = "20001210";
    string filelocation = "D:\\OneDrive\\Study\\Data Structures and Algorithms\\experiment\\week_6\\Pi.txt";
    location = KMP(filelocation,T);
    cout  << "20001210's location in Pi.txt is " << location << endl;
    return 0;
}