// SORTING //
/*
 * Essentially a hybrid sorting function
 * Hybrid of quick sort, insertion sort & heap sort
 * Best case O(n log n)
 * Worst case O(n^2)
*/
#define INSERTION_SORT_THRESHOLD 16

void insertionSort(int arr[], int left, int right)
{
  for (int i=left+1; i <= right; i++) {
    int key = arr[i];
    int j = i-1;
    while (j >= left && arr[j] > key) {
      arr[j+1] = arr[j];
      j--;
    }
    arr[j+1] = key;
  }
}

void heapify(int arr[], int n, int i)
{
  int largest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  if (left < n && arr[left] > arr[largest]) largest = left;
  if (right < n && arr[right] > arr[largest]) largest = right;

  if (largest != i) {
    int temp = arr[i];
    arr[i] = arr[largest];
    arr[largest] = temp;
    heapify(arr, n, largest);
  }
}

void heapSort(int arr[], int n)
{
  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i);

  for (int i=n-1; i >= 0; i--) {
    int temp = arr[0];
    arr[0] = arr[i];
    arr[i] = temp;

    heapify(arr, i, 0);
  }
}

void quickSort(int arr[], int left, int right, int maxDepth)
{
  if (right - left <= INSERTION_SORT_THRESHOLD) {
    insertionSort(arr, left, right);
    return;
  }

  if (maxDepth == 0) {
    heapSort(arr + left, right - left + 1);
    return;
  }

  int pivot = arr[right];
  int i = left-1;

  for (int j=left; j < right; j++) {
    if (arr[j] <= pivot) {
      i++;
      int temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
    }
  }

  int temp = arr[i+1];
  arr[i+1] = arr[right];
  arr[right] = temp;

  int pivotIndex = i+1;

  quickSort(arr, left, pivotIndex-1, maxDepth-1);
  quickSort(arr, pivotIndex+1, right, maxDepth-1);
}

void sort(int arr[], int n)
{
  quickSort(arr, 0, n-1, 2*(int)log(n));
}
