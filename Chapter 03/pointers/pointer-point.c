#include <stdio.h>

typedef struct point {
  double x, y;
} point;

void move_point_horisontally(point *p, double amount)
{
  p->x += amount;
}

void move_point_vertically(point *p, double amount)
{
  p->y += amount;
}

void move_point(point *p, double delta_x, double delta_y)
{
  move_point_horisontally(p, delta_x);
  move_point_vertically(p, delta_y);
}

void print_point(point *p)
{
  printf("point <%.2f, %.2f>\n", p->x, p->y);
}

typedef struct rectangle {
  point upper_left;
  point lower_right;
} rectangle;

void move_rectangle(rectangle *rect,
                    double delta_x,
                    double delta_y)
{
  move_point(&rect->upper_left, delta_x, delta_y);
  move_point(&rect->lower_right, delta_x, delta_y);
}

void print_rectangle(rectangle *rect)
{
  printf("rectangle:\n");
  print_point(&rect->upper_left);
  print_point(&rect->lower_right);
  printf("\n");
}

int main(void)
{
  point p = { .x = 0.0, .y = 0.0 };
  print_point(&p);

  move_point(&p, 10, 10);
  print_point(&p);
  printf("\n");

  rectangle rect = {
    .upper_left =  {.x =  0.0, .y = 10.0},
    .lower_right = {.x = 10.0, .y =  0.0}
  };
  print_rectangle(&rect);

  move_rectangle(&rect, 10, 10);
  print_rectangle(&rect);

  return 0;
}
