/* 
 * sorting algorithms
 */

#include <stdio.h>

/* 
 * helper functions
 */

int min(int a, int b)
{
	if (a <= b)
		return a;
	else
		return b;
}

void swap(int a[], int i, int j)
{
	int temp;
	temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}

int binary(int a[], int low, int high, int value)
{
	while (low <= high) {
		int mid = (low+high+(low+high)%2) / 2;
		if (value < a[mid])
			return binary(a, low, mid-1, value);
		else if (value > a[mid])
			return binary(a, mid+1, high, value);
		else
			return mid;
	}
	return low;

}

void print_array(int array[], int length)
{
	int i;

	for (i = 0; i < length; i++)
		printf("%d ", array[i]);
	printf("\n");
}

/*
 * select sort 
 * time : O(n^2) 
 * space : O(1)
 */
void select(int a[], int length)
{
	int i, j, min, min_index;

	for (i = 0; i < length; i++) {
		min = a[i];
		min_index = i;
		/* find minimum element */
		for (j = i + 1; j < length; j++) {
			if (a[j] < min) {
				min = a[j];
				min_index = j; 
			}
		}
		if (min < a[i]) /* swap */
			swap(a, i, min_index);
	}
}

/* 
 * insert sort using linear search 
 * time : O(n^2)
 * space : O(1)
 */
void insert_ls(int a[], int length)
{
	int i, j;

	for (i = 0; i < length; i++) {
		for (j = i; j > 0; j--) {
			if (a[j] < a[j-1])
				swap(a, j, j-1);
		}
	}	
}

/* 
 * insert sort using binary search 
 * time : O(n^2)
 * space : O(1)
 */
void insert_b(int a[], int length)
{
	int i, j, temp;
	int insert_index;

	for (i = 0; i < length; i++) {
		insert_index = binary(a, 0, i, a[i]);
		temp = a[i];
		/* right shift */
		for (j = i; j > insert_index; j--)
			a[j] = a[j-1];
		/* insert */
		a[insert_index] = temp;
	}	
}

/* 
 * bublle sort, every round get smallest value 
 * time : O(n^2)
 * space : O(1)
 */
void bubble_s(int a[], int length) 
{
	int i, j;
	int flag = 0;

	for (i = 0; i < length; i++) {
		for (j = length-1; j > i; j--) {
			if (a[j-1] > a[j]) {
				swap(a, j-1, j);
				flag = 1;
			}
		}
		if (flag == 0)
			return;
	}
}

/* 
 * bublle sort, every round get largest value 
 * time : O(n^2)
 * space : O(1)
 */
void bubble_b(int a[], int length)
{
	int i, j;
	int flag = 0;

	for (i = length; i >= 0; i--) {
		for (j = 0; j <= i; j++) {
			if (a[j-1] > a[j]) {
				swap(a, j-1, j);
				flag = 1;
			}
		}
		if (flag == 0)
			return;
	}
}

/* 
 * merge sort 
 * time : O(nlogn)
 * space : O(n)
 */
void merge(int a[], int b[], int result[], int len1, int len2)
{
	int i = 0, j = 0, k = 0;

	while (i != len1 && j != len2) {
		if (a[i] <= b[j])
			result[k++] = a[i++];
		else
			result[k++] = b[j++];
	}

	while (j != len2)
		result[k++] = b[j++];
	while (i != len1)
		result[k++] = a[i++];
}

void merge_sort(int a[], int length)
{
	int i, j;

	if (length == 1)
		return;

	/* split into 2 parts */
	int len1 = length/2;
	int len2 = length - length/2;
	int part1[len1];
	int part2[len2];
	for (i = 0; i < len1; i++) 
		part1[i] = a[i];
	for (j = len1; j < length; j++)
		part2[j - len1] = a[j];	

	merge_sort(part1, len1);
	merge_sort(part2, len2);
	merge(part1, part2, a, len1, len2);
}

