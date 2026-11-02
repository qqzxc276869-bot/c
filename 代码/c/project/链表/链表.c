#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
typedef struct sqlist
{
    int data;               // 当前结点保存的数据
    struct sqlist* next;    // 下一个结点的地址
} sqlist;
sqlist* createnode(int value) {
    sqlist* node =malloc(sizeof(sqlist));
    if (node == NULL)
    {
        printf("内存分配失败\n");
        return NULL;;
    }
    sqlist* temp=node;
    temp->data = value;
    temp->next = NULL;
    return node;
}

//1初始化链表
sqlist* init(int value)
{
    sqlist* list = createnode(value);

    if (list == NULL)
    {
        return NULL;
    }
    printf("初始化成功\n");
    return list;
}
//2. 打印链表
//
void printflist(sqlist* list)
{
    sqlist* temp = list;
    if (temp == NULL)
    {
        printf("未初始化\n");
        return;
    }
    printf("链表元素: ");
    while (temp)
    {
        printf("%d", temp->data);
        if (temp->next != NULL)
        {
            printf("->");
        }
        temp = temp->next;
    }
    printf("\n");
}
//3. 头插
void headadd(sqlist**list, int value)
{
    sqlist* temp;
    if (list == NULL)
        return;
    temp = createnode(value);
    if (temp == NULL)
        return;
    temp->next = *list;
    temp->data = value;
    *list = temp;
    printf("头插成功\n");
}
//4. 尾插 
void endadd(sqlist** list, int value)
{
    sqlist* temp;
    sqlist* endnode;
    if (list == NULL)
        return;
    endnode= createnode(value);
    if (endnode == NULL)
        return;
    /*
   如果链表为空，新结点直接成为第一个结点
   */
    if (*list == NULL)
    {
        *list = endnode;
        printf("尾插成功\n");
        return;
    }
    temp = *list;
    while (temp->next!=NULL)
    {
        temp = temp->next;
    }
    temp->next = endnode;
    printf("尾插成功\n");
}
//5. 头删
void headdel(sqlist** list)
{
    sqlist* temp;
    if (list == NULL || *list == NULL)
    {
        printf("链表为空,无法头删\n");
        return;
    }
    temp = *list;
    *list = temp->next;
    printf("删除的元素是%d\n", temp->data);
    free(temp);

}
//6. 尾删
void enddel(sqlist** list)
{
    sqlist* temp;
    sqlist* endnode;
    if (list == NULL || *list == NULL)
    {
        printf("链表为空,无法尾删\n");
        return;
    }
    if ((*list)->next == NULL)
    {
        printf("删除的元素是%d", (*list)->data);
        free(*list);
        *list = NULL;
        return;
    }
    temp = *list;
    while (temp->next->next != NULL)
    {
        temp = temp->next;
    }
    endnode = temp->next;
    printf("删除的元素是%d", endnode->data);
    free(endnode);
   temp->next= NULL;
}
//7. 在指定结点之后插入
void afteradd(sqlist* pos,int value)
{
    sqlist* newnode;

    if (pos == NULL)
    {
        printf("没有找到指定结点\n");
        return;
    }

    newnode = createnode(value);

    if (newnode == NULL)
    {
        return;
    }

    /*
    新结点指向pos原来的下一个结点
    */
    newnode->next = pos->next;

    /*
    pos指向新结点
    */
    pos->next = newnode;

    printf("插入成功\n");
}
//8. 删除指定结点之后的结点
void afterdel(sqlist* pos)
{
    sqlist* temp=pos;
    if (pos == NULL)
    {
        printf("没有找到指定结点\n");
        return;
    }
    if (pos->next == NULL)
    {
        printf("该结点后面没有可以删除的结点\n");
        return;
    }
    temp = pos->next;
    pos->next = temp->next;
    printf("删除的元素是%d\n", temp->data);
    free(temp);
}
//9. 销毁链表
void destroylist(sqlist** list)
{
    if (list == NULL)
    {
        return;
    }
    while (*list != NULL)
    {
        sqlist* temp = (*list)->next;
        free(*list);
        *list = temp;
    }
}
//10. 查找元素
sqlist *find(sqlist* list, int value)
{
    sqlist* temp=list;
    while (temp != NULL)
    {
        if (temp->data == value)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
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
    printf("* 7. 在指定结点之后插入                    *\n");
    printf("* 8. 删除指定结点之后的结点                *\n");
    printf("* 9. 销毁链表                              *\n");
    printf("* 10. 查找元素                             *\n");
    printf("* 11. 清屏                                 *\n");
    printf("* 0. 退出                                  *\n");
    printf("* ======================================== *\n");
}
int main() {
    sqlist* list = NULL;
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
                }
                printf("请输入第一个元素：");
                scanf("%d", &value);
                list =init(value);
                break;
            case 2:
                printflist(list);
                break;
            case 3:
                printf("请输入头插元素：");
                scanf("%d", &value);
                headadd(&list, value);
                break;
            case 4:
                printf("请输入尾插元素：");
                scanf("%d", &value);
                endadd(&list, value);
                break;
            case 5:
                headdel(&list);
                break;
            case 6:
                enddel(&list);
                break;
            case 7:
                printf("请输入目标结点的值：");
                scanf("%d", &target);
                printf("请输入需要插入的元素：");
                scanf("%d", &value);
                /*
                查找目标结点
                */
                pos = find(list, target);
                /*
                在目标结点后面插入
                */
                afteradd(pos, value);
                break;
            case 8:
                printf("请输入目标结点的值：");
                scanf("%d", &target);
                /*
                找到目标结点
                */
                pos = find(list, target);
                /*
                删除目标结点后面的结点
                */
                afterdel(pos);
                break;
            case 9:
                destroylist(&list);
                printf("链表已经销毁\n");
                break;
            case 10:
                printf("请输入查找的元素：");
                scanf("%d", &value);
                result = find(list, value);
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
            case 11:
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
