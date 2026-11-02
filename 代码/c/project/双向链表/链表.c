#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
typedef struct sqlist
{
    int data;               // 当前结点保存的数据
    struct sqlist* next;    // 下一个结点的地址
    struct sqlist* pre;
} sqlist;
sqlist* createnode() {
    sqlist* list = malloc(sizeof(sqlist));
    if (list == NULL)
    {
        printf("内存分配失败\n");
        return NULL;;
    }
    return list;
}
//1初始化链表
sqlist* init()
{
    sqlist* list = createnode();

    if (list == NULL)
    {
        return NULL;
    }
    list->next = list;
    list->pre = list;
    printf("初始化成功\n");
    return list;
}
//2. 打印链表
void printflist(sqlist *list)
{
    sqlist* temp = list;
    if (list == NULL)
    {
        printf("未初始化\n");
        return;
    }
    while (temp->next !=list)
    {
        printf("%d\n", temp->next->data);
        temp = temp->next;
    }
}
//3. 头插
void headadd(sqlist* list, int value)
{
    if (list == NULL)
    {
        printf("未初始化");
        return ;
    }
    sqlist* phead;
    sqlist* ptr = createnode();
    if (ptr == NULL)
    {
        printf("未初始化\n");
        return;
    }
    ptr->data = value;
    phead = list->next;
    list->next = ptr;
    ptr->pre = list;
    ptr->next = phead;
    phead->pre = ptr;
    printf("插入成功\n");
}
//4. 尾插 
void endadd(sqlist* list, int value)
{
    if (list == NULL)
    {
        printf("未初始化");
        return;
    }
    sqlist* pend;
    sqlist* ptr = createnode();  
    ptr->data = value;
    pend = list->pre;
    list->pre = ptr;
    ptr->next = list;
    ptr->pre = pend;
    pend->next = ptr;
    printf("插入成功");
}
//5. 头删
void headdel(sqlist* list)
{
    // 先判断头结点是否存在
    if (list == NULL)
    {
        printf("链表未初始化\n");
        return;
    }
    sqlist* ptr = list->next;
    // 双向循环链表为空
    if (list->next == list)
    {
        printf("链表为空，无法头删\n");
        return;
    }
    list->next = ptr->next;
    ptr->next->pre = list;
  
    printf("删除的元素是%d\n", ptr->data);
    free(ptr);
 }
//6. 尾删
void enddel(sqlist* list)
{
    // 先判断头结点是否存在
    if (list == NULL)
    {
        printf("链表未初始化\n");
        return;
    }
    sqlist* ptr = list->pre;
    // 双向循环链表为空
    if (list->next == list)
    {
        printf("链表为空，无法尾删\n");
        return;
    }
    list->pre = ptr->pre;
    ptr->pre->next = list;
    printf("删除的元素是%d\n", ptr->data);
    free(ptr);
    printf("尾删成功\n");
}
//7. 查找元素
sqlist* find(sqlist* list, int value)
{
    if (list == NULL)
    {
        printf("链表未初始化\n");
        return NULL;
    }
    sqlist* ptr = list;
    while (ptr->next != list)
    {
        if (ptr->next->data== value)
        {
            return ptr->next;
        }
        ptr = ptr->next;
    }
    return NULL;
}
//8. 链表节点的修改
void change(sqlist* pos, int value)
{
    if (pos == NULL)
    {
        printf("没有找到指定结点\n");
        return;
    }
    pos->data = value;
    printf("修改后的节点元素为%d\n", pos->data);
}
//9. 中间插入
void afteradd(sqlist* pos,int value)
{
    if (pos == NULL)
    {
        printf("没有找到指定结点\n");
        return;
    }
    sqlist* ptr = createnode();
    if (ptr == NULL)
    {
        return;
    }
    ptr->data = value;
    sqlist* phead = pos->next;
    pos->next = ptr;
    ptr->pre = pos;
    ptr->next = phead;
    phead->pre = ptr;
    printf("中间插入成功\n");
}
//10中间删除
void afterdel(sqlist* list, sqlist* pos)
{
    sqlist* ptr;

    if (list == NULL || pos == NULL)
    {
        printf("没有找到指定结点\n");
        return;
    }

    if (pos->next == list)
    {
        printf("该结点后面没有数据结点\n");
        return;
    }

    ptr = pos->next;
    pos->next = ptr->next;
    ptr->next->pre = pos;

    printf("删除的元素是%d\n", ptr->data);
    free(ptr);
}
//11. 销毁链表
void destroylist(sqlist** list)
{
    if (list == NULL || *list == NULL)
    {
        printf("链表未初始化\n");
        return;
    }
    sqlist* ptr = (*list)->next;
    sqlist* phead = NULL;
    while (ptr != *list)
    {
        phead = ptr->next;
        free(ptr);
        ptr = phead;
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
int main() {
    sqlist* list=NULL;
    sqlist* result;
    sqlist* pos;
    int order,value,target;
    help_menu();
    while (1)
    {
            printf("请输入您要执行的指令：");

            if (scanf("%d", &order) != 1)
            {
                printf("输入无效，程序结束\n");
                break;
            }
            switch (order)
            {
            case 0:
                destroylist(&list);
                printf("程序已经退出\n");
                return 0;
            case 1:

                if (list != NULL)
                {
                    printf("链表已经初始化，请先销毁原链表\n");
                    break;
                };
                list =init();
                break;
            case 2:
                printflist(list);
                break;
            case 3:
                printf("请输入头插元素：");
                scanf("%d", &value);
                headadd(list, value);
                break;
            case 4:
                printf("请输入尾插元素：");
                scanf("%d", &value);
                endadd(list, value);
                break;
            case 5:
                headdel(list);
                break;
            case 6:
                enddel(list);
                break;
            case 7:
                printf("请输入查找的元素：");
                scanf("%d", &value);
                result = find(list,value);
                if (result == NULL)
                {
                    printf("没有找到元素%d\n", value);
                }
                else
                {
                    printf(
                        "找到元素%d，结点地址为：%p\n",
                        result->data,
                        (void*)result
                    );
                }
                break;
            case 8:
                printf("请输入修改的元素：");
                scanf("%d", &value);
                pos= find(list, value);
                printf("请输入要修改的元素：");
                scanf("%d", &value);
               change(pos, value);
               break;
            case 9://中间插入
                printf("请输入目标结点的值：");
                scanf("%d", &target);
                /*
                找到目标结点
                */
                pos = find(list, target);
                printf("请输入插入目标结点的值：");
                scanf("%d", &value);
                afteradd(pos,value);
                break;
            case 10://中间删除
                printf("请输入目标结点的值：");
                scanf("%d", &target);
                /*
                找到目标结点
                */
                pos = find(list, target);
                /*
                删除目标结点后面的结点
                */
                afterdel(list,pos);
                break;
            case 11:
                destroylist(&list);
                printf("链表已经销毁\n");
                break;
            case 12:
                system("cls");
                help_menu();
                break;
            default:
                printf("指令错误，请输入0～11 \n");
                break;
            }
        }
        /*
        防止异常退出循环后存在内存泄漏
        */
        destroylist(&list);
        return 0;
    }
