#include <stdio.h>
#include <stdlib.h>

typedef struct LNode {
    int data;
    struct LNode* next;
} LNode, * LinkList;

// 初始化
void InitList(LinkList* L) {
    *L = (LNode*)malloc(sizeof(LNode));
    (*L)->next = NULL;
}

// 头插
void HeadInsert(LinkList L, int e) {
    LNode* s = (LNode*)malloc(sizeof(LNode));
    s->data = e;
    s->next = L->next;
    L->next = s;
}

// 按位插（第i位）
void ListInsert(LinkList L, int i, int e) {
    LNode* p = L;
    for (int j = 0; j < i - 1; j++) p = p->next;
    LNode* s = (LNode*)malloc(sizeof(LNode));
    s->data = e;
    s->next = p->next;
    p->next = s;
}

// 尾删
void TailDelete(LinkList L, int* e) {
    LNode* p = L;
    while (p->next->next) p = p->next;
    LNode* q = p->next;
    *e = q->data;
    p->next = NULL;
    free(q);
}

// 销毁
void DestroyList(LinkList* L) {
    LNode* temp;
    while (*L) {
        temp = *L;
        *L = (*L)->next;
        free(temp);
    }
}
