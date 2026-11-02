#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
typedef  struct LinkList {
    int data;
    struct LinkList* next;
}LinkList;
//生成链表
LinkList* createLinkList(int *arr, int len)
{
    LinkList* list = NULL;
    list = (LinkList*)malloc(sizeof(LinkList));
	list->data = arr[0];
    list->next = NULL;
    LinkList* p = list;
    for (int i = 1; i < len; i++)
    {
        LinkList* node = (LinkList*)malloc(sizeof(LinkList));
        node->data = arr[i];
        node->next = NULL;
        p->next = node;
        p = node;
    }
	return list;
}
//打印链表
void printlist(LinkList *list)
{
    if (list == NULL)
    {
        return;
    }
    LinkList* temp = list;
    while (temp)
    {
        printf("%d\t", temp->data);
        temp = temp->next;
    }
}
//反转链表
LinkList* changelist(LinkList* list)

{
    if (list == NULL)
    {
        return;
    }
    LinkList* pre = NULL;
    LinkList *next, * cur;
    cur = list;
    while (cur)
    {
        next = cur->next;
        cur->next = pre;
        pre = cur;
        cur = next;
    }
    return pre;
}
//销毁链表
void destory(LinkList** list)
{
    if (list == NULL || *list == NULL) {
        printf("没有链表，不需要销毁");
        return;
    }
    LinkList* temp =NULL;
    while (*list)
    {
        temp = *list;
        *list = (*list)->next;
        free(temp);
        temp = NULL;
    }
}
int main() 
{
    LinkList* list = NULL;
    int  arr[5] = { 10,20,30,40,50 };
    list = createLinkList(arr, 5);
    if (list != NULL) {
        printf("链表创建成功\n");

    }
    printf("翻转前：");
    printlist(list);

    list = changelist(list);
    printf("翻转后：");
    printlist(list);

    destory(&list);
    return 0;
}
