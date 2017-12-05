#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {

  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  char token;

  if (world_rank == 0) {
    token = 't';
    // L'initiateur est le processus 0
    // Il envoie le token
    MPI_Send(&token, 1, MPI_BYTE, world_rank + 1, 0, MPI_COMM_WORLD);
    // A la fin, l'initiateur recoit le token et la transmission est terminée
    MPI_Recv(&token, 1, MPI_BYTE, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%d recoit le message %c\n",world_rank, token );

  } else {
      MPI_Recv(&token, 1, MPI_BYTE, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%d recoit le message %c\n",world_rank, token );
        if (world_rank == (world_size -1 )) {
          MPI_Send(&token, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
        } else {
          MPI_Send(&token, 1, MPI_BYTE, world_rank + 1, 0, MPI_COMM_WORLD);
        }
  }

  MPI_Finalize();

}
