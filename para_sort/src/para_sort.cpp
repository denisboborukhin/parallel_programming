#include <cstddef>
// #include <mpi.h>

#include <iostream>
#include <vector>
#include <random>

std::vector<double> generate_arr(const size_t size)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> random(1.0, 100.0);

    std::vector<double> res(size);
    for (int i = 0; i != size; ++i) {
        res[i] = random(rd);
    }
    
    return res;
}

void merge(std::vector<double>& arr, const size_t left, const size_t mid, const size_t right)
{
    std::vector<double> temp(right - left + 1);
    
    auto k = 0;
    auto i = left, j = mid + 1;
    while (i <= mid && j <= right) {
        auto first = arr[i];
        auto second = arr[j];
        if (first > second) {
            temp[k] = second;
            j++;
        } else {
            temp[k] = first;
            i++;
        }
        k++;
    }
    while (i <= mid) {
        temp[k] = arr[i];
        k++, i++;
    }
    while (j <= right) {
        temp[k] = arr[j];
        k++, j++;
    }
    
    for (auto count = left; count != right + 1; ++count) {
        arr[count] = temp[count - left];
    }
}

void mergeSort(std::vector<double>& arr, size_t left, size_t right)
{
    if (left >= right) {
        return;
    }
    
    size_t mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    
    merge(arr, left, mid, right);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return 0;
    }
    
    int n = atoi(argv[1]);

    int commsize = 0;
    int my_rank = 0;

    auto arr = generate_arr(n);
    mergeSort(arr, 0, n - 1);

    // MPI_Init(&argc, &argv);
    // MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    // MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    //
    // MPI_Finalize();
}