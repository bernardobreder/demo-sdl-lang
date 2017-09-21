#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uil.h"
#include "widget.h"
#include "struct.h"
#include "lexical.h"
#include "list.h"

struct uil_context_t {
	struct uil_t* uil;
	struct lng_token_t* token;
	unsigned int tokenIndex;
	unsigned int tokenSize;
};

static char* uil_object_to_chars(unsigned char* text, unsigned int len);

static int uil_object_to_int(unsigned char* text, unsigned int len);

static unsigned int uil_object_to_uint(unsigned char* text, unsigned int len);

static void uil_object_to_5int(unsigned char* text, unsigned int len, int* pa, int* pb, int* pc, int* pd, int* pe);

static struct list_t* uil_execute_array(struct uil_context_t* context);

static struct widget_t* uil_execute_struct(struct uil_context_t* context);

struct widget_t* uil_eval(unsigned char* text) {
	struct uil_t* self = uil_new(text);
	if (!self) {
		return 0;
	}
	unsigned int tokenSize;
	struct lng_token_t* tokens = lexical((const unsigned char*) self->text, &tokenSize);
	if (!tokens) {
		uil_free(self);
		return 0;
	}
	self->tokens = tokens;
	self->tokenSize = tokenSize;
	struct widget_t* widget = uil_execute(self);
	uil_free(self);
	return widget;
}

struct uil_t* uil_new(unsigned char* text) {
	struct uil_t* self = (struct uil_t*) calloc(1, sizeof(struct uil_t));
	if (!self) {
		return 0;
	}
	self->text = text;
	return self;
}

void uil_free(struct uil_t* self) {
	if (self->text) {
		free(self->text);
	}
	if (self->tokens) {
		free(self->tokens);
	}
	free(self);
}

static struct list_t* uil_execute_array(struct uil_context_t* context) {
	struct list_t* list = 0;
	if (context->tokenIndex < context->tokenSize && context->token->type == '[') {
		context->token++;
		context->tokenIndex++;
		int index = 0;
		for (; context->tokenIndex < context->tokenSize && context->token->type != ']';) {
			if (context->token->type == UIL_ID) {
				struct widget_t* value = uil_execute_struct(context);
				if (!list) {
					list = list_new(10);
				}
				list_add(&list, value);
				if (context->tokenIndex < context->tokenSize && context->token->type == ',') {
					context->token++;
					context->tokenIndex++;
				}
				index++;
			} else {
				context->token++;
				context->tokenIndex++;
			}
		}
		context->token++;
		context->tokenIndex++;
	}
	return list;
}

static struct widget_t* uil_execute_widget(struct uil_context_t* context) {
	unsigned char* name = context->uil->text + context->token->begin;
	unsigned int len = context->token->size;
	switch (len) {
	case 4 : {
		if (name[0] == 't' && name[1] == 'e' && name[2] == 'x' && name[3] == 't') {
			return (struct widget_t*) text_new();
		} else if (name[0] == 'a' && name[1] == 'r' && name[2] == 'e' && name[3] == 'a') {
			return (struct widget_t*) area_new();
		}
		break;
	}
	case 5 : {
		if (name[0] == 'l' && name[1] == 'a' && name[2] == 'b' && name[3] == 'e' && name[4] == 'l') {
			return (struct widget_t*) label_new();
		} else if (name[0] == 'i' && name[1] == 'm' && name[2] == 'a' && name[3] == 'g' && name[4] == 'e') {
			return (struct widget_t*) image_new();
		}
		break;
	}
	case 6 : {
		if (name[0] == 's' && name[1] == 'c' && name[2] == 'r' && name[3] == 'o' && name[4] == 'l' && name[5] == 'l') {
			return (struct widget_t*) scroll_new(0);
		} else if (name[0] == 'b' && name[1] == 'u' && name[2] == 't' && name[3] == 't' && name[4] == 'o' && name[5] == 'n') {
			return (struct widget_t*) button_new();
		} else if (name[0] == 'b' && name[1] == 'o' && name[2] == 'r' && name[3] == 'd' && name[4] == 'e' && name[5] == 'r') {
			return (struct widget_t*) border_new();
		}
		break;
	}
	case 8 : {
		if (name[0] == 'v' && name[1] == 'e' && name[2] == 'r' && name[3] == 't' && name[4] == 'i' && name[5] == 'c' && name[6] == 'a' && name[7] == 'l') {
			return (struct widget_t*) vertical_new(10);
		}
		break;
	}
	case 10 : {
		if (name[0] == 'h' && name[1] == 'o' && name[2] == 'r' && name[3] == 'i' && name[4] == 'z' && name[5] == 'o' && name[6] == 'n' && name[7] == 't' && name[8] == 'a' && name[9] == 'l') {
			return (struct widget_t*) horizontal_new(10);
		}
		break;
	}
	}
	return (struct widget_t*) label_new();
}

