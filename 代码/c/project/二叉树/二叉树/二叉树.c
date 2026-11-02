#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// 二叉树链式存储结点结构
typedef struct BTreeNode {
    char data;                // 数据域
    struct BTreeNode* LChild; // 左孩子指针
    struct BTreeNode* RChild; // 右孩子指针
} BTree;

// 先序递归创建二叉树（#表示空结点）
void init(BTree** root) {
    char str;
    // 跳过所有空白符（换行、空格、制表符），只读取有效字符
    scanf(" %c", &str);

    if (str == '#') {
        *root = NULL;
    }
    else {
        *root = (BTree*)malloc(sizeof(BTree));
        if (*root == NULL) {
            printf("内存分配失败！\n");
            return;
        }
        (*root)->data = str;
        (*root)->LChild = NULL; // 创建节点时即初始化左右指针为空
        (*root)->RChild = NULL;
        init(&(*root)->LChild); // 递归创建左子树
        init(&(*root)->RChild); // 递归创建右子树
    }
}

// 先序遍历：根 → 左 → 右
void preOrder(BTree* root) {
    if (root == NULL) {
        return;
    }
    printf("%c ", root->data);
    preOrder(root->LChild);
    preOrder(root->RChild);
}

// 主函数测试
int main() {
    BTree* root = NULL;
    printf("请输入先序序列（#表示空结点，可带空格）：\n");
    init(&root);

    printf("先序遍历结果：");
    preOrder(root);
    printf("\n");

    return 0;
}

