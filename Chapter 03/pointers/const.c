#include <stdio.h>

int main(void)
{
  int *             i_p   = 0;
  int const *       ic_p  = 0;
  int * const       i_pc  = 0;
  int const * const ic_pc = 0;

#if 0
  i_p = i_p;     // Ok, T * => T *
  ic_p = ic_p;   // Ok, T * => T * (T = U const)
  i_pc = i_pc;   // No! You cannot assign to T const
  ic_pc = ic_pc; // No! You cannot assign to T const
#endif

  //i_p = ic_p; // No! T const * => T *
  i_p = i_pc; // Ok, T * const => T *
  //i_p = ic_pc; // No, there is a T const * => T *

  ic_p = i_p;   // Ok, T * => T const *
  ic_p = i_pc;  // Ok, T const => T then U * => U const *
  ic_p = ic_pc; // Ok, T * const => T * (T = U const)

  // Another layour of indirection
  #if 0
    int **               i_p_p   = 0;
    int const **         ic_p_p  = 0;
    int * const *        i_pc_p  = 0;
    int const * const *  ic_pc_p = 0;
  #endif
  typedef int * T;
  typedef int const * U;
  T *        i_p_p   = 0;
  U *        ic_p_p  = 0;
  T const *  i_pc_p  = 0;
  U const *  ic_pc_p = 0;
  // plus the ones with const last, but they
  // are simply const versions and we learn
  // nothing new from them.

  // We do not have const pointers here, so we can only apply
  // T * => T * and T * => T const *, i.e. add const to the
  // pointed at object. If the pointed at objects have different
  // types then we cannot assign. Call the pointed at objects T and
  // U for the right-hand side and left-hand side, respectively

  // No, no and no
  // i_p_p = ic_p_p; No, T = int *, U = int const *, T != U
  // i_p_p = i_pc_p; No, T = int *, U = int * const, removing const not allowed
  // i_p_p = ic_pc_p; No, T = int *, U = int const * const, T != U

  // No, no and no
  // ic_p_p = i_p_p; No, T = int const *, U = int *, T != U
  // ic_p_p = i_pc_p; No, T = int const *, U = int * const, T != U
  // ic_p_p = ic_pc_p; No, removing const

  i_pc_p = i_p_p; // Ok, T const * => T *, T = int *, U = int *
  // i_pc_p = ic_p_p; No, T = int * const, U = int const *, T != U
  // i_pc_p = ic_pc_p; No, T = int const *, U = int * const, T != U

  // ic_pc_p = i_p_p; No, int const * const != int *
  ic_pc_p = ic_p_p; // Ok, Adding a const to int const *
  //ic_pc_p = i_pc_p; No, int const * const != int * const


  printf("to turn off warning... %p %p %p %p\n",
         (void *)i_p, (void *)ic_p, (void *)i_pc, (void *)ic_pc);
  printf("to turn off warning... %p %p %p %p\n",
         (void *)i_p_p, (void *)ic_p_p, (void *)i_pc_p, (void *)ic_pc_p);


  return 0;
}
