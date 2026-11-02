#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
typedef struct sqlist
{
    int data;               // 当前结点保存的数据
    struct sqlist* next;    // 下一个结点的地址
    struct sqlist* pre;
} sqlist;
sqlist* createlist()
{
    sqlist* list = malloc(sizeof(sqlist));
    if (list == NULL)
    {
        printf("空间开辟失败");
        return;
    }
    return list;
}
//1初始化链表
sqlist* init()
{
    sqlist* list = createlist();
    list->next = list;
    list->pre = list;
    printf("初始化成功\n");
    return list;
}
//2. 打印链表
void printflist(sqlist *list)
{
    sqlist* temp;
    if (list == NULL)
    {
        printf("空间开辟失败");
        return;
    };
    temp = list->next;
    while (temp!=list)
    {
        printf("%d ",temp->data);
        temp = temp->next;
    }
    printf("\n");
 
}
//3. 头插
void headadd(sqlist *list,int value)
{
    if (list == NULL) {
        return;
    }
    sqlist* ptr=createlist();
    if(ptr==NULL)
    {
        printf("开辟失败");
        return;
    }
    ptr->data = value;
    sqlist* temp;
    temp = list->next;
    list->next = ptr;
    ptr->pre = list;
    ptr->next = temp;
    temp->pre = ptr;
    printf("插入%d成功\n",value);
 
}
//4. 尾插 
void endadd(sqlist* list, int value)
{
    if (list == NULL) {
        return;
    }
    sqlist* ptr = createlist();
    if (ptr == NULL)
    {
        printf("开辟失败");
        return;
    }
    sqlist* temp=list->pre;
    ptr->data = value;
    list->pre = ptr;
    ptr->next = list;
    ptr->pre = temp;
    temp->next = ptr;
    printf("尾插%d成功\n", ptr->data);
 
}
//5. 头删
void headdel(sqlist* list)
{
    if (list == NULL) {
        printf("未初始化\n");
        return;
    }
    sqlist* temp;
    temp= list->next;
    if (list->next == list)
    {
        printf("链表为空\n");
        return;
    }
    list->next = temp->next;
    temp->next->pre = temp;
    printf("头删成功\n");
}
//6. 尾删
void enddel(sqlist* list)
{
    if (list == NULL) {
        printf("未初始化\n");
        return;
    }
    sqlist* temp;
    temp = list->pre;
    if (list->pre == list)
    {
        printf("链表为空\n");
        return;
    }
    list->pre = temp->pre;
    temp->pre->next = list;
    printf("尾删成功\n");
}
//7. 查找元素
sqlist* find(sqlist* list,int value)
{
    if (list == NULL) {
        printf("未初始化\n");
        return NULL;
    }
    sqlist* temp;
    temp = list->next;
    if (list->pre == list)
    {
        printf("链表为空\n");
        return NULL;
    }
    while (temp != list)
    {
        if (temp->data ==value)
        {
            printf("查找成功 ,元素%d的地址为 %p\n", temp->data, (void*)temp);
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
//8. 链表节点的修改
void change(sqlist* pos,int value)
{
    pos->data = value;
}
//9. 中间插入
void afteradd(sqlist* pos, int value)
{
    headadd(pos, value);
}
//10中间删除
void afterdel(sqlist *list,sqlist *pos)
{
    if (list == NULL || pos == NULL)
    {
        printf("没找到指定节点\n");
        return;
    }
    if (list->next == list)
    {
        printf("链表为空\n");
        return;
    }
    sqlist* ptr=pos;
    pos->pre->next = pos->next;
    pos->next->pre = pos->pre;
    printf("删除的元素是%d\n", ptr->data);
    free(ptr);
}
//11. 销毁链表
void destroy(sqlist** list)
{
    if (list == NULL || *list == NULL)
    {
        printf("链表未初始化\n");
        return;
    }
    sqlist* ptr = (*list)->next;
    sqlist* temp = NULL;
    while (ptr != *list)
    {
        temp = ptr->next;
        free(ptr);
        ptr = temp;
    }
    free(*list);
    *list = NULL;
    printf("销毁成功\n");
}
/* 0.菜单 */
void help_menu(void)
{
    printf("\n");
    printf("          欢迎操作单链表\n");
    printf("* ======================================== *\n");
    printf("* 1. 初始化链表                            *\n");
    printf("* 2. 打印链表                              *\n");
    printf("* 3. 头插                                  *\n");
    printf("* 4. 尾插                                  *\n");
    printf("* 5. 头删                                  *\n");
    printf("* 6. 尾删                                  *\n");
    printf("* 7. 查找                                  *\n");
    printf("* 8. 修改链表的值                          *\n");
    printf("* 9. 中间插入                              *\n");
    printf("* 10. 中间删除                             *\n");
    printf("* 11. 销毁链表                             *\n");
    printf("* 12. 清屏                                 *\n");
    printf("* 0. 退出                                  *\n");
    printf("* ======================================== *\n");
}
int main()
{
    sqlist* list = NULL;
    sqlist* pos;
    int order,value,agevalue;
    /*
    防止异常退出循环后存在内存泄漏
    */
    help_menu();
    while (1)
    {
        printf("请输入指令: ");
        scanf("%d", &order);
        switch (order) {

        case 1://初始化链表
            list = init();
            break;
        case 2://打印链表
            printflist(list);
            break;
        case 3://头插
            printf("请输入插入的元素:");
            scanf("%d", &value);
            headadd(list,value);
            break;
        case 4:
            //尾插
            printf("请输入插入的元素:");
            scanf("%d", &value);
            endadd(list, value);
            break;
        case 5:
            //头删
            headdel(list);
            break;
        case 6:
            //尾删
            enddel(list);
            break;
        case 7://查找
            printf("要查找的目标元素:");
            scanf("%d", &value);
            pos=find(list, value);
            if (pos==NULL)
            {
                printf("查找失败");
            }
            break;
        case 8://修改链表的值
            printf("请输入要修改的值 ");
            scanf("%d", &agevalue);
           pos= find(list, agevalue);
           printf("请输入修改后的值 ");
           scanf("%d", &value);
           change(pos, value);
            break;
        case 9://中间插入
            printf("请输入要插入在哪里 ");
            scanf("%d", &agevalue);
            pos = find(list, agevalue);
            printf("请输入要插入的值 ");
            scanf("%d", &value);
            afteradd(pos, value);
            break;
        case 10://中间删除
            printf("请输入要删除哪里 ");
            scanf("%d", &agevalue);
            pos = find(list, agevalue);
            afterdel(list,pos);
            break;
        case 11://销毁
            destroy(&list);
            break;
        case 12://清屏
            system("cls");
            help_menu();
            break;
        case 0:
            return;
        }
    }
}
