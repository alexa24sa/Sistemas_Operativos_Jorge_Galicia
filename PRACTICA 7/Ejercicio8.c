#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Para sleep
#include <sys/types.h> /* Servidor de la memoria compartida */
#include <sys/ipc.h> /* (ejecutar el servidor antes de ejecutar el cliente)*/
#include <sys/shm.h> //se incluye la librería de shmmat 
#include <sys/ipc.h> // to includes the uses of key with ftok
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#define TAM_MEM 10
#define SIZE 10 

void printMatrix(float mat[SIZE][SIZE]);
void printMatrixI(float mat[SIZE][SIZE]);
int invertMatrix(float A[SIZE][SIZE], float inverse[SIZE][SIZE]);
void printMatrixI(float mat[SIZE][SIZE]);
void leer_matriz(int shm_id, float matrix[SIZE][SIZE]);
void escribir_matriz(int shm_id, float matrix[SIZE][SIZE]);



int main (){
  int i, j, k;
  float temp;
  int status;
  pid_t pidA, pidB;
  key_t key1, key2, key3, key4, key5, key6;
  int shm_id1, shm_id2, shm_id3, shm_id4, shm_id5, shm_id6;
  
  int fd = open("/tmp/ipc_key_file", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Error creating/opening IPC key file");
        return 1;
    }
    close(fd);  // Cerrar el descriptor de archivo después de crearlo
  
  
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

    float buffer1[10][10], buffer2[10][10], buffer3[10][10], buffer4[10][10], buffer5[10][10], buffer6[10][10], buffer7[10][10], buffer8[10][10], buffer9[10][10], buffer10[10][10], buffer11[10][10], buffer12[10][10];
    //inicializa una matriz de 10x10
    // Inicializar la matriz C, y buffers de resultado a 0
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
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
            buffer7[i][j] = 0;
            buffer8[i][j] = 0;
            buffer9[i][j] = 0;
            buffer10[i][j] = 0;
            buffer11[i][j] = 0;
            buffer12[i][j] = 0;
        }
    }

    int num; //imprime el tamaño en bytes
    
    //Asignamos las llaves de memoria compartida:
    key1 = ftok ("/tmp/ipc_key_file", 20); //con ftok obtenemos la llave asociada al share memory file de ID 20
    key2 = ftok ("/tmp/ipc_key_file", 21); //con ftok obtenemos la llave asociada la share memory file de ID 21
    //key3 = ftok ("shmfile", 22); //con ftok obtenemos la llave asociada la share memory file de ID 22
    //key4 = ftok ("shmfile", 23); //con ftok obtenemos la llave asociada la share memory file de ID 23
    //revisamos que la obtención de memoria no haya causado error:
    if(key1 == -1 || key2 == -1 /*|| key3 == -1 || key4 == -1*/){
        printf ("HA HABIDO UN ERROR EN LA OBTENCI%cN DE LA LLAVE ASOCIADO A ALGÚN BLOQUE DE MEMORIA\n", 224);
        exit(1);
    } 
    
    //una vez asignadas las llaves, pedimos ese bloque de memoria asociado a la llave que se acaba de obtener con ftok
    //para poder obtener ese trozo de memoria asociado a ese bloque usamos shmget:
    //CREAMOS LOS SEGMENTOS DE MEMORIA COMPARTIDA
    shm_id1 = shmget(key1, sizeof(float) * SIZE * SIZE, 0666 | IPC_CREAT); //obtiene el bloque de memoria asociado a la key 1 y le asigna un bloque de memoria nuevo 
    shm_id2 = shmget(key2, sizeof(float) * SIZE * SIZE, 0666 | IPC_CREAT); //obtiene el bloque de memoria asociado a la key 2 y le asigna un bloque de memoria nuevo 
    
     if(shm_id1 == -1 || shm_id2 == -1 /*|| key3 == -1 || key4 == -1*/){
        printf ("HA HABIDO UN ERROR PARA SHMGET EN LA OBTENCI%cN DE LA LLAVE ASOCIADO A ALGÚN BLOQUE DE MEMORIA\n", 224);
        exit(1);
    }
    
    pidA = fork();
    
    switch(pidA){
        case 0: //SOY EL PROCESO HIJO 
        // SE EJECUTARÁN LAS ACCIONES DEL PROCESO HIJO QUE RECIBE LAS MATRICES DEL PADRE
        //Leemos las matrices provenientes del padre:
        leer_matriz(shm_id1, buffer3); //recibe la matriz 1
        leer_matriz(shm_id2, buffer4); //recibe la matriz 2
        //IMPRIMIMOS PARA VERIFICAR QUE SI SE ESTÁN RECIBIENDO LAS MATRICES:
        printf("Matriz 1 en HIJO:\n");
        printMatrix(buffer3);
        printf("Matriz 2 en HIJO:\n");
        printMatrix(buffer4);
        
        //realizamos la multiplicacion de las matrices:
        for (i = 0; i < 10; i++){
            for (j = 0; j < 10; j++){
                for (int k = 0; k < 10; k++){
                    multiplicacion[i][j] += buffer3[i][k] * buffer4[k][j];
                }
            }
        }
        
        //wait(0);
        //VERIFICAR EL FUNCIONAMIENTO CON IMPRESIONES:
        printf("Multiplicacion en HIJO:\n");
        printMatrix(multiplicacion);
        
        //creamos nuevos espacios y variables auxiliares para el proceso de escritura por memoria compartida:
        //Asignamos las llaves de memoria compartida:
        key3 = ftok ("/tmp/ipc_key_file", 22); //con ftok obtenemos la llave asociada al share memory file de ID 22
        key4 = ftok ("/tmp/ipc_key_file", 23); //con ftok obtenemos la llave asociada la share memory file de ID 23
        key5 = ftok ("/tmp/ipc_key_file", 24); //con ftok obtenemos la llave asociada la share memory file de ID 24
        //revisamos que la obtención de memoria no haya causado error:
        if(key3 == -1 || key4 == -1 || key5 == -1 /*|| key4 == -1*/){
            printf ("HA HABIDO UN ERROR EN LA OBTENCI%cN DE LA LLAVE ASOCIADO A ALGÚN BLOQUE DE MEMORIA\n", 224);
            exit(1);
        } 
        //una vez asignadas las llaves, pedimos ese bloque de memoria asociado a la llave que se acaba de obtener con ftok
        //para poder obtener ese trozo de memoria asociado a ese bloque usamos shmget:
        //CREAMOS LOS SEGMENTOS DE MEMORIA COMPARTIDA
        shm_id3 = shmget(key3, sizeof(float) * SIZE * SIZE, 0666 | IPC_CREAT); //obtiene el bloque de memoria asociado a la key 3 y le asigna un bloque de memoria nuevo 
        shm_id4 = shmget(key4, sizeof(float) * SIZE * SIZE, 0666 | IPC_CREAT); //obtiene el bloque de memoria asociado a la key 4 y le asigna un bloque de memoria nuevo 
        shm_id5 = shmget(key5, sizeof(float) * SIZE * SIZE, 0666 | IPC_CREAT); //obtiene el bloque de memoria asociado a la key 5 y le asigna un bloque de memoria nuevo
        
        
        if(shm_id3 == -1 || shm_id4 == -1 || shm_id5 == -1 /*|| key4 == -1*/){
            printf ("HA HABIDO UN ERROR PARA SHMGET EN LA OBTENCI%cN DE LA LLAVE ASOCIADO A ALGÚN BLOQUE DE MEMORIA\n", 224);
            exit(1);
        }
        
        
        //MANDAMOS LAS MATRICES INICIALES AL HIJO:
        
        escribir_matriz(shm_id3, buffer3); //MANDAMOS LA MATRIZ 1 AL NIETO 
        escribir_matriz(shm_id4, buffer4); //MANDAMOS LA MATRIZ 2 AL NIETO
        escribir_matriz(shm_id5, multiplicacion); //MANDAMOS EL RESULTADO DE LA MULTIPLICACIÓN AL PADRE
        
        
        pidB = fork(); // creamos el proceso nieto
        if (pidB == 0){
            // SE EJECUTARÁ LA SUMA DE MATRICES
            //SE LEEN LAS MATRICES QUE RECIBE:
            leer_matriz(shm_id3, buffer5); //Recibe la Matriz 1
            leer_matriz(shm_id4, buffer6); //Recibe la Matriz 2
            
            //SE REALIZA LA SUMA CON LAS CORRESPONDIENTES MATRICES:
            for (int i = 0; i < SIZE; i++){
                for (int j = 0; j < SIZE; j++){
                    suma[i][j] = buffer5[i][j] + buffer6[i][j];
                }
            }
            
            //wait(0);
            printf("Suma en NIETO:\n");
            printMatrix(suma);
            
            //MANDAMOS LA MATRIZ DE SUMA MEDIANTE MEMORIA COMPARTIDA:
            //creamos nuevos espacios y variables auxiliares para el proceso de escritura por memoria compartida:
            //Asignamos las llaves de memoria compartida:
            key6 = ftok ("/tmp/ipc_key_file", 24); //con ftok obtenemos la llave asociada al share memory file de ID 24
       
            //revisamos que la obtención de memoria no haya causado error:
            if(key6 == -1 /*|| key4 == -1 || key3 == -1 || key4 == -1*/){
                printf ("HA HABIDO UN ERROR EN LA OBTENCI%cN DE LA LLAVE ASOCIADO A ALGÚN BLOQUE DE MEMORIA\n", 224);
                exit(1);
            } 
            //una vez asignadas las llaves, pedimos ese bloque de memoria asociado a la llave que se acaba de obtener con ftok
            //para poder obtener ese trozo de memoria asociado a ese bloque usamos shmget:
            //CREAMOS LOS SEGMENTOS DE MEMORIA COMPARTIDA
            shm_id6 = shmget(key6, sizeof(float) * SIZE * SIZE, 0666 | IPC_CREAT); //obtiene el bloque de memoria asociado a la key 3 y le asigna un bloque de memoria nuevo 
        
            if(shm_id6 == -1 /*|| shm_id4 == -1 || shm_id5 == -1 /*|| key4 == -1*/){
                printf ("HA HABIDO UN ERROR PARA SHMGET EN LA OBTENCI%cN DE LA LLAVE ASOCIADO A ALGÚN BLOQUE DE MEMORIA\n", 224);
                exit(1);
            }
            //SE MANDA LA MATRIZ DE SUMA AL PADRE
            escribir_matriz(shm_id6, suma);
            
            exit (0);
        }else{
            wait(0);
            wait(0);
            wait(0);
            //wait(0);
            exit(0);
        }
        
        exit (0);
        
        break;
        
        case -1: 
            printf("HUBO UN ERROR EN LA CREACIÓN DEL PROCESO\n");
            exit(1);
            
        break;
        
        default:
            //SOY EL PROCESO PADRE:
            //SE MANDAN LAS MATRICES INICIALES AL PROCESO HIJO
            //escribimos los valores de las dos matrices iniciales:
            //se escribe la matriz 1
            escribir_matriz(shm_id1, matriz1);
            //se escribe la matriz 2
            escribir_matriz(shm_id2, matriz2);
            
            wait(0);
            wait(0);
            wait(0);
            //wait(0);
            
            //---------------------------------------------------------
            //RECIBIMOS VALORES:
            leer_matriz(shm_id5, buffer9); //se lee el valor de la multiplicacion
            leer_matriz(shm_id6, buffer8); //se lee el valor de la suma
            printf("Multiplicacion en PADRE:\n");
            printMatrix(buffer9);
            printf("Suma en PADRE:\n");
            printMatrix(buffer8);
            
            
            //funciC3n que obtiene la inversa de una matriz:
            if (invertMatrix(buffer5, inversa1)) {
                printf("Inversa de la matriz 1:\n");
                printMatrixI(inversa1);
            } else {
                printf("La matriz no es invertible.\n");
            }

  
            if (invertMatrix(buffer6, inversa2)) {
                printf("Inversa de la matriz 2:\n");
                printMatrixI(inversa2);
            } else {
                printf("La matriz no es invertible.\n");
            }
            
        break;
        
    }
    
    for(i = 0; i<6; i++){
        wait(0);
    }
    
    
    // Eliminar segmentos de memoria compartida
    shmctl(shm_id1, IPC_RMID, NULL);
    shmctl(shm_id2, IPC_RMID, NULL);
    shmctl(shm_id3, IPC_RMID, NULL);
    shmctl(shm_id4, IPC_RMID, NULL);
    shmctl(shm_id5, IPC_RMID, NULL);
    shmctl(shm_id6, IPC_RMID, NULL);
    
    
    return 0;
}