static error_t uil_execute_set(struct uil_context_t* context, struct widget_t* widget, unsigned char* keyName, unsigned int keyLen, unsigned char* valueName, unsigned int valueLen,
		unsigned short valueType) {
	switch (keyLen) {
	case 2 : {
		if (keyName[0] == 'i' && keyName[1] == 'd') {
			char* text = uil_object_to_chars(valueName, valueLen);
			if (text) {
				widget_set_id(widget, text);
			}
			context->token++;
			context->tokenIndex++;
			return 0;
		} else if (widget->classid == WIDGET_BORDER_CLASSID && valueType == UIL_ID && keyName[0] == 'u' && keyName[1] == 'p') {
			struct widget_t* value = uil_execute_struct(context);
			border_add_child((struct border_t*) widget, value, BORDER_UP);
			return 0;
		}
		break;
	}
	case 4 : {
		if (valueType == UIL_STRING && keyName[0] == 't' && keyName[1] == 'e' && keyName[2] == 'x' && keyName[3] == 't') {
			char* text = uil_object_to_chars(valueName, valueLen);
			if (text) {
				switch (widget->classid) {
				case WIDGET_LABEL_CLASSID :
					label_set_text((struct label_t*) widget, text);
				case WIDGET_TEXT_CLASSID :
					text_set_text((struct text_t*) widget, text);
				case WIDGET_AREA_CLASSID :
					area_set_text((struct area_t*) widget, text);
				case WIDGET_BUTTON_CLASSID :
					button_set_text((struct button_t*) widget, text);
				}
			}
			context->token++;
			context->tokenIndex++;
			return 0;
		} else if (widget->classid == WIDGET_BORDER_CLASSID && valueType == UIL_ID && keyName[0] == 'd' && keyName[1] == 'o' && keyName[2] == 'w' && keyName[3] == 'n') {
			struct widget_t* value = uil_execute_struct(context);
			border_add_child((struct border_t*) widget, value, BORDER_DOWN);
			return 0;
		} else if (widget->classid == WIDGET_BORDER_CLASSID && valueType == UIL_ID && keyName[0] == 'l' && keyName[1] == 'e' && keyName[2] == 'f' && keyName[3] == 't') {
			struct widget_t* value = uil_execute_struct(context);
			border_add_child((struct border_t*) widget, value, BORDER_LEFT);
			return 0;
		} else if (widget->classid == WIDGET_IMAGE_CLASSID && keyName[0] == 'p' && keyName[1] == 'a' && keyName[2] == 't' && keyName[3] == 'h') {
			char* text = uil_object_to_chars(valueName, valueLen);
			if (text) {
				image_set_path((struct image_t*) widget, text);
			}
			context->token++;
			context->tokenIndex++;
			return 0;
		} else if (widget->classid == WIDGET_IMAGE_CLASSID && keyName[0] == 'c' && keyName[1] == 'l' && keyName[2] == 'i' && keyName[3] == 'p') {
			int x, y, w, h;
			uil_object_to_5int(valueName, valueLen, &x, &y, &w, &h, 0);
			image_set_clip((struct image_t*) widget, x, y, w, h);
			context->token++;
			context->tokenIndex++;
			return 0;
		}
		break;
	}
	case 5 : {
		if (keyName[0] == 'w' && keyName[1] == 'i' && keyName[2] == 'd' && keyName[3] == 't' && keyName[4] == 'h') {
			int value = uil_object_to_int(valueName, valueLen);
			widget_set_pref_width(widget, value);
			context->token++;
			context->tokenIndex++;
			return 0;
		} else if (widget->classid == WIDGET_BORDER_CLASSID && valueType == UIL_ID && keyName[0] == 'r' && keyName[1] == 'i' && keyName[2] == 'g' && keyName[3] == 'h' && keyName[4] == 't') {
			struct widget_t* value = uil_execute_struct(context);
			border_add_child((struct border_t*) widget, value, BORDER_RIGHT);
			return 0;
		}
		break;
	}
	case 6 : {
		if (keyName[0] == 'h' && keyName[1] == 'e' && keyName[2] == 'i' && keyName[3] == 'g' && keyName[4] == 'h' && keyName[5] == 't') {
			int value = uil_object_to_int(valueName, valueLen);
			widget_set_pref_height(widget, value);
			context->token++;
			context->tokenIndex++;
			return 0;
		} else if (keyName[0] == 'm' && keyName[1] == 'a' && keyName[2] == 'r' && keyName[3] == 'g' && keyName[4] == 'i' && keyName[5] == 'n') {
			int left, top, right, bottom, middle;
			uil_object_to_5int(valueName, valueLen, &left, &top, &right, &bottom, &middle);
			widget_set_margin(widget, &left, &top, &right, &bottom, &middle);
			context->token++;
			context->tokenIndex++;
			return 0;
		} else if (widget->classid == WIDGET_BORDER_CLASSID && valueType == UIL_ID && keyName[0] == 'c' && keyName[1] == 'e' && keyName[2] == 'n' && keyName[3] == 't' && keyName[4] == 'e'
				&& keyName[5] == 'r') {
			struct widget_t* value = uil_execute_struct(context);
			border_add_child((struct border_t*) widget, value, BORDER_CENTER);
			return 0;
		}
		break;
	}
	case 7 : {
		if (keyName[0] == 'c' && keyName[1] == 'o' && keyName[2] == 'n' && keyName[3] == 't' && keyName[4] == 'e' && keyName[5] == 'n' && keyName[6] == 't') {
			if (valueType == UIL_ID) {
				struct widget_t* value = uil_execute_struct(context);
				if (value) {
					switch (widget->classid) {
					case WIDGET_SCROLL_CLASSID :
						scroll_set_widget((struct scroll_t*) widget, value);
						break;
					}
				}
				return 0;
			} else if (valueType == '[') {
				struct list_t* list = uil_execute_array(context);
				if (list) {
					struct widget_t** widgets = (struct widget_t**) list_iterator(list);
					unsigned int n, size = list_size(list);
					for (n = 0; n < size; n++) {
						struct widget_t* child = *(widgets++);
						switch (widget->classid) {
						case WIDGET_BORDER_CLASSID : {
							border_add_child((struct border_t*) widget, child, n);
							break;
						}
						case WIDGET_VERTICAL_CLASSID : {
							if (child) {
								vertical_add_child((struct vertical_t*) widget, child);
							}
							break;
						}
						case WIDGET_HORIZONTAL_CLASSID : {
							if (child) {
								horizontal_add_child((struct horizontal_t*) widget, child);
							}
							break;
						}
						}
					}
					list_free(list, 0);
				}
				return 0;
			}
		} else if (keyName[0] == 'b' && keyName[1] == 'g' && keyName[2] == 'c' && keyName[3] == 'o' && keyName[4] == 'l' && keyName[5] == 'o' && keyName[6] == 'r') {
			unsigned int value = uil_object_to_uint(valueName, valueLen);
			widget_set_bgcolor((struct widget_t*) widget, value);
			return 0;
		} else if (keyName[0] == 'p' && keyName[1] == 'a' && keyName[2] == 'd' && keyName[3] == 'd' && keyName[4] == 'i' && keyName[5] == 'n' && keyName[6] == 'g') {
			int left, top, right, bottom;
			uil_object_to_5int(valueName, valueLen, &left, &top, &right, &bottom, 0);
			widget_set_padding(widget, &left, &top, &right, &bottom);
			return 0;
		}
		break;
	}
	case 8 : {
		if (keyName[0] == 'f' && keyName[1] == 'o' && keyName[2] == 'n' && keyName[3] == 't' && keyName[4] == 's' && keyName[5] == 'i' && keyName[6] == 'z' && keyName[7] == 'e') {
			unsigned int value = uil_object_to_uint(valueName, valueLen);
			switch (widget->classid) {
			case WIDGET_LABEL_CLASSID : {
				label_set_font_size((struct label_t*) widget, value);
				break;
			}
			}
			return 0;
		}
		break;
	}
	case 9 : {
		if (keyName[0] == 'f' && keyName[1] == 'o' && keyName[2] == 'r' && keyName[3] == 'e' && keyName[4] == 'c' && keyName[5] == 'o' && keyName[6] == 'l' && keyName[7] == 'o' && keyName[8] == 'r') {
			unsigned int value = uil_object_to_uint(valueName, valueLen);
			switch (widget->classid) {
			case WIDGET_LABEL_CLASSID : {
				label_set_forecolor((struct label_t*) widget, value);
				break;
			}
			}
			return 0;
		} else if (keyName[0] == 'f' && keyName[1] == 'o' && keyName[2] == 'n' && keyName[3] == 't' && keyName[4] == '-' && keyName[5] == 's' && keyName[6] == 'i' && keyName[7] == 'z'
				&& keyName[8] == 'e') {
			unsigned int value = uil_object_to_uint(valueName, valueLen);
			switch (widget->classid) {
			case WIDGET_LABEL_CLASSID : {
				label_set_font_size((struct label_t*) widget, value);
				break;
			}
			}
			return 0;
		}
		break;
	}
	case 11 : {
		if (keyName[0] == 'b' && keyName[1] == 'o' && keyName[2] == 'r' && keyName[3] == 'd' && keyName[4] == 'e' && keyName[5] == 'r' && keyName[6] == '-' && keyName[7] == 's' && keyName[8] == 'i'
				&& keyName[9] == 'z' && keyName[10] == 'e') {
			unsigned int value = uil_object_to_uint(valueName, valueLen);
			widget_set_border_size(widget, value);
			return 0;
		}
		break;
	}
	case 12 : {
		if (keyName[0] == 'b' && keyName[1] == 'o' && keyName[2] == 'r' && keyName[3] == 'd' && keyName[4] == 'e' && keyName[5] == 'r' && keyName[6] == '-' && keyName[7] == 'c' && keyName[8] == 'o'
				&& keyName[9] == 'l' && keyName[10] == 'o' && keyName[11] == 'r') {
			unsigned int value = uil_object_to_uint(valueName, valueLen);
			widget_set_border_color(widget, value);
			return 0;
		}
		break;
	}
	}
	return 1;
}

