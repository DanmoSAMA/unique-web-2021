#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10

typedef int status;
typedef int ElemType;
typedef int KeyType;
//数据元素类型定义

typedef struct {
    //二叉树结点类型定义
    KeyType  key;
    char others[20];
} TElemType;

typedef struct BiTNode {
    //二叉链表结点的定义
    TElemType  data;
    struct BiTNode* lchild, * rchild;
} BiTNode, * BiTree;

void visit(BiTree T);
status CreateBiTree(BiTree& T, TElemType definition[]);
status CreateNonRecursive(BiTree& T, TElemType definition[]);
status PreRecursive(BiTree T, void (*visit)(BiTree));
status PreNonRecursive(BiTree T, void (*visit)(BiTree));
status InRecursive(BiTree T, void (*visit)(BiTree));
status InNonRecursive(BiTree T, void (*visit)(BiTree));
status PostRecursive(BiTree T, void (*visit)(BiTree));
status PostNonRecursive(BiTree T, void (*visit)(BiTree));
//status LevelRecursive(BiTree T, int level, void (*visit)(BiTree)); // bug
status LevelNonRecursive(BiTree T, void (*visit)(BiTree));

BiTree T;
TElemType definition[100];
BiTNode* stack[256];
int top, t, sign, level, LEVEL, book[100];

int main() {
    int e = 0, i = 0, check = 0, op = 1;
    BiTNode* p = NULL;
    while (op) {
        system("cls");	printf("\n\n");
        printf("      	基于二叉链表的二叉树实现--菜单 \n");
        printf("-------------------------------------------------\n");
        printf("    	  0. Exit\n");
        printf("    	  1. CreateBiTree       2. CreateNonRecursive \n");
        printf("    	  3. PreRecursive       4. PreNonRecursive    4.  \n");
        printf("    	  5. InRecursive        6. InNonRecursive \n");
        printf("    	  7. PostRecursive      8. PostNonRecursive\n");
        printf("    	  9. LevelRecursive     10. LevelNonRecursive\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0~10]:");
        scanf("%d",&op);
        switch (op) {
        case 0:
            break;
        case 1:
        case 2:
            printf("请输入带空子树的二叉树先序遍历序列definition：");
            do {
                scanf("%d%s",& definition[i].key, definition[i].others);
            } while (definition[i++].key != -1);
            check = CreateBiTree(T, definition);
            if (check == OK) {
                printf("二叉树创建成功！\n");
            }
            else printf("二叉树创建失败！\n");
            getchar(); getchar();
            break;
        case 3:
        case 4:
            check = PreRecursive(T, visit);
            if (check) printf("\n以上为二叉树的先序遍历序列！");
            else printf("空树无法遍历！");
            getchar(); getchar();
            break;
        case 5:
        case 6:
            check = InRecursive(T, visit);
            if (check) printf("\n以上为二叉树的中序遍历序列！");
            else printf("空树无法遍历！");
            getchar(); getchar();
            break;
        case 7:
        case 8:
            check = PostRecursive(T, visit);
            if (check) printf("\n以上为二叉树的后序遍历序列！");
            else printf("空树无法遍历！");
            getchar(); getchar();
            break;
        /*
        case 9:
            check = LevelRecursive(T, level, visit);
            if (check) printf("\n以上为二叉树的层序遍历序列！");
            else printf("空树无法遍历！");
            getchar(); getchar();
            break;
        */
        case 10:
            check = LevelNonRecursive(T, visit);
            if (check) printf("\n以上为二叉树的层序遍历序列！");
            else printf("空树无法遍历！");
            getchar(); getchar();
            break;
        default:
            printf("输入的指令错误，请重新输入！");
            getchar(); getchar();
        }
    }
    printf("欢迎下次再使用本系统！\n");
    return 0;
}

status CreateBiTree(BiTree& T, TElemType definition[])
// 递归方式创建二叉树
{
    if (definition[t].key == 0) {
        T = NULL;
        t++;
        return OK;
    }
    if (book[definition[t].key]) {
        return ERROR;
    }
    T = (BiTree)malloc(sizeof(BiTNode));
    T->data.key = definition[t].key;
    book[definition[t].key] = 1;
    strcpy(T->data.others, definition[t].others);
    t++;

    sign = CreateBiTree(T->lchild, definition);
    if (sign == ERROR) {
        return ERROR;
    }
    sign = CreateBiTree(T->rchild, definition);
    if (sign == ERROR) {
        return ERROR;
    }
    return OK;
}
BiTNode* NewNode(TElemType& data)
// 初始化结点
{
    BiTNode* ptr = (BiTNode*)malloc(sizeof(BiTNode));
    ptr->data = data;
    ptr->lchild = NULL;
    ptr->rchild = NULL;
    return ptr;
}

