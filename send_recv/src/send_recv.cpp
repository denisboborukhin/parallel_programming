#include <mpi.h>

#include <iostream>
#include <iomanip>   
#include <thread>
#include <vector>

constexpr auto sender_rank = 0;
constexpr auto receiver_rank = 1;

int main(int argc, char* argv[])
{
    int commsize = 0;
    int my_rank = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    for (int i = 0; i != 4; ++i) {
        int num_bytes = 1 << (7 * i);
        if (my_rank == sender_rank) {
            std::vector<char> buffer(num_bytes, 1);
            auto begin = std::chrono::high_resolution_clock::now();
            MPI_Rsend(buffer.data(), buffer.size(), MPI_CHAR, receiver_rank, 0, MPI_COMM_WORLD);
            auto end = std::chrono::high_resolution_clock::now();

            auto duration = end - begin;
            std::cout << "sent " << std::left << std::setw(10) << num_bytes << " bytes; time = " <<
                std::setw(10) << duration.count() << " mcs\n";
        }

        if (my_rank == receiver_rank) {
            std::this_thread::sleep_for(std::chrono::milliseconds{2000});

            std::vector<char> recv_buffer(num_bytes, 0);
            MPI_Recv(recv_buffer.data(), num_bytes, MPI_CHAR, sender_rank, 0, MPI_COMM_WORLD, nullptr);
        }
    }

    MPI_Finalize();
}