static struct widget_t* uil_execute_struct(struct uil_context_t* context) {
	struct widget_t* widget = uil_execute_widget(context);
	context->token++;
	context->tokenIndex++;
	if (context->tokenIndex < context->tokenSize) {
		if (context->token->type == '{') {
			context->token++;
			context->tokenIndex++;
			for (; context->tokenIndex < context->tokenSize && context->token->type != '}';) {
				if (context->token->type == UIL_ID) {
					unsigned char* keyName = context->uil->text + context->token->begin;
					unsigned int keyLen = context->token->size;
					context->token++;
					context->tokenIndex++;
					if (context->tokenIndex < context->tokenSize) {
						if (context->token->type < UIL_FIRST) {
							context->token++;
							context->tokenIndex++;
						}
						if (context->tokenIndex < context->tokenSize) {
							unsigned char* valueName = context->uil->text + context->token->begin;
							unsigned int valueLen = context->token->size;
							unsigned short valueType = context->token->type;
							if (uil_execute_set(context, widget, keyName, keyLen, valueName, valueLen, valueType)) {
								switch (valueType) {
								case UIL_STRING :
								case UIL_NUMBER :
								case UIL_HEX :
									context->token++;
									context->tokenIndex++;
									break;
								case UIL_ID :
									uil_execute_struct(context);
								}
							}
						}
					}
				} else {
					context->token++;
					context->tokenIndex++;
				}
			}
			context->token++;
			context->tokenIndex++;
		} else {
			context->token++;
			context->tokenIndex++;
		}
	}
	return widget;
}

