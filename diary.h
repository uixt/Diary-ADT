// DIARY_SUCCESS indicates successful execution of the function.
extern const int DIARY_SUCCESS;
// DIARY_ALREADY_STORED indicates that a key was already stored in the hashtable.
extern const int DIARY_ALREADY_STORED;
// DIARY_NOT_STORED indicates that a key was not stored in the hashtable.
extern const int DIARY_NOT_STORED;


// structures
struct bstnode;
struct bst;
struct hashtable;

// requires: all functions require valid (non-NULL) parameters

// diary_create(hash_length) creates an empty hash table where hash_length
//      is the length of the hashtable in bits 
// effects: allocates heap memory; client must call diary_destroy
// requires: hash_length must be between 2 and 16 inclusive
// time: O(n), where n is the length of the hash table
struct hashtable *diary_create(int hash_length);

// diary_destroy(ht) frees all resources allocated in the hashtable ht.
// effects: frees ht
// time: O(n + m), where n is the length of ht and m is the number of
//   items in ht
void diary_destroy(struct hashtable *ht);

// diary_insert(ht, title, date, entry) inserts an bstnode containing 
//   date, entry, and title (the key) into the hash table ht. 
//   The function returns
//   * DIARY_SUCCESS if title has been inserted into ht or
//   * DIARY_ALREADY_STORED if title is already stored in ht.
// time: O(m + s), where s is the length of title and m is the number of
//       items in ht
int diary_insert(struct hashtable *ht, 
              const char *title,
              const char *date,
              const char *entry) ;

// diary_remove(ht, title) removes the key title from the hash table ht. The
//   function returns
//   * DIARY_SUCCESS if title has been removed from ht or
//   * DIARY_NOT_STORED if title was not stored in ht.
// time: O(m + t), where t is the length of title and m is the number of
//       items in ht
int diary_remove(struct hashtable *ht, const char *title);

// diary_print(ht) prints the contents of hash table ht.
// effects: creates output
// time: O(n + m * t), where n is the length of ht, t is the max length of a title,
//       m is the number of items in ht
void diary_print(const struct hashtable *ht);

// diary_entry_retrieve(title, ht) returns the bstnode with 
//      the key title.
// time: O(log(n) + t), where n is the length of ht,
//      and t is the length of title
struct bstnode *diary_entry_retrieve(const char *title, const struct hashtable *ht);

// print_by_date(ht) prints the contents of the dynamic array.
// effects: creates output
// time: O(m), where m is the number of items in the dynamic array
//      (as well as the hashtable)
void print_by_date(const struct hashtable *ht);

// print_entry(ht, title) finds the bstnode with the key title
//      and prints its title, date, and entry.
// effects: creates output
// time: O(log(n) + t), where n is the length of ht,
//      and t is the length of title
int print_entry(const struct hashtable *ht, char *title);