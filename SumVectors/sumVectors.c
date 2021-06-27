#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int size, num_threads;
float *vecX;
float *vecY;
float *vecZ;

struct arg_ {
    float *vectorX;
    float *vectorY;
    float *vectorZ;
    sem_t *mutex;
    int init;
    int end;
};

void *preencheVetores( void *arguments ) {
    struct arg_ *args = ( struct arg_ * ) arguments;
    for ( int i = args->init; i <= args->end;i++) {
        sem_wait(args->mutex);
        *( args->vectorX + i ) = 1.0 + (( float ) rand() / RAND_MAX);
        *( args->vectorY + i ) = 1.0 + (( float ) rand() / RAND_MAX);
        sem_post(args->mutex);
    }
}

void printVector( float *vector, int size ) {
    for( int i = 0; i < size; i++ ) {
        printf( "Vector[%i] : %f\n", i, vector[i] );
    }
}

void *sumVector( void *arguments ) {
    struct arg_ *args = ( struct arg_ * ) arguments;

    for ( int i = args->init; i <= args->end; i++) {
        sem_wait(args->mutex);
        // while (! *( args->vectorX + i) && ! *( args->vectorY + i)) {}
        *( args->vectorZ + i ) = *( args->vectorX + i ) + *( args->vectorY + i );
        sem_post(args->mutex);
    }
}


void main() {
    clock_t tempo;
    num_threads = 16;
    size = 160000000;
    int ubuntu = (size/num_threads);

    vecX = ( float* ) malloc( sizeof( float ) * (size) );
    vecY = ( float* ) malloc( sizeof( float ) * (size) );
    vecZ = ( float* ) malloc( sizeof( float ) * (size) );

    sem_t *mutex = ( sem_t * ) malloc( num_threads* sizeof( sem_t ) );

    pthread_t * threadsP = ( pthread_t * ) malloc( num_threads* sizeof( pthread_t ) );

    pthread_t * threadsS = ( pthread_t * ) malloc( num_threads* sizeof( pthread_t ) );

    struct arg_ *args = ( struct arg_ * ) malloc( num_threads* sizeof( struct arg_ ) );
    tempo = clock();
    for( int i = 0; i < num_threads; i++ ) {
        sem_init( &mutex[i], 0, 1 );

        args[i].vectorX = vecX;
        args[i].vectorY = vecY;
        args[i].vectorZ = vecZ;
        args[i].mutex = (mutex + i);
        args[i].init = i * ubuntu;
        args[i].end = ((i+1) * ubuntu) -1;
        
        pthread_create( &threadsP[i], NULL, preencheVetores, (void *) & args[i] );
        pthread_create( &threadsS[i], NULL, sumVector, ( void * ) & args[i] );
        sem_destroy( &mutex[i]);
    }

    for( int i = 0; i < num_threads; ++i ) {
        pthread_join( threadsP[i], NULL );
        pthread_join( threadsS[i], NULL );
    }

    printf( "\n Time:%f\n",( clock() - tempo ) / ( double ) CLOCKS_PER_SEC );
    pthread_exit( NULL );
    // printVector(vecX, size);

    // printVector(vecY, size);
    // printVector(vecZ, size);
}
