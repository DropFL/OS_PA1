add_executable( main main.c MFQ.c queue.c scheduler.c process.c option.c )
add_executable( proc_test proc_test.c process.c option.c )
add_executable( queue_test queue_test.c queue.c scheduler.c process.c option.c )