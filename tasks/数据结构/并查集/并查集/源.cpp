#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct edge {
	int u;
	int v;
	int w;
}edge;	// 存储边的关系

typedef edge ElemType;

int Partion(ElemType A[], int low, int high);
void QuickSort(ElemType A[], int low, int high);
void Init();
int Getf(int v);
int Merge(int v, int u);

edge e[32];
int f[32], n, m, sum, count;

int main() {
	int i;
	printf("请输入图的顶点数、边数：\n");
	scanf("%d %d", &n, &m);	// 顶点数和边数
	printf("请输入每条边的顶点1、顶点2、权值：\n");
	for (i = 0; i < m; i++) scanf("%d %d %d", &e[i].u, &e[i].v, &e[i].w);

	Init();

	QuickSort(e, 0, m - 1);

	for (i = 0; i < m; i++) {
		if (Merge(e[i].u, e[i].v)) {
			count++;
			sum += e[i].w;
		}
		if (count == n - 1) break;
	}
	printf("\n最小生成树的权值之和为：%d", sum);

	getchar(); getchar();
	return 0;
}


int Partion(ElemType A[], int low, int high) {
	ElemType pivot = A[low];
	while (low < high) {
		while (low < high && A[high].w >= pivot.w) {
			high--;
		}
		A[low] = A[high];
		while (low < high && A[low].w <= pivot.w) {
			low++;
		}
		A[high] = A[low];
	}
	A[low] = pivot;
	return low;
}

void QuickSort(ElemType A[], int low, int high) {
	if (low < high) {
		int pivotpos = Partion(A, low, high);
		QuickSort(A, low, pivotpos - 1);
		QuickSort(A, pivotpos + 1, high);
	}
}

void Init() 
//并查集初始化
{
	int i;
	for (i = 0; i < n; i++)
		f[i] = i;
	return;
}

int Getf(int v) 
//找首领
{	
	if (f[v] == v)
		return v;
	else {
		// 路径压缩
		f[v] = Getf(f[v]);
		return f[v];
	}
}

int Merge(int v, int u) 
//合并两子集
{
	int t1, t2;
	t1 = Getf(v);
	t2 = Getf(u);
	if (t1 != t2) {
		f[t2] = t1;
		return 1;
	}
	return 0;
}

/*
输入测试：
6 9
2 4 11
3 5 13
4 6 3
5 6 4
2 3 6
4 5 7
1 2 1
3 4 9
1 3 2 

sum == 19
*/