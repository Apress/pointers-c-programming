#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef void * cls;
typedef struct obj { cls *cls; } obj;

#define basetype(x, base) ((base *)x)
#define vtbl(inst, base)  basetype(((obj *)inst)->cls, base)

void *alloc_cls(size_t cls_size)
{
  cls *cls = malloc(cls_size);
  if (!cls) abort(); // error handling
  return cls;
}
#define INIT_CLS(p, init)     \
do {                          \
  if (!p) {                   \
    p = alloc_cls(sizeof *p); \
    init(p);                  \
  }                           \
} while(0)

void *alloc_obj(size_t obj_size, cls cls)
{
  obj *obj = malloc(obj_size);
  if (!obj) abort(); // error handling
  obj->cls = cls;
  return obj;
}
#define NEW_OBJ(p, cls) alloc_obj(sizeof *p, cls)


// Expressions .................

typedef struct base_expr *EXP;

// Base class, class definition
typedef struct base_expr_cls {
  void   (*print)(EXP);
  double (*eval) (EXP);
} base_expr_cls;

// dynamic dispatch
void   print(EXP e) { vtbl(e, base_expr_cls)->print(e); }
double eval (EXP e) { return vtbl(e, base_expr_cls)->eval(e); }

void init_base_expr_cls(base_expr_cls *cls)
{
  cls->print = 0; // abstract method
  cls->eval  = 0; // abstract method
}

// Base class, object definition
typedef struct base_expr { obj obj; } base_expr;
// Base class, methods (init does nothing)
void init_base_expr(base_expr *inst) {}


// Value expressions
typedef struct value_expr_cls {
  base_expr_cls base_expr_cls;
} value_expr_cls;
typedef struct value_expr {
  base_expr base_expr;
  double value;
} value_expr;

// Concrete class, so must have a struct
value_expr_cls *VALUE_EXPR_CLS = 0; // must be initialised

void value_expr_print(EXP val)
{
  printf("%.3f", ((value_expr *)val)->value);
}

double value_expr_eval(EXP val)
{
  return ((value_expr *)val)->value;
}

void init_value_expr_cls(value_expr_cls *cls)
{
  init_base_expr_cls(basetype(cls, base_expr_cls));
  // override virtual functions
  base_expr_cls *base_expr = basetype(cls, base_expr_cls);
  base_expr->print = value_expr_print;
  base_expr->eval  = value_expr_eval;
}

void init_value_expr(value_expr *val, double value)
{
  init_base_expr(basetype(val, base_expr));
  val->value = value;
}

// constructor
EXP value(double value)
{
  INIT_CLS(VALUE_EXPR_CLS, init_value_expr_cls);
  value_expr *val = NEW_OBJ(val, VALUE_EXPR_CLS);
  init_value_expr(val, value);
  return (EXP)val;
}

// Binary expressions
typedef struct binexpr_cls {
  base_expr_cls base_expr_cls;
} binexpr_cls;
typedef struct binexpr {
  base_expr base_expr;
  char symb; EXP left, right;
} binexpr;

void print_binexpr(EXP exp)
{
  binexpr *binop = (binexpr *)exp;
  putchar('('); print(binop->left); putchar(')');
  putchar(binop->symb);
  putchar('('); print(binop->right); putchar(')');
}

void init_binexpr_cls(binexpr_cls *cls)
{
  init_base_expr_cls(basetype(cls, base_expr_cls));
  base_expr_cls *base_expr = basetype(cls, base_expr_cls);
  base_expr->print = print_binexpr;
}

void init_binexpr(binexpr *binop, char symb,
                  EXP left, EXP right)
{
  init_base_expr(basetype(binop, base_expr));
  binop->symb = symb;
  binop->left = left;
  binop->right = right;
}

// Addition
typedef struct add_expr_cls {
  binexpr_cls binexpr_cls;
} add_expr_cls;
typedef struct add_expr {
  binexpr binexpr;
} add_expr;

// Concrete class, so must have a struct
add_expr_cls *ADD_EXPR_CLS = 0; // must be initialised

double eval_add_expr(EXP expr)
{
  binexpr *base = basetype(expr, binexpr);
  return eval(base->left) + eval(base->right);
}

void init_add_expr_cls(add_expr_cls *cls)
{
  init_binexpr_cls(basetype(cls, binexpr_cls));
  base_expr_cls *base_expr = basetype(cls, base_expr_cls);
  base_expr->eval = eval_add_expr;
}

