#include <stdlib.h>
#include <pthread.h>

#define NHASH	29
#define HASH(id)	(((unsigned long)id)%NHASH)
