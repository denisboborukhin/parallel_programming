#include <chrono>
#include <cstddef>
#include <mpi.h>

#include <iomanip>   
#include <thread>

#include <iostream>
#include <vector>
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
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    
    while (j <= high) {
        temp[k++] = arr[j++];
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

    int commsize = 0;
    int my_rank = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    auto rand_arr = generate_arr(n);
    // auto lin_arr = generate_lin_arr(n);
    // auto reverse_arr = generate_reverse_arr(n);
    
    auto original_array = rand_arr;
    if (my_rank == 0) {
        std::cout << "orig: ";
        printArr(original_array, n);
    }

	int size = n / commsize;
	
    double *sub_array= new double[size];
	MPI_Scatter(original_array, size, MPI_DOUBLE, sub_array, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
    printArr(sub_array, size);
	mergeSort(sub_array, 0, size - 1);
    printArr(sub_array, size);
	
    double *res = nullptr;
	if (my_rank == 0) {
        double *res = new double[n];
    }
	
	MPI_Gather(sub_array, size, MPI_DOUBLE, res, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	if (my_rank == 0) {
        std::cout << "result: \n";
		for (auto c = 0; c < n; c++) {
            std::cout << res[c] << " ";
        }
        std::cout << "\n";
        delete[] res;
    }

    delete[] original_array;
    delete[] sub_array;

    MPI_Finalize();
}