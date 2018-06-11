#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <vector>
#include <map>

using namespace std;
pthread_rwlock_t rwlock;
int rnd = 10000000;

void *thread_r( void *arg )
{
    volatile int a;
    for ( int i = 0; i < rnd; i++ )
    {    
        pthread_rwlock_rdlock(&rwlock);
        pthread_rwlock_unlock(&rwlock);
    }   

    return NULL;
}

void *thread_w( void *arg )
{
    volatile int b;
    for ( int i = 0; i < rnd; i++ )
    {
        pthread_rwlock_wrlock(&rwlock);
        pthread_rwlock_unlock(&rwlock);
    }   
    return NULL;
}

int main( int argc, char *argv[] )
{
    int err;
    size_t rdThdNum = 0, wrThdNum = 0;
    struct timeval endTime, startTime;
    if ( argc > 2 )
    {
        rdThdNum = atoi( argv[1] );
        wrThdNum = atoi( argv[2] );
    } else if ( argc > 1 )
    {
        rdThdNum = atoi( argv[1] );
        wrThdNum = 1;
    } else 
    {
        rdThdNum = 1;
        wrThdNum = 1;
    }
    fprintf( stdout, "exec [rdThdNum=%d] [wrThdNum=%d]\n", rdThdNum, wrThdNum );

    pthread_t rdThd[rdThdNum], wrThd[wrThdNum];
    pthread_rwlock_init( &rwlock, NULL );

    gettimeofday( &startTime, NULL );
    for ( size_t i = 0; i < rdThdNum; i++  )
    {
        err = pthread_create( &rdThd[i], NULL, thread_r, (void *)0 );
        if ( err != 0 )
        {
            printf( "create new thread error:%s\n", strerror( err ) );
            exit( 0 );
        }
    }   

    for ( size_t i = 0; i < wrThdNum; i++  )
    {
        err = pthread_create( &wrThd[i], NULL, thread_w, (void *)0 );
        if ( err != 0 )
        {
            printf( "create new thread error:%s\n", strerror( err ) );
            exit( 0 );
        }
    }

    for ( size_t i = 0; i < rdThdNum; i++  )
    {
        err = pthread_join( rdThd[i], NULL );
        if ( err != 0 )
        {
            printf( "wait thread done error:%s\n", strerror( err ) );
            exit( 1 );
        }
    }

    for ( size_t i = 0; i < wrThdNum; i++  )
    {
        err = pthread_join( wrThd[i], NULL );
        if ( err != 0 )
        {
            printf( "wait thread done error:%s\n", strerror( err ) );
            exit( 1 );
        }
    }


    gettimeofday( &endTime, NULL );

    float timeCost = ( endTime.tv_sec - startTime.tv_sec ) + 
        ( endTime.tv_usec - startTime.tv_usec ) / 1000000.0;

    fprintf( stdout, "time cost: %fs\n", timeCost );
    pthread_rwlock_destroy( &rwlock );
    exit(0);
}
