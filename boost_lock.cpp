#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <boost/thread/mutex.hpp>
#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"
#include "boost/scoped_ptr.hpp"

int i=0;
boost::mutex boost_mutex;

void *thread_1( void *arg )
{
    int j;
    volatile int a;
    for ( j=0; j < 10000000; j++ )
    {
        boost::mutex::scoped_lock lock( boost_mutex );
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
        boost::mutex::scoped_lock lock( boost_mutex );
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
