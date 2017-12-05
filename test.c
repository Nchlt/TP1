#include <mpi.h>
#include <stdio.h>

double f(int pid) {
  double pid_double = (double) pid;
  return ( 4 / ( 1 + (pid_double * pid_double) ) );
}

double fnode(int pid, double s) {
  double pid_double = (double) pid;
  double terme_1 = f(pid_double * s);
  double terme_2 = f((pid_double + 1) * s );
  return ( s/2 * (terme_1 + terme_2) );

}

int main(int argc, char** argv) {

  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  double calcul;
  double world_size_double = (double) world_size;
  double s = 1/(world_size_double-1);

  if (world_rank == 0) {
    calcul = fnode(world_rank, s);
    // L'initiateur est le processus 0
    // Il envoie le token
    MPI_Send(&calcul, 1, MPI_DOUBLE, world_rank + 1, 0, MPI_COMM_WORLD);
    // A la fin, l'initiateur recoit le token et la transmission est terminée
    MPI_Recv(&calcul, 1, MPI_DOUBLE, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Résultat du calcul = %f\n", calcul);

  } else {
      MPI_Recv(&calcul, 1, MPI_DOUBLE, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        calcul = calcul + fnode(world_rank, s);
        if (world_rank == (world_size -1 )) {
          MPI_Send(&calcul, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        } else {
          MPI_Send(&calcul, 1, MPI_DOUBLE, world_rank + 1, 0, MPI_COMM_WORLD);
        }
  }

  MPI_Finalize();

}
