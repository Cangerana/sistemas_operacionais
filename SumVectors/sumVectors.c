#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int size, amt_threads;
float *vecX;
float *vecY;
float *vecZ;

struct t_arg {
    int init;
    int end;
    int released;
    float *vectorX;
    float *vectorY;
    float *vectorZ;
    sem_t *mutex;
};

void *preencheVetores( void *arguments ) {
    struct t_arg *args = ( struct t_arg * ) arguments;

    sem_wait( args->mutex );
    
    for ( int i = args->init; i <= args->end;i++) {
        *( args->vectorX + i ) = 1.0 + (( float ) rand() / RAND_MAX );
        *( args->vectorY + i ) = 1.0 + (( float ) rand() / RAND_MAX );
    }

    args->released = 1;
    sem_post(args->mutex);
}

void printVector( float *vector, int size ) {
    for( int i = 0; i < size; i++ ) {
        printf( "Vector[%i] : %f\n", i, vector[i] );
    }
}

void *sumVector( void *arguments ) {
    struct t_arg *args = ( struct t_arg * ) arguments;

    while ( !args->released ) {} // Verifing if that portion of vector is fill

    sem_wait(args->mutex);
    for ( int i = args->init; i <= args->end; i++ ) {
        *( args->vectorZ + i ) = *( args->vectorX + i ) + *( args->vectorY + i );
    }
    sem_post( args->mutex );
}

void main() {
    // printf( "TYPE VECTOR SIZE >> " );
    // scanf( "%d", &size );

    // printf( "TYPE AMOUNT OF THREADS >> " );
    // scanf( "%d", &amt_threads );
    size = 160000000; // 25.390
    amt_threads = 8; 
    int portion = ( size / amt_threads);

    vecX = ( float* ) malloc( sizeof( float ) * (size) );
    vecY = ( float* ) malloc( sizeof( float ) * (size) );
    vecZ = ( float* ) malloc( sizeof( float ) * (size) );

    sem_t *mutex = ( sem_t * ) malloc( amt_threads* sizeof( sem_t ) );

    pthread_t *threadsP = ( pthread_t * ) malloc( amt_threads* sizeof( pthread_t ) );

    pthread_t *threadsS = ( pthread_t * ) malloc( amt_threads* sizeof( pthread_t ) );

    struct t_arg *args = ( struct t_arg * ) malloc( amt_threads* sizeof( struct t_arg ) );

    time_t start,end;
    time (&start);
    printf("IM STARTING");
    for ( int i = 0; i < amt_threads; i++ ) {
        sem_init( &mutex[i], 0, 1 );

        args[i].vectorX = vecX;
        args[i].vectorY = vecY;
        args[i].vectorZ = vecZ;
        args[i].mutex = ( mutex + i );
        args[i].init = i * portion;
        args[i].end = ( ( i + 1 ) * portion ) - 1;
        args[i].released = 0;

        printf(" The thread %i will SUM from %i to %i \n",i, args[i].init, args[i].end);
        
        pthread_create( &threadsP[i], NULL, preencheVetores, ( void * ) & args[i] );
        pthread_create( &threadsS[i], NULL, sumVector, ( void * ) & args[i] );

        sem_destroy( &mutex[i]);
    }

    for( int i = 0; i < amt_threads; ++i ) {
        pthread_join( threadsP[i], NULL );
        pthread_join( threadsS[i], NULL );
    }

    time (&end);

    float dif = difftime (end,start);

    printf( "\n Time: %.3lf Seconds\n", dif);

    // printVector(vecZ, size); // Uncomment if wanna see the result

    pthread_exit( NULL );
}
