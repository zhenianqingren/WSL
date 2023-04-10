#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int x[2] = {1, 2};
int y[2] = {3, 4};
int z[2];

int addcnt;

int main()
{
    void *handle;
    void (*addvec)(int *, int *, int *, int);
    char *error;

    /*Dynamically load the shared library containing addvec()*/
    handle = dlopen("/home/mice/repository/WSL/CSAPP/libvector.so",
                    RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    /*Get a pointer to the addvec() function we just loaded*/
    addvec = dlsym(handle, "addvec");
    addcnt = *((int *)dlsym(handle, "addcnt"));

    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }

    /*Now we can call addvec() just like other function*/
    addvec(x, y, z, 2);
    printf("z = ( %d , %d )\n", z[0], z[1]);
    printf("addcnt = %d\n", addcnt);

    /*Unload the shared library*/
    if (dlclose(handle) < 0)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    return 0;
}