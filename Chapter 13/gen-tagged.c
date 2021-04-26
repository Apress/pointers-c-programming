#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <sys/mman.h>
void *alloc_code_bock(size_t size)
{
  // allocate page-aligned memory with mmap():
  char *buf = mmap(
    0, size,
    PROT_WRITE, MAP_ANON | MAP_PRIVATE,
    -1, 0
  );
  if (buf == MAP_FAILED) return 0; // NULL for error
  else return buf;
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



// You can get PAGESIZE from POSIX sysconf(_SC_PAGESIZE);
// from sysconf() from <unistd.h>
#define PAGESIZE 4096

#define LOW_BITS 0
#if LOW_BITS
// You can get the number of free bits from
// POSIX ffs() from <strings.h>  as ffs(page_size) - 1;
#define CODE_SIZE_BITS  12
#define CODE_SIZE_MASK  ((1ull << CODE_SIZE_BITS) - 1)
#define MAX_CODE_PAGES  CODE_SIZE_MASK
#define CODE_PTR_MASK   (~CODE_SIZE_MASK)

#include <stdint.h>
#define jit_ptr(f,s)    (void *)((uint64_t)f | s)
#define jit_pages(p)    ((uint64_t)p & CODE_SIZE_MASK)
// using compiler extension __typeof__ for cast
#define jit_func(p)     ((__typeof__(p))((uint64_t)p & CODE_PTR_MASK))

#else // HIGH_BITS

#define CODE_SIZE_BITS  16
#define MAX_CODE_PAGES  ((1ull << CODE_SIZE_BITS) - 1)
#define CODE_PTR_MASK   ((1ull << 48) - 1)
#define CODE_SIZE_MASK  (~CODE_PTR_MASK)

#include <stdint.h>
#define jit_ptr(f,s) \
  (void *)(((uint64_t)f & CODE_PTR_MASK) | (s << 48))
#define jit_pages(p) \
  (((uint64_t)p & CODE_SIZE_MASK) >> 48)

#define upper_bits(p) \
  ~(((uint64_t)p & (1ull << 47)) - 1) // upper 16 set if 47 set
#define lower_bits(p) \
  ((uint64_t)p & CODE_PTR_MASK)
#define canonical_ptr(p) \
   (lower_bits(p) | upper_bits(p))

// using compiler extension __typeof__ for cast
#define jit_func(p) \
    ((__typeof__(p))(canonical_ptr(p)))

#endif

// to avoid function/void pointer warnings
#define jit_free(p) jit_free_void((void*)(p))
void jit_free_void(void *p)
{
  free_code_block((void *)jit_func(p), jit_pages(p));
}


void *create_exec_buf(unsigned char const *code, size_t size)
{
  size_t pages = (size + PAGESIZE - 1) / PAGESIZE;
  if (pages > MAX_CODE_PAGES) {
    // some meaningful error handling here...
    // but we are talking about allocating a single
    // code block with 16+ million bytes
    return 0;
  }
  size_t alloc_size = PAGESIZE * pages;

  // allocate page-aligned memory with mmap():
  char *buf = alloc_code_bock(alloc_size);
  if (!buf) return 0;
  memcpy(buf, code, size);
  buf = set_exec_prot(buf, alloc_size);
  if (!buf) return 0;
  return jit_ptr(buf, pages);
}


// Example functions------------------------------------------
typedef int (*ii_fn)(int);
ii_fn adder(int j) // will make int f(int x) { return x + j; }
{
  unsigned char code[] = {
    // lea eax,[rdi+<j>] (0x87 because we use 32-bit int now)
    // j starts at index 2...
    0x8d, 0x87, 0x00, 0x00, 0x00, 0x00,
    // ret
    0xc3
  };
  // the int starts at index 2 and goes in the next four
  // bytes, little endian...
  unsigned char *j_code = code + 2;
  for (int offset = 0; offset < 4; offset++) {
    j_code[offset] = (j >> 8 * offset) & 0xff;
  }

  return (ii_fn)create_exec_buf(code, sizeof code);
}

typedef int (*iii_fn)(int, int);
iii_fn add_mod_k(int k) // int f(int x, int y) { return (x + y) % k; }
{
  assert(k > 0);
  unsigned char code[] = {
    // add the two arguments
    0x8d, 0x04, 0x37,               // lea    eax,[rdi+rsi*1]

    // clear divident
    0xba, 0x00, 0x00, 0x00, 0x00,   // mov    edx, 0x0

    // divide eax by edi
    // (k starts at offset 9)
    0xb9, 0x00, 0x00, 0x00, 0x00,  // mov    edi,<k>
    0xf7, 0xf1,                    // div    ecx

    // remainder (in edx) to eax register
    0x89, 0xd0,                    // mov    eax,edx
    // and return...
    0xc3                           // ret
  };

  // the int starts at index 2 and goes in the next four
  // bytes, little endian...
  unsigned char *k_code = code + 9;
  for (int offset = 0; offset < 4; offset++) {
    k_code[offset] = (k >> 8 * offset) & 0xff;
  }

  return (iii_fn) create_exec_buf(code, sizeof code);
}


int main(void)
{
  // Using a structure wrapper
  ii_fn add2 = adder(2);
  ii_fn add5 = adder(5);
  for (int i = 0; i < 5; i++) {
    printf("%d, %d, %d\n", i,
    jit_func(add2)(i), jit_func(add5)(i));
  }
  jit_free(add2);
  jit_free(add5);

  iii_fn add_mod2 = add_mod_k(2);
  iii_fn add_mod5 = add_mod_k(5);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      printf("(%d + %d) mod k = %d (k=2) %d (k=5)\n",
            i, j,
            jit_func(add_mod2)(i, j),
            jit_func(add_mod5)(i, j));
    }
  }
  jit_free(add_mod2);
  jit_free(add_mod5);

  return 0;
}
