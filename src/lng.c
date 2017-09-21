/*
 * lng.c
 *
 *  Created on: 27/07/2013
 *      Author: bernardobreder
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "lexical.h"

#define LNG_NUM_TYPE_ID 1
#define LNG_STR_TYPE_ID 2
#define LNG_BOOL_TYPE_ID 3
#define LNG_ARRAY_TYPE_ID 4
#define LNG_EXPRESSION_ID 6
#define LNG_IF_ID 7
#define LNG_WHILE_ID 8
#define LNG_REPEAT_ID 9
#define LNG_FOR_ID 10
#define LNG_DECLARE_ID 11
#define LNG_BLOCK_ID 5
#define LNG_ID_ID 12
#define LNG_INC_PRE_ID 14
#define LNG_INC_POS_ID 15
#define LNG_DEC_PRE_ID 16
#define LNG_DEC_POS_ID 17
#define LNG_NOT_ID 18
#define LNG_SUM_ID 19
#define LNG_SUB_ID 20
#define LNG_MUL_ID 21
#define LNG_DIV_ID 22
#define LNG_MOD_ID 23
#define LNG_OR_ID 24
#define LNG_AND_ID 25
#define LNG_EQUAL_ID 26
#define LNG_NOT_EQUAL_ID 27
#define LNG_LOWER_ID 28
#define LNG_LOWER_EQUAL_ID 29
#define LNG_GREATER_ID 30
#define LNG_GREATER_EQUAL_ID 31
#define LNG_ASSIGN_ID 32
#define LNG_ASSIGN_SUM_ID 33
#define LNG_ASSIGN_SUB_ID 34
#define LNG_ASSIGN_MUL_ID 35
#define LNG_ASSIGN_DIV_ID 36
#define LNG_ASSIGN_MOD_ID 37
#define LNG_ASSIGN_OR_ID 38
#define LNG_ASSIGN_AND_ID 39
#define LNG_STR_ID 100
#define LNG_NUM_ID 101
#define LNG_BOOL_ID 102
#define LNG_ARRAY_ID 103
#define LNG_NULL_ID 104
#define LNG_TRUE_ID 105
#define LNG_FALSE_ID 106

#define LEXICAL_IF_ID 500
#define LEXICAL_DO_ID 501
#define LEXICAL_WHILE_ID 502
#define LEXICAL_FOR_ID 503
#define LEXICAL_BREAK_ID 504
#define LEXICAL_CONTINUE_ID 505
#define LEXICAL_NUM_ID 506
#define LEXICAL_STR_ID 507
#define LEXICAL_BOOL_ID 508
#define LEXICAL_VAR_ID 509
#define LEXICAL_BEGIN_ID 510
#define LEXICAL_END_ID 511
#define LEXICAL_TRUE_ID 512
#define LEXICAL_FALSE_ID 513
#define LEXICAL_OR_ID 514
#define LEXICAL_AND_ID 515

static const char* EXPECTED_TYPE_EOF = "expected <type> and not <eof>";
static const char* EXPECTED_TYPE_IN_TYPE = "expected '<id>, num, str, bool' in the type declaration";
static const char* EXPECTED_END_BLOCK = "expected 'end' to close the block statement";
//static const char* EXPECTED_TYPE_DECLARE = "expected '<type>' to declare a variable";
static const char* EXPECTED_ID_DECLARE = "expected '<id>' to declare a variable";
//static const char* EXPECTED_EXP_DECLARE = "expected '<exp>' to declare a value of variable";
static const char* EXPECTED_COMMAND_IN_COMMAND = "expected '<command>' in the command statement";
static const char* EXPECTED_REPEAT_WHILE = "expected 'while' in repeat statement";
//static const char* EXPECTED_CONDITION_IN_IF = "expected '<exp>' in if declaration";
//static const char* EXPECTED_COMMAND_IN_IF = "expected '<command>' in if declaration";
//static const char* EXPECTED_CONDITION_IN_WHILE = "expected '<exp>' in while declaration";
//static const char* EXPECTED_COMMAND_IN_WHILE = "expected '<command>' in while declaration";
static const char* EXPECTED_LITERAL_IN_EXP = "expected '(, #, $, <number>, <bool>, <string>, <boolean>, <id>' in expression";
static const char* EXPECTED_ID_IN_CALL = "expected '<id>' in the id statement";
static const char* EXPECTED_CLOSE_ARRAY_IN_ID = "expected ']' in the id statement";
static const char* EXPECTED_CLOSE_CALL_IN_ID = "expected ')' in the id statement";
static const char* EXPECTED_CLOSE_IN_LITERAL = "expected ')' in the literal expression";

struct lng_context_t {
	unsigned char test;
	const unsigned char* text;
	struct lng_token_t* token;
	unsigned int tokenSize;
	const char* error;
	unsigned char error_code;
	const struct lng_token_t* error_token;
	unsigned char* memory;
	unsigned int memory_size;
};

struct node_t {
	unsigned char classid;
};

struct type_node_t {
	unsigned char classid;
};

struct id_type_node_t {
	unsigned char classid;
	struct lng_token_t* left;
};

struct num_type_node_t {
	unsigned char classid;
};

struct str_type_node_t {
	unsigned char classid;
};

struct bool_type_node_t {
	unsigned char classid;
};

struct array_type_node_t {
	unsigned char classid;
	struct rvalue_node_t* left;
};

struct command_node_t {
	unsigned char classid;
};

struct expression_node_t {
	unsigned char classid;
	struct rvalue_node_t* value;
};

struct if_node_t {
	unsigned char classid;
	struct rvalue_node_t* condition;
	struct command_node_t* command;
};

struct while_node_t {
	unsigned char classid;
	struct rvalue_node_t* condition;
	struct command_node_t* command;
};

struct repeat_node_t {
	unsigned char classid;
	struct rvalue_node_t* condition;
	struct command_node_t* command;
};

struct for_node_t {
	unsigned char classid;
};

struct declare_node_t {
	unsigned char classid;
	struct type_node_t* type;
	struct lng_token_t* token;
	struct rvalue_node_t* value;
};

struct block_node_t {
	unsigned char classid;
	struct command_node_t* commands;
	unsigned int command_count;
};

struct lvalue_node_t {
	unsigned char classid;
};

struct rvalue_node_t {
	unsigned char classid;
};

struct id_node_t {
	unsigned char classid;
	struct lng_token_t* left;
};

struct str_node_t {
	unsigned char classid;
	struct lng_token_t* left;
};

struct num_node_t {
	unsigned char classid;
	struct lng_token_t* left;
	double value;
};

struct bool_node_t {
	unsigned char classid;
};

struct null_node_t {
	unsigned char classid;
};

struct true_node_t {
	unsigned char classid;
};

struct false_node_t {
	unsigned char classid;
};

struct array_node_t {
	unsigned char classid;
	struct value_node_t* index;
};

struct inc_pre_node_t {
	unsigned char classid;
	struct value_node_t* left;
};

struct inc_pos_node_t {
	unsigned char classid;
	struct value_node_t* left;
};

struct dec_pre_node_t {
	unsigned char classid;
	struct value_node_t* left;
};

struct dec_pos_node_t {
	unsigned char classid;
	struct value_node_t* left;
};

struct not_node_t {
	unsigned char classid;
	struct value_node_t* left;
};

struct sum_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct sub_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct mul_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct div_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct mod_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct or_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct and_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct equal_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct not_equal_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct lower_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct lower_equal_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct greater_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct greater_equal_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct greater_greater_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct lower_lower_node_t {
	unsigned char classid;
	struct value_node_t* left;
	struct value_node_t* right;
};

struct assign_node_t {
	unsigned char classid;
	struct lvalue_node_t* left;
	struct rvalue_node_t* right;
};

struct assing_sum_node_t {
	unsigned char classid;
	struct rvalue_node_t* left;
	struct rvalue_node_t* right;
};

struct assign_sub_node_t {
	unsigned char classid;
	struct rvalue_node_t* left;
	struct rvalue_node_t* right;
};

struct assign_mul_node_t {
	unsigned char classid;
	struct rvalue_node_t* left;
	struct rvalue_node_t* right;
};

struct assign_div_node_t {
	unsigned char classid;
	struct rvalue_node_t* left;
	struct rvalue_node_t* right;
};

struct assign_mod_node_t {
	unsigned char classid;
	struct rvalue_node_t* left;
	struct rvalue_node_t* right;
};

struct assign_or_node_t {
	unsigned char classid;
	struct rvalue_node_t* left;
	struct rvalue_node_t* right;
};

struct assign_and_node_t {
	unsigned char classid;
	struct rvalue_node_t* left;
	struct rvalue_node_t* right;
};

struct unary_node_t {
	unsigned char classid;
	struct rvalue_node_t* left;
};

struct binary_node_t {
	unsigned char classid;
	struct rvalue_node_t* left;
	struct rvalue_node_t* right;
};

struct ternary_node_t {
	unsigned char classid;
	struct rvalue_node_t* left;
	struct rvalue_node_t* center;
	struct rvalue_node_t* right;
};

struct call_node_t {
	unsigned char classid;
};

struct field_get_node_t {
	unsigned char classid;
};

struct type_node_t* lng_type(struct lng_context_t* context);

struct rvalue_node_t* lng_exp(struct lng_context_t* context);

struct node_t* lng_block_command(struct lng_context_t* context);

struct node_t* lng_var_command(struct lng_context_t* context);

struct node_t* lng_if_command(struct lng_context_t* context);

struct node_t* lng_for_command(struct lng_context_t* context);

struct node_t* lng_while_command(struct lng_context_t* context);

struct node_t* lng_do_command(struct lng_context_t* context);

struct rvalue_node_t* lng_exp_command(struct lng_context_t* context);

struct command_node_t* lng_command(struct lng_context_t* context);

struct null_node_t null_node_object = { };

struct true_node_t true_node_object = { };

struct false_node_t false_node_object = { };

struct null_node_t* null_node_new() {
	return &null_node_object;
}

struct true_node_t* true_node_new() {
	return &true_node_object;
}

struct false_node_t* false_node_new() {
	return &false_node_object;
}

void null_node_head(struct null_node_t* self) {
}

void null_node_body(struct null_node_t* self) {
}

void null_node_write(struct null_node_t* self) {
}

void null_node_read(struct null_node_t* self) {
}

void null_node_build(struct null_node_t* self) {
}

void null_node_free(struct null_node_t* self) {
}

struct str_node_t* str_node_new(struct lng_token_t* token) {
	struct str_node_t* self = (struct str_node_t*) calloc(1, sizeof(struct str_node_t));
	if (!self) {
		return 0;
	}
	self->classid = LNG_STR_ID;
	return 0;
}

void str_node_head(struct str_node_t* self) {
}

void str_node_body(struct str_node_t* self) {
}

void str_node_write(struct str_node_t* self) {
}

void str_node_read(struct str_node_t* self) {
}

void str_node_build(struct str_node_t* self) {
}

void str_node_free(struct str_node_t* self) {
	free(self);
}

struct num_node_t* num_node_new(struct lng_token_t* token) {
	struct num_node_t* self = (struct num_node_t*) calloc(1, sizeof(struct num_node_t));
	if (!self) {
		return 0;
	}
	self->classid = LNG_STR_ID;
	return 0;
}

void num_node_head(struct num_node_t* self) {
}

void num_node_body(struct num_node_t* self) {
}

void num_node_write(struct num_node_t* self) {
}

void num_node_read(struct num_node_t* self) {
}

void num_node_build(struct num_node_t* self) {
}

void num_node_free(struct num_node_t* self) {
	free(self);
}

struct bool_node_t* bool_node_new(struct lng_token_t* token) {
	struct bool_node_t* self = (struct bool_node_t*) calloc(1, sizeof(struct bool_node_t));
	if (!self) {
		return 0;
	}
	self->classid = LNG_STR_ID;
	return 0;
}

void bool_node_head(struct bool_node_t* self) {
}

void bool_node_body(struct bool_node_t* self) {
}

void bool_node_write(struct bool_node_t* self) {
}

void bool_node_read(struct bool_node_t* self) {
}

void bool_node_build(struct bool_node_t* self) {
}

void bool_node_free(struct bool_node_t* self) {
	free(self);
}

struct array_node_t* array_node_new(struct lng_token_t* token) {
	struct array_node_t* self = (struct array_node_t*) calloc(1, sizeof(struct array_node_t));
	if (!self) {
		return 0;
	}
	self->classid = LNG_STR_ID;
	return 0;
}

void array_node_head(struct array_node_t* self) {
}

void array_node_body(struct array_node_t* self) {
}

void array_node_write(struct array_node_t* self) {
}

void array_node_read(struct array_node_t* self) {
}

void array_node_build(struct array_node_t* self) {
}

void array_node_free(struct array_node_t* self) {
	free(self);
}

struct id_node_t* id_node_new(struct lng_token_t* token) {
	struct id_node_t* self = (struct id_node_t*) calloc(1, sizeof(struct id_node_t));
	if (!self) {
		return 0;
	}
	self->classid = LNG_ID_ID;
	return 0;
}

void id_node_head(struct id_node_t* self) {
}

void id_node_body(struct id_node_t* self) {
}

void id_node_write(struct id_node_t* self) {
}

void id_node_read(struct id_node_t* self) {
}

void id_node_build(struct id_node_t* self) {
}

void id_node_free(struct id_node_t* self) {
	free(self);
}

void lng_keyword(const unsigned char* text, struct lng_token_t* token, unsigned int tokenSize) {
	int n;
	for (n = 0; n < tokenSize; n++) {
		if (token->type == UIL_ID) {
			const unsigned char* word = text + token->begin;
			switch (token->size) {
			case 2 : {
				if (word[0] == 'i' && word[1] == 'f') {
					token->type = LEXICAL_IF_ID;
				} else if (word[0] == 'd' && word[1] == 'o') {
					token->type = LEXICAL_DO_ID;
				} else if (word[0] == 'o' && word[1] == 'r') {
					token->type = LEXICAL_OR_ID;
				}
				break;
			}
			case 3 : {
				if (word[0] == 'f' && word[1] == 'o' && word[2] == 'r') {
					token->type = LEXICAL_FOR_ID;
				} else if (word[0] == 'e' && word[1] == 'n' && word[2] == 'd') {
					token->type = LEXICAL_END_ID;
				} else if (word[0] == 'n' && word[1] == 'u' && word[2] == 'm') {
					token->type = LEXICAL_NUM_ID;
				} else if (word[0] == 's' && word[1] == 't' && word[2] == 'r') {
					token->type = LEXICAL_STR_ID;
				} else if (word[0] == 'v' && word[1] == 'a' && word[2] == 'r') {
					token->type = LEXICAL_VAR_ID;
				} else if (word[0] == 'a' && word[1] == 'n' && word[2] == 'd') {
					token->type = LEXICAL_AND_ID;
				}
				break;
			}
			case 4 : {
				if (word[0] == 'b' && word[1] == 'o' && word[2] == 'o' && word[3] == 'l') {
					token->type = LEXICAL_BOOL_ID;
				}
				if (word[0] == 't' && word[1] == 'r' && word[2] == 'u' && word[3] == 'e') {
					token->type = LEXICAL_TRUE_ID;
				}
				break;
			}
			case 5 : {
				if (word[0] == 'w' && word[1] == 'h' && word[2] == 'i' && word[3] == 'l' && word[4] == 'e') {
					token->type = LEXICAL_WHILE_ID;
				} else if (word[0] == 'b' && word[1] == 'e' && word[2] == 'g' && word[3] == 'i' && word[4] == 'n') {
					token->type = LEXICAL_BEGIN_ID;
				} else if (word[0] == 'b' && word[1] == 'r' && word[2] == 'e' && word[3] == 'a' && word[4] == 'k') {
					token->type = LEXICAL_BREAK_ID;
				} else if (word[0] == 'f' && word[1] == 'a' && word[2] == 'l' && word[3] == 's' && word[4] == 'e') {
					token->type = LEXICAL_FALSE_ID;
				}
				break;
			}
			case 8 : {
				if (word[0] == 'c' && word[1] == 'o' && word[2] == 'n' && word[3] == 't' && word[4] == 'i' && word[5] == 'n' && word[6] == 'u' && word[7] == 'e') {
					token->type = LEXICAL_CONTINUE_ID;
				}
				break;
			}
			}
		}
		token++;
	}
}

struct type_node_t* lng_type(struct lng_context_t* context) {
	switch (context->token->type) {
	case UIL_EOF : {
		context->error = EXPECTED_TYPE_EOF;
		context->error_token = context->token;
		return 0;
	}
	case UIL_ID : {
		if (context->test) {
			context->memory_size += sizeof(struct id_type_node_t);
		}
		context->token++;
		return (void*) 1;
	}
	case LNG_NUM_TYPE_ID : {
		context->token++;
		return (void*) 1;
	}
	case LNG_STR_TYPE_ID : {
		context->token++;
		return (void*) 1;
	}
	case LNG_BOOL_TYPE_ID : {
		context->token++;
		return (void*) 1;
	}
	default : {
		context->error = EXPECTED_TYPE_IN_TYPE;
		context->error_token = context->token;
		return 0;
	}
	}
}

struct rvalue_node_t** lng_params_exp(struct lng_context_t* context) {
	context->token++;
	for (;;) {
		switch (context->token->type) {
		case UIL_EOF : {
			context->error = EXPECTED_CLOSE_CALL_IN_ID;
			context->error_token = context->token;
			return 0;
		}
		case ')' : {
			context->token++;
			return (void*) 1;
		}
		default : {
			struct rvalue_node_t* exp = lng_exp(context);
			if (!exp) {
				return 0;
			}
			switch (context->token->type) {
			case UIL_EOF : {
				context->error = "EOF";
				break;
			}
			case ',' : {
				context->token++;
				break;
			}
			case ')' : {
				context->token++;
				return (void*) 1;
			}
			}
		}
		}
	}return 0;
}

struct rvalue_node_t* lng_id_exp(struct lng_context_t* context) {
	context->token++;
	for (;;) {
		switch (context->token->type) {
		case '.' : {
			context->token++;
			if (context->token->type != UIL_ID) {
				context->error = EXPECTED_ID_IN_CALL;
				context->error_token = context->token;
				return 0;
			}
			context->token++;
			break;
		}
		case '[' : {
			context->token++;
			struct rvalue_node_t* exp = lng_exp(context);
			if (!exp) {
				return 0;
			}
			if (context->token->type != ']') {
				context->error = EXPECTED_CLOSE_ARRAY_IN_ID;
				context->error_token = context->token;
				return 0;
			}
			context->token++;
			break;
		}
		case '(' : {
			lng_params_exp(context);
			break;
		}
		default : {
			return (struct rvalue_node_t*) 1;
		}
		}
	}return 0;
}

struct rvalue_node_t* lng_literal_exp(struct lng_context_t* context) {
	switch (context->token->type) {
	case '(' : {
		context->token++;
		struct rvalue_node_t* exp = lng_exp(context);
		if (!exp) {
			context->error = "ffff";
			context->error_token = context->token;
			return 0;
		}
		if (context->token->type != ')') {
			context->error = EXPECTED_CLOSE_IN_LITERAL;
			context->error_token = context->token;
			return 0;
		}
		context->token++;
		break;
	}
	case '#' : {
		if (context->test) {
			context->memory_size += sizeof(struct str_node_t);
		}
		context->token++;
		context->token++;
		break;
	}
	case '$' : {
		if (context->test) {
			context->memory_size += sizeof(struct str_node_t);
		}
		context->token++;
		context->token++;
		break;
	}
	case UIL_ID : {
		if (context->test) {
			context->memory_size += sizeof(struct id_node_t);
		}
		context->token++;
		break;
	}
	case LEXICAL_TRUE_ID : {
//		return (struct rvalue_node_t*) &true_node_object;
		context->token++;
		break;
	}
	case LEXICAL_FALSE_ID : {
//		return (struct rvalue_node_t*) &false_node_object;
		context->token++;
		break;
	}
	case UIL_NUMBER : {
		if (context->test) {
			context->memory_size += sizeof(struct num_node_t);
		}
		context->token++;
		break;
	}
	case UIL_STRING : {
		if (context->test) {
			context->memory_size += sizeof(struct str_node_t);
		}
		context->token++;
		break;
	}
	default : {
		context->error = EXPECTED_LITERAL_IN_EXP;
		context->error_token = context->token;
		return 0;
	}
	}
	for (;;) {
		switch (context->token->type) {
		case '.' : {
			if (context->test) {
				context->memory_size += sizeof(struct field_get_node_t);
			}
			context->token++;
			if (context->token->type != UIL_ID) {
				context->error = EXPECTED_ID_IN_CALL;
				context->error_token = context->token;
				return 0;
			}
			context->token++;
			break;
		}
		case '[' : {
			if (context->test) {
				context->memory_size += sizeof(struct array_node_t);
			}
			context->token++;
			struct rvalue_node_t* exp = lng_exp(context);
			if (!exp) {
				return 0;
			}
			if (context->token->type != ']') {
				context->error = EXPECTED_CLOSE_ARRAY_IN_ID;
				context->error_token = context->token;
				return 0;
			}
			context->token++;
			break;
		}
		case '(' : {
			if (context->test) {
				context->memory_size += sizeof(struct call_node_t);
			}
			lng_params_exp(context);
			break;
		}
		default : {
			return (struct rvalue_node_t*) 1;
		}
		}
	}
	return 0;
}

struct rvalue_node_t* lng_unary_exp(struct lng_context_t* context) {
	switch (context->token->type) {
	case '!' : {
		if (context->test) {
			context->memory_size += sizeof(struct not_node_t);
		}
		context->token++;
		return lng_literal_exp(context);
		break;
	}
	default : {
		return lng_literal_exp(context);
	}
	}
}

struct rvalue_node_t* lng_bit_exp(struct lng_context_t* context) {
	struct rvalue_node_t* left = lng_unary_exp(context);
	if (!left) {
		return 0;
	}
	for (;;) {
		switch (context->token->type) {
		case UIL_GREATER_GREATER : {
			if (context->test) {
				context->memory_size += sizeof(struct greater_greater_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_unary_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		case UIL_LOWER_LOWER : {
			if (context->test) {
				context->memory_size += sizeof(struct lower_lower_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_unary_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		default : {
			return left;
		}
		}
	}return 0;
}

struct rvalue_node_t* lng_mul_exp(struct lng_context_t* context) {
	struct rvalue_node_t* left = lng_bit_exp(context);
	if (!left) {
		return 0;
	}
	for (;;) {
		switch (context->token->type) {
		case '*' : {
			if (context->test) {
				context->memory_size += sizeof(struct mul_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_bit_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		case '/' : {
			if (context->test) {
				context->memory_size += sizeof(struct div_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_bit_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		default : {
			return left;
		}
		}
	}return 0;
}

struct rvalue_node_t* lng_sum_exp(struct lng_context_t* context) {
	struct rvalue_node_t* left = lng_mul_exp(context);
	if (!left) {
		return 0;
	}
	for (;;) {
		switch (context->token->type) {
		case '+' : {
			if (context->test) {
				context->memory_size += sizeof(struct sum_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_mul_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		case '-' : {
			if (context->test) {
				context->memory_size += sizeof(struct sub_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_mul_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		default : {
			return left;
		}
		}
	}return 0;
}

struct rvalue_node_t* lng_compare_exp(struct lng_context_t* context) {
	struct rvalue_node_t* left = lng_sum_exp(context);
	if (!left) {
		return 0;
	}
	for (;;) {
		switch (context->token->type) {
		case UIL_EQUAL_EQUAL : {
			if (context->test) {
				context->memory_size += sizeof(struct equal_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_sum_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		case UIL_DIFERENT : {
			if (context->test) {
				context->memory_size += sizeof(struct not_equal_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_sum_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		case '>' : {
			if (context->test) {
				context->memory_size += sizeof(struct greater_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_sum_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		case UIL_GREATER_EQUAL : {
			if (context->test) {
				context->memory_size += sizeof(struct greater_equal_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_sum_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		case '<' : {
			if (context->test) {
				context->memory_size += sizeof(struct lower_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_sum_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		case UIL_LOWER_EQUAL : {
			if (context->test) {
				context->memory_size += sizeof(struct lower_equal_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_sum_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		default : {
			return left;
		}
		}
	}return 0;
}

struct rvalue_node_t* lng_and_exp(struct lng_context_t* context) {
	struct rvalue_node_t* left = lng_compare_exp(context);
	if (!left) {
		return 0;
	}
	for (;;) {
		switch (context->token->type) {
		case LEXICAL_AND_ID : {
			if (context->test) {
				context->memory_size += sizeof(struct and_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_compare_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		default : {
			return left;
		}
		}
	}return 0;
}

struct rvalue_node_t* lng_or_exp(struct lng_context_t* context) {
	struct rvalue_node_t* left = lng_and_exp(context);
	if (!left) {
		return 0;
	}
	for (;;) {
		switch (context->token->type) {
		case LEXICAL_OR_ID : {
			if (context->test) {
				context->memory_size += sizeof(struct or_node_t);
			}
			context->token++;
			struct rvalue_node_t* right = lng_and_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		default : {
			return left;
		}
		}
	}return 0;
}

struct rvalue_node_t* lng_ternary_exp(struct lng_context_t* context) {
	struct rvalue_node_t* left = lng_or_exp(context);
	if (!left) {
		return 0;
	}
	for (;;) {
		switch (context->token->type) {
		case '?' : {
			if (context->test) {
				context->memory_size += sizeof(struct ternary_node_t);
			}
			context->token++;
			struct rvalue_node_t* center = lng_or_exp(context);
			if (!center) {
				return 0;
			}
			if (context->token->type != ':') {
				context->error = EXPECTED_CLOSE_ARRAY_IN_ID;
				context->error_token = context->token;
				return 0;
			}
			context->token++;
			struct rvalue_node_t* right = lng_or_exp(context);
			if (!right) {
				return 0;
			}
			break;
		}
		default : {
			return left;
		}
		}
	}return 0;
}

struct rvalue_node_t* lng_exp(struct lng_context_t* context) {
	return lng_ternary_exp(context);
}

struct node_t* lng_block_command(struct lng_context_t* context) {
	int command_count = 0;
	if (context->test) {
		context->memory_size += sizeof(struct block_node_t);
	}
	context->token++;
	for (;;) {
		switch (context->token->type) {
		case UIL_EOF : {
			context->error = EXPECTED_END_BLOCK;
			context->error_token = context->token;
			return 0;
		}
		case LEXICAL_END_ID : {
			if (context->test) {
				context->memory_size += sizeof(struct command_node_t*) * command_count;
			}
			context->token++;
			return (void*) 1;
		}
		default : {
			struct command_node_t* command = lng_command(context);
			if (!command) {
				return 0;
			}
			command_count++;
		}
		}
	}
	return 0;
}

struct command_node_t** lng_commands(struct lng_context_t* context) {
	int command_count = 0;
	if (context->test) {
		context->memory_size += sizeof(struct block_node_t);
	}
	for (; context->token->type != UIL_EOF;) {
		struct command_node_t* cmd = lng_command(context);
		if (!cmd) {
			printf("%s at lin:%d and col:%d\n", context->error, context->error_token->line, context->error_token->column);
			return 0;
		}
		command_count++;
	}
	if (context->test) {
		context->memory_size += sizeof(struct command_node_t*) * command_count;
	}
	return (void*) 1;
}

struct node_t* lng_var_command(struct lng_context_t* context) {
	if (context->test) {
		context->memory_size += sizeof(struct declare_node_t);
	}
	context->token++;
	struct type_node_t* type = lng_type(context);
	if (!type) {
		return 0;
	}
	if (context->token->type != UIL_ID) {
		context->error = EXPECTED_ID_DECLARE;
		context->error_token = context->token;
		return 0;
	}
//	char* id = context->text + context->token->begin;
	context->token++;
	if (context->token->type == '=') {
		context->token++;
		struct rvalue_node_t* value = lng_exp(context);
		if (!value) {
			return 0;
		}
	}
	return (void*) 1;
}

struct node_t* lng_if_command(struct lng_context_t* context) {
	if (context->test) {
		context->memory_size += sizeof(struct if_node_t);
	}
	context->token++;
	struct rvalue_node_t* exp = lng_exp(context);
	if (!exp) {
		return 0;
	}
	struct command_node_t* command = lng_command(context);
	if (!command) {
		return 0;
	}
	return (void*) 1;
}

struct node_t* lng_for_command(struct lng_context_t* context) {
	return 0;
}

struct node_t* lng_while_command(struct lng_context_t* context) {
	if (context->test) {
		context->memory_size += sizeof(struct while_node_t);
	}
	context->token++;
	struct rvalue_node_t* exp = lng_exp(context);
	if (!exp) {
		return 0;
	}
	struct command_node_t* command = lng_command(context);
	if (!command) {
		return 0;
	}
	return (void*) 1;
}

struct node_t* lng_do_command(struct lng_context_t* context) {
	if (context->test) {
		context->memory_size += sizeof(struct repeat_node_t);
	}
	context->token++;
	struct command_node_t* command = lng_command(context);
	if (!command) {
		return 0;
	}
	if (context->token->type != LNG_WHILE_ID) {
		context->error = EXPECTED_REPEAT_WHILE;
		context->error_token = context->token;
		return 0;
	}
	context->token++;
	struct rvalue_node_t* exp = lng_exp(context);
	if (!exp) {
		return 0;
	}
	return (void*) 1;
}

struct rvalue_node_t* lng_exp_command(struct lng_context_t* context) {
	struct rvalue_node_t* value = lng_exp(context);
	if (!value) {
		return 0;
	}
	return value;
}

struct command_node_t* lng_command(struct lng_context_t* context) {
	switch (context->token->type) {
	case UIL_EOF : {
		context->error = EXPECTED_COMMAND_IN_COMMAND;
		context->error_token = 0;
		return 0;
	}
	case LEXICAL_BEGIN_ID : {
		return (struct command_node_t*) lng_block_command(context);
	}
	case LEXICAL_VAR_ID : {
		return (struct command_node_t*) lng_var_command(context);
	}
	case LEXICAL_IF_ID : {
		return (struct command_node_t*) lng_if_command(context);
	}
	case LEXICAL_FOR_ID : {
		return (struct command_node_t*) lng_for_command(context);
	}
	case LEXICAL_WHILE_ID : {
		return (struct command_node_t*) lng_while_command(context);
	}
	case LEXICAL_DO_ID : {
		return (struct command_node_t*) lng_do_command(context);
	}
	default : {
		return (struct command_node_t*) lng_exp_command(context);
	}
	}
}

struct command_node_t* lng_execute(struct lng_context_t* context) {
	return (struct command_node_t*) lng_command(context);
}

struct node_t* lng(const unsigned char* text) {
	unsigned int tokenSize;
	struct lng_token_t* tokens = lexical(text, &tokenSize);
	if (!tokens) {
		return 0;
	}
	lng_keyword(text, tokens, tokenSize);
	struct lng_context_t context;
	memset(&context, 0, sizeof(struct lng_context_t));
	context.test = 1;
	context.text = text;
	context.token = tokens;
	context.tokenSize = tokenSize;
	free(calloc(1, context.memory_size));
	lng_commands(&context);
	context.test = 1;
	context.token = tokens;
	context.memory_size = 0;
	lng_commands(&context);
	free(tokens);
	return 0;
}
