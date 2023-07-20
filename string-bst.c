#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string-bst.h" 


struct bstnode {
  char *title;
  char *date;
  char *entry;
  char *da_entry;
  struct bstnode *left;
  struct bstnode *right;
  int count;
};

struct bst {
  struct bstnode *root;
};

struct hashtable {
  int hash_length;
  int hash_size;
  int num_entries;
  char **by_date;
  struct bst **data;
};


 struct bstnode *bstnode_search(const char *title, struct bst *t) {
  assert(title);
  assert(t);
 
  struct bstnode *target = t->root;
  
  if (target) {
    // find the target (and its parent)
    int compare = strcasecmp(title, target->title); // if str1 < sr 2, compare < 0
    while (target && compare != 0) {
      if (compare < 0) {
        target = target->left;
      } else {
        target = target->right;
      }
    
    if (target) {
      compare = strcasecmp(title, target->title);
    } else {
      break;
    }
    
  }
  } else {
    return NULL; // key not found
  }

  return target;

 }

 void da_entry_remove(char * da_entry, char ** by_date, 
                      int *num_entries) {
    assert(da_entry);
    assert(by_date);
    assert(num_entries);

    int pos = 0;
    int search = 1;
    int len = *num_entries;

    // find the position. 
    // should the case of the DA not existing be accounted for?
    for (int i = 0; search != 0; ++i) {
      search = strcasecmp(da_entry, by_date[i]);
      pos = i;
    }
    
    // rearrange the DA
    for (int i = pos; i < len - 1; ++i) {
        char *temp = by_date[i + 1];
        strcpy(by_date[i], temp);
        }

    free(by_date[len - 1]);
    by_date[len - 1] = NULL;
    //*num_entries -= 1;
    return;
 }


struct bst *bst_create(void) {
  struct bst *t = malloc(sizeof(struct bst));
  t->root = NULL;
  return t;
}



// bstnode_destroy(node) deallocates all dynamically allocated memory
//   within the entire bst
// effects : the memory at node is invalid (freed)
// time : O(n)
void bstnode_destroy(struct bstnode *node) {
  if (node) {

    free(node->title);  
    free(node->date); 
    free(node->entry); 
    free(node->da_entry);  
    bstnode_destroy(node->left);
    bstnode_destroy(node->right); 
    node->entry = NULL;
    node->da_entry = NULL;
    free(node);
    node = NULL;
  }
}



void bst_destroy(struct bst *t) {
  assert(t);
  bstnode_destroy(t->root);
  free(t);
}


int bstnode_count(struct bstnode *node) {
  //assert(node);
  if(node) {
    return 1 + bstnode_count(node->left) + bstnode_count(node->right);
  } else {
    return 0;
  }
        
}

int bst_count(struct bst *t) {
  assert(t);
  return bstnode_count(t->root);
}



struct bstnode *new_leaf(const char *title, 
                         const char *date,
                         const char *entry // if constant does this mean the pointer cannot be assigned
                         ) {
  assert(title);
  assert(date);
  assert(entry);
  
  // allocate memory
  struct bstnode *leaf = malloc(sizeof(struct bstnode)); 
  leaf->title = malloc(strlen(title) * sizeof(char *) + 1); 
  leaf->date = malloc(strlen(date) * sizeof(char *) + 1); 
  leaf->entry = malloc(strlen(entry) * sizeof(char *)  + 1);
  leaf->da_entry = malloc(sizeof(char *) * 112);

  // create the DA entry
  strcpy(leaf->da_entry, date);
  strcat(leaf->da_entry, ": ");
  strcat(leaf->da_entry, title);

  leaf->left = NULL;
  leaf->right = NULL;
  leaf->count = 1;

  strcpy(leaf->title, title); 
  strcpy(leaf->date, date); 
  strcpy(leaf->entry, entry);
  
  return leaf;
}

