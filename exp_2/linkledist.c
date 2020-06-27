#include <malloc.h>
#include <stdio.h>

typedef struct Node {
    int val;
    struct Node* next;
} Node, *MyLinkedList;

//创建一个链表，即创建一个头结点
MyLinkedList myLinkedListCreate() {
    Node* head = (Node*)malloc(sizeof(Node));
    if (head == NULL) {
        printf("内存分配失败\n");
        return 0;
    }
    head->val = 0;
    head->next = NULL;
    return head;
}

//在链表首元结点之前添加一个结点，作为新的首元结点
void myLinkedListAddAtHead(MyLinkedList obj, int val) {
    Node* p;
    p = (Node*)malloc(sizeof(Node));
    if (p != NULL) {
        p->val = val;
        p->next = obj->next;
        obj->next = p;
    }
}

//在链表尾结点之后添加一个结点，作为新的尾结点
void myLinkedListAddAtTail(MyLinkedList obj, int val) {
    Node* p;
    Node* temp;
    temp = obj;
    p = (Node*)malloc(sizeof(Node));
    if (p != NULL) {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        p->val = val;
        p->next = temp->next;
        temp->next = p;
    }
}

//在指定位置插入一个结点，值为val
void myLinkedListAddAtIndex(MyLinkedList obj, int index, int val) {
    Node* temp;
    temp = obj;
    for (int i = 0; i < index; i++) {
        temp = temp->next;
        if (temp == NULL) printf("插入位置无效\n");
    }
    Node* p = (Node*)malloc(sizeof(Node));
    p->val = val;
    p->next = temp->next;
    temp->next = p;
}

//删除指定位置的结点
void myLinkedListDeleteAtIndex(MyLinkedList obj, int index) {
    Node* temp;
    temp = obj;
    for (int i = 0; i < index; i++) {
        temp = temp->next;
        if (temp == NULL) printf("没有该节点\n");
    }
    Node* del = temp->next;
    temp->next = temp->next->next;
    free(del);
}

//获取索引号为index的节点的数值，如果索引号错误则返回-1
int myLinkedListGet(MyLinkedList obj, int index) {
    Node* temp;
    temp = obj;
    for (int i = 0; i < index; i++) {
        temp = temp->next;
        if (temp == NULL) {
            printf("没有该节点\n");
            return -1;
        }
    }
    return (temp->next->val);
}

//打印链表
void showAll(MyLinkedList obj) {
    if (obj->next == NULL) return;
    MyLinkedList mov = obj->next;
    while (mov != NULL) {
        printf("%d ", mov->val);
        mov = mov->next;
    }
    printf("\n");
}

//释放链表
void myLinkedListFree(MyLinkedList obj) {
    MyLinkedList Transit;
    while (obj->next != NULL) {
        Transit = obj;
        obj = obj->next;
        free(Transit);
    }
    free(obj);
    return;
}

int main() {
    MyLinkedList obj = myLinkedListCreate();
    int i;
    for (i = 2; i < 6; i++) {
        myLinkedListAddAtTail(obj, i * i);
    }

    printf("初始的四个结点：");
    showAll(obj);

    myLinkedListAddAtHead(obj, 1);
    printf("首元结点前插入：");
    showAll(obj);

    myLinkedListAddAtIndex(obj, 2, 6);
    printf("在指定坐标插入：");
    showAll(obj);

    myLinkedListDeleteAtIndex(obj, 3);
    printf("在指定坐标删除：");
    showAll(obj);

    printf("获取指定坐标数值：%d\n", myLinkedListGet(obj, 3));

    myLinkedListFree(obj);

    return 0;
}
