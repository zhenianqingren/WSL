// #define malloc(size) mymalloc(size)
// #define free(ptr) myfree(ptr)

void *malloc(unsigned long size);
void free(void *ptr);

void *mymalloc(unsigned long size);
void myfree(void *ptr);
