// #include <boost/multiprecision/fwd.hpp>
#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <mpi.h>

using namespace std;
using namespace boost::multiprecision;

cpp_dec_float_50 factorial(int number)
{
    cpp_dec_float_50 num = 1;
    for (int i = 1; i <= number; i++)
        num = num * i;
    return num;
}

// Define a function that performs the reduction operation on cpp_dec_float_50 values
void cpp_dec_float_50_reduce(void* in_val, void* inout_val, int* len, MPI_Datatype* datatype) {
    const boost::multiprecision::cpp_dec_float_50& a = *static_cast<const boost::multiprecision::cpp_dec_float_50*>(in_val);
    boost::multiprecision::cpp_dec_float_50& b = *static_cast<boost::multiprecision::cpp_dec_float_50*>(inout_val);
    b += a;
}


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int digits = atoi(argv[1]);
    int part_size = digits / size;
    int start = rank * part_size;
    int end = (rank == size - 1) ? digits : start + part_size;

    cpp_dec_float_50 e = 0;
    // cpp_dec_float_50 x = 1;
    cpp_dec_float_50 x = 1 / factorial(start);
    for (int i = start; i < end; i++) {
        cpp_dec_float_50 term = x;
        e += term;
        std::cout << rank << ":" << term << std::endl;
        x = x / (i + 1);

    }

    cpp_dec_float_50 global_e;

    // Create an MPI operation that uses the cpp_dec_float_50_reduce function
    MPI_Op cpp_dec_float_50_op;
    MPI_Op_create(&cpp_dec_float_50_reduce, 1, &cpp_dec_float_50_op);
    MPI_Reduce(&e, &global_e, 1, MPI_CXX_DOUBLE_COMPLEX, cpp_dec_float_50_op, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout.precision(digits);
        cout << "e(" << digits << ") = " << global_e << endl;
    }

    MPI_Op_free(&cpp_dec_float_50_op);
    MPI_Finalize();
    return 0;
}
