#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Rand_num(int a[], int n, int T)
{
    srand(T);
    for(int i = 0; i < n; i++)
    {
        a[i] = rand();
    }
}

void BubbleSort(int a[], int n)
{
    int i, j, temp;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - 1 - i; j++)
        {
            if (a[j] > a[j + 1])
            {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

void QuickSort(int a[], int left, int right)
{
    if(left >= right)
        return ;
    int i = left, j = right, key = a[left];
    while(i < j)
    {
        while(i < j && key <= a[j])
            j--;
        a[i] = a[j];
        while(i < j && key >= a[i])
            i++;
        a[j] = a[i];
    }
    a[i] = key;
    QuickSort(a, left, i - 1);
    QuickSort(a, i + 1, right);
}

int main()
{
    int N = 40000;
    int a[40000];
    time_t T;
    T = time(NULL);

    Rand_num(a, N, T);

    int t1;
    int t2;
    int t3;
    int t4;

    t1 = clock();
    BubbleSort(a, N);
    t2 = clock();

    Rand_num(a, N, T);

    t3 = clock();
    QuickSort(a, 0, N - 1);
    t4 = clock();
    printf("BubbleSort is %4d ms.\n", t2 - t1);
    printf(" QuickSort is %4d ms.\n", t4 - t3);

    return 0;
}
