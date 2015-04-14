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
int i;
int count = 2;
int result = 1;
start = MPI_Wtime();
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

long random = 2801763489;

if (rank ==0){
  while(count <= random){
  for (i=1;i<size;i++){
    MPI_Send(&count, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
    count++;
    MPI_Recv(&result, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
      if(result == 0){
        printf("It's not a prime");
        t = MPI_Wtime() - start;
        printf("\ntotal execution time: %f\n",t);
        exit(0);
      }
  }
  }
}

else{
  while(count <= random){
  MPI_Recv(&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
  if (random % count == 0) result = 0;
  MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
}
MPI_Finalize();
return 0;
}
