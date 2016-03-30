#ifndef COLLECTION_H_
#define COLLECTION_H_

#include <string.h>

typedef struct _collection collection_t;

collection_t *  collection_new(size_t item_size);
void            collection_destroy(collection_t * collection);
void            collection_add(collection_t * collection, void * item);
int             collection_at(collection_t * collection, int index, void * item);
size_t          collection_count(collection_t * collection);
#endif  // COLLECTION_H_
