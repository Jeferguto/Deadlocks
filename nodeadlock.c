#include <stdio.h>   // Librería estándar para entrada y salida
#include <stdlib.h>  // Librería estándar de C para funciones de utilidad
#include <pthread.h> // Librería para trabajar con hilos (threads) en C
#include <unistd.h>  // Librería para usar la función sleep

// Declaración de dos mutex (recursos) para simular los recursos compartidos
pthread_mutex_t resource1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource2 = PTHREAD_MUTEX_INITIALIZER;

// Variables globales que se utilizarán como contadores compartidos
int counter1 = 0;   // Contador que será incrementado por el hilo 1
int counter2 = 100; // Contador que será decrementado por el hilo 2

// Función que ejecutará el hilo 1
void* thread1(void* arg) {
    while(1) { // Bucle infinito
        // Hilo 1 intenta bloquear el recurso 1 (adquirir el recurso)
        pthread_mutex_lock(&resource1);
        // Si logra bloquearlo, incrementa counter1
        printf("Hilo 1 bloqueó recurso 1, incrementando contador 1: %d\n", ++counter1);
        sleep(1);  // Simula un retardo en la ejecución del hilo (1 segundo)
        
        // Hilo 1 sigue incrementando el contador seis veces antes de intentar bloquear el recurso 2
        for (int i = 0; i < 6; i++) {
            printf("Hilo 1 avanzando contador 1: %d\n", ++counter1);
            sleep(1);  // Simula un retardo entre cada incremento
        }

        // Hilo 1 intenta bloquear el recurso 2
        printf("Hilo 1 intentando bloquear recurso 2...\n");
        pthread_mutex_lock(&resource2); // Ahora ambos hilos adquirirán recursos en orden
        printf("Hilo 1 bloqueó recurso 2\n");

        // Libera los recursos
        pthread_mutex_unlock(&resource2);
        pthread_mutex_unlock(&resource1);

        sleep(1);  // Pausa para darle tiempo a otros hilos de ejecutarse
    }

    return NULL;
}

// Función que ejecutará el hilo 2
void* thread2(void* arg) {
    while(1) { // Bucle infinito
        // Hilo 2 intenta bloquear el recurso 1 (adquirir el recurso)
        pthread_mutex_lock(&resource1); // Ahora el hilo 2 también bloqueará el recurso 1 primero
        // Si logra bloquearlo, decrementa counter2
        printf("Hilo 2 bloqueó recurso 1, decrementando contador 2: %d\n", --counter2);
        sleep(1);  // Simula un retardo en la ejecución del hilo (1 segundo)
        
        // Hilo 2 sigue decrementando el contador seis veces antes de intentar bloquear el recurso 2
        for (int i = 0; i < 6; i++) {
            printf("Hilo 2 avanzando contador 2: %d\n", --counter2);
            sleep(1);  // Simula un retardo entre cada decremento
        }

        // Hilo 2 intenta bloquear el recurso 2
        printf("Hilo 2 intentando bloquear recurso 2...\n");
        pthread_mutex_lock(&resource2);
        printf("Hilo 2 bloqueó recurso 2\n");

        // Libera los recursos
        pthread_mutex_unlock(&resource2);
        pthread_mutex_unlock(&resource1);

        sleep(1);  // Pausa para darle tiempo a otros hilos de ejecutarse
    }

    return NULL;
}

// Función principal (main)
int main() {
    pthread_t t1, t2;  // Declaración de dos identificadores de hilos

    // Crear los dos hilos y asignarles las funciones correspondientes
    pthread_create(&t1, NULL, thread1, NULL);  // Crea el hilo 1 que ejecuta la función thread1
    pthread_create(&t2, NULL, thread2, NULL);  // Crea el hilo 2 que ejecuta la función thread2

    // Espera a que los hilos finalicen (aunque en este caso no ocurrirá debido al deadlock)
    pthread_join(t1, NULL);  // Espera a que termine el hilo 1
    pthread_join(t2, NULL);  // Espera a que termine el hilo 2

    return 0;  // Finaliza el programa
}
