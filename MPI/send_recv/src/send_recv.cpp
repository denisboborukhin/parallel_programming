#include <mpi.h>

#include <iostream>
#include <iomanip>   
#include <thread>
#include <vector>
#include <list>
#include <functional>

const int SENDER_RANK = 0;
const int RECEIVER_RANK = 1;

void send(const std::vector<char>& buffer)
{
    std::cout << "send " << std::left << std::setw(10) << buffer.size() << " bytes with " <<
        "'MPI_Send ';";
    MPI_Send(buffer.data(), buffer.size(), MPI_CHAR, RECEIVER_RANK, 0, MPI_COMM_WORLD);
}

void Rsend(const std::vector<char>& buffer)
{
    std::cout << "send " << std::left << std::setw(10) << buffer.size() << " bytes with " <<
        "'MPI_Rsend';";
    MPI_Rsend(buffer.data(), buffer.size(), MPI_CHAR, RECEIVER_RANK, 0, MPI_COMM_WORLD);
}

void Ssend(const std::vector<char>& buffer)
{
    std::cout << "send " << std::left << std::setw(10) << buffer.size() << " bytes with " <<
        "'MPI_Ssend';";
    MPI_Ssend(buffer.data(), buffer.size(), MPI_CHAR, RECEIVER_RANK, 0, MPI_COMM_WORLD);
}

void Bsend(const std::vector<char>& buffer)
{
    std::cout << "send " << std::left << std::setw(10) << buffer.size() << " bytes with " <<
        "'MPI_Bsend';";
    MPI_Bsend(buffer.data(), buffer.size(), MPI_CHAR, RECEIVER_RANK, 0, MPI_COMM_WORLD);
}

int main(int argc, char* argv[])
{
    int commsize = 0;
    int my_rank = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    std::list<std::function<void(const std::vector<char>&)>> funcs;
    funcs.emplace_back(send);
    funcs.emplace_back(Rsend);
    funcs.emplace_back(Ssend);
    funcs.emplace_back(Bsend);

    for (auto func: funcs) {
        for (int i = 0; i != 4; ++i) {
            int num_bytes = 1 << (7 * i);
            if (my_rank == SENDER_RANK) {
                auto bytes = num_bytes << 2;

                std::vector<char> buff(bytes, 0);
                MPI_Buffer_attach(buff.data(), buff.size());

                std::vector<char> buffer(num_bytes, 1);
                auto begin = std::chrono::high_resolution_clock::now();
                func(buffer);
                // func(buffer.data(), buffer.size(), MPI_CHAR, RECEIVER_RANK, 0, MPI_COMM_WORLD);
                auto end = std::chrono::high_resolution_clock::now();

                MPI_Buffer_detach(buffer.data(), &bytes);

                auto duration = end - begin;
                std::cout << " time = " <<
                    std::setw(10) << duration.count() << " mcs\n";
            }

            if (my_rank == RECEIVER_RANK) {
                std::this_thread::sleep_for(std::chrono::milliseconds{2000});

                std::vector<char> recv_buffer(num_bytes, 0);
                MPI_Recv(recv_buffer.data(), num_bytes, MPI_CHAR, SENDER_RANK, 0, MPI_COMM_WORLD, nullptr);

            }
        }

        if (my_rank == SENDER_RANK) {
            std::cout << "-------------------------------------------------------------\n";
        }
    }

    MPI_Finalize();
}