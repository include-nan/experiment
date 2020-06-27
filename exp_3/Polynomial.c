#include "malloc.h"
#include "stdio.h"

typedef struct term {
    float coef;    //系数
    int expn;      //指数
} term, ElemType;  // term用于本ADT，ElemType 为 LinkList 的数据对象名

typedef struct Node {
    term ElemType;
    struct Node* next;
} Node, *LinkList;

//建立头节点
LinkList LinkListCreate() {
    Node* head = (Node*)malloc(sizeof(Node));
    if (head == NULL) {
        printf("ERROR!\n");
        return 0;
    }
    head->ElemType.coef = 0;
    head->ElemType.expn = 0;
    head->next = NULL;
    return head;
}

//释放链表
void DestoryPolyn(LinkList obj) {
    if (obj == NULL) return;
    LinkList Transit;
    while (obj->next != NULL) {
        Transit = obj;
        obj = obj->next;
        free(Transit);
    }
    free(obj);
    return;
}

//在链表尾结点之后添加一个结点，作为新的尾结点
void CreatPolyn(LinkList obj, float coef, int expn) {
    Node* p;
    Node* temp;
    temp = obj;
    p = (Node*)malloc(sizeof(Node));
    if (p != NULL) {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        p->ElemType.coef = coef;
        p->ElemType.expn = expn;
        p->next = temp->next;
        temp->next = p;
    }
}

//两个链表相加
LinkList AddPolyn(LinkList objA, LinkList objB) {
    if (objA->next == NULL && objB->next == NULL) {
        DestoryPolyn(objB);
    } else if (objA->next != NULL && objB->next == NULL) {
        DestoryPolyn(objB);
    } else if (objA->next == NULL && objB->next != NULL) {
        objA = objB;
    } else {
        Node* tempA = objA->next;
        Node* tempB = objB->next;
        Node* nowA = objA;
        Node* nowB;
        while (tempA != NULL && tempB != NULL) {
            if (tempA->ElemType.expn < tempB->ElemType.expn) {
                nowA = tempA;
                tempA = tempA->next;
            } else if (tempA->ElemType.expn == tempB->ElemType.expn) {
                tempA->ElemType.coef += tempB->ElemType.coef;
                nowA = tempA;
                tempA = tempA->next;
                nowB = tempB;
                tempB = tempB->next;
                free(nowB);
            } else {
                nowB = tempB->next;
                tempB->next = tempA;
                nowA->next = tempB;
                tempB = nowB;
            }
        }
        if (tempB) {
            nowA->next = tempB;
        }
    }
    return objA;
}

//两个链表相乘
LinkList MultiPlyPolyn(LinkList objA, LinkList objB) {
    if (!objA || !objB) return NULL;
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->ElemType.coef = 0;
    temp->ElemType.expn = 0;
    temp->next = NULL;
    Node *ta = objA, *tb = objB, *ptp = temp;
    // 先用ta的第一项乘tb的所有项
    while (tb) {
        CreatPolyn(ptp, ta->ElemType.coef * tb->ElemType.coef,
                   ta->ElemType.expn + tb->ElemType.expn);
        tb = tb->next;
    }
    // 将随后的项目插入到得到的多项式中
    ta = ta->next;
    while (ta) {
        tb = objB;
        ptp = temp;
        while (tb) {
            int e = ta->ElemType.expn + tb->ElemType.expn;
            int c = ta->ElemType.coef * tb->ElemType.coef;
            // 查找插入的位置
            while (ptp->next && ptp->next->ElemType.expn > e) {
                ptp = ptp->next;
            }
            if (ptp->next && ptp->next->ElemType.expn == e) {
                // 直接加上c
                ptp->next->ElemType.coef += c;
                if (ptp->next->ElemType.coef != 0) {
                    // =0 删除该节点
                    Node* tmp = ptp->next;
                    ptp->next = tmp->next;
                    free(tmp);
                    tmp = NULL;
                }
            } else {
                // ptp->next < e 插入一个新节点
                Node* tmp = (Node*)malloc(sizeof(Node));
                tmp->ElemType.coef = c;
                tmp->ElemType.expn = e;
                tmp->next = ptp->next;
                ptp->next = tmp;
                // 由于按照指数递减,ta乘以下一项的指数比当前结果小,故
                // ptp指向该项,下次从这里开始搜索
                ptp = tmp;
            }
            tb = tb->next;
        }
        ta = ta->next;
    }
    // 删除头部的空节点
    Node* t = temp;
    temp = temp->next;
    free(t);
    t = NULL;
    return temp;
}

//获取链表长度
int PolynLength(LinkList obj) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp = obj;
    int length = 0;
    if (temp == NULL) return 0;
    while (temp->next != NULL) {
        length++;
        temp = temp->next;
    }
    return length;
}

//打印链表
void PrintPolyn(LinkList obj) {
    if (obj->next == NULL) return;
    LinkList mov = obj->next;
    while (mov != NULL) {
        if (mov->ElemType.coef != 0)
            printf("<%.1f, %d> ", mov->ElemType.coef, mov->ElemType.expn);
        mov = mov->next;
    }
    printf("\n");
}

int main() {
    LinkList objA = LinkListCreate();
    LinkList objB = LinkListCreate();
    float coef;
    int expn;
    printf("请输入多项式pA的系数和指数(输入0 0结束)\n");
    //    scanf("%f %d", &coef, &expn);
    //  CreatPolyn(objA, coef, expn);
    while (1) {
        scanf("%f %d", &coef, &expn);
        if (coef == 0 && expn == 0) break;
        CreatPolyn(objA, coef, expn);
    }
    printf("请输入多项式pB的系数和指数(输入0 0结束)\n");
    //   scanf("%f %d", &coef, &expn);
    //  CreatPolyn(objB, coef, expn);
    while (1) {
        scanf("%f %d", &coef, &expn);
        if (coef == 0 && expn == 0) break;
        CreatPolyn(objB, coef, expn);
    }

    LinkList obj = LinkListCreate();
    printf("请输入数字选择计算加法：1 乘法：2\n");
    int i = 0;
    scanf("%d", &i);

    printf("多项式pA:\n");
    PrintPolyn(objA);
    printf("多项式pB:\n");
    PrintPolyn(objB);

    if (i == 1) {
        obj = AddPolyn(objA, objB);
        printf("相加的结果:\n");
        PrintPolyn(obj);
    } else {
        obj = MultiPlyPolyn(objA, objB);
        printf("相乘的结果:\n");
        PrintPolyn(obj);
    }
    DestoryPolyn(objA);
    return 0;
}
