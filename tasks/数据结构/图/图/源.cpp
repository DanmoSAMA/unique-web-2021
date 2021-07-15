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
status DFSRecursive(ALGraph& G, int i, void (*visit)(VertexType));
status DFSNonRecursive(ALGraph& G, void (*visit)(VertexType));
status BFSTraverse(ALGraph& G, void (*visit)(VertexType));
void GetIndegree(ALGraph G);
status TopoSort(ALGraph& G, void (*visit)(VertexType));

ALGraph G;
VertexType V[30];
VertexType value;
KeyType VR[100][2];
KeyType u, v;

int visited1[32];
int visited2[32];
int indegree[32];

int main() {
    int op = 1, i = 0, check = 0;
    while (op) {
        system("cls");	printf("\n\n");
        printf("      	      基于邻接表实现的有向图--菜单 \n");
        printf("-------------------------------------------------\n");
        printf("    	  0. Exit\n");
        printf("    	  1. CreateCraph     2. BFSTraverse\n");
        printf("    	  3. DFSRecursive    4. DFSNonRecursive\n");
        printf("    	  5. TopoSort\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0~5]:");
        scanf("%d", &op);
        switch (op) {
        case 0:
            break;
        case 1:
            printf("请输入要创建的有向图：");
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
                printf("该有向图为空，遍历失败！");
                getchar(); getchar();
                break;
            }
            printf("该有向图的广度优先遍历结果为：\n");
            BFSTraverse(G, visit);
            getchar(); getchar();
            break;
        case 3:
            if (G.vexnum == 0) {
                printf("该有向图为空，遍历失败！");
                getchar(); getchar();
                break;
            }
            printf("该有向图的深度优先遍历结果为：\n");
            DFSRecursive(G, 0, visit);
            getchar(); getchar();
            break;
        case 4:
            if (G.vexnum == 0) {
                printf("该有向图为空，遍历失败！");
                getchar(); getchar();
                break;
            }
            printf("该有向图的深度优先遍历结果为：\n");
            DFSNonRecursive(G, visit);
            getchar(); getchar();
            break;
        case 5:
            if (G.vexnum == 0) {
                printf("该有向图为空，拓扑排序失败！");
                getchar(); getchar();
                break;
            }
            if (check = TopoSort(G, visit)) printf("\n该有向图的拓扑排序如上所示");
            else printf("拓扑排序失败！");
            getchar(); getchar();
            break;
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
        // for (int i = 0; i < G.vexnum; ++i)
        //     if (G.vertices[i].data.key == v->key)   // 查看关键字是否重复
        //         return ERROR;

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

        // for (ArcNode* e = G.vertices[firstNode].firstarc; e; e = e->nextarc)
        //     if (e->adjvex == secondNode) return ERROR; // 存在编号相同的结点

        ArcNode* firstEdge = (ArcNode*)malloc(sizeof(ArcNode)); // 创建表结点并连接在头结点之后
        firstEdge->adjvex = secondNode;
        firstEdge->nextarc = G.vertices[firstNode].firstarc;    // 头插法
        G.vertices[firstNode].firstarc = firstEdge;

        // ArcNode* secondEdge = (ArcNode*)malloc(sizeof(ArcNode));
        // secondEdge->adjvex = firstNode; 
        // secondEdge->nextarc = G.vertices[secondNode].firstarc;
        // G.vertices[secondNode].firstarc = secondEdge;

        G.arcnum++;
    }

    return G.vexnum > 0 ? OK : ERROR;
}

void visit(VertexType data) {
    printf("%d %s\n", data.key, data.others);
}

status DFSRecursive(ALGraph& G, int i, void (*visit)(VertexType))
// 深度优先搜索遍历（递归）
{
    ArcNode* p;
    visited1[i] = 1;
    VNode v = G.vertices[i];
    visit(v.data);
    p = v.firstarc;
    while (p) {
        if (!visited1[p->adjvex]) {
            DFSRecursive(G, p->adjvex, visit);
        }
        p = p->nextarc;
    }
    return OK;
}

status DFSNonRecursive(ALGraph& G, void (*visit)(VertexType))
// 深度优先搜索遍历（非递归）
{
    int stack[32] = { 0 };
    int top = 0;

    for (int i = 0; i < G.vexnum; i++) {
        if (!visited2[i]) {
            stack[top++] = i;
            visited2[i] = 1;
        }
        while (top) {
            int iv = stack[--top];
            VNode v = G.vertices[iv];
            visit(v.data);

            int begin = top;    // 从当前结点出发，访问的第一个结点序号
            for (ArcNode* e = v.firstarc; e; e = e->nextarc) {
                if (!visited2[e->adjvex]) {
                    stack[top++] = e->adjvex;
                    visited2[e->adjvex] = 1;
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

void GetIndegree(ALGraph G) {
    // 存每个结点的入度
    for (int i = 0; i < G.vexnum; i++) {
        for (int j = 0; j < G.vexnum; j++) {
            VNode v = G.vertices[j];
            for (ArcNode* e = v.firstarc; e; e = e->nextarc)
                if(e->adjvex == i)
                    indegree[i]++;
        }
    }
}

status TopoSort(ALGraph& G, void (*visit)(VertexType)) {
    GetIndegree(G);
    int stack[32] = { 0 };
    int top = 0, count = 0, i;
    for (i = 0; i < G.vexnum; i++)
        if (!indegree[i]) stack[top++] = i; // 将入度为0的顶点入栈
    while (top) {
        i = stack[--top];
        count++;    // 当前已输出的顶点数
        VNode v = G.vertices[i];
        visit(v.data);
        for (auto p = G.vertices[i].firstarc; p; p = p->nextarc) {
            // 将所有i指向的顶点的入度减1，并将入度减为0的顶点入栈
            int v = p->adjvex;
            if (!(--indegree[v])) stack[top++] = v;
        }
    }
    if (count < G.vexnum) return ERROR;
    return OK;
}
