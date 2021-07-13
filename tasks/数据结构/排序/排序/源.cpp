#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef int ElemType;

int Partion(ElemType A[], int low, int high);			// 快排的划分操作
void BuildMaxHeap(ElemType A[], int len);				// 建立大根堆
void HeadAdjust(ElemType A[], int k, int len);			// 将元素k为根的子树进行调整
void Merge(ElemType A[], int low, int mid, int high);	// 二路归并

void InsertSort(ElemType A[], int n);					// 时间 O(n^2)，空间 O(1)
void InsertHalfSort(ElemType A[], int n);				// 时间 n^2，空间1
void QuickSort(ElemType A[], int low, int high);		// 时间 O(n*log2(n))，空间 O(log2(n))
void HeapSort(ElemType A[], int len);					// 时间 O(n*log2(n))，空间 O(1)
void MergeSort(ElemType A[], int low, int high);		// 时间 O(n*log2(n))，空间 O(n)

ElemType *B = (ElemType *)malloc(16 * sizeof(ElemType));// 归并排序辅助数组

int main() {
	int numArr[16] = { 49, 38, 65, 97, 76, 13, 27, 49};

	// InsertSort(numArr, 8);
	// InsertHalfSort(numArr, 8);
	// HeapSort(numArr, 7);
	// QuickSort(numArr, 0, 7);
	MergeSort(numArr, 0, 7);

	for (int i = 0; i < 8; i++) {
		printf("%d ", numArr[i]);
	}
}

void InsertSort(ElemType A[], int n) {
	int i, j, temp;
	for (i = 1; i < n; i++) {
		if (A[i] < A[i - 1]) {
			temp = A[i];
			for (j = i - 1; temp < A[j]; j--) {
				// 边比较边移动元素
				A[j + 1] = A[j];
			}
			A[j + 1] = temp;
		}
	}
}

void InsertHalfSort(ElemType A[], int n) {
	int i, j, temp, low, high, mid;
	for (i = 1; i < n; i++) {
		if (A[i] < A[i - 1]) {
			temp = A[i];
			low = 0; high = i - 1;
			// 比较，查出要插入的位置
			while (low <= high) {
				mid = (low + high) / 2;
				if (A[mid] > temp) {
					high = mid - 1;
				}
				else {
					low = mid + 1;
				}
			}
			// 移动元素
			for (j = i - 1; j > high; j--) {
				A[j + 1] = A[j];
			}
			A[j + 1] = temp;
		}
	}
}

void BuildMaxHeap(ElemType A[], int len) {
	for (int i = len / 2; i >= 0; i--) {
		// 从下往上建立大根堆
		HeadAdjust(A, i, len);
	}
}

void HeadAdjust(ElemType A[], int k, int len) {
	/*
		该函数执行一次后，只有局部满足大根堆的特性
		所以需要用BuildMaxHeap函数执行for循环，从而得到大根堆
	*/
	int temp = A[k]; // 暂存子树的根结点
	for (int i = k * 2; i <= len; i *= 2) {
		if (i < len && A[i] < A[i + 1]) {
			i++;
		}
		if (temp >= A[i]) {
			// 此时左右孩子都小于根结点，无需调整
			break;
		}
		else {
			A[k] = A[i];
			k = i;
		}
	}
	A[k] = temp;
}

void HeapSort(ElemType A[], int len) {
	// 初始化大根堆，将根和最后一个元素交换，对大根堆进行调整
	BuildMaxHeap(A, len);
	for (int i = len; i > 0; i--) {
		int temp = A[0];
		A[0] = A[i];
		A[i] = temp;
		HeadAdjust(A, 0, i - 1);
	}
}

int Partion(ElemType A[], int low, int high) {
	ElemType pivot = A[low];
	while (low < high) {
		while (low < high && A[high] >= pivot) {
			high--;
		}
		A[low] = A[high];
		while (low < high && A[low] <= pivot) {
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

void Merge(ElemType A[], int low, int mid, int high) {
	int i, j, k;
	for (k = low; k <= high; k++) {
		B[k] = A[k];
	}
	for (i = low, j = mid + 1, k = i; i <= mid && j <= high; k++) {
		if (B[i] <= B[j]) {
			A[k] = B[i++];
		}
		else {
			A[k] = B[j++];
		}
	}
	while (i <= mid) {
		A[k++] = B[i++]; // 若第一个表未检测完，复制
	}
	while (j <= high) {
		A[k++] = B[j++]; // 若第二个表未检测完，复制
	}
}

void MergeSort(ElemType A[], int low, int high) {
	if (low < high) {
		int mid = (low + high) / 2;
		MergeSort(A, low, mid);
		MergeSort(A, mid + 1, high);
		Merge(A, low, mid, high);
	}
}