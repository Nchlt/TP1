#include <mpi.h>
#include <iostream>
#include <cstdlib>

using namespace std;

// Fonction de comparaison pour le tri qsort
int compare(const void * a, const void * b) {
  return ( *(int*)a - *(int*)b );
}

int rand(int min, int max) {
  int res = rand()%(max-min + 1) + min;
  return res;
}

// Fonction qui prend deux tableaux de taille tab_size et réorganise le tableau
// tab_res pour qu'il ne garde que les plus petits éléments
void process_tab(int *tab1, int *tab2, int tab_size, int *tab_res) {
  int *new_tab = (int*) malloc(sizeof(int) * (2*tab_size));

  //On fusionne les deux tableau dans new_tab
  for(int i = 0; i < tab_size; i++) {
    new_tab[i] = tab1[i];
  }
  for(int i = tab_size; i < 2*tab_size; i++) {
    new_tab[i] = tab2[i];
  }
  // On trie
  qsort(new_tab, 2*tab_size, sizeof(int), compare);

  // On garde les elt_per_proc premiers éléments
  for(int i = 0; i < tab_size; i++) {
    tab_res[i] = new_tab[i];
  }

}

int main(int argc, char *argv[]) {

  int pid;  // Processsus id
  int size; // Wolrd size
  int tab_size = 100;
  int elt_per_proc;
  int etape = -1;

  // Allocation et création d'un tableau d'entier initilisé aléatoirement.
  int *tab = (int*) malloc(sizeof(int) * tab_size );
  for(int i = 0; i < tab_size; i++) {
    tab[i] = rand(0, 100);
  }
  // Tableau local avec elt_per_proc éléments
  int *my_tab = (int*) malloc(sizeof(int) * elt_per_proc);

  // Tableau local temporaire pour stoker le tableau recu
  int *my_tmp_tab = (int*) malloc(sizeof(int) * elt_per_proc);

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
  // if(pid==0) {
  //   cout << "Avant de trier : \n";
  //   cout << "[ ";
  //   for(int i =0; i < elt_per_proc ; i++) {
  //     cout << my_tab[i] << " ; ";
  //   }
  //   cout << "]\n";
  // }

  qsort(my_tab, elt_per_proc, sizeof(int), compare);
  // Point de rdv, l'étape -1 de tri interne est terminée pour tous les processus
  MPI_Barrier(MPI_COMM_WORLD);
  etape = 0;

  cout << "Etape 0 terminée\n";

  // On lance les p étapes
  while(etape < size) {
    cout << "Etape " << etape << " en cours \n";
    if(etape%2 == 0) {
    // Etape pair
      // Processus pairs
      if(pid%2 == 0) {
        // Les pair recoivent de pid + 1 y compris le processus 0
        MPI_Recv(my_tmp_tab, elt_per_proc, MPI_INT, pid + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //process_tab(int *tab1, int *tab2, int tab_size, int *tab_res)
        process_tab(my_tmp_tab, my_tab, elt_per_proc, my_tab);
        if(pid != 0) {
          // Le processus 0 ne peux pas envoyer à son pid - 1
          MPI_Send(my_tab, elt_per_proc, MPI_INT, pid - 1, 0, MPI_COMM_WORLD);
        }
      } else { // Processus impairs
        MPI_Recv(my_tab, elt_per_proc, MPI_INT, pid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        process_tab(my_tmp_tab, my_tab, elt_per_proc, my_tab);
        MPI_Send(my_tab, elt_per_proc, MPI_INT, pid + 1, 0, MPI_COMM_WORLD);
      }
    }
    if(etape%2 != 0) {
      // Etape impair
      if(pid%2 == 0) {
        if(pid != 0) {
          // Les pair recoivent de pid - 1 sauf le processus 0 qui n'a pas de pid - 1
          MPI_Recv(my_tab, elt_per_proc, MPI_INT, pid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          process_tab(my_tmp_tab, my_tab, elt_per_proc, my_tab);

        }
        // Les pairs envoient au pid + 1
        MPI_Send(my_tab, elt_per_proc, MPI_INT, pid + 1, 0, MPI_COMM_WORLD);

      } else { // Processus impairs
        MPI_Recv(my_tab, elt_per_proc, MPI_INT, pid + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        process_tab(my_tmp_tab, my_tab, elt_per_proc, my_tab);
        MPI_Send(my_tab, elt_per_proc, MPI_INT, pid - 1, 0, MPI_COMM_WORLD);
      }
    }
    // Avant de passer à l'étape suivante, il s'assurer que tous les processus
    // on terminé l'étape
    MPI_Barrier(MPI_COMM_WORLD);
    etape++;
    cout << "Etape " << etape << " terminée \n";
  }


  // if(pid==0) {
  //   cout << "Après triage : \n";
  //   cout << "[ ";
  //   for(int i =0; i < elt_per_proc ; i++) {
  //     cout << my_tab[i] << " ; ";
  //   }
  //   cout << "]\n";
  // }


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
