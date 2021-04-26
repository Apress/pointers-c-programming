#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN_BUF_SIZE 1024

struct gap_buf {
  size_t size;
  size_t cursor;
  size_t gap_end;
  char *buffer;
};

struct gap_buf *new_buffer(size_t init_size)
{
  struct gap_buf *buf = malloc(sizeof *buf);
  if (!buf) return 0;
  buf->buffer = malloc(MAX(init_size, MIN_BUF_SIZE));
  if (!buf->buffer) {
    free(buf);
    return 0;
  }
  buf->size = init_size;
  buf->cursor = 0;
  buf->gap_end = init_size;
  return buf;
}

void free_buffer(struct gap_buf *buf)
{
  if (!buf) return;
  free(buf->buffer);
  free(buf);
}

#define gb_front(buf) ((buf)->cursor)
#define gb_back(buf)  ((buf)->size - (buf)->gap_end)
#define gb_used(buf)  (gb_front(buf) + gb_back(buf))

void move_back(struct gap_buf *buf,
               char *new_buf, size_t new_size)
{
  memmove(new_buf + new_size - gb_back(buf),
          buf->buffer + buf->gap_end,
          gb_back(buf));
}

void shrink_buffer(struct gap_buf *buf,
                   size_t new_size)
{
  // We do not resize if we lose data!
  if (new_size < gb_used(buf)) return;

  // Move the segment to the right of the cursor
  move_back(buf, buf->buffer, new_size);

  // Update struct -- set end before updating
  // the size or gb_back(buf) will be wrong
  buf->gap_end = new_size - gb_back(buf);
  buf->size = new_size;

  // Allocate a smaller buffer
  new_size = MAX(new_size, MIN_BUF_SIZE);
  char *new_buf = realloc(buf->buffer, new_size);
  if (new_buf) buf->buffer = new_buf;
}

bool grow_buffer(struct gap_buf *buf, size_t new_size)
{
  // We cannot grow to a smaller (or equal) size
  if (buf->size >= new_size) return false;

  // Allocate a larger buffer
  new_size = MAX(new_size, MIN_BUF_SIZE);
  char *new_buf = realloc(buf->buffer, new_size);
  if (!new_buf) return false;

  // Move the segment to the right of the cursor
  move_back(buf, new_buf, new_size);

  // Update struct -- set end before updating
  // the size or gb_back(buf) will be wrong
  buf->buffer = new_buf;
  buf->gap_end = new_size - gb_back(buf);
  buf->size = new_size;

  return true;
}


#define capped_dbl_size(s) \
  ((s) < SIZE_MAX / 2) ? (2 * (s)) : SIZE_MAX

bool insert_character(struct gap_buf *buf, char c)
{
  if (buf->cursor == buf->gap_end) {
    size_t new_size = capped_dbl_size(buf->size);
    bool grow_success = grow_buffer(buf, new_size);
    if (!grow_success) return false;
  }

  buf->buffer[buf->cursor++] = c;
  return true; // success
}

void cursor_left(struct gap_buf *buf)
{
  if (buf->cursor > 0) {
    buf->buffer[--buf->gap_end] =
        buf->buffer[--buf->cursor];
  }
}

void cursor_right(struct gap_buf *buf)
{
  if (buf->gap_end < buf->size) {
    buf->buffer[buf->cursor++] =
      buf->buffer[buf->gap_end++];
  }
}

void backspace(struct gap_buf *buf)
{
  if (buf->cursor > 0)
    buf->cursor--;
  if (gb_used(buf) < buf->size / 4)
    shrink_buffer(buf, buf->size / 2);
}

void delete(struct gap_buf *buf)
{
  if (buf->gap_end < buf->size)
    buf->gap_end++;
  if (gb_used(buf) < buf->size / 4)
    shrink_buffer(buf, buf->size / 2);
}

char *extract_text(struct gap_buf *buf)
{
  // Yes, it is insanely unlikely to happen, but if it does
  // then we do not have space for the zero terminal...
  if (SIZE_MAX == gb_used(buf)) return 0;

  char *string = malloc(gb_used(buf) + 1);
  if (!string) return 0;

  strncpy(string, buf->buffer, buf->cursor);
  strncpy(string + buf->cursor,
          buf->buffer + buf->gap_end,
          buf->size - buf->gap_end);
  string[gb_used(buf)] = '\0';
  return string;
}


int main(void)
{
  struct gap_buf *buf = new_buffer(2);
  if (!buf) {
    perror("Couldn't allocate buffer");
    exit(1);
  }

  char *x = 0;

  insert_character(buf, 'f');
  insert_character(buf, 'o');
  insert_character(buf, 'o');
  x = extract_text(buf);
  printf("%s\n", x);
  free(x);

  cursor_left(buf);
  insert_character(buf, 'x');
  x = extract_text(buf);
  printf("%s\n", x);
  free(x);
  cursor_left(buf);
  cursor_left(buf);
  insert_character(buf, 'y');
  cursor_right(buf);
  cursor_right(buf);
  cursor_right(buf);
  cursor_right(buf);
  insert_character(buf, 'z');
  x = extract_text(buf);
  printf("%s\n", x);
  free(x);

  backspace(buf);
  x = extract_text(buf);
  printf("%s\n", x);
  free(x);
  cursor_left(buf);
  cursor_left(buf);
  delete(buf);
  x = extract_text(buf);
  printf("%s\n", x);
  free(x);

  free_buffer(buf);

  return 0;
}
