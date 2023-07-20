
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "diary.h"
#include "string-bst.h"
#include "hash-functions.h"


// -----------------------------------------------------------------------
const int DIARY_SUCCESS        = 0;
const int DIARY_ALREADY_STORED = 1;
const int DIARY_NOT_STORED     = 2;
// -----------------------------------------------------------------------

#define CMD                   4


struct bstnode {
  char *title;
  char* date;
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


struct hashtable *diary_create(int hash_length) {
  assert(2 <= hash_length && hash_length <= 16);
  struct hashtable *ht = malloc(sizeof(struct hashtable));
  int hash_size = 1;
  for (int i = 0; i < hash_length; ++i) {
    hash_size *= 2;
  }

  ht->hash_length = hash_length;
  ht->hash_size = hash_size;
  ht->num_entries = 0;
  ht->by_date = NULL;
  ht->data = malloc(sizeof(struct bst *) * hash_size);
  for(int i = 0; i < hash_size; ++i) {
    ht->data[i] = bst_create();
  }

  return ht;
}


void diary_destroy(struct hashtable *ht) {
  assert(ht);
  int hash_size = ht->hash_size;
  int num_entries = ht->num_entries;

  // free the DA
  for(int i = num_entries - 1; i > -1; --i) {
    free(ht->by_date[i]);
  }
  // free the hashtable
  for(int i = 0; i < hash_size; ++i) {
    bst_destroy(ht->data[i]);
  }

  free(ht->by_date);
  free(ht->data);
  free(ht);
}


int diary_insert(struct hashtable *ht, 
              const char *title,
              const char *date,
              const char *entry) {
  assert(ht);
  assert(title);
  assert(date);
  assert(entry);
  
  int h_val = hash_str(title, ht->hash_length); 
  int prev_count = 0;
  struct bst *t = ht->data[h_val];


  if (t) {
    prev_count = bst_count(t);
  }

  bst_insert(title, date, entry, t); 
  int new_count = bst_count(t);
  
  if (prev_count == new_count) {
    return DIARY_ALREADY_STORED;
  } else {
    int i = ht->num_entries;
    ht->num_entries += 1;

    // allocate memory for array
    ht->by_date = realloc(ht->by_date, sizeof(char *) * ht->num_entries);

    // allocate memory in index i
    ht->by_date[i] = malloc(sizeof(char *) * 112); // can store 100 chars

    struct bstnode *target = bstnode_search(title, t);
    strcpy(ht->by_date[i], target->da_entry);

    return DIARY_SUCCESS;
  }  
}

// See hashtable.h for documentation
int diary_remove(struct hashtable *ht, const char *title) {
  assert(ht);
  assert(title);
  
  int h_val = hash_str(title, ht->hash_length);
  int prev_count = 0;

  // is the bst empty?
  if(ht->data[h_val]->root) {
    prev_count = bst_count(ht->data[h_val]);
  } else {
    return DIARY_NOT_STORED; // 2
  }

  // Remove the bstnode
  bst_remove(title, 
             ht->data[h_val], 
             ht->by_date, 
             ht->num_entries);
  

  // Has the count changed?
  int new_count = 0;
  if(ht->data[h_val]->root) { 
    new_count = bst_count(ht->data[h_val]);
  }
  
  if (prev_count == new_count) {
    return DIARY_NOT_STORED;
  } else {
    ht->num_entries -= 1;
    return DIARY_SUCCESS;
  }  
}

void diary_print(const struct hashtable *ht) {
  assert(ht);
  int hash_size = ht->hash_size;
  for (int i = 0; i < hash_size; ++i) {
    if (ht->data[i]->root) {
      update_count(ht->data[i]->root);
    }
    printf("%d:", i);
    bst_print(ht->data[i]);
    }
}

struct bstnode *diary_entry_retrieve(const char *title, const struct hashtable *ht) {
  assert(title);
  assert(ht);

