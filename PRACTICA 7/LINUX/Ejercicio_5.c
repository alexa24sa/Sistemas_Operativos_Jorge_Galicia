//para ejecutarlo en terminal de Linux usar: gcc nombre_archivo.c -o nombre_ejecutable
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define SIZE 10
void printMatrix(float mat[SIZE][SIZE]);
int invertMatrix(float A[SIZE][SIZE], float inverse[SIZE][SIZE]);
void printMatrixI(float mat[SIZE][SIZE]);

int main(void)
{
      int i, j, k;
  float temp;
  int status;
  int fd1[2], fd2[2], fd3[2], fd4[2], fd5[2], fd6[2];
//se declara el file descriptor 1 de tamaC1o 2
  pid_t pidA, pidB;
  float matriz1[10][10] = {
    {11, 2, 3, 4, 5, 6, 7, 8, 9, 10},
    {1, 22, 3, 4, 5, 6, 7, 8, 9, 10},
    {1, 2, 33, 4, 5, 6, 7, 8, 9, 10},
    {1, 2, 3, 44, 5, 6, 7, 8, 9, 10},
    {1, 2, 3, 4, 55, 6, 7, 8, 9, 10},
    {1, 2, 3, 4, 5, 66, 7, 8, 9, 10},
    {1, 2, 3, 4, 5, 6, 77, 8, 9, 10},
    {1, 2, 3, 4, 5, 6, 7, 88, 9, 10},
    {1, 2, 3, 4, 5, 6, 7, 8, 99, 10},
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 110}
};

  float matriz2[10][10] = {
    {120, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {10, 130, 8, 7, 6, 5, 4, 3, 2, 1},
    {10, 9, 140, 7, 6, 5, 4, 3, 2, 1},
    {10, 9, 8, 150, 6, 5, 4, 3, 2, 1},
    {10, 9, 8, 7, 160, 5, 4, 3, 2, 1},
    {10, 9, 8, 7, 6, 170, 4, 3, 2, 1},
    {10, 9, 8, 7, 6, 5, 180, 3, 2, 1},
    {10, 9, 8, 7, 6, 5, 4, 190, 2, 1},
    {10, 9, 8, 7, 6, 5, 4, 3, 200, 1},
    {10, 9, 8, 7, 6, 5, 4, 3, 2, 210}
};

  // Matriz para almacenar el resultado
  float multiplicacion[10][10], suma[10][10], inversa1[10][10], inversa2[10][10];

  float buffer1[10][10], buffer2[10][10], buffer3[10][10], buffer4[10][10], buffer5[10][10], buffer6[10][10];
//inicializa una matriz de 10x10
  // Inicializar la matriz C, y buffers de resultado a 0
  for (int i = 0; i < SIZE; i++)
{
  for (int j = 0; j < SIZE; j++)
{
  multiplicacion[i][j] = 0;
  suma[i][j] = 0;
  inversa1[i][j] = 0;
  inversa2[i][j] = 0;
  buffer1[i][j] = 0;
  buffer2[i][j] = 0;
  buffer3[i][j] = 0;
  buffer4[i][j] = 0;
  buffer5[i][j] = 0;
  buffer6[i][j] = 0;

}
}

  int num; //imprime el tamaC1o en bytes

  pipe (fd1); //TODO: Se crea el pipe 1
  pipe (fd2);
  pipe (fd3);
  pipe (fd4);
  pipe (fd5);
  pipe (fd6);
  
  //pipe(fd2); // TODO: Se crea el pipe 2
  //pidB = fork(): 
  pidA = fork ();
  switch (pidA)
{
case 0:
//caso hijo
  close (fd1[1]);
// se cierra la escritura del file descriptor 1
      for (int i = 0; i < SIZE; i++){
     for (int j = 0; j < SIZE; j++){
    read (fd1[0], &buffer1[i][j], sizeof (float)); //recibe matriz 1
    /*se lee la primera matriz por el pipe */   
     }
  }
  close (fd1[0]);
// se cierra la lectura del file descriptor 1
  
  printf("Matriz 1:\n");
  printMatrix(buffer1);
  
      wait(&status);

  close (fd2[1]);
// se cierra la escritura del file descriptor 2
  for (int i = 0; i < SIZE; i++){
     for (int j = 0; j < SIZE; j++){
    read (fd2[0], &buffer2[i][j], sizeof (float)); //recibe matriz 2
    /*se lee la segunda matriz por el pipe */  
     }
  }
  close (fd2[0]);
// se cierra la lectura del file descriptor 2
  
  printf("Matriz 2:\n");
  printMatrix(buffer2);
      wait(&status);

  //SE REALIZA LA MULTIPLICACICIÓN
  for (i = 0; i < 10; i++)
{
  for (j = 0; j < 10; j++)
{
  for (int k = 0; k < 10; k++)
{
  multiplicacion[i][j] += buffer1[i][k] * buffer2[k][j];
}
}
}
printf("MULTIPLICACICIÓN 1:\n");
printMatrix(multiplicacion);
  //close (fd3[0]);           // se cierra la lectura del file descriptor 3
  for (int i = 0; i < SIZE; i++){
            for (int j = 0; j < SIZE; j++){
                write (fd3[1], &multiplicacion[i][j], sizeof(float));
            }
       }
  /*se manda la multiplicacion por el pipe */
      //close (fd3[1]); // se cierra la escritura del file descriptor 3

  pidB = fork ();
// fork que se usara para crear el hijo dentro del hijo
  if (pidB == 0)
{
//  close (fd5[1]);
// se cierra la escritura del file descriptor 1
  for (int i = 0; i < SIZE; i++){
            for (int j = 0; j < SIZE; j++){
        read (fd5[0], &buffer3[i][j], sizeof (float)); // recibe buffer1
        /*se lee la primera matriz por el pipe */   
      }
       }
//  close (fd5[0]);
  
 // se cierra la lectura del file descriptor 1
  
//  close (fd6[1]);
// se cierra la escritura del file descriptor 2
  for (int i = 0; i < SIZE; i++){
            for (int j = 0; j < SIZE; j++){
        read (fd6[0], &buffer4[i][j], sizeof (float)); // recibe buffer 2
        /*se lee la segunda matriz por el pipe */  
      }
       }
//  close (fd6[0]);
  
 // se cierra la lectura del file descriptor 2
  
  //REALIZAMOS ALGORITMO DE SUMA DE MATRICES
  for (int i = 0; i < SIZE; i++)
{
  for (int j = 0; j < SIZE; j++)
{
  suma[i][j] = buffer3[i][j] + buffer4[i][j];
}
}

          //MANDAMOS LA SUMA POR LA TUBERIA 4
//  close (fd4[0]);
// se cierra la lectura del file descriptor 1
  for (int i = 0; i < SIZE; i++){
            for (int j = 0; j < SIZE; j++){
            write (fd4[1], &suma[i][j], sizeof (float));
            }
       }
  /*se manda la matriz suma por el pipe */
//  close (fd4[1]);
// se cierra la escritura del file descriptor 1
  
  wait(0);
  exit (0);

}
  else
{
  //PROCESO PARA PASAR LAS MATRICES AL HIJO:
//  close (fd5[0]);
// se cierra la lectura del file descriptor 5
  for (int i = 0; i < SIZE; i++){
            for (int j = 0; j < SIZE; j++){
                write (fd5[1], &buffer1[i][j], sizeof (float)); //pasa el buffer 1 por la pipe 5
            }
       }
  /*se manda la primera matriz por el pipe */
    //  close (fd5[1]);
    // se cierra la escritura del file descriptor 5
  
    //  close (fd6[0]);
    // se cierra la lectura del file descriptor 6
  for (int i = 0; i < SIZE; i++){
            for (int j = 0; j < SIZE; j++){
                write (fd6[1], &buffer2[i][j], sizeof (float)); //pasa el buffer 2 por la pipe 6
            }
       }
      /*se manda la segunda matriz por el pipe */
    //  close (fd6[1]);
    // se cierra la escritura del file descriptor 6
    wait (&status);
    //salimos del padre mientras no lo ocupemos
    }

    exit (0);

    break;
    case -1:
    // caso error en la ejecuciC3n
     printf ("ERROR AL EJECUTAR, RETORNA -1\n");

    break;
    default:
    //SE MANDAN LOS SIGUIENTES VALORES AL HIJO 
    close (fd1[0]);
    // se cierra la lectura del file descriptor 1
    for (int i = 0; i < SIZE; i++){
            for (int j = 0; j < SIZE; j++){
                write (fd1[1], &matriz1[i][j], sizeof (float));
            }
       }
    /*se manda la primera matriz por el pipe */
    close (fd1[1]);
    // se cierra la lectura del file descriptor 1
        
        
      close (fd2[0]); // se cierra la lectura del file descriptor 1
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
                write (fd2[1], &matriz2[i][j], sizeof (float));
            }
       }
    /*se manda la segunda matriz por el pipe */
    close (fd2[1]);
    // se cierra la escritura del file descriptor 1


    //DESPUÉS DE RECIBIR:
    //close (fd3[1]);
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            read (fd3[0], &buffer5[i][j], sizeof (float)); //recibe Multiplicacion
        /*se lee la primera matriz por el pipe */   
        }
    }
    //close (fd3[0]);
    
      printf("Multiplicacion de matrices:\n");
      printMatrix(buffer5);


    //  close (fd2[1]);
    // se cierra la escritura del file descriptor 1
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            read (fd4[0], &buffer6[i][j], sizeof (float)); //recibe suma
        /*se lee la segunda matriz por el pipe */   
        }
    }
    //  close (fd2[0]);
    // se cierra la lectura del file descriptor 1
  
      printf("Suma de matrices:\n");
      printMatrix(buffer6);

        break; 
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Directorio actual: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }

    // Tras calcular la inversa:
    if (invertMatrix(buffer5, inversa1)) {
        printf("Inversa de la matriz de multiplicación:\n");
        printMatrixI(inversa1);

        char filename[1024];
        sprintf(filename, "%s/inversa1.txt", cwd);
        FILE *file = fopen(filename, "w");
        if (file != NULL) {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    fprintf(file, "%8.6f ", inversa1[i][j]);
                }
                fprintf(file, "\n");
            }
            fclose(file);
        } else {
            printf("No se pudo abrir el archivo para escribir la inversa1.\n");
        }
    } else {
        printf("La matriz de multiplicación no es invertible.\n");
    }

    if (invertMatrix(buffer6, inversa2)) {
        printf("Inversa de la matriz de suma:\n");
        printMatrixI(inversa2);

        char filename[1024];
        sprintf(filename, "%s/inversa2.txt", cwd);
        FILE *file = fopen(filename, "w");
        if (file != NULL) {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    fprintf(file, "%8.6f ", inversa2[i][j]);
                }
                fprintf(file, "\n");
            }
            fclose(file);
        } else {
            printf("No se pudo abrir el archivo para escribir la inversa2.\n");
        }
    } else {
        printf("La matriz de suma no es invertible.\n");
    }

    return 0;
}

