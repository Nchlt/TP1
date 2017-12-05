#include <mpi.h>
#include <iostream>
#include <cstdlib>

using namespace std;

// Fonction de comparaison pour le tri qsort
int compare (const void * a, const void * b) {
  return ( *(int*)a - *(int*)b );
}

int rand(int min, int max) {
  int res = rand()%(max-min + 1) + min;
  return res;
}

int main(int argc, char *argv[]) {

  int pid;  // Processsus id
  int size; // Wolrd size
  int tab_size = 100;
  int elt_per_proc;

  // Allocation et création d'un tableau d'entier initilisé aléatoirement.
  int *tab = (int*) malloc(sizeof(int) * tab_size );
  for(int i = 0; i < tab_size; i++) {
    tab[i] = rand(0, 100);
  }
  int *my_tab = (int*) malloc(sizeof(int) * elt_per_proc);


  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  MPI_Comm_size(MPI_COMM_WORLD, &size);

  elt_per_proc = tab_size / size;

  // On distribu une le tableau de manière égale entre tout les processus
  MPI_Scatter(tab, elt_per_proc, MPI_INT, my_tab, elt_per_proc, MPI_INT, 0, MPI_COMM_WORLD );
  //
  // if (pid==0) {
  //   for(int i = 0; i < elt_per_proc; i++) {
  //     cout << my_tab[i] << " ";
  //   }
  // }
  if(pid==0) {
    cout << "Avant de trier : \n";
    cout << "[ ";
    for(int i =0; i < elt_per_proc ; i++) {
      cout << my_tab[i] << " ; ";
    }
    cout << "]\n";
  }

  qsort(my_tab, elt_per_proc, sizeof(int), compare);

  if(pid==0) {
    cout << "Après triage : \n";
    cout << "[ ";
    for(int i =0; i < elt_per_proc ; i++) {
      cout << my_tab[i] << " ; ";
    }
    cout << "]\n";
  }


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
