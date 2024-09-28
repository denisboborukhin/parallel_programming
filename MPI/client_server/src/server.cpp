#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    // Open port.
    char port_name[MPI_MAX_PORT_NAME];
    MPI_Open_port(MPI_INFO_NULL, port_name);
    printf("Server opened port: %s\n", port_name);

    // Publish port name and accept client.
    MPI_Comm client;
    MPI_Publish_name("name", MPI_INFO_NULL, port_name);
    printf("Server published service name\n");
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &client);
    printf("Server accepted connection\n");

    // Receive data from client.
    char message[100];
    MPI_Recv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, 0, client, MPI_STATUS_IGNORE);
    printf("Сервер получил сообщение: %s\n", message);

    MPI_Unpublish_name("name", MPI_INFO_NULL, port_name);

    MPI_Finalize();
    return 0;
}