void insert_bstnode(const char *title, 
                    const char *date,
                    const char *entry, 
                    struct bstnode *node) {
  assert(title);
  assert(entry); 
  assert(node);

  int compare = strcasecmp(title, node->title); // if str1 < sr 2, compare < 0
  //if (i < node->tag) 
  if (compare < 0) {
    if (node->left) {
      insert_bstnode(title, date, entry, node->left);
    } else {
      node->left = new_leaf(title, date, entry);
    }
  } else if (compare > 0) {
      if (node->right) {
        insert_bstnode(title, date, entry, node->right);
    } else {
        node->right = new_leaf(title, date, entry);
    }
  } else { // tag already exists
  }
}
    
void bst_insert(const char *title, 
                const char *date,
                const char *entry,
                struct bst *t) {
  assert(title);
  assert(date);
  assert(entry);
  assert(t);
 
  if (t->root) {
    insert_bstnode(title, date, entry, t->root);
  } else {
    t->root = new_leaf(title, date, entry);
  }
}

void bst_remove(const char *title,
                struct bst *t,
                char ** by_date,
                int num_entries) {
  assert(title);
  assert(t);
 
  struct bstnode *target = t->root;
  struct bstnode *target_parent = NULL;

  // find the target (and its parent)
  int compare = strcasecmp(title, target->title); // if str1 < sr 2, compare < 0
  while (target && compare != 0) {
    target_parent = target;
    if (compare < 0) {
      target = target->left;
    } else {
      target = target->right;
    }
    
    if (target) {
      compare = strcasecmp(title, target->title);
    } else {
      break;
    } 
  }

  if (target == NULL) {
    printf("This entry does not exist.\n");
    return; // key not found
  }
  // find the node to "replace" the target
  struct bstnode *replacement = NULL;
  if (target->left == NULL) {
    replacement = target->right;
  } else if (target->right == NULL) {
    replacement = target->left;
  } else {
    // neither child is NULL
    // find the replacement node and its parent
    replacement = target->right;
    struct bstnode *replacement_parent = target;
    while (replacement->left) {
      replacement_parent = replacement;
      replacement = replacement->left;
    }

    // update the child links for theas replacement and its parent
    replacement->left = target->left;
    if (replacement_parent != target) {
      replacement_parent->left = replacement->right;
      replacement->right = target->right;
    }
  }

    da_entry_remove(target->da_entry, by_date, &num_entries);
  // free the target, and update the target's parent
    free(target->title);  
    free(target->date); 
    free(target->entry); 
    free(target->da_entry);  
    target->entry = NULL;
    target->da_entry = NULL;
    free(target);
    target = NULL;
  
  if (target_parent == NULL) {
    t->root = replacement;
  } else {
    int compare_parent = strcasecmp(title, target_parent->title);//key_compare(title, target_parent->title);
    if (compare_parent > 0) {
      target_parent->right = replacement;
    } else {
      target_parent->left = replacement;
    }
  }
}


// bstnode_print(node, first, depth) prints node. 
// requires : node and first are not NULL
// effects : prints output, may mutate *first
// time : O(1)
static void bstnode_print(struct bstnode *node, 
                          bool *first,
                          int depth) {
  assert(node);
  assert(first);
  if (*first) {
    *first = false;
  } else {
    printf(", ");
  }

  printf("%d-", depth);
  printf(node->title);
}

// bstnodes_print(node, first, depth) prints node's subtree in
//   in order from smallest to largest. 
// effects : prints output
// time : O(n)
static void bstnodes_print(struct bstnode *node, 
                           bool *first, 
                           int depth) { 
  assert(first);
  ++depth;
  if (node) {
    bstnodes_print(node->left, first, depth);
    bstnode_print(node, first, depth);
    bstnodes_print(node->right, first, depth);
  }
  
}

void bst_print (struct bst *t) {
  assert(t);  
  bool first = true;
  printf("{");
  bstnodes_print(t->root, &first, -1);
  printf("}\n");
}


