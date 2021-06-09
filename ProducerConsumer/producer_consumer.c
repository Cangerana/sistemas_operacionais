#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t mutex;
int *buffer;
int tail = -1;
int buffer_size = 0;
int amount_cons = 0;
int amount_prod = 0;
int id_producer = 0;
int id_consumer = 0;


int *createBuffer(int b_size) {
    int *buffer = malloc(sizeof(int) * b_size);

    for (int i = 0; i < b_size; i++) {
        *(buffer + i) = -1;
    }
    return buffer;
}

int produce_value() {
    tail++;
    *(buffer + tail) = abs(rand());
    return *(buffer + tail);
}

void *producer(void *id_thread) {
    // int *tid = id_thread;
    int tid = id_producer++;

    while (1) {
        sem_wait(&mutex);
        if (tail > buffer_size - 2) {
            printf("Papai, n consigo ser produtivo! im the %d \n",tid);
        }

        else {
            printf("To produzindo papai -> [%i]!!! tem orgulho de mim agora?! im the %d \n", produce_value(), tid);
        }
        sem_post(&mutex);
        sleep(2);
    }
}

int consumeValue() {
    int value = *(buffer + tail);
    *(buffer + tail--) = -1;
    return value;
}

void *consumer(void *id_thread) {
    int tid = id_consumer++;
  
    while (1) {
        sem_wait(&mutex);

        while (1) {
            if (tail < 0) {
                printf("Papai, quero consumir, da na minha boquinha!! im the %d \n", tid);
            }

            else {
                int value = consumeValue();
                printf("eu consumi o valor %d  im the %d \n", value, tid);
            }
            sem_post(&mutex);
            sleep(3);
        }    
    }
}

int max(int num1, int num2) {
    return (num1 > num2 ) ? num1 : num2;
}

void startThreads(int amt_prod, int amt_cons) {
    int amt_max = max(amt_prod, amt_cons);

    pthread_t consumers[amt_cons];
    pthread_t producers[amt_prod];


    for(int i = 0; i < amt_max; i++) {
        if(i < amt_cons) {
            printf("Criando o consumidor %d dale  ! \n",i);
            pthread_create(&consumers[i], NULL, consumer, &i);
        }

        if(i < amt_prod) {
            printf("Criando o produtor  %d dale  ! \n",i);
            pthread_create(&producers[i], NULL, producer, &i);
        }
    }
        pthread_exit(NULL);
}

int main() {
    int aux;
    printf("TYPE AMOUNT OF CONSUMERS >>");

    printf("TYPE AMOUNT OF CONSUMERS >>");
    scanf("%d", &amount_cons);
   
    printf("TYPE AMOUNT OF PRODUCERS >>");
    scanf("%d", &amount_prod);
   
    printf("INSERT BUFFER SIZE >>");
    scanf("%d", &buffer_size);

    buffer = createBuffer(buffer_size);

    sem_init(&mutex, 0, 1);
    startThreads(amount_prod, amount_cons);
    sem_destroy(&mutex);

    return 0;
}