status CreateNonRecursive(BiTree& T, TElemType definition[])
// 非递归创建二叉树
{
    int i = 0; top = -1;
    BiTNode* temp;
    T = NewNode(definition[i++]);

    for (int i = 0; definition[i].key != -1; ++i)
        for (int j = i + 1; definition[j].key != -1; ++j)
            if (definition[i].key && definition[i].key == definition[j].key)
                return ERROR;

    if (definition[i].key == 0 || definition[i].key == -1)
        return OK;

    while (definition[i].key != -1) {
        while (!definition[i].key) {
            stack[++top] = T;
            temp = NewNode(definition[i++]);
            T = T->lchild = temp;
            if (definition[i].key == -1) return OK;
        }
        i++;
        if (definition[i].key) {
            stack[++top] = T;
            temp = NewNode(definition[i++]);
            T = T->rchild = temp;
            if (definition[i].key == -1) return OK;
        }
        else {
            while (top && !definition[i].key) {
                T = stack[top--];
                if (definition[++i].key == -1) return OK;
            }
            if (!definition[i].key == -1) return OK;
            temp = NewNode(definition[i++]);
            T = T->rchild = temp;
        }
    }
}

void visit(BiTree T) {
    printf(" %d,%s", T->data.key, T->data.others);
}

status PreRecursive(BiTree T, void (*visit)(BiTree))
// 先序遍历二叉树T（递归）
{
    if (T == NULL) return ERROR;
    visit(T);
    PreRecursive(T->lchild, visit);
    PreRecursive(T->rchild, visit);
    return OK;
}

status PreNonRecursive(BiTree T, void (*visit)(BiTree)) 
// 先序遍历二叉树T（非递归）
{
    if (T == NULL) return ERROR;
    top = -1;
    while (T || top != -1) {
        if (T) {
            visit(T);
            stack[++top] = T;
            T = T->lchild;
        }
        else {
            T = stack[top--];
            T = T->rchild;
        }
    }
    return OK;
}

status InRecursive(BiTree T, void (*visit)(BiTree))
// 中序遍历二叉树T（递归）
{
    if (T == NULL) return ERROR;
    InRecursive(T->lchild, visit);
    visit(T);
    InRecursive(T->rchild, visit);
    return OK;
}

status InNonRecursive(BiTree T, void (*visit)(BiTree))
// 中序遍历二叉树T（非递归）
{
    if (T == NULL) return ERROR;
    top = -1;
    while (T || top != -1) {
        if (T) {
            stack[++top] = T;
            T = T->lchild;
        }
        else {
            T = stack[top--];
            visit(T);
            T = T->rchild;
        }
    }
    return OK;
}

status PostRecursive(BiTree T, void (*visit)(BiTree)) 
// 后序遍历二叉树T（递归）
{
    if (T == NULL) return ERROR;
    PostRecursive(T->lchild, visit);
    PostRecursive(T->rchild, visit);
    visit(T);
    return OK;
}

status PostNonRecursive(BiTree T, void (*visit)(BiTree))
// 后序遍历二叉树T（非递归）
{
    if (T == NULL) return ERROR;
    BiTNode* r = NULL; // 最近访问过的结点
    top = -1;
    while (T || top != -1) {
        if (T) {
            stack[++top] = T;
            T = T->lchild;
        }
        else {
            T = stack[top];
            if (T->rchild && T->rchild != r) {
                T = T->rchild;
                stack[++top] = T;
                T = T->lchild;
            }
            else {
                T = stack[top--];
                visit(T);
                r = T;
                T = NULL;
            }
        }
    }
    return OK;
}

/*
status LevelRecursive(BiTree T, int level, void (*visit)(BiTree))
// 层序遍历（递归）
{
    if (T == NULL) return ERROR;
    visit(T);
    if (level <= LEVEL) LevelRecursive(T->lchild, ++level, visit);
    else return OK;
    level--;
    if (level <= LEVEL) LevelRecursive(T->rchild, ++level, visit);
    else return OK;
    level--;
    LEVEL++;
}
*/

status LevelNonRecursive(BiTree T, void (*visit)(BiTree))
// 层序遍历（非递归）
{
    if (T == NULL) return ERROR;
    int head = 0, rear = 0;

    if (T) stack[rear++] = T;

    while (head < rear) {
        T = stack[head++];
        visit(T);

        if (T->lchild) stack[rear++] = T->lchild;
        if (T->rchild) stack[rear++] = T->rchild;
    }
    return OK;
}

