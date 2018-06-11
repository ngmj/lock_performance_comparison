# lock_performance_comparison
pthread锁 读写锁 boost 区域锁 读写所性能对比


1、raw_lock.cpp raw_rwlock.cpp 
为对比互斥锁和读写锁 的锁耗时（加完立即释放，读线程1个，写线程1个）
结论：互斥锁加解锁的耗时要比读写锁的快（1、raw_lock耗时1.832699s；2、raw_rwlock耗时3.620338s）

2、simple_lock.cpp simple_rwlock.cpp 
对一段临界区进行操作时，读写锁的性能对比（读线程100个，写线程1个）
结论：如果临界区是一个简单的代码，比如简单的一个赋值操作，那么互斥锁的性能要高于读写锁（1、simple_lock耗时1.221884s；2、simple_rwlock耗时2.235480s）
      如果临界区是一个map的查找，虽然只有20个kv，但是读写锁性能也要高于互斥锁（1、simple_lock耗时14.058138s；2、simple_rwlock耗时9.445691s）

3、lock.cpp rwlock.cpp
对一段临界区进行操作时，读写锁的性能对比（读线程100个，写线程1个）
结论：由于临界区是个比较耗时的操作，计算两个向量的余弦相似度，故读写锁性能要远高于互斥锁（1、lock耗时2.925601s；2、rwlock耗时0.320296s）