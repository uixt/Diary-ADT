int read_int(void);

// read_str() reads one word from input and returns it as a string.
// effect: allocates heap memory; client must free
// time:   O(n) where n is the length of the word
char *read_str(void);
