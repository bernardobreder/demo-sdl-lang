#ifndef UIL_H_
#define UIL_H_

#include "struct.h"

struct uil_t* uil_new(unsigned char* text);

struct widget_t* uil_eval(unsigned char* text);

void uil_free(struct uil_t* self);

error_t uil_syntax(struct uil_t* self);

struct widget_t* uil_execute(struct uil_t* self);

#endif
