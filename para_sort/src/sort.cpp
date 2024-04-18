#include <chrono>
#include <cstddef>

#include <iomanip>   
#include <iostream>
#include <random>

double* generate_arr(const size_t size)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> random(1.0, 100.0);

    double* res = new double[size];
    for (int i = 0; i != size; ++i) {
        res[i] = random(rd);
    }
    
    return res;
}

double* generate_reverse_arr(const size_t size)
{
    double* res = new double[size];
    for (int i = 0; i != size; ++i) {
        res[i] = size - i;
    }
    
    return res;
}

double* generate_lin_arr(const size_t size)
{
    double* res = new double[size];
    for (int i = 0; i != size; ++i) {
        res[i] = i;
    }
    
    return res;
}

void merge(double* arr, const int low, const int mid, const int high)
{
    int i = low, j = mid + 1, k = 0;
    double *temp = new double[high - low + 1];
    
    while (i <= mid && j <= high) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            k++, i++;
        } else {
            temp[k] = arr[j];
            k++, j++;
        }
    }
    
    while (i <= mid) {
        temp[k] = arr[i];
        i++, k++;
    }
    
    while (j <= high) {
        temp[k] = arr[j];
        j++, k++;
    }
    
    for (i = low, k = 0; i <= high; i++, k++) {
        arr[i] = temp[k];
    }
    
    delete[] temp;
}

void mergeSort(double* arr, int low, int high)
{
    if (low < high) {     
        int mid = low + (high - low) / 2;
        mergeSort(arr, low, mid); 
        mergeSort(arr, mid + 1, high);
        merge(arr, low, mid, high); 
    }
}

std::chrono::high_resolution_clock::duration sort(double* arr, const size_t size, const std::string& message)
{
    auto begin = std::chrono::high_resolution_clock::now();
    mergeSort(arr, 0, size - 1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - begin;
    std::cout << message << " time = " << std::setw(10) << duration.count() << " mcs\n";
    return duration;
}

void printArr(double* arr, const size_t size)
{
    for (auto i = 0; i != size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return 0;
    }
    
    int n = atoi(argv[1]);

    auto begin = std::chrono::high_resolution_clock::now();

    double* array = generate_arr(n);
    // array = generate_lin_arr(n);
    // array = generate_reverse_arr(n);

    sort(array, n, "non parallel");
}