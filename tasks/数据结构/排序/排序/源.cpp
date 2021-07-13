#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef int ElemType;

int Partion(ElemType A[], int low, int high);			// ���ŵĻ��ֲ���
void BuildMaxHeap(ElemType A[], int len);				// ���������
void HeadAdjust(ElemType A[], int k, int len);			// ��Ԫ��kΪ�����������е���
void Merge(ElemType A[], int low, int mid, int high);	// ��·�鲢

void InsertSort(ElemType A[], int n);					// ʱ�� O(n^2)���ռ� O(1)
void InsertHalfSort(ElemType A[], int n);				// ʱ�� n^2���ռ�1
void QuickSort(ElemType A[], int low, int high);		// ʱ�� O(n*log2(n))���ռ� O(log2(n))
void HeapSort(ElemType A[], int len);					// ʱ�� O(n*log2(n))���ռ� O(1)
void MergeSort(ElemType A[], int low, int high);		// ʱ�� O(n*log2(n))���ռ� O(n)

ElemType *B = (ElemType *)malloc(16 * sizeof(ElemType));// �鲢����������

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
				// �߱Ƚϱ��ƶ�Ԫ��
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
			// �Ƚϣ����Ҫ�����λ��
			while (low <= high) {
				mid = (low + high) / 2;
				if (A[mid] > temp) {
					high = mid - 1;
				}
				else {
					low = mid + 1;
				}
			}
			// �ƶ�Ԫ��
			for (j = i - 1; j > high; j--) {
				A[j + 1] = A[j];
			}
			A[j + 1] = temp;
		}
	}
}

void BuildMaxHeap(ElemType A[], int len) {
	for (int i = len / 2; i >= 0; i--) {
		// �������Ͻ��������
		HeadAdjust(A, i, len);
	}
}

void HeadAdjust(ElemType A[], int k, int len) {
	/*
		�ú���ִ��һ�κ�ֻ�оֲ��������ѵ�����
		������Ҫ��BuildMaxHeap����ִ��forѭ�����Ӷ��õ������
	*/
	int temp = A[k]; // �ݴ������ĸ����
	for (int i = k * 2; i <= len; i *= 2) {
		if (i < len && A[i] < A[i + 1]) {
			i++;
		}
		if (temp >= A[i]) {
			// ��ʱ���Һ��Ӷ�С�ڸ���㣬�������
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
	// ��ʼ������ѣ����������һ��Ԫ�ؽ������Դ���ѽ��е���
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
		A[k++] = B[i++]; // ����һ����δ����꣬����
	}
	while (j <= high) {
		A[k++] = B[j++]; // ���ڶ�����δ����꣬����
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