#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "boost/thread/shared_mutex.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"
#include "boost/scoped_ptr.hpp"

int i=0;
boost::shared_mutex rwMutex;

typedef boost::shared_lock<boost::shared_mutex> ReadLock;
typedef boost::unique_lock<boost::shared_mutex> WriteLock;

void *thread_1( void *arg )
{
    int j;
    volatile int a;
    for ( j=0; j < 10000000; j++ )
    {
        ReadLock lock( rwMutex );
        a = i;
    }   
    return NULL;
}

void *thread_2( void *arg )
{
    int j;
    volatile int b;
    for ( j = 0; j < 10000000; j++ )
    {
        WriteLock lock( rwMutex );
        b = i;
    }   
    return NULL;
}

int main()
{
    int err;
    boost::scoped_ptr<boost::thread> th1;
    boost::scoped_ptr<boost::thread> th2;
    th1.reset( new boost::thread( boost::bind( &thread_1, (void*)0 ) ) );
    th2.reset( new boost::thread( boost::bind( &thread_2, (void*)0 ) ) );

    th1->join();
    th2->join();

    printf("i=%d\n",i);
    exit(0);
}
