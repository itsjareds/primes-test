#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int size, rank;
  double t, start;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  int i, j;
  int count = 2;
  int result = 1;
  start = MPI_Wtime();
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  long random = 2801763489;

  if (rank == 0){
    for (i=1;i<size && count<random;i++) {
      j = i + 1;
      MPI_Send(&j, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
      count++;
    }
    while(count<random) {
      MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      if(result == 0) break;
      MPI_Send(&count, 1, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
      count++;
    }
  }

  else{
    while(1) {
      MPI_Recv(&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
      if (count == -1) break;
      if (random == count)
        result = 1;
      else if (random % count == 0)
        result = 0;
      MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }

  if (rank == 0) {
    printf("It's %s prime\n", result?"a":"not a");
    t = MPI_Wtime() - start;

    j = -1;
    for (i=1;i<size;i++) {
      MPI_Send(&j, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
    }

    printf("total execution time: %f\n",t);
  }
  MPI_Finalize();
  return 0;
}
