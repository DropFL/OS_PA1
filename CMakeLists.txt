add_executable( main main.c MFQ.c queue.c scheduler.c process.c option.c )
add_executable( proc_test option.c process.c proc_test.c )