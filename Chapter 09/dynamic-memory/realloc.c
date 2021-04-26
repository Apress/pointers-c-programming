#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
  int *ip = malloc(10 * sizeof *ip);
  if (!ip) {
    // handle error
  }

  int *new_ip = realloc(ip, 100 * sizeof *new_ip);
  if (!new_ip) {
    // handle error
  }
  ip = new_ip;

  free(ip);

  size_t old_size = 10 * sizeof *ip;
  size_t new_size = 10 * old_size;
  ip = malloc(old_size);
  if (!ip) exit(1);

  // ip = realloc(ip, new_size)
  new_ip = malloc(new_size);
  memmove(new_ip, ip, old_size);
  free(ip);
  ip = new_ip;


  free(ip);

  return 0;
}