  int h_val = hash_str(title, ht->hash_length);
  struct bstnode *target = bstnode_search(title, ht->data[h_val]);
  return target;
}

void print_by_date(const struct hashtable *ht) {
  assert(ht);
  int num_entries = ht->num_entries;
  printf("\nYour current entries:\n");
  for (int i = num_entries - 1; i > -1; --i) {
    printf("  ");
    printf("%s\n", ht->by_date[i]);
  }
}

int print_entry(const struct hashtable *ht, char *title) {
  assert(ht);
  int h_val = hash_str(title, ht->hash_length);
  struct bstnode *target = bstnode_search(title, ht->data[h_val]);
  
  if (target) {
    printf("\n%s\n", target->date);
    printf("%s\n", target->title);
    printf("\n%s\n", target->entry);
    return DIARY_SUCCESS;

  } else {
    return DIARY_NOT_STORED;
  }
  
}
/*
Testing Harness

int main(void) {

  char *date1 = "06-24-2023";
  char *entry1 = "Dear Diary,\nToday was swell.\n";
  char *title1 = "how today went.";

  char *date2 = "06-25-2023";
  char *entry2 = "Dear Diary,\nToday was swell. Moreso than yesterday.\n";
  char *title2 = "After The Storm.";


  // repeated title
  char *date3 = "06-25-2023";
  char *entry3 = "Dear Diary,\nToday was swell. Moreso than yesterday.\n";
  char *title3 = "After The Storm.";

  // collision on hashtable (2)
  char *date4 = "06-26-2023";
  char *entry4 = "Dear Diary,\nToday was not as swell.\n";
  char *title4 = "Afte.";

  // another collision on ht (2)
  char *date5 = "06-27-2023";
  char *entry5 = "Dear Diary,\nToday nothing happened.\n";
  char *title5 = "gpte.";

  // another collision on ht (2)
  char *date6 = "06-28-2023";
  char *entry6 = "Dear Diary,\nToday I ate a bagel.\n";
  char *title6 = "d.";


  struct hashtable *ht = diary_create(2);
    int ss = diary_insert(ht, title1, date1, entry1);
    int se2 = diary_insert(ht, title2, date2, entry2);
    int se3 = diary_insert(ht, title3, date3, entry3);
    int se4 = diary_insert(ht, title4, date4, entry4);
    int se5 = diary_insert(ht, title5, date5, entry5);
    int se6 = diary_insert(ht, title6, date6, entry6);

  if (CMD == 1) {
    printf("\nStarting Test 1.\n");
    printf("%d, %d, %d, %d, %d, %d\n", 
            ss, se2, se3, se4, se5, se6);


//////////////////////////////////////////////////////////////////////////
// testing diary_remove()
// test 1: removing each entry individually
  }  else if (CMD == 2) {
    printf("\nStarting Test 2.\n");
    print_by_date(ht);
    diary_print(ht);
    int sss = diary_remove(ht, title1);
    printf("%d\n", sss);
    
// test 2: removing all entries including one that doesn't exist
  } else if (CMD == 3) {
    printf("\nStarting Test 3.\n");
    print_by_date(ht);
    diary_print(ht);

    printf("\nStart of diary_remove() calls.\n");
    
    int sss = diary_remove(ht, title1);
    int sse2 = diary_remove(ht, title2); //error
    int sse3 = diary_remove(ht, title3); //repeat
    int sse4 = diary_remove(ht, title4);
    int sse5 = diary_remove(ht, title5);
    int sse6 = diary_remove(ht, title6);
    int sse7 = diary_remove(ht, "This title does not exist"); // entry doesn't exist
    printf("%d, %d, %d, %d, %d, %d, %d\n", 
            sss, sse2, sse3, sse4, sse5, sse6, sse7);
  } else if (CMD == 4) {
    print_entry(ht, title2);

  }

  print_by_date(ht);
  diary_print(ht);
  diary_destroy(ht);
  printf("You slayed.");
  
}


*/