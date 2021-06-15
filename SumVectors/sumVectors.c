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

void *preencheVetores( void *s ) {
    int *size = ( int * )s;
    vecX = malloc( sizeof( float ) * *size );
    vecY = malloc( sizeof( float ) * *size );
    vecZ = malloc( sizeof( float ) * *size );

    for (int i = 0; i < *size; ++i)
    {
        *( vecX + i ) = ( float ) rand() / RAND_MAX;
        *( vecY + i ) = ( float ) rand() / RAND_MAX;
    }
}

void printVector( float *vector, int size ) {
    for( int i = 0; i < size; i++ ) {
        printf( "Vector[%i] : %f\n", i + 1, vector[i] );
    }
}

void *sumVector( void *arg ) {
    int i, tid, portion_size, start, end;

    tid = *( int * )( arg );
    portion_size = size / num_threads;
    start = tid * portion_size;
    end = ( tid + 1 ) * portion_size;

    for ( i = start; i < end; ++i ) {
        *( vecZ + i ) = *( vecX + i ) + *( vecY + i );
    }
}

void main() {
    clock_t tempo;
    
    size = 10000000;
    num_threads = 1;

    pthread_t * threads;
    threads = ( pthread_t * ) malloc( num_threads* sizeof(  pthread_t ) );

    preencheVetores( ( void* ) &size );

	tempo = clock();

    for( int i = 0; i < num_threads; i++ ) {       
        int *tid;
        tid = ( int * ) malloc( sizeof( int ) );
        *tid = i;
        pthread_create( &threads[i], NULL, sumVector, ( void * ) tid );
    }

    for( int i = 0; i < num_threads; ++i ) {
        pthread_join( threads[i], NULL );
    }

    printf( "\nDone\n" );
	printf( "\n Time:%f\n",( clock() - tempo ) / ( double ) CLOCKS_PER_SEC );

    pthread_exit( NULL );
}