void init_add_expr(add_expr *expr, EXP left, EXP right)
{
  init_binexpr(basetype(expr, binexpr), '+', left, right);
}

// constructor
EXP add(EXP left, EXP right)
{
  INIT_CLS(ADD_EXPR_CLS, init_add_expr_cls);
  add_expr *expr = NEW_OBJ(expr, ADD_EXPR_CLS);
  init_add_expr(expr, left, right);
  return (EXP)expr;
}

// Subtraction
typedef struct sub_expr_cls {
  binexpr_cls binexpr_cls;
} sub_expr_cls;
typedef struct sub_expr {
  binexpr binexpr;
} sub_expr;

// Concrete class, so must have a struct
sub_expr_cls *SUB_EXPR_CLS = 0; // must be initialised

double eval_sub_expr(EXP expr)
{
  binexpr *base = basetype(expr, binexpr);
  return eval(base->left) - eval(base->right);
}

void init_sub_expr_cls(sub_expr_cls *cls)
{
  init_binexpr_cls(basetype(cls, binexpr_cls));
  base_expr_cls *base_expr = basetype(cls, base_expr_cls);
  base_expr->eval = eval_sub_expr;
}

void init_sub_expr(add_expr *expr, EXP left, EXP right)
{
  init_binexpr(basetype(expr, binexpr), '-', left, right);
}

// constructor
EXP sub(EXP left, EXP right)
{
  INIT_CLS(SUB_EXPR_CLS, init_sub_expr_cls);
  add_expr *expr = NEW_OBJ(expr, SUB_EXPR_CLS);
  init_sub_expr(expr, left, right);
  return (EXP)expr;
}

// Variables
typedef struct var_expr *VAR;
typedef struct var_expr_cls {
  base_expr_cls base_expr_cls;
  void (*bind)  (VAR var, EXP val);
  void (*unbind)(VAR var);
} var_expr_cls;
typedef struct var_expr {
  base_expr base_expr;
  char const *name;
  double value;
} var_expr;

// new virtual functions
void bind(VAR var, EXP e) { vtbl(var, var_expr_cls)->bind(var, e); }
void unbind(VAR var)      { vtbl(var, var_expr_cls)->unbind(var); }

// implementations of new virtual functions
void var_expr_bind  (VAR var, EXP e) { var->value = eval(e); }
void var_expr_unbind(VAR var)        { var->value = NAN; }

// Concrete class, so must have a struct
var_expr_cls *VAR_EXPR_CLS = 0; // must be initialised

// overriding virtual functions
void var_expr_print(EXP expr)
{
  VAR var = (VAR)expr;
  if (isnan(var->value)) { // isnan from <math.h>
    printf("%s", var->name);
  } else {
    printf("%f", var->value);
  }
}

double var_expr_eval(EXP expr)
{
  VAR var = (VAR)expr;
  return var->value;
}

void init_var_expr_cls(var_expr_cls *cls)
{
  init_base_expr_cls(basetype(cls, base_expr_cls));
  // override virtual functions
  base_expr_cls *base_expr = basetype(cls, base_expr_cls);
  base_expr->print = var_expr_print;
  base_expr->eval  = var_expr_eval;
  // new virtual functions
  cls->bind = var_expr_bind;
  cls->unbind = var_expr_unbind;
}

void init_var_expr(var_expr *var, char const *name)
{
  init_base_expr(basetype(var, base_expr));
  var->name = name;
  var->value = NAN; // NAN from <math.h>
}

// constructor
VAR var(char const *name)
{
  INIT_CLS(VAR_EXPR_CLS, init_var_expr_cls);
  VAR var = NEW_OBJ(var, VAR_EXPR_CLS);
  init_var_expr(var, name);
  return var;
}

int main(void)
{
  VAR x = var("x");
  EXP expr = add(value(1.0), sub((EXP)x, value(2.0)));
  // prints 'x' for x and evaluates to nan
  print(expr); putchar('\n');
  printf("evaluates to %f\n", eval(expr));

  // set x to 42
  bind(x, add(value(40.0), value(2.0)));
  // now prints 42 for x ane evaluates to 41
  print(expr); putchar('\n');
  printf("evaluates to %f\n", eval(expr));

  return 0;
}
