#include "id_filter.h"
#include "doc_table.h"
#include "rmalloc.h"
#include "id_list.h"

/* Create a new IdFilter from a list of redis strings. count is the number of strings, guaranteed to
 * be less than or equal to the length of args */
IdFilter NewIdFilter(RedisModuleString **args, int count, DocTable *dt) {

  IdFilter ret = {.ids = NULL, .keys = args, .size = 0};
  if (count <= 0) {
    return ret;
  }
  ret.ids = rm_calloc(count, sizeof(t_docId));
  for (int i = 0; i < count; i++) {

    t_docId did = DocTable_GetId(dt, RedisModule_StringPtrLen(args[i], NULL));
    if (did) {
      ret.ids[ret.size++] = did;
    }
  }

  return ret;
}

IndexIterator *NewIdFilterIterator(IdFilter *f) {

  if (f->ids == NULL || f->size == 0) {
    return NULL;
  }

  return NewIdListIterator(f->ids, f->size);
}