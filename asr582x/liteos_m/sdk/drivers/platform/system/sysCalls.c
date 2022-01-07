#include <wchar.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <reent.h>
#include "duet_cm4.h"
#include "printf_uart.h"
/////////////////////////////////////

char *itoa(char *p, unsigned x) {
	p += 3*sizeof(int);
	*--p = 0;
	do {
		*--p = '0' + x % 10;
		x /= 10;
	} while (x);
	return p;
}

const char *__locale_ctype_ptr (void)
{
    return "1.0.0-LiteOS_M";
}

#if 0
_ssize_t _write_r(struct _reent *ptr, int fd, const void *buf, size_t nbytes)
{
    const char *tmp = buf;
    int         i   = 0;

    if (buf == NULL)
    {
        return 0;
    }

    if ((fd == 1) || (fd == 2))
    {
        for (i = 0; i < nbytes; i++)
        {
            uart_put_char(*tmp);
            tmp++;
        }
        return nbytes;
    }
    else
    {
        return -1;
    }
}
 ///////////////////////////////////
 #if 1
 int _isatty (int fd)
 {
     return 1;
 }
 
 /////////////////////////////////
 int _lseek(int i, int k, int j)
 {
      return -1;
 }
 #endif
 ////////////////////////////////
 int _read (int fd, char *pBuffer, int size)
 {
     return size;
 }
 ////////////////////////////////////
 int _fstat (int fd, struct stat *pStat)
 {
      pStat->st_mode = S_IFCHR;
      return 0;
 }
 ////////////////////////////////////
 #if 1
 int _close(int i)
 {
      return -1;
 }
#endif
#if 0
caddr_t _sbrk(int increment)
{
      extern char end __asm__("end");
      register char *pStack __asm__("sp");

      static char *s_pHeapEnd;

      if (!s_pHeapEnd)
            s_pHeapEnd = &end;

      if (s_pHeapEnd + increment > pStack)
           return (caddr_t)-1;

      char *pOldHeapEnd = s_pHeapEnd;
      s_pHeapEnd += increment;
      return (caddr_t)pOldHeapEnd;
 }
 #endif
int _kill(int pid, int sig)
{
        //errno = EINVAL;
        return -1;
}
int _getpid(void)
{
	return -1;
}
int _open_r(struct _reent *ptr, const char *file, int flags, int mode)
{
    return -1;
}

__attribute__((weak)) void _exit (int status)
{
        _kill(status, -1);
        while (1) {}            /* Make sure we hang here */
}

#if 0
//void _sbrk_r(void)
//{
//}

//void _close_r(void)
//{
//}

//void _lseek_r(void)
//{
//}

//void _read_r(void)
//{
//}
#endif
#endif
 /************end *************************/