#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {

  int pid;  // Processsus id
  int size; // Wolrd size
  int matrix_rows = 10;
  int matrix_col  = 10;
  int matrix_size = matrix_rows * matrix_col;

  double **matrix = (double**) malloc(sizeof(double) * matrix_size );
  matrix [0][0] = 2.3;
  cout << matrix[0][0] << endl;

  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  MPI_Comm_size(MPI_COMM_WORLD, &size);



  MPI_Finalize();

}

// Exemples de codes :

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
