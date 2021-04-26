#ifndef STREE_H
#define STREE_H

#include <stdbool.h>

typedef struct node {
  struct node *parent;
  struct node *left;
  struct node *right;
} node;

typedef struct stree_type {
  void const *  (*key)  (node *n);
  int           (*cmp)  (void const *x,
                         void const *y);
  void          (*print)(node *n);
  void          (*free) (node *n);
} stree_type;
typedef struct stree {
  node root; // dummy node
  stree_type type;
} stree;

stree    *new_tree(stree_type type);
#define empty_tree(t) ((t)->root.left == 0)
void    print_tree(stree *tree);
void   insert_node(stree *tree, node *n);
node    *find_node(stree *tree, void const *key);
void   remove_node(node *n);
void   delete_node(stree *tree, node *n);
void     free_tree(stree *tree);

static inline
bool contains(stree *tree, void const *key)
{ return !!find_node(tree, key); }

static inline
void delete_key(stree *tree, void const *key)
{
  node *x = find_node(tree, key);
  if (x) delete_node(tree, x);
}

#endif
