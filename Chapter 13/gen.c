#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/* Windows would use:
void *alloc_code_block(size_t size)
{
  DWORD type = MEM_RESERVE | MEM_COMMIT;
  return VirtualAlloc(NULL, size, type, PAGE_READWRITE);
}

void *set_exec_prot(void *buf, size_t size)
{
  DWORD old;
  BOOL res = VirtualProtect(buf, size, PAGE_EXECUTE_READ, &old);
  return res ? buf : 0;
}

void free_code_block(void *buf, size_t size)
{
  // The function can fail, but we ignore it...
  VirtualFree(buf, 0, MEM_RELEASE);
}
*/


#include <sys/mman.h>

// allocate page-aligned memory with mmap()
void *alloc_code_block(size_t size)
{
  int protection = PROT_WRITE;
  // MAP_ANONYMOUS not POSIX but necessary on some systems
  int flags = MAP_ANONYMOUS | MAP_PRIVATE;
  char *buf = mmap(0, size, protection, flags, -1, 0);
  return (buf != MAP_FAILED) ? buf : 0;
}

void *set_exec_prot(void *buf, size_t size)
{
  int res = mprotect(buf, size, PROT_READ | PROT_EXEC);
  if (res == -1) {
    // munmap can fail, but there is nothing we
    // can do about it here...
    munmap(buf, size);
    return 0;
  }
  return buf;
}

void free_code_block(void *buf, size_t size)
{
  // munmap can fail, but there is nothing we
  // can do about it here...
  munmap(buf, size);
}




int main(void)
{
  // Adds two to its input and returns
  unsigned char code[] = {
    0x8d, 0x47, 0x02,     // lea eax,[rdi+0x2]
    0xc3                  // ret
  };
  /*
  Solaris, Linux, FreeBSD and macOS uses the System V AMD64 ABI
  where the first integer/pointer argument comes in register rdi.
  On windows, it would come in rcx.
  */

  // Raw memory...
  void *code_block = alloc_code_block(sizeof code);
  if (!code_block) abort();
  memcpy(code_block, code, sizeof code);
  code_block = set_exec_prot(code_block, sizeof code);
  if (!code_block) abort();

  int (*f)(int) = (int (*)(int))code_block;
  printf("%d\n", f(3));
  free_code_block(code_block, sizeof code);

  return 0;
}
