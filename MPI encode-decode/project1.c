#include <time.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>



char * encrypt(char  *array, int num_elements) {
    int i;
    for (i = 0; i < num_elements; i++) {
         array[i]=array[i]+5;
    }
    return array;
}

char * dycrept(char  *array, int num_elements) {
    int i;
    for (i = 0; i < num_elements; i++) {
         array[i]=array[i]-5;
    }
    return array;
}

void main(int argc, char **argv) {


    MPI_Init(NULL, NULL);
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    int number_processors;
    MPI_Comm_size(MPI_COMM_WORLD, &number_processors);

    int x;
    int length;
    int step;
    char name[512];
    int reminder;
    //Take the input and send the needed data to other process
    if(process_rank==0)
    {
      printf("Enter a String to Encrypt or Decrypt\n");
      fgets(name,sizeof(name),stdin);
      printf("\nPlease choose following options:\n");
      printf("1 = Encrypt the string.\n");
      printf("2 = Decrypt the string.\n");
      scanf("%d", &x);
      length=0;

      while(name[length]!='\0')
      {
        length++;
      }




      step=length/number_processors;
      MPI_Bcast(&step, 1,MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Bcast(&x, 1,MPI_INT, 0, MPI_COMM_WORLD);

    }


    MPI_Bcast(&step, 1,MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&x, 1,MPI_INT, 0, MPI_COMM_WORLD);

    char *sub_char_names = (char *)malloc(sizeof(char) * step);
    MPI_Scatter(name, step, MPI_CHAR, sub_char_names,
                step, MPI_CHAR, 0, MPI_COMM_WORLD);
    char * sub_enc;

    if(x==1)
    {
       sub_enc = encrypt(sub_char_names, step);
    }
    if(x==2)
    {
       sub_enc = dycrept(sub_char_names, step);
    }

    char *sub_encs = NULL;
    if (process_rank == 0) {
        sub_encs = (char *)malloc(sizeof(char) * length);

    }
    MPI_Gather(sub_enc, step, MPI_CHAR, sub_encs, step, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (process_rank == 0) {
      reminder=length%number_processors;
      int start=step*number_processors;
      for (int i = 0; i < reminder; i++) {
        if(x==1)
        {
          sub_encs[start+i]=name[start+i]+5;
        }
        if(x==2)
        {
          sub_encs[start+i]=name[start+i]-5;

        }
      }
      printf("The result is :");

      for (int i = 0; i < length-1; i++) {
           printf("%c",sub_encs[i]);
      }
      printf("\n");

//      clock_t toc = clock();
//      printf("Elapsed for processor %d : %f seconds\n", process_rank,(double)(toc - tic) / CLOCKS_PER_SEC);
    }
    MPI_Finalize();



}
