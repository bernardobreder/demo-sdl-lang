/*
 * list.c
 *
 *  Created on: 13/07/2013
 *      Author: bernardobreder
 */

#include <stdlib.h>
#include "list.h"

struct list_t* list_new(unsigned int max) {
	struct list_t* self = (struct list_t*) calloc(1, 1 * sizeof(struct list_t) + max * sizeof(void*));
	if (!self) {
		return 0;
	}
	self->size = 0;
	self->max = max;
	self->data = 0;
	return self;
}

struct list_t* list_new_full(unsigned int size) {
	struct list_t* self = list_new(size);
	if (!self) {
		return 0;
	}
	self->size = size;
	return self;
}

void list_free(struct list_t* self, void* freeFunc) {
	if (freeFunc) {
		void (*freeChildFunc)(void*) = freeFunc;
		void** data = &self->data;
		while (*data) {
			freeChildFunc(*data);
			data++;
		}
	}
	free(self);
}

unsigned char list_add(struct list_t** pself, void* value) {
	struct list_t* self = *pself;
	unsigned int size = self->size;
	unsigned int max = self->max;
	void** data = &self->data;
	if (size == max) {
		unsigned int new_max = max * 2;
		struct list_t* new_self = (struct list_t*) realloc(self, 1 * sizeof(struct list_t) + new_max * sizeof(void*));
		if (!new_self) {
			return 1;
		}
		self = *pself = new_self;
		max = self->max = new_max;
		data = &self->data;
	}
	data[size] = value;
	data[size + 1] = 0;
	self->size++;
	return 0;
}

unsigned int list_size(struct list_t* self) {
	return self->size;
}

void* list_get(struct list_t* self, unsigned int index) {
	void** data = &self->data;
	return data[index];
}

void list_set(struct list_t* self, unsigned int index, void* value) {
	void** data = &self->data;
	data[index] = value;
}

const void** list_iterator(struct list_t* self) {
	return (const void**) &self->data;
}

void* list_remove(struct list_t* self, unsigned int index) {
	void** data = &self->data;
	data += index;
	void* value = *data;
	while (*data) {
		*data = data[1];
		data++;
	}
	self->size--;
	return value;
}

void list_clear(struct list_t* self, void* freeFunc) {
	void** data = &self->data;
	if (freeFunc) {
		void (*freeChildFunc)(void*) = freeFunc;
		while (*data) {
			freeChildFunc(*data);
			data++;
		}
	}
	*data = 0;
	self->size = 0;
}
