#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  // Variables
  int pid;
  int size;
  double max = 150.5;
  double min = 1.0;

  // Initialisation de MPI
  MPI_Init(&argc, &argv);

  // Récupération du rank des processus
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  // Récupération du nombre de porecessus
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double *matrix;
  double vector[10];
  double *lines;

  lines = (double*) malloc( 50 * sizeof(double) );
  matrix = (double*) malloc( 100 * sizeof(double) );




  if(pid == 0) {
    // Le processsus 0 initialise la matrice et le vecteur
    double vector[10] = {10.0, 2.2, 4.3, 1.4, 2.2, 4.2, 7.4, 8.7, 15.8, 10.2};
    for(int i = 0; i < 100; i++) {
      matrix[i] = ((double) rand()*(max-min)/(double)RAND_MAX-min);
    }

    for(int i = 1; i <= size - 1; i++) {
      //On envoie le vecteur à tout les processus a remplacer par un bcast
      MPI_Send(&vector, 10, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);

    }
  }
    MPI_Scatter(matrix, 50, MPI_DOUBLE, lines, 50, MPI_DOUBLE, 0, MPI_COMM_WORLD );


   if(pid != 0){
      MPI_Recv(&vector, 10, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      for(int i = 0; i < 50; i++) {
        cout << " Le processus " << pid << " recoit matrix[" << i << "] = " <<
        lines[i] << "\n";
      }


      cout << " Le processus " << pid << " recoit " << vector[2] << "\n";
    }

  free(lines);
  MPI_Finalize();

}
