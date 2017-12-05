#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {

  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int msg;

  //while (1) {
    if(world_rank % 2 == 0) {
      msg = msg + world_rank + 1;
      MPI_Send(&msg, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);
      printf("%d envoit le message %d\n",world_rank, msg );
      MPI_Recv(&msg, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%d recoit le message %d\n",world_rank, msg );

    } else {
      MPI_Recv(&msg, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%d recoit le message %d\n",world_rank, msg );
      msg = 10 * msg;
      MPI_Send(&msg, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD);
      printf("%d envoit le message %d\n",world_rank, msg );
    }
  //}

  MPI_Finalize();

}
