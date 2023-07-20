#include <stdbool.h>

struct bstnode;
struct bst;




// bstnode_search(title, t) consumes a title and bst and returns the 
//    bstnode that matches the title.
// time: O(log(n) + s) where n is the number of items in t, 
//      and s is the length of title
struct bstnode *bstnode_search(const char *title, struct bst *t);

void da_entry_remove(char * da_entry, char ** by_date, 
                     int *num_entries);


// bst_create() creates a new BST
// effects: allocates memory, must be freed by bst_destroy
struct bst *bst_create(void);

// bst_destroy(t) frees the allocated memory contained in t
// effects: frees memory of t
// time: O(n), where n is the number of nodes in t
void bst_destroy(struct bst *t);

// bstnode_count(node) counts the nodes in a bstnode's subtree.
// time: O(n), where n is the number of subtrees in node
int bstnode_count(struct bstnode *node);

// bst_count(t) counts the amount of nodes in a bst t.
// time: O(n), where n is the number of nodes in t.
int bst_count(struct bst *t);

// new_leaf(title, date, entry) creates a new bstnode that contains
//      the given variables
// effects: allocates memory
// time: O(t + d + e), where t is the length of title, d is the
//      length of date, e is the length of entry
struct bstnode *new_leaf(const char *title, 
                         const char *date,
                         const char *entry);

// insert_bstnode(title, date, entry, node) inserts a new bstnode
//      at the correct position within the tree.
// effects: allocates memory
// time: O(log(n) + nl), where n is the number of subtrees to node, and 
//      nl is the efficiency of new_leaf(title, date, entry)
void insert_bstnode(const char *title, 
                    const char *date,
                    const char *entry, 
                    struct bstnode *node);

// bst_inesrt(title, date, entry, t) is the wrapper function to 
//      insert_bstnode(title, date, entry, node).
// effects: allocates memory
// time: O(log(n) + nl), where n is the number of subtrees to t, and 
//      nl is the efficiency of new_leaf(title, date, entry)
void bst_insert(const char *title, 
                const char *date,
                const char *entry,
                struct bst *t);

// bst_remove(title, t, by_date, num_entries) removes the bstnode with key
//      title from the bst t, and removes the item with key title from 
//      the dynamic array by_date.
// effects: frees memory of bstnode with key title
void bst_remove(const char *title,
                struct bst *t,
                char ** by_date,
                int num_entries);

// bst_print(t) prints the contents of the tree t from smallest to largest.
// effects: creates output
// time: O(n), where n is the number of subtrees in t.
void bst_print (struct bst *t);
