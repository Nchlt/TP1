#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {

  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  char msg;

  if(world_rank == 0) {
    msg = 'a';
    MPI_Send(&msg, 1, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
  } else {
    MPI_Recv(&msg, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%d recoit le message %c\n",world_rank, msg );
  }

  MPI_Finalize();

}
