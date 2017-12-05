#include <mpi.h>
#include <iostream>

using namespace std;

// Fonction qui renvoie un double aléatoire dans [min; max]
// utilisée pour intialisé une matrice exemple
double rand(double min, double max) {
  double res = ((double) rand()*(max-min)/(double)RAND_MAX-min);
  return res;
}

// Fonction de multiplcation
double mult(int indice, double *matrix, int m_size, double *vector, int v_size) {
  double res = 0.0;
  // On pourrait optimiser ici en itérant en premier la plus petite taille
  for(int i = indice * m_size; i < m_size + (indice * m_size); i++ ) {
    for(int j = 0; j < v_size; j++) {
      res = res + (matrix[i] * vector[j]);
    }
  }
  return res;
}

int main(int argc, char *argv[]) {

  int pid;  // Processsus id
  int size;
  int matrix_size = 10; // Matrice carrée, n = m
  int vector_size = 15;
  int rows_per_proc;
  //int rows_per_proc = (size) / matrix_size;

  // Le vecteur initialisé par le processus 0
  double *vector = (double*) malloc(sizeof(double) * vector_size );
  // La matrice qui sera initialisée par le procesus 0
  double *matrix;
  // Le vecteur résultat
  double *result_vector = (double*) malloc(sizeof(double) * matrix_size );
  // Le morceau de matrice qui sera distribué à chaque processus
  double *my_matrix = (double*) malloc(sizeof(double) * rows_per_proc * matrix_size );
  // Le resultat local de la multiplcation
  double *my_result_matrix = (double*) malloc(sizeof(double) * rows_per_proc );

  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  MPI_Comm_size(MPI_COMM_WORLD, &size);

  rows_per_proc = (matrix_size*matrix_size) / (size);

  if(pid == 0) {
    //Le processus 0 initialise le vecteur et la matrice à multiplier
    matrix = (double*) malloc(sizeof(double) * matrix_size * matrix_size );

    for(int i = 0; i < matrix_size * matrix_size; i++) {
      matrix[i] = rand(1.0, 150.5);
    }
    for(int i = 0; i < vector_size; i++) {
      vector[i] = rand(1.0, 150.5);
    }
  }

  // On scatter la matrice du processus 0 entre les size - 1 processus
  MPI_Scatter(matrix, rows_per_proc * matrix_size, MPI_DOUBLE, my_matrix,
    rows_per_proc * matrix_size, MPI_DOUBLE, 0, MPI_COMM_WORLD );
  // On transmet le vecteur à tout les processus
  MPI_Bcast(vector, vector_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  cout << pid << " : " << vector[5] << endl; // test
  // (int indice, double *matrix, int m_size, double *vector, int v_size)

  for(int i = 0; i < rows_per_proc ; i++) {
    my_result_matrix[i] = mult(i, my_matrix, rows_per_proc*matrix_size, vector, vector_size );
    //cout << rows_per_proc << " ";
  }


    // for(int i = 0; i < rows_per_proc; i++) {
    //   cout << " ; " << my_result_matrix[i] << " ; ";
    // }


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