// Función para imprimir matrices
void printMatrix(float mat[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%8.1f", mat[i][j]);
        }
        printf("\n");
    }
}

void printMatrixI(float mat[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%8.6f", mat[i][j]);
        }
        printf("\n");
    }
}

// Función para calcular la inversa de una matriz
int invertMatrix(float A[SIZE][SIZE], float inverse[SIZE][SIZE]){
    float temp;
    int i, j, k;

    // Inicializar la matriz inversa como la matriz identidad
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (i == j) {
                inverse[i][j] = 1.0;
            } else {
                inverse[i][j] = 0.0;
            }
        }
    }

    // Método de Gauss-Jordan
    // Método de Gauss-Jordan
    for (i = 0; i < SIZE; i++) {
        temp = A[i][i];
        for (j = 0; j < SIZE; j++) {
            A[i][j] /= temp;
            inverse[i][j] /= temp;
        }
        for (j = 0; j < SIZE; j++) {
            if (i != j) {
                temp = A[j][i];
                for (k = 0; k < SIZE; k++) {
                    A[j][k] -= A[i][k] * temp;
                    inverse[j][k] -= inverse[i][k] * temp;
                }
            }
        }
    }


    // Comprobar si la matriz es invertible
    for (i = 0; i < SIZE; i++) {
        if (A[i][i] == 0) {
            return 0; // No invertible
        }
    }

    return 1; // Invertible
}
