#include <stdio.h>

#define OFFSET(type) (&(((node *)0)->type))
#define ADDR(type, typeAddr) ((char *)(typeAddr) - (char *)OFFSET(type))
#define NODEADDR(type, typeAddr) ((node *)ADDR(type, typeAddr))


struct node
{
    node *prev;
    int val;
    short ch;
    node *next;
};

int main()
{
    node var = {NULL, 0, '0', NULL};
    node *p = &var;
    printf("%p\n", p);
    printf("%ld\n", sizeof(node *));
    printf("%p\n", &(p->prev));
    printf("%p\n", &(p->val));
    printf("%p\n", &(p->ch));
    printf("%p\n", &(p->next));
    printf("%p\n", NODEADDR(prev, &(p->prev)));
    printf("%p\n", NODEADDR(val, &(p->val)));
    printf("%p\n", NODEADDR(ch, &(p->ch)));
    printf("%p\n", NODEADDR(next, &(p->next)));
    return 0;
}
