#include <stdio.h>

typedef struct point {
  double x, y;
} point;

point move_point_horisontally(point p, double amount)
{
  p.x += amount;
  return p;
}

point move_point_vertically(point p, double amount)
{
  p.y += amount;
  return p;
}

point move_point(point p, double delta_x, double delta_y)
{
  p = move_point_horisontally(p, delta_x);
  p = move_point_vertically(p, delta_y);
  return p;
}

void print_point(point p)
{
  printf("point <%.2f, %.2f>\n", p.x, p.y);
}

typedef struct rectangle {
  point upper_left;
  point lower_right;
} rectangle;

rectangle move_rectangle(rectangle rect,
                         double delta_x,
                         double delta_y)
{
  rect.upper_left =
    move_point(rect.upper_left, delta_x, delta_y);
  rect.lower_right =
    move_point(rect.lower_right, delta_x, delta_y);
  return rect;
}

void print_rectangle(rectangle rect)
{
  printf("rectangle:\n");
  print_point(rect.upper_left);
  print_point(rect.lower_right);
  printf("\n");
}

int main(void)
{
  point p = { .x = 0.0, .y = 0.0 };
  print_point(p);

  p = move_point(p, 10, 10);
  print_point(p);
  printf("\n");

  rectangle rect = {
    .upper_left =  {.x =  0.0, .y = 10.0},
    .lower_right = {.x = 10.0, .y =  0.0}
  };
  print_rectangle(rect);

  rect = move_rectangle(rect, 10, 10);
  print_rectangle(rect);

  return 0;
}