// Asumiendo que las definiciones de funciones y declaraciones son las mismas


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

// Función para copiar una matriz de la memoria compartida a una matriz local
/*void leer_matriz(int shm_id, float matrix[SIZE][SIZE]) {
    int (*shm_matrix)[SIZE] = shmat(shm_id, NULL, 0);
    if (shm_matrix == (void*) -1) {
        perror("shmat");
        exit(1);
    }
    memcpy(matrix, shm_matrix, sizeof(float) * SIZE * SIZE);
    shmdt(shm_matrix);
}*/
void leer_matriz(int shm_id, float matrix[SIZE][SIZE]) {
    float (*shm_matrix)[SIZE] = shmat(shm_id, NULL, 0);
    if (shm_matrix == (void *) -1) {
        perror("Error attaching shared memory");
        exit(1);
    }
    memcpy(matrix, shm_matrix, sizeof(float) * SIZE * SIZE);
    if (shmdt(shm_matrix) == -1) {
        perror("Error detaching shared memory");
    }
}


// Función para copiar una matriz local a la memoria compartida
/*void escribir_matriz(int shm_id, float matrix[SIZE][SIZE]) {
    int (*shm_matrix)[SIZE] = shmat(shm_id, NULL, 0);
    if (shm_matrix == (void*) -1) {
        perror("shmat");
        exit(1);
    }
    memcpy(shm_matrix, matrix, sizeof(float) * SIZE * SIZE);
    shmdt(shm_matrix);
}*/

void escribir_matriz(int shm_id, float matrix[SIZE][SIZE]) {
    float (*shm_matrix)[SIZE] = shmat(shm_id, NULL, 0);
    if (shm_matrix == (void *) -1) {
        perror("Error attaching shared memory");
        exit(1);
    }
    memcpy(shm_matrix, matrix, sizeof(float) * SIZE * SIZE);
    if (shmdt(shm_matrix) == -1) {
        perror("Error detaching shared memory");
    }
}
