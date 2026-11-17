#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
#define CICLOS_COMIDA 3

pthread_mutex_t tenedores[NUM_FILOSOFOS];

void* ciclo_filosofo(void* arg) {
    int id = *(int*)arg;
    int izquierdo = id;
    int derecho = (id + 1) % NUM_FILOSOFOS;

    for (int i = 0; i < CICLOS_COMIDA; i++) {
        printf("Filosofo %d esta pensando.\n", id);
        usleep(500000);

        if (id % 2 == 0) {
            pthread_mutex_lock(&tenedores[izquierdo]);
            pthread_mutex_lock(&tenedores[derecho]);
        } else {
            pthread_mutex_lock(&tenedores[derecho]);
            pthread_mutex_lock(&tenedores[izquierdo]);
        }

        printf("Filosofo %d tomo los tenedores %d y %d y esta COMIENDO (vez %d).\n", 
               id, izquierdo, derecho, i + 1);
        usleep(500000);

        pthread_mutex_unlock(&tenedores[izquierdo]);
        pthread_mutex_unlock(&tenedores[derecho]);
        printf("Filosofo %d termino de comer y solto los tenedores.\n", id);
    }

    printf(">>> Filosofo %d ha terminado su cena y se retira.\n", id);
    return NULL;
}

int main() {
    pthread_t hilos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_init(&tenedores[i], NULL);
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        if (pthread_create(&hilos[i], NULL, ciclo_filosofo, &ids[i]) != 0) {
            perror("Error al crear el hilo");
            return 1;
        }
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_destroy(&tenedores[i]);
    }

    printf("\nTodos los filosofos han terminado de cenar.\n");
    return 0;
}