struct widget_t* uil_execute(struct uil_t* self) {
	if (!self) {
		return 0;
	}
	struct uil_context_t context;
	context.uil = self;
	context.token = self->tokens;
	context.tokenIndex = 0;
	context.tokenSize = self->tokenSize;
	return uil_execute_struct(&context);
}

static unsigned int uil_hex_to_uint(unsigned char* text, unsigned int len) {
	text += 2;
	len -= 2;
	unsigned int value = 0;
	if (len) while (len) {
		char c = *(text++);
		if (c != '0') {
			unsigned int pow16;
			switch (c) {
			case '1' :
				pow16 = 1;
				break;
			case '2' :
				pow16 = 2;
				break;
			case '3' :
				pow16 = 3;
				break;
			case '4' :
				pow16 = 4;
				break;
			case '5' :
				pow16 = 5;
				break;
			case '6' :
				pow16 = 6;
				break;
			case '7' :
				pow16 = 7;
				break;
			case '8' :
				pow16 = 8;
				break;
			case '9' :
				pow16 = 9;
				break;
			case 'a' :
			case 'A' :
				pow16 = 10;
				break;
			case 'b' :
			case 'B' :
				pow16 = 11;
				break;
			case 'c' :
			case 'C' :
				pow16 = 12;
				break;
			case 'd' :
			case 'D' :
				pow16 = 13;
				break;
			case 'e' :
			case 'E' :
				pow16 = 14;
				break;
			case 'f' :
			case 'F' :
				pow16 = 15;
				break;
			default :
				return 0xFFFFFFFF;
			}
			value += pow16 << ((len - 1) * 4);
		}
		len--;
	}
	return value;
}

