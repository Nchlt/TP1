#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {

  int pid;  // Processsus id
  int size; // Wolrd size

  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  MPI_Comm_size(MPI_COMM_WORLD, &size);

  cout << "Processus " << pid << "/" << size << " says Hello !" << endl;

  MPI_Finalize();

}

// Exemples de codes :

// Fonction qui renvoie un int aléatoire dans [min;max]
// int rand(int min, int max) {
//   int res = rand()%(max-min + 1) + min;
//   return res;
// }

// Fonction qui renvoie un double aléatoire dans [min; max]
// double rand(double min, double max) {
//   double res = ((double) rand()*(max-min)/(double)RAND_MAX-min);
//   return res;
// }

// Allocation dynamique d'un tableau
// double *tab;
// tab = (double*) malloc( 50 * sizeof(double) );

// Scatter
// MPI_Scatter(tab, 25, MPI_DOUBLE, my_tab, 25, MPI_DOUBLE, 0, MPI_COMM_WORLD );

// Send
// MPI_Send(&msg, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);

// Recv
// MPI_Recv(&msg, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

// Bcast
// MPI_Bcast(&msg, 1, MPI_INT, 0, MPI_COMM_WORLD);

// Reduce
// MPI_Reduce(&my_mean, &summ, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
