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
//����Ԫ�����Ͷ���

typedef struct {
    //������������Ͷ���
    KeyType  key;
    char others[20];
} TElemType;

typedef struct BiTNode {
    //����������Ķ���
    TElemType  data;
    struct BiTNode *lchild,  *rchild;
} BiTNode,  *BiTree;

void visit(BiTree T);
status CreateBiTree(BiTree &T, TElemType definition[]);
status PreRecursive(BiTree T, void (*visit)(BiTree));
status PreNonRecursive(BiTree T, void (*visit)(BiTree));
status InRecursive(BiTree T, void (*visit)(BiTree));
status InNonRecursive(BiTree T, void (*visit)(BiTree));
status PostRecursive(BiTree T, void (*visit)(BiTree));
status PostNonRecursive(BiTree T, void (*visit)(BiTree));
status LevelOrderTraverse(BiTree T, void (*visit)(BiTree));
status CreateBiTree(BiTree& T, TElemType definition[]);

BiTree T;
TElemType definition[100];
BiTNode *stack[256];
int top, t, sign, book[100];

int main() {
    int e = 0, i = 0, check = 0, op = 1;
    BiTNode *p = NULL;
    while (op) {
        system("cls");	printf("\n\n");
        printf("      	���ڶ�������Ķ�����ʵ��--�˵� \n");
        printf("-------------------------------------------------\n");
        printf("    	  0. Exit\n");
        printf("    	  1. CreateBiTree       2. PreRecursive\n");
        printf("    	  3. PreNonRecursive    4. InRecursive \n");
        printf("    	  5. InNonRecursive     6. PostRecursive\n");
        printf("    	  7. PostNonRecursive   8. LevelOrderTraverse\n");
        printf("    	  9. DFSRecursive       10. DFSNonRecursive\n");
        printf("    	  11. BFS               12. CreateNonRecursive\n");
        printf("-------------------------------------------------\n");
        printf("    ��ѡ����Ĳ���[0~11]:");
        scanf("%d", &op);
        switch (op) {
        case 0:
            break;
        case 1:
            printf("��������������Ķ����������������definition��");
            do {
                scanf("%d%s", &definition[i].key, definition[i].others);
            } while (definition[i++].key != -1);
            check = CreateBiTree(T, definition);
            if (check == OK) {
                printf("�����������ɹ���\n");
            }
            else printf("����������ʧ�ܣ�\n");
            getchar(); getchar();
            break;
        case 2:
            check = PreRecursive(T, visit);
            if (check) printf("\n����Ϊ������������������У�");
            else printf("�����޷�������");
            getchar(); getchar();
            break;
        case 3:
            check = PreNonRecursive(T, visit);
            if (check) printf("\n����Ϊ������������������У�");
            else printf("�����޷�������");
            getchar(); getchar();
            break;
        case 4:
            check = InRecursive(T, visit);
            if (check) printf("\n����Ϊ������������������У�");
            else printf("�����޷�������");
            getchar(); getchar();
            break;
        case 5:
            check = InNonRecursive(T, visit);
            if (check) printf("\n����Ϊ������������������У�");
            else printf("�����޷�������");
            getchar(); getchar();
            break;
        case 6:
            check = PostRecursive(T, visit);
            if (check) printf("\n����Ϊ�������ĺ���������У�");
            else printf("�����޷�������");
            getchar(); getchar();
            break;
        case 7:
            check = PostNonRecursive(T, visit);
            if (check) printf("\n����Ϊ�������ĺ���������У�");
            else printf("�����޷�������");
            getchar(); getchar();
            break;
        default:
            printf("�����ָ��������������룡");
            getchar(); getchar();
        }
    }
    printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
    return 0;
}

BiTNode *NewNode(TElemType &data)
// ��ʼ�����
{
    BiTNode *ptr = (BiTNode*)malloc(sizeof(BiTNode));
    ptr->data = data;
    ptr->lchild = NULL;
    ptr->rchild = NULL;
    return ptr;
}

status CreateBiTree(BiTree &T, TElemType definition[])
// �ݹ鷽ʽ����������
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

void visit(BiTree T) {
    printf(" %d,%s", T->data.key, T->data.others);
}

status PreRecursive(BiTree T, void (*visit)(BiTree))
// �������������T���ݹ飩
{
    if (T == NULL) return ERROR;
    visit(T);
    PreRecursive(T->lchild, visit);
    PreRecursive(T->rchild, visit);
    return OK;
}

status PreNonRecursive(BiTree T, void (*visit)(BiTree)) 
// �������������T���ǵݹ飩
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
// �������������T���ݹ飩
{
    if (T == NULL) return ERROR;
    InRecursive(T->lchild, visit);
    visit(T);
    InRecursive(T->rchild, visit);
    return OK;
}

status InNonRecursive(BiTree T, void (*visit)(BiTree))
// �������������T���ǵݹ飩
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
// �������������T���ݹ飩
{
    if (T == NULL) return ERROR;
    PostRecursive(T->lchild, visit);
    PostRecursive(T->rchild, visit);
    visit(T);
    return OK;
}

status PostNonRecursive(BiTree T, void (*visit)(BiTree))
// �������������T���ǵݹ飩
{
    if (T == NULL) return ERROR;
    BiTNode *r = NULL; // ������ʹ��Ľ��
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

status LevelOrderTraverse(BiTree T, void (*visit)(BiTree))
// �������
{
    if (T == NULL) return ERROR;
    BiTree nodes[256];
    int head = 0, rear = 0;

    if (T) nodes[rear++] = T;

    while (head < rear) {
        T = nodes[head++];
        visit(T);

        if (T->lchild) nodes[rear++] = T->lchild;
        if (T->rchild) nodes[rear++] = T->rchild;
    }
    return OK;
}

