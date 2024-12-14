#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#define ISIZE 1000
#define JSIZE 1000

std::vector<std::vector<double>> getArrOMP(const size_t size1,
                                           const size_t size2) {
    std::vector<std::vector<double>> result{};
    result.resize(size1);
    for (auto &&row : result) {
        row.resize(size2);
    }

    return result;
}

double **getArrMPI(const int size1, const int size2) {
    double **res = new double *[size1];

    for (auto i = 0; i < size1; ++i) {
        res[i] = new double[size2];
    }
    return res;
}

void deleteArrMPI(double **arr, const int size1) {
    for (auto i = 0; i < size1; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
}

template <typename Container>
void saveToFile(const std::string &appName, const size_t nThreads,
                const Container &data, const int x, const int y) {
    auto filename = appName + "_" + std::to_string(nThreads) + ".dump";
    auto repoPath = std::filesystem::path{__FILE__}.parent_path();
    auto filepath = repoPath / "results" / filename;

    auto ostream = std::ofstream{filepath};
    for (auto i = 0; i != x; ++i) {
        for (auto j = 0; j != y; ++j) {
            ostream << std::fixed << data[i][j] << " ";
        }
        ostream << "\n";
    }

    std::cout << "Saved to: " << filepath << std::endl;
}

void printInfo(uint32_t threads, double time_sec) {
    std::cout << "Number of threads: " << threads << std::endl;
    std::cout << "Elapsed time(sec): " << time_sec << std::endl;
}