/* 
 * quick sort 
 * time : average - O(n*logn), worst - O(n^2)
 * space : stack depth, average - O(logn), worst - O(n)
 */
int partition(int a[], int low, int high)
{
	int pivot = a[low];	/* choose first element as partition value */

	while (low < high) {
		while (low < high && a[high] >= pivot)
			high--;
		a[low] = a[high];
		while (low < high && a[low] <= pivot) 
			low++;
		a[high] = a[low];
	}
	a[low] = pivot;
	return low;	/* return index of partition value */
}

void quick_sort(int a[], int low, int high)
{
	if (low < high) {
		int pivot_index = partition(a, low, high);
		quick_sort(a, low, pivot_index-1);
		quick_sort(a, pivot_index+1, high);
	}
}

/*
 * shell sort
 * time : O(n^2)
 * space : O(1)
 */
void shell(int a[], int length)
{
	int step, i, j;

	for (step = length/2; step >= 1; step = step/2) {
		for (i = step; i < length; i++) {
			for (j = i-step; j >= 0 && a[j] > a[j+step]; j -= step) {
				swap(a, j, j+step);
			}
			print_array(a, 10);
		}
	}
}

/*
 * max heap sort : careful, the first element is not useful (pseudo node)
 * time : O(n*logn)
 * space : O(1)
 */
void adjust_down_max(int a[], int k, int length)
{
	int i = 2*k;

	while (i < length) {
		// printf("i is %d\n", i);
		if (i < length-1 && a[i] < a[i+1])
			i++;	/* get the bigger child */
		if (a[k] >= a[i])
			break;
		else {
			swap(a, i, k);
			// printf("before exchange, k is %d\n", k);
			k = i;
			i = 2*k;
			// printf("now, k is %d\n", k);
		}
	}
}

void build_max_heap(int a[], int length)
{
	int i;

	for (i = (length-1)/2; i >= 1; i--) {
		adjust_down_max(a, i, length);
		// print_array(a, length);
	}
}

void heap_sort_max(int a[], int length)
{
	int i;

	build_max_heap(a, length);
	printf("after build max heap : \n");
	print_array(a, length);
	for (i = length-1; i > 1; i--) {
		// printf("%d\n", a[1]);
		swap(a, i, 1);
		adjust_down_max(a, 1, i);
		// printf("adjust length %d\n", i);
		// print_array(a, i);
	}
}

/*
 * min heap sort : careful, the first element is not useful (pseudo node)
 * time : O(n*logn)
 * space : O(1)
 */
void adjust_down_min(int a[], int k, int length)
{
	int i = 2*k;

	while (i < length) {
		// printf("i is %d\n", i);
		if (i < length-1 && a[i] > a[i+1])
			i++;	/* get the smaller child */
		if (a[k] <= a[i])
			break;
		else {
			swap(a, i, k);
			// printf("before exchange, k is %d\n", k);
			k = i;
			i = 2*k;
			// printf("now, k is %d\n", k);
		}
	}
}

void build_min_heap(int a[], int length)
{
	int i;

	for (i = (length-1)/2; i >= 1; i--) {
		adjust_down_min(a, i, length);
		// print_array(a, length);
	}
}

void heap_sort_min(int a[], int length)
{
	int i;

	build_min_heap(a, length);
	printf("after build min heap : \n");
	print_array(a, length);
	for (i = length-1; i > 1; i--) {
		// printf("%d\n", a[1]);
		swap(a, i, 1);
		adjust_down_min(a, 1, i);
		// printf("adjust length %d\n", i);
		// print_array(a, i);
	}
}
int main(int argc, char *argv[]) {
	int array[10] = {2, 4, 3, 1, 6, 3, 7, 10, 0, 5};

	printf("Original array : \n");
	print_array(array, 10);

	/* sort */
	// quick_sort(array, 0, 9);
	heap_sort_max(array, 10);

	printf("After sorting : \n");
	print_array(array, 10);

	return 0;
}