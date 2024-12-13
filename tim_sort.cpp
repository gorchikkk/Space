#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

// Функция для сортировки вставками
void insertionSort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Функция для слияния двух run с режимом галопа
void mergeWithGalloping(std::vector<int>& arr, int left, int mid, int right) {
    int len1 = mid - left + 1;
    int len2 = right - mid;

    std::vector<int> leftArr(len1);
    std::vector<int> rightArr(len2);

    for (int i = 0; i < len1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < len2; j++)
        rightArr[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    int gallopCounterLeft = 0;  // Счетчик для правого массива
    int gallopCounterRight = 0; // Счетчик для левого массива

    // Основное слияние с режимом галопа для обоих массивов
    while (i < len1 && j < len2) {
        if (leftArr[i] <= rightArr[j]) {
            gallopCounterRight = 0; // Сброс галоп-счётчика правого массива
            gallopCounterLeft++;    // Увеличение галоп-счётчика для левого массива

            if (gallopCounterLeft >= 7) { // Галоп из левого массива
                int gallopCount = 1;
                while (i + gallopCount < len1 && leftArr[i + gallopCount] <= rightArr[j]) {
                    gallopCount *= 2;
                }
                gallopCount /= 2;

                for (int g = 0; g <= gallopCount && i < len1; g++) {
                    arr[k++] = leftArr[i++];
                }
                gallopCounterLeft = 0; // Сброс после галопа
            } else {
                arr[k++] = leftArr[i++]; // Обычное слияние
            }
        } else {
            gallopCounterLeft = 0;  // Сброс галоп-счётчика левого массива
            gallopCounterRight++;   // Увеличение галоп-счётчика для правого массива

            if (gallopCounterRight >= 7) { // Галоп из правого массива
                int gallopCount = 1;
                while (j + gallopCount < len2 && rightArr[j + gallopCount] < leftArr[i]) {
                    gallopCount *= 2;
                }
                gallopCount /= 2;

                for (int g = 0; g <= gallopCount && j < len2; g++) {
                    arr[k++] = rightArr[j++];
                }
                gallopCounterRight = 0; // Сброс после галопа
            } else {
                arr[k++] = rightArr[j++]; // Обычное слияние
            }
        }
    }

    // Слияние оставшихся элементов
    while (i < len1) {
        arr[k++] = leftArr[i++];
    }

    while (j < len2) {
        arr[k++] = rightArr[j++];
    }
}

// minrun
int minRunLength(int n) {
    int r = 0;
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r + 32;
}

// Основная функция Timsort
void timSort(std::vector<int>& arr) {
    int n = arr.size();
    int minRun = minRunLength(n);

    for (int start = 0; start < n; start += minRun) {
        int end = std::min(start + minRun - 1, n - 1);
        insertionSort(arr, start, end);
    }

    for (int size = minRun; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = std::min(left + size - 1, n - 1);
            int right = std::min((left + 2 * size - 1), (n - 1));

            if (mid < right) {
                mergeWithGalloping(arr, left, mid, right);
            }
        }
    }
}

// Функция для генерации случайных чисел
void fillArray(std::vector<int>& arr) {
    for (int& num : arr) {
        num = rand() % 999 + 1;
    }
}

// Функция для отображения массива
void printArray(const std::vector<int>& arr) {
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    int length;

    std::cout << "Enter the length of the array (from 50 to 300): ";
    std::cin >> length;

    if (length < 50 || length > 300) {
        std::cout << "Invalid length! Please enter a value between 50 and 300." << std::endl;
        return 1;
    }

    std::vector<int> arr(length);
    fillArray(arr);

    std::cout << "Original array: ";
    printArray(arr);

    timSort(arr);

    std::cout << "Sorted array: ";
    printArray(arr);

    return 0;
}

