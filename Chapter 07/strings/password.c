#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool validate_password(char const *password)
{
  char valid_password = false;
  char buffer[10];

  printf("Password: ");
  gets(buffer);

  if (strcmp(password, buffer) == 0)
    valid_password = true;

  return valid_password;
}

int main(void)
{
  char const *password = "foobar";
  bool valid = validate_password(password);
  if (valid) {
    printf("The password is valid\n");
    return 1;
  } else {
    printf("The password is invalid\n");
    return 0;
  }
}
