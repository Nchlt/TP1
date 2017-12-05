#include <mpi.h>
#include <iostream>

using namespace std;

// Fonction de calcul de la moyenne locale
double my_mean_f(double *t) {
  int length = 25;
  double sum = 0;
  for(int i = 0; i < 25; i++) {
    sum = sum + t[i];
  }
  return sum/length;
}
// Fonction qui renvoie un double aléatoire dans [min;max]
double rand(double min, double max) {
  double res = ((double) rand()*(max-min)/(double)RAND_MAX-min);
  return res;
}

int main(int argc, char *argv[]) {

  int pid;  // Processsus id
  int size; // Wolrd size

  double *tab;
  double *my_tab = (double*) malloc( 25 * sizeof(double) );
  double my_mean;
  double summ;


  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if(pid == 0) {
    tab = (double*) malloc( 50 * sizeof(double) );
    for(int i = 0; i < 50; i++) {
      tab[i] = rand(0.0, 20.0);
    }
    summ = 0;
  }

  MPI_Scatter(tab, 25, MPI_DOUBLE, my_tab, 25, MPI_DOUBLE, 0, MPI_COMM_WORLD );
  // for(int i = 0; i < 25; i++) {
  //   cout << pid << " : " << my_tab[i] << " ";
  // }
  // cout << endl;
  // for(int i = 0; i < 50; i++) {
  //   cout << pid << " : " << tab[i] << " " ;
  // }
  // cout << "\n";
  my_mean = my_mean_f(my_tab);
  cout << "Processus " << pid << " my_mean = " << my_mean << endl;
  MPI_Reduce(&my_mean, &summ, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if(pid == 0) {
    cout << "Global sum = " << summ << endl;
    cout << "Mean = " << summ / 2 << endl;
  }


  MPI_Finalize();

}
