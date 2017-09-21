/*
 * list.h
 *
 *  Created on: 13/07/2013
 *      Author: bernardobreder
 */

#ifndef LIST_H_
#define LIST_H_

#include "struct.h"

struct list_t* list_new(unsigned int max);

struct list_t* list_new_full(unsigned int size);

void list_free(struct list_t* self, void* freeFunc);

unsigned char list_add(struct list_t** pself, void* value);

unsigned int list_size(struct list_t* self);

void* list_get(struct list_t* self, unsigned int index);

void list_set(struct list_t* self, unsigned int index, void* value);

const void** list_iterator(struct list_t* self);

void* list_remove(struct list_t* self, unsigned int index);

void list_clear(struct list_t* self, void* freeFunc);

#endif /* LIST_H_ */
