#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <vector>

#include "fun.hpp" // Cosine( vec1, vec2 )

using namespace std;

volatile float rst = 0;
pthread_mutex_t mutex;
vector<float> vec1, vec2;
int d = 1000*1000;

void *thread_r( void *arg )
{
    for ( int i = 0; i < 1; i++ )
    {
        pthread_mutex_lock( &mutex );
        
        rst = Cosine( vec1, vec2 );
        
        pthread_mutex_unlock( &mutex );
    }   
    return NULL;
}

void *thread_w( void *arg )
{
    srand( time(NULL) );
    for ( int i = 0; i < 1; i++ )
    {
        pthread_mutex_lock( &mutex );

        vec1.clear(), vec2.clear();
        vec1.reserve(d), vec2.reserve(d);
        
        for ( int j = 0; j < d; j++ )
        {
            vec1.push_back( rand() % d );
            vec2.push_back( rand() % d );    
        }
        
        pthread_mutex_unlock( &mutex );
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

    srand(time(NULL));
    for ( int j = 0; j < d; j++ )
    {
        vec1.push_back( rand() % d );
        vec2.push_back( rand() % d );    
    }
        
    pthread_t rdThd[rdThdNum], wrThd[wrThdNum];
    pthread_mutex_init( &mutex, NULL );

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

    fprintf( stdout, "rst=%f, time cost: %fs\n", rst, timeCost );
    pthread_mutex_destroy( &mutex );
    exit( 0 );
}