static int uil_number_to_int(unsigned char* text, unsigned int len) {
	if (len <= 0) {
		return 0;
	}
	int value = 0;
	unsigned char isNegative = 0;
	if (text[0] == '-') {
		isNegative = 1;
		text++;
		len--;
	}
	while (len) {
		char c = *(text++);
		if (c != '0') {
			unsigned int pow10;
			switch (len) {
			case 1 :
				pow10 = 1;
				break;
			case 2 :
				pow10 = 10;
				break;
			case 3 :
				pow10 = 100;
				break;
			case 4 :
				pow10 = 1000;
				break;
			case 5 :
				pow10 = 10000;
				break;
			case 6 :
				pow10 = 100000;
				break;
			case 7 :
				pow10 = 1000000;
				break;
			case 8 :
				pow10 = 10000000;
				break;
			case 9 :
				pow10 = 100000000;
				break;
			case 10 :
				pow10 = 1000000000;
				break;
			default :
				return 2147483647;
			}
			value += (c - '0') * pow10;
		}
		len--;
	}
	if (isNegative) {
		value = -value;
	}
	return value;
}

static char* uil_object_to_chars(unsigned char* text, unsigned int len) {
	char c = *text;
	if (c == '\"' || c == '\'') {
		char* new_text = (char*) malloc((len - 2 + 1) * sizeof(unsigned char));
		if (!text) {
			return 0;
		}
		strncpy((char*) new_text, (char*) text + 1, len - 2);
		new_text[len - 2] = 0;
		return new_text;
	} else {
		char* new_text = (char*) malloc((len + 1) * sizeof(unsigned char));
		if (!text) {
			return 0;
		}
		strncpy((char*) new_text, (char*) text, len);
		new_text[len] = 0;
		return new_text;
	}
}

