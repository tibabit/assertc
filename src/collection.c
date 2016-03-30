#include "collection.h"

#include <stdlib.h>

#define COLLECTION_CAPACITY_BIN_SIZE    16

typedef struct _collection
{
    void * items;
    size_t count;
    size_t capacity;
    size_t item_size;
}collection_t;

collection_t * collection_new(size_t item_size)
{
    collection_t * collection = calloc(1, sizeof(collection_t));

    if (collection == NULL)
    {
        return NULL;
    }
    collection->item_size = item_size;

    return collection;
}
void collection_destroy(collection_t * collection)
{
    if (collection->items)
    {
        free(collection->items);
    }
    free(collection);
}
void collection_add(collection_t * collection, void * item)
{
    if (collection->count == collection->capacity)
    {
        collection->items = realloc(collection->items,
                (collection->count + COLLECTION_CAPACITY_BIN_SIZE) * collection->item_size);
    }
    memcpy(collection->items + collection->count * collection->item_size, item, collection->item_size);
    collection->count++;
}
int collection_at(collection_t * collection, int index, void * item)
{
    if (index < collection->count)
    {
        memcpy(item, collection->items + index * collection->item_size, collection->item_size);
        return 1;
    }
    return 0;
}
size_t collection_count(collection_t * collection)
{
    return collection->count;
}
