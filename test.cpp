#include <iostream>

using namespace std;


int main(int argc, char *argv[]) {
  double *tab;
  tab = (double*) malloc(10*sizeof(double));

  double max = 150.5;
  double min = 5.0;

  for(int i = 0; i < 5; i++) {
    tab[i] = ((double) rand()*(max-min)/(double)RAND_MAX-min);
  }

  cout << "[ ";
  for(int i = 0; i < 5; i++) {
    cout << " " << tab[i] << " ";
  }
  cout << " ]" << endl;

  free(tab);
  return 0;
}
