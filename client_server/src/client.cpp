#include "mpi.h"

int main(int argc, char *argv[])
{   
    MPI_Init(&argc, &argv);

    // Look up for server's port name.
    char port_name[MPI_MAX_PORT_NAME];
    MPI_Lookup_name("name", MPI_INFO_NULL, port_name);
    printf("Client found service name\n");

    // Connect to server.
    MPI_Comm server;
    MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &server);
    printf("Client connected to server\n");

    // Send data to server.
    char message[100] = "Hi, server!";
    MPI_Send(message, 100, MPI_CHAR, 0, 0, server);
    printf("Client send message: %s\n", message);

    MPI_Finalize();
    return 0;
}