#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "stree.h"

stree *new_tree(stree_type type)
{
  // key and cmp are always needed. The rest
  // are optional.
  if (!(type.key && type.cmp)) return 0;
  stree *tree = malloc(sizeof *tree);
  if (tree) {
    *tree = (stree) {
      .root = { .parent = 0, .left = 0, .right = 0 },
      .type = type
    };
  }
  return tree;
}

// Find parent and child
node **find_loc(stree *tree, void const *key,
                node **n, node **p)
{
  void const * (*get_key)(node *n) = tree->type.key;
  int (*cmp)(void const *x, void const *y) = tree->type.cmp;

  while (*n) {
    int cmpres = cmp(key, get_key(*n));
    if (cmpres == 0) return n;
    *p = *n;
    if (cmpres < 0) n = &(*n)->left;
    else            n = &(*n)->right;
  }
  return n;
}

void insert_node(stree *tree, node *n)
{
  node *parent = &tree->root;
  node **real_tree = &parent->left;
  void const *key = tree->type.key(n);
  node **target = find_loc(tree, key, real_tree, &parent);

  if (*target) { // remove the old node
    delete_node(tree, *target);
  }

  *target = n;
  n->parent = parent;
  n->left = n->right = 0; // leaf
}

node *find_node(stree *tree, void const *key)
{
  node *parent = &tree->root;
  node **real_tree = &parent->left;
  return *find_loc(tree, key, real_tree, &parent);
}

node **rightmost(node **n, node **p)
{
  while ((*n)->right) {
    *p = *n;
    n = &(*n)->right;
  }
  return n;
}

void remove_node(node *n)
{
  if (!n->parent) {
    // parentless nodes are not in the tree
    // (they have probably been removed before)
    return;
  }

  // Get the location to replace.
  node **loc = (n == n->parent->left)
      ? &n->parent->left : &n->parent->right;

  if (!(n->left && n->right)) {
    // has an empty child...
    *loc = n->left ? n->left : n->right;
    if (*loc) (*loc)->parent = n->parent;
  } else {
    node *rm_parent = n;
    node **rm_ref = rightmost(&n->left, &rm_parent);
    node *rm = *rm_ref;
    *rm_ref = rm->left;
    if (*rm_ref) (*rm_ref)->parent = rm_parent;

    // we cannot simply move the value now, but must
    // reconnect the pointers...
    *loc = rm; // makes *loc point to rm
    *rm = *n;  // copies the struct (i.e. the pointers)
    // When copying the structs like this, we only copy
    // the bits that are in the type they have, so only the
    // three pointers and not whatever else might sit in the
    // actual nodes.
  }

  // now, to make our code safer, we NULL the pointers
  // before we call the free function.
  n->left = n->right = n->parent = 0;
}

void delete_node(stree *tree, node *n)
{
  remove_node(n);
  if (tree->type.free)
    tree->type.free(n);
}


// Just recursion this time; the techniques for avoid it
// hasn't changed.
static void default_print(node *n)
{
  printf("<node %p>", (void*)n);
}
void print_node(void (*print)(node *n), node *n)
{
  if (!n) return;
  putchar('(');
    print_node(print, n->left);
    putchar(','); print(n); putchar(',');
    print_node(print, n->right);
  putchar(')');
}
void print_tree(stree *tree)
{
  void (*print)(node *) =
    tree->type.print ? tree->type.print : default_print;
  print_node(print, tree->root.left);
}

void free_nodes_rec(void (*free)(node *n), node *n)
{
  if (!n) return;
  free_nodes_rec(free, n->left);
  free_nodes_rec(free, n->right);
  if (free) {
    // remove pointers before callback
    n->left = n->right = n->parent = 0;
    free(n);
  }
}
void free_tree(stree *tree)
{
  free_nodes_rec(tree->type.free, tree->root.left);
  free(tree);
}
