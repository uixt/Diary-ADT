#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "read.h"
#include "diary.h"
#include "string-bst.h"
#include "hash-functions.h"

#define invalid_cmd   -1

void print_welcome(void) {

    printf("\n\n");
    printf("   _.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._\n");
    printf(" ,'_.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._`.\n");
    printf("( (                                                         ) )\n");
    printf(" ) )                                                       ( (\n");
    printf("( (                                                         ) )\n");
    printf(" ) )         .:*~*:. Welcome to your Diary! .:*~*:.        ( (\n");
    printf("( (                                                         ) )\n");
    printf(" ) )            Press 1 to view your current entries.      ( (\n");
    printf("( (             Press 2 to create a new entry.              ) )\n");
    printf(" ) )            Press 3 to delete an entry.                ( (\n");
    printf("( (             Press 4 to view an entry.                   ) )\n");
    printf(" ) )            Press 5 to end this session.               ( (\n");
    printf("( (                                                         ) )\n");
    printf(" ) )                                                       ( (\n");
    printf("( (_.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._) )\n");
    printf(" `._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._,'\n");
    printf("\n\n");

    return;
}

void print_goodbye(void) {

    printf("\n\n");
    printf("   _.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._\n");
    printf(" ,'_.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._`.\n");
    printf("( (                                                         ) )\n");
    printf(" ) )                                                       ( (\n");
    printf("( (                                                         ) )\n");
    printf(" ) )                                                       ( (\n");
    printf("( (                                                         ) )\n");
    printf(" ) )            *~+ Thank you for sharing :) +~*           ( (\n");
    printf("( (                                                         ) )\n");
    printf(" ) )                                                       ( (\n");
    printf("( (                                                         ) )\n");
    printf(" ) )                                                       ( (\n");
    printf("( (                                                         ) )\n");
    printf(" ) )                                                       ( (\n");
    printf("( (_.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._) )\n");
    printf(" `._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._,'\n");
    printf("\n\n");
    return;
}

int main(void) {
    int cmd = 0;

    int hash_length = 2;
    struct hashtable *ht = diary_create(hash_length);

    enum Commands {
    print_entries = 1,
    new_entry,
    delete_entry,
    view_entry,
    end_session,
};

    while (1) {
        print_welcome();
        cmd = read_int(); 

        if (cmd >= 1 && cmd < 5) {
            switch (cmd) {

            case print_entries:
                printf("\n=^..^=   =^..^=   =^..^=    =^..^=    =^..^=    =^..^=\n");
                print_by_date(ht);
                printf("\n=^..^=   =^..^=   =^..^=    =^..^=    =^..^=    =^..^=\n");
            break;

            case new_entry:
                printf("\n=^..^=   =^..^=   =^..^=    =^..^=    =^..^=    =^..^=\n");
                printf("\nEnter a title for the entry:\n");
                char *title = read_str();
                struct bstnode *titlenode = diary_entry_retrieve(title, ht);

                if (titlenode) {
                    printf("\nAn entry with that name already exists.\n");
                    printf("\n=^..^=   =^..^=   =^..^=    =^..^=    =^..^=    =^..^=\n");
                } else {
                    printf("\nEnter today's date in the format MM-DD-YYYY:\n");
                    char *date = read_str();

                    

                    printf("\nType your entry:\n");
                    char *entry = read_str();

                    printf("\nYour entry '%s' has been added :)\n", title);
                    printf("\n=^..^=   =^..^=   =^..^=    =^..^=    =^..^=    =^..^=\n");
                    diary_insert(ht, title, date, entry);
                    free(date);
                    
                    free(entry);
                }
                
                free(title);

            break;

            case delete_entry:
                printf("\n=^..^=   =^..^=   =^..^=    =^..^=    =^..^=    =^..^=\n");
                printf("\nEnter the title of the entry you would like to remove:\n");
                char *title_delete = read_str();

                int remove = diary_remove(ht, title_delete);
                if (remove == DIARY_SUCCESS) {
                    printf("\nThe entry '%s' was deleted.\n", title_delete);
                } else {
                    printf("\nThe entry '%s' does not exist.\n", title_delete);
                }
                printf("\n=^..^=   =^..^=   =^..^=    =^..^=    =^..^=    =^..^=\n");
                free(title_delete);
            break;
            
            case view_entry:
                printf("\n=^..^=   =^..^=   =^..^=    =^..^=    =^..^=    =^..^=\n");
                printf("Enter the title of the entry you would like to view:\n");
                char *title_view = read_str();
                printf("\n~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~\n");
                int print = print_entry(ht, title_view);
                if (print == DIARY_NOT_STORED) { //fix
                    printf("\nThe entry '%s' does not exist >:(\n", title_view);
                }
                printf("\n~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~\n");
                printf("\n=^..^=   =^..^=   =^..^=    =^..^=    =^..^=    =^..^=\n");
            break;
            }
        } else if (cmd == end_session) {
            print_goodbye();
            diary_destroy(ht);
            break;
        } else {
            printf("\nPlease enter a vaild command!\n");
        }
    }

}