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

long random = 100000007;

if (rank ==0){
  while(count < random){
  for (i=1;i<size;i++){
  if (count < random){
    MPI_Send(&count, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
    count++;
    MPI_Recv(&result, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
      if(result == 0){
	printf("\ncount:%d\n",count);
        printf("It's not a prime");
        t = MPI_Wtime() - start;
        printf("\ntotal execution time: %f\n",t);
        MPI_Abort(MPI_COMM_WORLD,0);
	exit(0);
      }
  }
  else {
    break;
  }
  }
  }
}

else{
  while(count < random){
  MPI_Recv(&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
  if (random % count == 0) result = 0;
  else result = 1;
  MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
}
printf("Finish");
t = MPI_Wtime() - start;
printf("\ntotal execution time: %f\n",t);
MPI_Abort(MPI_COMM_WORLD, 0);
return 0;
}
