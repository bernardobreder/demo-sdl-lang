/*
 * lexical.c
 *
 *  Created on: 27/07/2013
 *      Author: bernardobreder
 */

#include <stdlib.h>
#include "struct.h"
#include "lexical.h"

struct lng_token_t* lexical(const unsigned char* text, unsigned int* tokenSize) {
	unsigned size = 0;
	{
		unsigned char* pc = (unsigned char*) text;
		unsigned char c = *pc;
		while (c) {
			if (c <= ' ') {
			} else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
				while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-') {
					c = *(++pc);
				}
				size++;
				pc--;
			} else if (c == '0' && pc[1] == 'x') {
				pc++;
				c = *(++pc);
				while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
					c = *(++pc);
				}
				size++;
				pc--;
			} else if ((c >= '0' && c <= '9') || c == '-') {
				c = *(++pc);
				while (c >= '0' && c <= '9') {
					c = *(++pc);
				}
				size++;
				pc--;
			} else if (c == '\"' || c == '\'') {
				c = *(++pc);
				while ((c != '\"' && c != '\'') || pc[-1] == '\\') {
					if (c <= 0x7F) {
						c = *(++pc);
					} else if ((c >> 5) == 0x6) {
						pc++;
						c = *(++pc);
					} else {
						pc += 2;
						c = *(++pc);
					}
				}
				size++;
			} else if (c == '/' && pc[1] == '*') {
				pc += 3;
				c = *pc;
				while (c != '/' || pc[-1] != '*') {
					if (c <= 0x7F) {
						c = *(++pc);
					} else if ((c >> 5) == 0x6) {
						pc++;
						c = *(++pc);
					} else {
						pc += 2;
						c = *(++pc);
					}
				}
			} else if (c == '/' && pc[1] == '/') {
				pc += 2;
				c = *pc;
				while (c != '\n') {
					if (c <= 0x7F) {
						c = *(++pc);
					} else if ((c >> 5) == 0x6) {
						pc++;
						c = *(++pc);
					} else {
						pc += 2;
						c = *(++pc);
					}
				}
			} else {
				switch (c) {
				case '>': {
					switch (pc[1]) {
					case '=': {
						pc++;
						break;
					}
					case '>': {
						pc++;
						break;
					}
					}
					break;
				}
				case '<': {
					switch (pc[1]) {
					case '=': {
						pc++;
						break;
					}
					case '<': {
						pc++;
						break;
					}
					}
					break;
				}
				case '!': {
					switch (pc[1]) {
					case '=': {
						pc++;
						break;
					}
					}
					break;
				}
				case '=': {
					switch (pc[1]) {
					case '=': {
						pc++;
						break;
					}
					}
					break;
				}
				case '+': {
					switch (pc[1]) {
					case '=': {
						pc++;
						break;
					}
					}
					break;
				}
				case '-': {
					switch (pc[1]) {
					case '=': {
						pc++;
						break;
					}
					}
					break;
				}
				case '*': {
					switch (pc[1]) {
					case '=': {
						pc++;
						break;
					}
					}
					break;
				}
				case '/': {
					switch (pc[1]) {
					case '=': {
						pc++;
						break;
					}
					}
					break;
				}
				}
				size++;
			}
			c = *(++pc);
		}
	}
	*tokenSize = size;
	struct lng_token_t* tokens = (struct lng_token_t*) malloc((size + 1) * sizeof(struct lng_token_t));
	if (!tokens) {
		return 0;
	}
	tokens[size].type = UIL_EOF;
	struct lng_token_t* token = tokens;
	unsigned int lin = 1;
	unsigned int col = 1;
	unsigned char* pc = (unsigned char*) text;
	unsigned char c = *pc;
	while (c) {
		if (c <= ' ') {
			if (c == '\n') {
				lin++;
				col = 1;
			} else {
				col++;
			}
		} else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
			unsigned short type = UIL_ID;
			int begin = pc - text;
			while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-') {
				c = *(++pc);
			}
			token->begin = begin;
			token->size = (pc - text) - begin;
			token->type = type;
			token->line = lin;
			token->column = col;
			token++;
			col += token->size;
			pc--;
		} else if (c == '0' && pc[1] == 'x') {
			unsigned short type = UIL_HEX;
			int begin = pc - text;
			pc++;
			c = *(++pc);
			while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
				c = *(++pc);
			}
			token->begin = begin;
			token->size = (pc - text) - begin;
			token->type = type;
			token->line = lin;
			token->column = col;
			token++;
			col += token->size;
			pc--;
		} else if ((c >= '0' && c <= '9') || c == '-') {
			unsigned short type = UIL_NUMBER;
			int begin = pc - text;
			c = *(++pc);
			col++;
			while (c >= '0' && c <= '9') {
				c = *(++pc);
				col++;
			}
			token->begin = begin;
			token->size = (pc - text) - begin;
			token->type = type;
			token->line = lin;
			token->column = col;
			token++;
			col += token->size;
			pc--;
		} else if (c == '\"' || c == '\'') {
			int clin = lin;
			int ccol = col;
			unsigned short type = UIL_STRING;
			int begin = pc - text;
			c = *(++pc);
			col++;
			while ((c != '\"' && c != '\'') || pc[-1] == '\\') {
				if (c <= 0x7F) {
					if (c == '\n') {
						lin++;
						col = 0;
					}
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
				col++;
			}
			token->begin = begin;
			token->size = (pc - text) - begin + 1;
			token->type = type;
			token->line = clin;
			token->column = ccol;
			token++;
		} else if (c == '/' && pc[1] == '*') {
			pc += 3;
			col += 3;
			c = *pc;
			while (c != '/' || pc[-1] != '*') {
				if (c <= 0x7F) {
					if (c == '\n') {
						lin++;
						col = 0;
					}
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
				col++;
			}
		} else if (c == '/' && pc[1] == '/') {
			pc += 2;
			c = *pc;
			while (c != '\n') {
				if (c <= 0x7F) {
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
			}
			lin++;
			col = 1;
		} else {
			switch (c) {
			case '>': {
				switch (pc[1]) {
				case '=': {
					token->begin = ++pc - text + 1;
					token->size = 2;
					token->type = UIL_GREATER_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				case '>': {
					token->begin = ++pc - text + 1;
					token->size = 2;
					token->type = UIL_GREATER_GREATER;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default: {
					token->begin = ++pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '<': {
				switch (pc[1]) {
				case '=': {
					token->begin = ++pc - text + 1;
					token->size = 2;
					token->type = UIL_LOWER_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				case '<': {
					token->begin = ++pc - text + 1;
					token->size = 2;
					token->type = UIL_LOWER_LOWER;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default: {
					token->begin = ++pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '!': {
				switch (pc[1]) {
				case '=': {
					token->begin = ++pc - text + 1;
					token->size = 2;
					token->type = UIL_DIFERENT;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default: {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '=': {
				switch (pc[1]) {
				case '=': {
					token->begin = ++pc - text + 1;
					token->size = 2;
					token->type = UIL_EQUAL_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default: {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '+': {
				switch (pc[1]) {
				case '=': {
					token->begin = ++pc - text + 1;
					token->size = 2;
					token->type = UIL_SUM_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				case '+': {
					token->begin = ++pc - text + 1;
					token->size = 2;
					token->type = UIL_SUM_SUM;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default: {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '-': {
				switch (pc[1]) {
				case '=': {
					token->begin = ++pc - text + 1;
					token->size = 2;
					token->type = UIL_SUB_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				case '-': {
					token->begin = ++pc - text + 1;
					token->size = 2;
					token->type = UIL_SUB_SUB;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default: {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '*': {
				switch (pc[1]) {
				case '=': {
					token->begin = ++pc - text + 1;
					token->size = 2;
					token->type = UIL_MUL_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default: {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '/': {
				switch (pc[1]) {
				case '=': {
					token->begin = ++pc - text + 1;
					token->size = 2;
					token->type = UIL_DIV_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default: {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			default: {
				token->begin = pc - text;
				token->size = 1;
				token->type = c;
				token->line = lin;
				token->column = col;
				token++;
				col++;
			}
			}
		}
		c = *(++pc);
	}
	return tokens;
}
