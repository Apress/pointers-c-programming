#include <stdio.h>
#include <stdlib.h>

enum button_events {
  MOUSEDOWN, MOUSEUP,
  CLICKED, DBLCLICKED,
  // more...
};
struct button;
typedef void (*button_cb)(struct button *,
                          enum button_events);

struct button {
  char *text;
  // lots of gui stuff here
  button_cb cb_func;
};

struct button *new_button         (char *text);
void           set_button_callback(struct button *b,
                                   button_cb cb_funct);


struct button *new_button(char *text)
{
  struct button *b = malloc(sizeof *b);
  if (b) {
    b->text = text;
    b->cb_func = 0;
    // ignore the rest
  }
  return b;
}

void install_button_callback(struct button *b,
                             button_cb cb_func)
{
  b->cb_func = cb_func;
}

void click_button(struct button *b)
{
  // do whatever GUI thing you want...
  if (b->cb_func)
    b->cb_func(b, CLICKED);
}

void my_callback(struct button *but,
                 enum button_events event)
{
  switch (event) {
    case CLICKED:
      printf("button %s was clicked\n", but->text);
      break;
    default:
      // nothing
      break;
  }
}

int main(void)
{
  struct button *but = new_button("my button");
  install_button_callback(but, my_callback);
  click_button(but);

  free(but);

  return 0;
}
