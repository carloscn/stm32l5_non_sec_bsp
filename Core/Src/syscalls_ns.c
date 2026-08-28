/**
 * @file syscalls_ns.c
 * @brief Minimal newlib syscall stubs for the TF-M NS build.
 *
 * newlib-nano pulls weak _write/_read/_sbrk from nosys.specs which emit
 * "not implemented" linker warnings; the TF-M NS toolchain turns warnings
 * fatal (-Wl,--fatal-warnings). Provide real (no-op) stubs so the link is
 * clean. All console output goes through hal_uart / osal_log, not stdio.
 */
#if defined(TFM_NS)

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

__attribute__((used)) int _write(int fd, const char *buf, int len)
{
    (void)fd; (void)buf;
    return len;                 /* pretend everything was written */
}

__attribute__((used)) int _read(int fd, char *buf, int len)
{
    (void)fd; (void)buf; (void)len;
    return 0;                   /* EOF */
}

__attribute__((used)) int _close(int fd)          { (void)fd; return -1; }
__attribute__((used)) int _lseek(int fd, int p, int w) { (void)fd; (void)p; (void)w; return 0; }
__attribute__((used)) int _fstat(int fd, struct stat *st) { (void)fd; st->st_mode = S_IFCHR; return 0; }
__attribute__((used)) int _isatty(int fd)         { (void)fd; return 1; }
__attribute__((used)) int _getpid(void)           { return 1; }
__attribute__((used)) int _kill(int pid, int sig) { (void)pid; (void)sig; errno = EINVAL; return -1; }

/* heap is FreeRTOS heap_4; _sbrk should never be called, but define it safely. */
__attribute__((used)) void *_sbrk(int incr)
{
    (void)incr;
    errno = ENOMEM;
    return (void *)-1;
}

#endif /* TFM_NS */
