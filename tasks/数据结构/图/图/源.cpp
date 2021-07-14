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
} VertexType;                       //顶点类型定义

typedef struct ArcNode {            //表结点类型定义
    int adjvex;                     //顶点位置编号 
    struct ArcNode* nextarc;	    //下一个表结点指针
} ArcNode;

typedef struct VNode {				//头结点及其数组类型定义
    VertexType data;       	        //顶点信息
    ArcNode* firstarc;      	    //指向第一条弧
} VNode, AdjList[MAX_VERTEX_NUM];

typedef  struct {                   //邻接表的类型定义
    AdjList vertices;     	        //头结点数组
    int vexnum, arcnum;   	        //顶点数、弧数
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
        printf("      	      基于邻接表实现的无向图--菜单 \n");
        printf("-------------------------------------------------\n");
        printf("    	  0. Exit\n");
        printf("    	  1. CreateCraph     2. DFSTraverse\n");
        printf("    	  3. BFSTraverse     4. TopoSort\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0~4]:");
        scanf("%d", &op);
        switch (op) {
        case 0:
            break;
        case 1:
            printf("请输入要创建的无向图：");
            do {
                scanf("%d%s", &V[i].key, V[i].others);  // 结点信息
            } while (V[i++].key != -1);
            i = 0;
            do {
                scanf("%d%d", &VR[i][0], &VR[i][1]);    // 弧信息
            } while (VR[i++][0] != -1);

            check = CreateCraph(G, V, VR);

            if (check == ERROR) printf("输入数据错误，无法创建!");
            else {
                printf("创建成功！");
            }
            getchar(); getchar();
            break;
        case 2:
            if (G.vexnum == 0) {
                printf("该无向图为空，遍历失败！");
                getchar(); getchar();
                break;
            }
            printf("该无向图的深度优先遍历结果为：\n");
            DFSTraverse(G, visit);
            getchar(); getchar();
            break;
        case 3:
            if (G.vexnum == 0) {
                printf("该无向图为空，遍历失败！");
                getchar(); getchar();
                break;
            }
            printf("该无向图的广度优先遍历结果为：\n");
            BFSTraverse(G, visit);
            getchar(); getchar();
            break;
        case 4:
        default:
            printf("指令错误！");
            getchar(); getchar();
        }
    }
    printf("欢迎下次再使用本系统！\n");
    return 0;
}

status CreateCraph(ALGraph& G, VertexType V[], KeyType VR[][2])
{
    G.vexnum = 0;
    G.arcnum = 0;

    for (auto v = V; v->key != -1; ++v) {
        if (G.vexnum >= MAX_VERTEX_NUM)             // 图的顶点数超过最大限度
            return ERROR;
        for (int i = 0; i < G.vexnum; ++i)
            if (G.vertices[i].data.key == v->key)   // 查看关键字是否重复
                return ERROR;

        G.vertices[G.vexnum].data = *v;
        G.vertices[G.vexnum].firstarc = NULL;
        G.vexnum++;
    }

    for (auto vr = VR; (*vr)[0] != -1; ++vr) {
        int firstNode = 0, secondNode = 0;             // 得到两个结点的编号
        while ((firstNode != G.vexnum) && (G.vertices[firstNode].data.key != (*vr)[0])) firstNode++;
        while ((secondNode != G.vexnum) && (G.vertices[secondNode].data.key != (*vr)[1])) secondNode++;

        if (firstNode == G.vexnum || secondNode == G.vexnum)
            return ERROR;

        for (ArcNode* e = G.vertices[firstNode].firstarc; e; e = e->nextarc)
            if (e->adjvex == secondNode) return ERROR; // 存在编号相同的结点

        ArcNode* firstEdge = (ArcNode*)malloc(sizeof(ArcNode)); // 创建表结点并连接在头结点之后
        firstEdge->adjvex = secondNode;
        firstEdge->nextarc = G.vertices[firstNode].firstarc;    // 头插法
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
// 深度优先搜索遍历（非递归）
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

            int begin = top;    // 从当前结点出发，访问的第一个结点序号
            for (ArcNode* e = v.firstarc; e; e = e->nextarc) {
                if (!visited[e->adjvex]) {
                    stack[top++] = e->adjvex;
                    visited[e->adjvex] = 1;
                }
            }
            int end = top - 1;  // 从当前结点出发，访问的最后一个结点序号
            while (begin < end) {
                // 栈顶应是最后试图访问的结点，因此调整栈中顺序
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
// 广度优先搜索遍历
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