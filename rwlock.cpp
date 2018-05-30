#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int i=0;
pthread_rwlock_t rwlock;

void *thread_1( void *arg )
{
    int j;
    volatile int a;
    for ( j = 0; j< 10000000; j++ )
    {
        pthread_rwlock_rdlock(&rwlock);
        a = i;
        pthread_rwlock_unlock(&rwlock);
    }   
    return NULL;
}

void *thread_2( void *arg )
{
    int j;
    volatile int b;
    for ( j = 0; j < 10000000; j++ )
    {
        pthread_rwlock_wrlock(&rwlock);
        b = i;
        pthread_rwlock_unlock(&rwlock);
    }   
    return NULL;
}

int main()
{
    int err;
    pthread_t th1,th2;
    pthread_rwlock_init( &rwlock, NULL );
    err = pthread_create( &th1, NULL, thread_1, (void *)0 );
    if ( err != 0 )
    {
        printf( "create new thread error:%s\n", strerror( err ) );
        exit( 0 );
    }   
    err = pthread_create( &th2, NULL, thread_2, (void *)0 );
    if ( err != 0 )
    {
        printf( "create new thread error:%s\n", strerror( err ) );
        exit( 0 );
    }

    err = pthread_join( th1, NULL );
    if ( err != 0 )
    {
        printf( "wait thread done error:%s\n", strerror( err ) );
        exit( 1 );
    }
    err = pthread_join( th2, NULL );
    if ( err != 0 )
    {
        printf( "wait thread done error:%s\n", strerror( err ) );
        exit( 1 );
    }

    printf( "i=%d\n", i );
    pthread_rwlock_destroy( &rwlock );
    exit(0);
}
