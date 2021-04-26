#include <stdio.h>

int compact0(int n, int array[n])
{
  int m = 0;
  for (int i = 0; i < n; i++) {
    if (array[i] > 0)
      array[m++] = array[i];
  }
  return m;
}

int eratosthenes(int n, int buf[n - 2])
{
  // Init
  for (int i = 2; i < n; i++) {
    buf[i - 2] = i;
  }

  // Sieve
  for (int i = 0; i*i < n - 2; i++) {
    if (buf[i] == 0) continue;
    int p = buf[i];
    for (int j = p*p; j < n; j += p) {
      buf[j - 2] = 0;
    }
  }

  // Compact
  return compact0(n - 2, buf);
}

void sieve_candidates(int **from, int **to, int p)
{
  int *output = *from;
  for (int *input = *from ; input < *to; input++) {
    if (*input % p != 0)
      *output++ = *input;
  }
  *to = output;
}

int eratosthenes_(int n, int buf[n - 2])
{
  // Init
  for (int i = 2; i < n; i++) {
    buf[i - 2] = i;
  }

  // Sieve
  int *candidates = buf;
  int *end = buf + n - 2;
  while (candidates < end) {
    int p = *candidates++; // Get prime and move it
    sieve_candidates(&candidates, &end, p);
  }

  return end - buf;
}

int *sieve_candidates_(int *from, int *to, int p)
{
  int *output = from;
  for (int *input = from ; input < to; input++) {
    if (*input % p != 0)
      *output++ = *input;
  }
  return output;
}

int eratosthenes__(int n, int buf[n - 2])
{
  // Init
  for (int i = 2; i < n; i++) {
    buf[i - 2] = i;
  }

  // Sieve
  int *candidates = buf;
  int *end = buf + n - 2;
  while (candidates < end) {
    int p = *candidates++; // Get prime and move it
    end = sieve_candidates_(candidates, end, p);
  }

  return end - buf;
}

void sieve_candidates__(int **from, int **to)
{
  int p = *(*from)++;
  int *output = *from;
  for (int *input = *from ; input < *to; input++) {
    if (*input % p != 0)
      *output++ = *input;
  }
  *to = output;
}

int eratosthenes___(int n, int buf[n - 2])
{
  // Init
  for (int i = 2; i < n; i++) {
    buf[i - 2] = i;
  }

  // Sieve
  int *candidates = buf;
  int *end = buf + n - 2;
  while (candidates < end) {
    sieve_candidates__(&candidates, &end);
  }

  return end - buf;
}

void print_array(int *begin, int *end)
{
  while (begin < end) {
    printf("%d ", *begin++);
  }
  printf("\n");
}

int main(void)
{
  int n = 100;
  int buf[n - 2];

  int m = eratosthenes(n, buf);
  print_array(buf, buf + m);
  m = eratosthenes_(n, buf);
  print_array(buf, buf + m);
  m = eratosthenes__(n, buf);
  print_array(buf, buf + m);
  m = eratosthenes___(n, buf);
  print_array(buf, buf + m);

  return 0;
}
