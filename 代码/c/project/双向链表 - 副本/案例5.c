#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

/* 链表结点 */
typedef struct sqlist
{
    int data;               // 当前结点保存的数据
    struct sqlist* next;    // 下一个结点的地址
} sqlist;


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
    printf("* 11. 查找元素                             *\n");
    printf("* 0. 退出                                  *\n");
    printf("* ======================================== *\n");
}


/* 创建一个新结点 */
sqlist* createNode(int value)
{
    sqlist* node = malloc(sizeof * node);

    if (node == NULL)
    {
        printf("内存分配失败\n");
        return NULL;
    }

    node->data = value;
    node->next = NULL;

    return node;
}


/* 1.初始化链表，创建第一个结点 */
sqlist* linkinit(int value)
{
    sqlist* list = createNode(value);

    if (list != NULL)
    {
        printf("初始化成功\n");
    }

    return list;
}


/* 2.打印链表 */
void printList(const sqlist* list)
{
    const sqlist* temp = list;

    if (temp == NULL)
    {
        printf("链表为空\n");
        return;
    }

    printf("链表元素：");

    while (temp != NULL)
    {
        printf("%d", temp->data);

        if (temp->next != NULL)
        {
            printf(" -> ");
        }

        temp = temp->next;
    }

    printf("\n");
}


/* 3.头插 */
void headAdd(sqlist** list, int value)
{
    sqlist* newNode;

    if (list == NULL)
    {
        return;
    }

    newNode = createNode(value);

    if (newNode == NULL)
    {
        return;
    }

    /*
    新结点指向原来的第一个结点
    */
    newNode->next = *list;

    /*
    修改外部头指针，使其指向新结点
    */
    *list = newNode;

    printf("头插成功\n");
}


/* 4.尾插 */
void tailAdd(sqlist** list, int value)
{
    sqlist* newNode;
    sqlist* temp;

    if (list == NULL)
    {
        return;
    }

    newNode = createNode(value);

    if (newNode == NULL)
    {
        return;
    }

    /*
    如果链表为空，新结点直接成为第一个结点
    */
    if (*list == NULL)
    {
        *list = newNode;
        printf("尾插成功\n");
        return;
    }

    /*
    找到最后一个结点
    */
    temp = *list;

    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newNode;

    printf("尾插成功\n");
}


/* 5.头删 */
void headDelete(sqlist** list)
{
    sqlist* temp;

    if (list == NULL || *list == NULL)
    {
        printf("链表为空，无法头删\n");
        return;
    }

    /*
    保存原来的第一个结点
    */
    temp = *list;

    /*
    让头指针指向第二个结点
    */
    *list = temp->next;

    printf("删除的元素是：%d\n", temp->data);

    free(temp);
}


/* 6.尾删 */
void tailDelete(sqlist** list)
{
    sqlist* temp;
    sqlist* last;

    if (list == NULL || *list == NULL)
    {
        printf("链表为空，无法尾删\n");
        return;
    }

    /*
    链表中只有一个结点
    */
    if ((*list)->next == NULL)
    {
        printf("删除的元素是：%d\n", (*list)->data);

        free(*list);
        *list = NULL;

        return;
    }

    /*
    找到倒数第二个结点
    */
    temp = *list;

    while (temp->next->next != NULL)
    {
        temp = temp->next;
    }

    /*
    temp指向倒数第二个结点
    temp->next指向最后一个结点
    */
    last = temp->next;

    printf("删除的元素是：%d\n", last->data);

    free(last);
    temp->next = NULL;
}


/* 查找结点 */
sqlist* findNode(sqlist* list, int value)
{
    sqlist* temp = list;

    while (temp != NULL)
    {
        /*
        注意这里是==，不是=
        */
        if (temp->data == value)
        {
            return temp;
        }

        temp = temp->next;
    }

    return NULL;
}


/* 7.在指定结点之后插入 */
void insertAfter(sqlist* pos, int value)
{
    sqlist* newNode;

    if (pos == NULL)
    {
        printf("没有找到指定结点\n");
        return;
    }

    newNode = createNode(value);

    if (newNode == NULL)
    {
        return;
    }

    /*
    新结点指向pos原来的下一个结点
    */
    newNode->next = pos->next;

    /*
    pos指向新结点
    */
    pos->next = newNode;

    printf("插入成功\n");
}


/* 8.删除指定结点之后的结点 */
void deleteAfter(sqlist* pos)
{
    sqlist* temp;

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

    /*
    temp保存需要删除的结点
    */
    temp = pos->next;

    /*
    跳过需要删除的结点
    */
    pos->next = temp->next;

    printf("删除的元素是：%d\n", temp->data);

    free(temp);
}


/* 9.销毁链表 */
void destroyList(sqlist** list)
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


/* 主函数 */
int main(void)
{
    sqlist* list = NULL;
    sqlist* pos;
    sqlist* result;

    int order;
    int value;
    int target;
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

            destroyList(&list);

            printf("程序退出\n");

            return 0;


        case 1:

            if (list != NULL)
            {
                printf("链表已经初始化，请先销毁原链表\n");
                break;
            }

            printf("请输入第一个元素：");
            scanf("%d", &value);

            list = linkinit(value);

            break;


        case 2:

            printList(list);

            break;


        case 3:

            printf("请输入头插元素：");
            scanf("%d", &value);

            headAdd(&list, value);

            break;


        case 4:

            printf("请输入尾插元素：");
            scanf("%d", &value);

            tailAdd(&list, value);

            break;


        case 5:

            headDelete(&list);

            break;


        case 6:

            tailDelete(&list);

            break;


        case 7:

            printf("请输入目标结点的值：");
            scanf("%d", &target);

            printf("请输入需要插入的元素：");
            scanf("%d", &value);

            /*
            查找目标结点
            */
            pos = findNode(list, target);

            /*
            在目标结点后面插入
            */
            insertAfter(pos, value);

            break;


        case 8:

            printf("请输入目标结点的值：");
            scanf("%d", &target);

            /*
            找到目标结点
            */
            pos = findNode(list, target);

            /*
            删除目标结点后面的结点
            */
            deleteAfter(pos);

            break;


        case 9:

            destroyList(&list);

            printf("链表已经销毁\n");

            break;


        case 10:

            printf("请输入查找的元素：");
            scanf("%d", &value);

            result = findNode(list, value);

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

            printf("指令错误，请输入0～10\n");

            break;
        }
    }

    /*
    防止异常退出循环后存在内存泄漏
    */
    destroyList(&list);

    return 0;
}