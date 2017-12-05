// Nouredine Nour
// TP PPD - MPI
// Multiplication matrice vecteur version parallèle

#include <mpi.h>
#include <iostream>

using namespace std;

// Une fonction pour afficher une matrice pour débug
void print_matrix(double *matrix, int size) {
    cout << "[ ";
    for(int i = 0; i < size - 1; i++) {
      if(i%size ==0 && i != 0) {
        cout << "\n";
        cout << matrix[i] << " ; ";
      } else {
        cout << matrix[i] << " ; ";
      }
    }
    cout << matrix[size-1]<< " ]\n";
}

// Fonction qui renvoie un double aléatoire dans [min; max]
// utilisée pour intialiser une matrice d'exemple
double rand(double min, double max) {
  double res = ((double) rand()*(max-min)/(double)RAND_MAX-min);
  return res;
}

// Fonction que chaque processus exécute pour faire sa part du calcul
void mult(int my_matrix_size, int rows_per_proc, double *my_res, double *vector, double *my_matrix) {
  double tmp_res = 0.0;
  int cpt_vector = 0;
  int line = 0;

  for(int i = 0; i < my_matrix_size; i++) {
    if( cpt_vector == rows_per_proc - 1) {
      // On sauvegarde le résultat et on passe à la ligne suivante:
      my_res[line] = tmp_res;
      tmp_res = 0.0;
      cpt_vector = 0;
      line++;
    } else {
      tmp_res += my_matrix[i] * vector[i];
      cpt_vector++;
    }
  }
}

int main(int argc, char *argv[]) {

  int pid;              // id des différents processus
  int size;             // nombre de processus
  int matrix_size = 4;  // Matrice carrée, n = m
  int vector_size = 4;  // Taille du vecteur multiplicateur
  int rows_per_proc;    // Nombre de ligne de la matrice que recoivent chaque processus

  // Le vecteur initialisé par le processus 0
  double *vector = (double*) malloc(sizeof(double) * vector_size );
  // La matrice qui sera initialisée par le procesus 0
  double *matrix = (double*) malloc(sizeof(double) * matrix_size * matrix_size );
  // Le vecteur résultat
  double *result_vector = (double*) malloc(sizeof(double) * matrix_size );
  // Le morceau de matrice qui sera distribué à chaque processus
  double *my_matrix = (double*) malloc(sizeof(double) * rows_per_proc * matrix_size );
  // Le resultat local de la multiplcation
  double *my_result_matrix = (double*) malloc(sizeof(double) * rows_per_proc );

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // On suppose que la taille de la matrice est un multiple du nombre de processus.
  rows_per_proc = (matrix_size) / (size);

  if(pid == 0) {
    //Le processus 0 initialise le vecteur et la matrice à multiplier
    for(int i = 0; i < matrix_size * matrix_size; i++) {
      matrix[i] = rand(10.0, 20.0);
    }
    for(int i = 0; i < vector_size; i++) {
      vector[i] = rand(2.0, 4.0);
    }

    cout << "\n";
    cout << "Matrice = ";
    print_matrix(matrix, matrix_size*matrix_size);
    cout << "\n";

    cout << "Vecteur = ";
    print_matrix(vector, vector_size);
    cout << "\n";
  }


  // On scatter la matrice du processus 0 entre les processus
  MPI_Scatter(matrix, rows_per_proc * matrix_size, MPI_DOUBLE, my_matrix,
    rows_per_proc * matrix_size, MPI_DOUBLE, 0, MPI_COMM_WORLD );
  // On transmet le vecteur à tout les processus
  MPI_Bcast(vector, vector_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Chaque processus fait sa part du calcul
  for(int i = 0; i < size - 1; i++) {
    mult(rows_per_proc * matrix_size, rows_per_proc, my_result_matrix, vector, my_matrix);
    //print_matrix(pid, my_result_matrix, rows_per_proc);
  }

  // Une fois le calcul terminé on réuni tout dans result_vector du processus 0
  MPI_Gather(my_result_matrix, rows_per_proc, MPI_DOUBLE, result_vector, rows_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if(pid==0) {
    cout << "Résultat = ";
    print_matrix(result_vector, matrix_size);
    cout << "\n";

  }

  MPI_Finalize();

}
