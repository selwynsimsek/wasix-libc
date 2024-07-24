#include <unistd.h>
#include <signal.h>
#include "syscall.h"
#ifdef __wasilibc_unmodified_upstream
#else
#include <wasi/api.h>
#endif
#include "libc.h"
#include "lock.h"
//#include "pthread_impl.h"
#include "aio_impl.h"

static void dummy(int x) { }
weak_alias(dummy, __aio_atfork);

pid_t _Fork(int copy_mem)
{
	pid_t ret;
	sigset_t set;
	__block_all_sigs(&set);
	__aio_atfork(-1);
	LOCK(__abort_lock);
#ifdef __wasilibc_unmodified_upstream
#ifdef SYS_fork
	ret = __syscall(SYS_fork);
#else
	ret = __syscall(SYS_clone, SIGCHLD, 0);
#endif
#else
	__wasi_pid_t pid = -1;
    int err = __wasi_proc_fork(copy_mem, &pid);
	if (err != 0) {
		ret = -err;
	} else {
		ret = (int)pid;
	}
#endif
return ret;
}