static int uil_object_to_int(unsigned char* text, unsigned int len) {
	if (len <= 0) {
		return 0;
	}
	char c = *text;
	if (c == '\"' || c == '\'') {
		return uil_number_to_int(text + 1, len - 1);
	} else if (len > 1 && text[1] == 'x') {
		if (len > 8) {
			unsigned char aux[len];
			aux[0] = '0';
			aux[1] = 'x';
			strncpy((char*) aux + 2, (char*) text + 4, len - 4);
			return uil_hex_to_uint(aux, len - 4);
		} else {
			return uil_hex_to_uint(text, len);
		}
	} else {
		return uil_number_to_int(text, len);
	}
}

static unsigned int uil_object_to_uint(unsigned char* text, unsigned int len) {
	char c = *text;
	if (c == '\"' || c == '\'') {
		int value = uil_number_to_int(text + 1, len - 1);
		return value < 0 ? 0 : value;
	} else if (len > 1 && text[1] == 'x') {
		return uil_hex_to_uint(text, len);
	} else {
		int value = uil_number_to_int(text, len);
		return value < 0 ? 0 : value;
	}
}

static void uil_object_to_5int(unsigned char* text, unsigned int len, int* pa, int* pb, int* pc, int* pd, int* pe) {
	int ra, rb, rc, rd, re;
	char c = *text;
	if (c == '\"' || c == '\'') {
		text++;
		len -= 2;
		unsigned char* aux = text;
		while (len && *aux <= ' ') {
			aux++;
			len--;
		}
		text = aux;
		while (len && *aux > ' ') {
			aux++;
			len--;
		}
		ra = rb = rc = rd = re = uil_object_to_int(text, aux - text);
		while (len && *aux <= ' ') {
			aux++;
			len--;
		}
		text = aux;
		while (len && *aux > ' ') {
			aux++;
			len--;
		}
		rb = rc = rd = re = uil_object_to_int(text, aux - text);
		while (len && *aux <= ' ') {
			aux++;
			len--;
		}
		text = aux;
		while (len && *aux > ' ') {
			aux++;
			len--;
		}
		rc = rd = re = uil_object_to_int(text, aux - text);
		while (len && *aux <= ' ') {
			aux++;
			len--;
		}
		text = aux;
		while (len && *aux > ' ') {
			aux++;
			len--;
		}
		rd = re = uil_object_to_int(text, aux - text);
		while (len && *aux <= ' ') {
			aux++;
			len--;
		}
		text = aux;
		while (len && *aux > ' ') {
			aux++;
			len--;
		}
		re = uil_object_to_int(text, aux - text);
	} else if (len > 1 && text[1] == 'x') {
		ra = rb = rc = rd = re = uil_hex_to_uint(text, len);
	} else {
		ra = rb = rc = rd = re = uil_number_to_int(text, len);
	}
	if (pa) {
		*pa = ra;
	}
	if (pb) {
		*pb = rb;
	}
	if (pc) {
		*pc = rc;
	}
	if (pd) {
		*pd = rd;
	}
	if (pe) {
		*pe = re;
	}
}

