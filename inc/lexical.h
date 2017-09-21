/*
 * lexical.h
 *
 *  Created on: 27/07/2013
 *      Author: bernardobreder
 */

#ifndef LEXICAL_H_
#define LEXICAL_H_

#include "struct.h"

struct node_t;

struct lng_token_t* lexical(const unsigned char* text, unsigned int* tokenSize);

struct node_t* lng(const unsigned char* text);

#endif /* LEXICAL_H_ */
