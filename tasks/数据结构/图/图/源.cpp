#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAX_VERTEX_NUM 20

typedef int status;
typedef int KeyType;
typedef struct {
    KeyType  key;
    char others[20];
} VertexType;                       //�������Ͷ���

typedef struct ArcNode {            //�������Ͷ���
    int adjvex;                     //����λ�ñ�� 
    struct ArcNode* nextarc;	    //��һ������ָ��
} ArcNode;

typedef struct VNode {				//ͷ��㼰���������Ͷ���
    VertexType data;       	        //������Ϣ
    ArcNode* firstarc;      	    //ָ���һ����
} VNode, AdjList[MAX_VERTEX_NUM];

typedef  struct {                   //�ڽӱ�����Ͷ���
    AdjList vertices;     	        //ͷ�������
    int vexnum, arcnum;   	        //������������
} ALGraph;

void visit(VertexType data);
status CreateCraph(ALGraph& G, VertexType V[], KeyType VR[][2]);
status DFSTraverse(ALGraph& G, void (*visit)(VertexType));
status BFSTraverse(ALGraph& G, void (*visit)(VertexType));
// status TopoSort();

ALGraph G;
VertexType V[30];
VertexType value;
KeyType VR[100][2];
KeyType u, v;

int main() {
    int op = 1, i = 0, check = 0;
    while (op) {
        system("cls");	printf("\n\n");
        printf("      	      �����ڽӱ�ʵ�ֵ�����ͼ--�˵� \n");
        printf("-------------------------------------------------\n");
        printf("    	  0. Exit\n");
        printf("    	  1. CreateCraph     2. DFSTraverse\n");
        printf("    	  3. BFSTraverse     4. TopoSort\n");
        printf("-------------------------------------------------\n");
        printf("    ��ѡ����Ĳ���[0~4]:");
        scanf("%d", &op);
        switch (op) {
        case 0:
            break;
        case 1:
            printf("������Ҫ����������ͼ��");
            do {
                scanf("%d%s", &V[i].key, V[i].others);  // �����Ϣ
            } while (V[i++].key != -1);
            i = 0;
            do {
                scanf("%d%d", &VR[i][0], &VR[i][1]);    // ����Ϣ
            } while (VR[i++][0] != -1);

            check = CreateCraph(G, V, VR);

            if (check == ERROR) printf("�������ݴ����޷�����!");
            else {
                printf("�����ɹ���");
            }
            getchar(); getchar();
            break;
        case 2:
            if (G.vexnum == 0) {
                printf("������ͼΪ�գ�����ʧ�ܣ�");
                getchar(); getchar();
                break;
            }
            printf("������ͼ��������ȱ������Ϊ��\n");
            DFSTraverse(G, visit);
            getchar(); getchar();
            break;
        case 3:
            if (G.vexnum == 0) {
                printf("������ͼΪ�գ�����ʧ�ܣ�");
                getchar(); getchar();
                break;
            }
            printf("������ͼ�Ĺ�����ȱ������Ϊ��\n");
            BFSTraverse(G, visit);
            getchar(); getchar();
            break;
        case 4:
        default:
            printf("ָ�����");
            getchar(); getchar();
        }
    }
    printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
    return 0;
}

status CreateCraph(ALGraph& G, VertexType V[], KeyType VR[][2])
{
    G.vexnum = 0;
    G.arcnum = 0;

    for (auto v = V; v->key != -1; ++v) {
        if (G.vexnum >= MAX_VERTEX_NUM)             // ͼ�Ķ�������������޶�
            return ERROR;
        for (int i = 0; i < G.vexnum; ++i)
            if (G.vertices[i].data.key == v->key)   // �鿴�ؼ����Ƿ��ظ�
                return ERROR;

        G.vertices[G.vexnum].data = *v;
        G.vertices[G.vexnum].firstarc = NULL;
        G.vexnum++;
    }

    for (auto vr = VR; (*vr)[0] != -1; ++vr) {
        int firstNode = 0, secondNode = 0;             // �õ��������ı��
        while ((firstNode != G.vexnum) && (G.vertices[firstNode].data.key != (*vr)[0])) firstNode++;
        while ((secondNode != G.vexnum) && (G.vertices[secondNode].data.key != (*vr)[1])) secondNode++;

        if (firstNode == G.vexnum || secondNode == G.vexnum)
            return ERROR;

        for (ArcNode* e = G.vertices[firstNode].firstarc; e; e = e->nextarc)
            if (e->adjvex == secondNode) return ERROR; // ���ڱ����ͬ�Ľ��

        ArcNode* firstEdge = (ArcNode*)malloc(sizeof(ArcNode)); // �������㲢������ͷ���֮��
        firstEdge->adjvex = secondNode;
        firstEdge->nextarc = G.vertices[firstNode].firstarc;    // ͷ�巨
        G.vertices[firstNode].firstarc = firstEdge;

        ArcNode* secondEdge = (ArcNode*)malloc(sizeof(ArcNode));
        secondEdge->adjvex = firstNode; 
        secondEdge->nextarc = G.vertices[secondNode].firstarc;
        G.vertices[secondNode].firstarc = secondEdge;

        G.arcnum++;
    }

    return G.vexnum > 0 ? OK : ERROR;
}

void visit(VertexType data) {
    printf("%d %s\n", data.key, data.others);
}

status DFSTraverse(ALGraph& G, void (*visit)(VertexType))
// ������������������ǵݹ飩
{
    int visited[32] = { 0 };
    int stack[32] = { 0 };
    int top = 0;

    for (int i = 0; i < G.vexnum; i++) {
        if (!visited[i]) {
            stack[top++] = i;
            visited[i] = 1;
        }
        while (top) {
            int iv = stack[--top];
            VNode v = G.vertices[iv];
            visit(v.data);

            int begin = top;    // �ӵ�ǰ�����������ʵĵ�һ��������
            for (ArcNode* e = v.firstarc; e; e = e->nextarc) {
                if (!visited[e->adjvex]) {
                    stack[top++] = e->adjvex;
                    visited[e->adjvex] = 1;
                }
            }
            int end = top - 1;  // �ӵ�ǰ�����������ʵ����һ��������
            while (begin < end) {
                // ջ��Ӧ�������ͼ���ʵĽ�㣬��˵���ջ��˳��
                int tmp = stack[begin];
                stack[begin] = stack[end];
                stack[end] = tmp;
                begin++;
                end--;
            }
        }
    }
    return OK;
}

status BFSTraverse(ALGraph& G, void (*visit)(VertexType))
// ���������������
{
    int visited[32] = { 0 }, queue[32] = { 0 }, head = 0, rear = 0;

    for (int i = 0; i < G.vexnum; ++i) {
        if (!visited[i]) {
            visited[i] = 1;
            queue[head++] = i;
        }

        while (rear < head) {
            int iv = queue[rear++];
            VNode v = G.vertices[iv];
            visit(v.data);

            for (ArcNode* e = v.firstarc; e; e = e->nextarc) {
                if (!visited[e->adjvex]) {
                    visited[e->adjvex] = 1;
                    queue[head++] = e->adjvex;
                }
            }
        }
    }
    return OK;
}