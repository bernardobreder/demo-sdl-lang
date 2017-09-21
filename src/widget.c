/*
 * widget.c
 *
 *  Created on: 12/07/2013
 *      Author: bernardobreder
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "so.h"
#include "gui.h"
#include "list.h"
#include "widget.h"

struct label_token_t {
	char* word;
	int length;
	unsigned int color;
	unsigned int style;
};

unsigned char widget_get_need_to_layout(struct widget_t* self) {
	return self->needToLayout;
}

unsigned char widget_get_need_to_paint(struct widget_t* self) {
	return self->needToPaint;
}

void widget_set_need_to_paint(struct widget_t* self, unsigned char flag) {
	self->needToPaint = flag;
}

void widget_set_need_to_layout(struct widget_t* self, unsigned char flag) {
	self->needToLayout = flag;
}

void widget_get_do_layout(struct widget_t* self) {
	if (!self->needToLayout) {
		struct widget_t* parent = self;
		while (parent) {
			parent->needToLayout = 1;
			parent = (struct widget_t*) parent->parent;
		}
	}
}

void widget_get_do_paint(struct widget_t* self) {
	if (!self->needToPaint) {
		struct widget_t* parent = self;
		while (parent) {
			parent->needToPaint = 1;
			parent = (struct widget_t*) parent->parent;
		}
	}
}

const char* widget_get_id(struct widget_t* self) {
	return self->id;
}

struct widget_t* widget_set_id(struct widget_t* self, char* id) {
	self->id = id;
	return self;
}

int widget_get_x(struct widget_t* self) {
	return self->x;
}

int widget_get_x_abs(struct widget_t* self) {
	int value = self->x + self->padding_left;
	struct widget_t* parent = (struct widget_t*) self->parent;
	while (parent) {
		value += parent->x;
		parent = (struct widget_t*) parent->parent;
	}
	return value;
}

int widget_get_y_abs(struct widget_t* self) {
	int value = self->y + self->padding_top;
	struct widget_t* parent = (struct widget_t*) self->parent;
	while (parent) {
		value += parent->y;
		parent = (struct widget_t*) parent->parent;
	}
	return value;
}

int widget_get_padding_left_abs(struct widget_t* self) {
	int value = self->padding_left;
	struct widget_t* parent = (struct widget_t*) self->parent;
	while (parent) {
		value += parent->padding_left + parent->margin_left;
		parent = (struct widget_t*) parent->parent;
	}
	return value;
}

int widget_get_padding_right_abs(struct widget_t* self) {
	int value = self->padding_right;
	struct widget_t* parent = (struct widget_t*) self->parent;
	while (parent) {
		value += parent->padding_right + parent->margin_right;
		parent = (struct widget_t*) parent->parent;
	}
	return value;
}

int widget_get_padding_top_abs(struct widget_t* self) {
	int value = self->padding_top;
	struct widget_t* parent = (struct widget_t*) self->parent;
	while (parent) {
		value += parent->padding_top + parent->margin_top;
		parent = (struct widget_t*) parent->parent;
	}
	return value;
}

int widget_get_padding_bottom_abs(struct widget_t* self) {
	int value = self->padding_bottom;
	struct widget_t* parent = (struct widget_t*) self->parent;
	while (parent) {
		value += parent->padding_bottom + parent->margin_bottom;
		parent = (struct widget_t*) parent->parent;
	}
	return value;
}

void widget_set_x(struct widget_t* self, int value) {
	self->x = value;
}

int widget_get_y(struct widget_t* self) {
	return self->y;
}

void widget_set_y(struct widget_t* self, int value) {
	self->y = value;
}

unsigned int widget_get_width(struct widget_t* self) {
	return self->width;
}

void widget_set_width(struct widget_t* self, int value) {
	self->width = value;
}

unsigned int widget_get_height(struct widget_t* self) {
	return self->height;
}

void widget_set_height(struct widget_t* self, int value) {
	self->height = value;
}

unsigned int widget_get_pref_width(struct widget_t* self) {
	return self->pref_width;
}

void widget_set_pref_width(struct widget_t* self, int value) {
	self->pref_width = value;
}

unsigned int widget_get_pref_height(struct widget_t* self) {
	return self->pref_height;
}

void widget_set_pref_height(struct widget_t* self, int value) {
	self->pref_height = value;
}

struct widget_t* widget_set_margin(struct widget_t* self, int* left, int* top, int* right, int* bottom, int* middle) {
	if (left) {
		self->margin_left = *left;
	}
	if (top) {
		self->margin_top = *top;
	}
	if (right) {
		self->margin_right = *right;
	}
	if (bottom) {
		self->margin_bottom = *bottom;
	}
	if (middle) {
		self->margin_middle = *middle;
	}
	widget_get_do_paint(self);
	return self;
}

void widget_get_margin(struct widget_t* self, int* left, int* top, int* right, int* bottom, int* middle) {
	if (left) {
		*left = self->margin_left;
	}
	if (top) {
		*top = self->margin_top;
	}
	if (right) {
		*right = self->margin_right;
	}
	if (bottom) {
		*bottom = self->margin_bottom;
	}
	if (middle) {
		*middle = self->margin_middle;
	}
}

struct widget_t* widget_set_padding(struct widget_t* self, int* left, int* top, int* right, int* bottom) {
	if (left) {
		self->padding_left = *left;
	}
	if (top) {
		self->padding_top = *top;
	}
	if (right) {
		self->padding_right = *right;
	}
	if (bottom) {
		self->padding_bottom = *bottom;
	}
	widget_get_do_paint(self);
	return self;
}

void widget_get_padding(struct widget_t* self, int* left, int* top, int* right, int* bottom) {
	if (left) {
		*left = self->padding_left;
	}
	if (top) {
		*top = self->padding_top;
	}
	if (right) {
		*right = self->padding_right;
	}
	if (bottom) {
		*bottom = self->padding_bottom;
	}
}

struct widget_t* widget_set_bgcolor(struct widget_t* self, unsigned int color) {
	if (self->background_color != color) {
		self->background_color = color;
		widget_get_do_paint((struct widget_t*) self);
	}
	return self;
}

unsigned int widget_get_bgcolor(struct widget_t* self) {
	return self->background_color;
}

struct widget_t* widget_set_border_color(struct widget_t* self, unsigned int color) {
	if (self->border_color != color) {
		self->border_color = color;
		widget_get_do_paint((struct widget_t*) self);
	}
	return self;
}

unsigned int widget_get_border_color(struct widget_t* self) {
	return self->border_color;
}

struct widget_t* widget_set_border_size(struct widget_t* self, unsigned int value) {
	if (self->border_size != value) {
		self->border_size = value;
		widget_get_do_layout((struct widget_t*) self);
		widget_get_do_paint((struct widget_t*) self);
	}
	return self;
}

unsigned int widget_get_border_size(struct widget_t* self) {
	return self->border_size;
}

void widget_layout(struct widget_t* self, struct frame_t* frame) {
	switch (self->classid) {
	case WIDGET_LABEL_CLASSID: {
		label_layout((struct label_t*) self, frame);
		break;
	}
	case WIDGET_TEXT_CLASSID: {
		text_layout((struct text_t*) self, frame);
		break;
	}
	case WIDGET_AREA_CLASSID: {
		area_layout((struct area_t*) self, frame);
		break;
	}
	case WIDGET_BUTTON_CLASSID: {
		button_layout((struct button_t*) self, frame);
		break;
	}
	case WIDGET_IMAGE_CLASSID: {
		image_layout((struct image_t*) self, frame);
		break;
	}
	case WIDGET_SCROLL_CLASSID: {
		scroll_layout((struct scroll_t*) self, frame);
		break;
	}
	case WIDGET_BORDER_CLASSID: {
		border_layout((struct border_t*) self, frame);
		break;
	}
	case WIDGET_VERTICAL_CLASSID: {
		vertical_layout((struct vertical_t*) self, frame);
		break;
	}
	case WIDGET_HORIZONTAL_CLASSID: {
		horizontal_layout((struct horizontal_t*) self, frame);
		break;
	}
	case WIDGET_GRIDBAG_CLASSID: {
		gridbag_layout((struct gridbag_t*) self, frame);
		break;
	}
	case WIDGET_GRID_CLASSID: {
		grid_layout((struct grid_t*) self, frame);
		break;
	}
	}
}

void widget_paint(struct widget_t* self, struct frame_t* frame) {
	SDL_Renderer* ren = frame_get_renderer(frame);
	{
		unsigned char background_color_alpha = color_get_alpha(self->background_color);
		if (background_color_alpha < 255) {
			SDL_Rect clip, rect;
			so_ui_get_clip(ren, &clip);
			widget_get_rect_padding(self, &rect);
			if (SDL_HasIntersection(&clip, &rect)) {
				SDL_IntersectRect(&clip, &rect, &rect);
				if (background_color_alpha > 0) {
					so_ui_render_blend(ren, SDL_BLENDMODE_BLEND);
				}
				so_ui_set_color_uint(ren, self->background_color);
				so_ui_render_fill_rect(ren, &rect);
				if (background_color_alpha > 0) {
					so_ui_render_blend(ren, SDL_BLENDMODE_NONE);
				}
			}
		}
	}
	{
		unsigned int border_size = self->border_size;
		if (border_size > 0) {
			unsigned int border_color = color_get_alpha(self->border_color);
			if (border_color < 255) {
				SDL_Rect clip, rect;
				so_ui_get_clip(ren, &clip);
				widget_get_rect_padding(self, &rect);
				if (SDL_HasIntersection(&clip, &rect)) {
					SDL_IntersectRect(&clip, &rect, &rect);
					so_ui_set_color_uint(ren, border_color);
					so_ui_render_draw_rect(ren, &rect, border_size);
				}
			}
		}
	}
	switch (self->classid) {
	case WIDGET_LABEL_CLASSID: {
		label_paint((struct label_t*) self, frame);
		break;
	}
	case WIDGET_TEXT_CLASSID: {
		text_paint((struct text_t*) self, frame);
		break;
	}
	case WIDGET_AREA_CLASSID: {
		area_paint((struct area_t*) self, frame);
		break;
	}
	case WIDGET_BUTTON_CLASSID: {
		button_paint((struct button_t*) self, frame);
		break;
	}
	case WIDGET_IMAGE_CLASSID: {
		image_paint((struct image_t*) self, frame);
		break;
	}
	case WIDGET_SCROLL_CLASSID: {
		scroll_paint((struct scroll_t*) self, frame);
		break;
	}
	case WIDGET_BORDER_CLASSID: {
		border_paint((struct border_t*) self, frame);
		break;
	}
	case WIDGET_VERTICAL_CLASSID: {
		vertical_paint((struct vertical_t*) self, frame);
		break;
	}
	case WIDGET_HORIZONTAL_CLASSID: {
		horizontal_paint((struct horizontal_t*) self, frame);
		break;
	}
	case WIDGET_GRIDBAG_CLASSID: {
		gridbag_paint((struct gridbag_t*) self, frame);
		break;
	}
	case WIDGET_GRID_CLASSID: {
		grid_paint((struct grid_t*) self, frame);
		break;
	}
	}
}

struct panel_t* widget_get_parent(struct widget_t* self) {
	return self->parent;
}

void* widget_get_key_press(struct widget_t* self) {
	return self->key_press;
}

void* widget_get_key_release(struct widget_t* self) {
	return self->key_release;
}

void* widget_get_mouse_press(struct widget_t* self) {
	return self->mouse_press;
}

void* widget_get_mouse_release(struct widget_t* self) {
	return self->mouse_release;
}

void* widget_get_mouse_motion(struct widget_t* self) {
	return self->mouse_motion;
}

void* widget_get_mouse_wheel(struct widget_t* self) {
	return self->mouse_wheel;
}

void* widget_get_mouse_dragin(struct widget_t* self) {
	return self->mouse_dragin;
}

void* widget_get_mouse_dragout(struct widget_t* self) {
	return self->mouse_dragout;
}

void* widget_get_focus_got(struct widget_t* self) {
	return self->focus_got;
}

void* widget_get_focus_lost(struct widget_t* self) {
	return self->focus_lost;
}

void widget_set_key_press(struct widget_t* self, void (*e)(struct widget_t* self, SDL_Event* event)) {
	self->key_press = e;
}

void widget_set_key_release(struct widget_t* self, void (*e)(struct widget_t* self, SDL_Event* event)) {
	self->key_release = e;
}

void widget_set_mouse_press(struct widget_t* self, void (*e)(struct widget_t* self, SDL_Event* event)) {
	self->mouse_press = e;
}

void widget_set_mouse_release(struct widget_t* self, void (*e)(struct widget_t* self, SDL_Event* event)) {
	self->mouse_release = e;
}

void widget_set_mouse_motion(struct widget_t* self, void (*e)(struct widget_t* self, SDL_Event* event)) {
	self->mouse_motion = e;
}

void widget_set_mouse_dragin(struct widget_t* self, void (*e)(struct widget_t* self, SDL_Event* event)) {
	self->mouse_dragin = e;
}

void widget_set_mouse_dragout(struct widget_t* self, void (*e)(struct widget_t* self, SDL_Event* event)) {
	self->mouse_dragout = e;
}

void widget_set_mouse_focus_got(struct widget_t* self, void (*e)(struct widget_t* self, SDL_Event* event)) {
	self->focus_got = e;
}

void widget_set_mouse_focus_lost(struct widget_t* self, void (*e)(struct widget_t* self, SDL_Event* event)) {
	self->focus_lost = e;
}

unsigned char widget_native_mouse_wheel(struct widget_t* self, int x, int y, SDL_Event* e) {
	switch (self->classid) {
	case WIDGET_SCROLL_CLASSID: {
		return scroll_native_mouse_wheel((struct scroll_t*) self, x, y, e);
		break;
	}
	case WIDGET_BORDER_CLASSID: {
		return border_native_mouse_wheel((struct border_t*) self, x, y, e);
		break;
	}
	case WIDGET_VERTICAL_CLASSID: {
		return vertical_native_mouse_wheel((struct vertical_t*) self, x, y, e);
		break;
	}
	case WIDGET_HORIZONTAL_CLASSID: {
		return horizontal_native_mouse_wheel((struct horizontal_t*) self, x, y, e);
		break;
	}
	case WIDGET_GRIDBAG_CLASSID: {
		return gridbag_native_mouse_wheel((struct gridbag_t*) self, x, y, e);
		break;
	}
	case WIDGET_GRID_CLASSID: {
		return grid_native_mouse_wheel((struct grid_t*) self, x, y, e);
		break;
	}
	}
	return 0;
}

void widget_free(struct widget_t* self) {
	switch (self->classid) {
	case WIDGET_LABEL_CLASSID: {
		label_free((struct label_t*) self);
		break;
	}
	case WIDGET_TEXT_CLASSID: {
		text_free((struct text_t*) self);
		break;
	}
	case WIDGET_AREA_CLASSID: {
		area_free((struct area_t*) self);
		break;
	}
	case WIDGET_BUTTON_CLASSID: {
		button_free((struct button_t*) self);
		break;
	}
	case WIDGET_IMAGE_CLASSID: {
		image_free((struct image_t*) self);
		break;
	}
	case WIDGET_SCROLL_CLASSID: {
		scroll_free((struct scroll_t*) self);
		break;
	}
	case WIDGET_BORDER_CLASSID: {
		border_free((struct border_t*) self);
		break;
	}
	case WIDGET_VERTICAL_CLASSID: {
		vertical_free((struct vertical_t*) self);
		break;
	}
	case WIDGET_HORIZONTAL_CLASSID: {
		horizontal_free((struct horizontal_t*) self);
		break;
	}
	case WIDGET_GRIDBAG_CLASSID: {
		gridbag_free((struct gridbag_t*) self);
		break;
	}
	case WIDGET_GRID_CLASSID: {
		grid_free((struct grid_t*) self);
		break;
	}
	}
	if (self->id) {
		free(self->id);
	}
}

static error_t label_texture_set(struct label_t* self, struct frame_t* frame) {
	if (self->text && !self->textures) {
		// Para cada espaço, cria um token do texto e o espaço.
		// Caso tenha mais de um espaço, será criado um token vazio com o espaço
		// Caso tenha um tab, será criado um token com vários espaços e com um espaço
		// Caso tenha uma quebra de linha, será criado uma nova lista de tokens
		SDL_Renderer* ren = frame_get_renderer(frame);
		struct font_t* font = font_get_default(self->font_size);
		if (!font) {
			return 1;
		}
		TTF_Font* fontData = font_get_data(font);
		SDL_Color color = { color_get_red(self->fore_color), color_get_green(self->fore_color), color_get_blue(self->fore_color), 255 };
		if (self->wrap) {
			unsigned int number_of_line = 1;
			unsigned int number_of_token = 1;
			char* aux = self->text;
			while (*aux) {
				char c = *(aux++);
				if (c == '\t') {
					number_of_token++;
				} else if (c <= ' ') {
				} else if (c <= '\n') {
					number_of_line++;
					number_of_token++;
				} else if (c <= ' ') {
					number_of_token++;
				} else {
				}
			}
		} else {
			unsigned int number_of_line = 1;
			char* aux = self->text;
			while (*aux) {
				char c = *(aux++);
				if (c == '\n') {
					number_of_line++;
				}
			}
			self->textures = (SDL_Texture**) malloc(number_of_line * sizeof(SDL_Texture*));
			int index = 0;
			int begin = 0;
			int end = 0;
			aux = self->text;
			while (*aux) {
				char c = *(aux++);
				if (c == '\n' || *aux == 0) {
					if (*aux == 0) {
						end++;
						aux++;
					}
					int len = end - begin;
					char str[len + 1];
					str[len] = 0;
					strncpy(str, aux - len, len);
					SDL_Surface* surface = TTF_RenderUTF8_Blended(fontData, str, color);
					if (!surface) {
						int n;
						for (n = 0; n < self->texture_size; n++) {
							so_ui_texture_free(self->textures[n]);
						}
						self->texture_size = 0;
						return 1;
					}
					SDL_Texture* texture = so_ui_texture_new(ren, surface);
					so_ui_surface_free(surface);
					if (!texture) {
						int n;
						for (n = 0; n < self->texture_size; n++) {
							so_ui_texture_free(self->textures[n]);
						}
						self->texture_size = 0;
						return 1;
					}
					self->textures[index++] = texture;
					self->texture_size++;
					begin = ++end;
				} else {
					end++;
				}
			}
		}
	}
	return 0;
}

static SDL_Texture* label_get_texture(struct label_t* self, struct frame_t* frame) {
	label_texture_set(self, frame);
	if (self->text && !self->textTexture) {
		SDL_Renderer* ren = frame_get_renderer(frame);
		struct font_t* font = font_get_default(self->font_size);
		if (!font) {
			return 0;
		}
		SDL_Color color = { color_get_red(self->fore_color), color_get_green(self->fore_color), color_get_blue(self->fore_color), 255 };
		TTF_Font* fontData = font_get_data(font);
		SDL_Surface* surface = TTF_RenderUTF8_Blended(fontData, self->text, color);
		if (!surface) {
			return 0;
		}
		SDL_Texture* texture = so_ui_texture_new(ren, surface);
		if (!texture) {
			so_ui_surface_free(surface);
			return 0;
		}
		so_ui_surface_free(surface);
		self->textTexture = texture;
	}
	return self->textTexture;
}

static void label_texture_free(struct label_t* self) {
	int n;
	SDL_Texture** textures = self->textures;
	for (n = 0; n < self->texture_size; n++) {
		SDL_DestroyTexture(*(textures++));
	}
}

struct label_t* label_new() {
	struct label_t* self = (struct label_t*) calloc(1, sizeof(struct label_t));
	if (!self) {
		return 0;
	}
	self->classid = WIDGET_LABEL_CLASSID;
	self->needToLayout = 1;
	self->needToPaint = 1;
	self->font_size = 36;
	self->background_color = color_set(255, 255, 255, 255);
	self->fore_color = color_set(0, 0, 0, 255);
	return self;
}

void label_free(struct label_t* self) {
	if (self->textTexture) {
		so_ui_texture_free(self->textTexture);
	}
	if (self->text) {
		free(self->text);
	}
	free(self);
}

const char* label_get_text(struct label_t* self) {
	return self->text;
}

struct label_t* label_set_text(struct label_t* self, char* text) {
	if (self->text != text) {
		self->text = text;
		if (self->textTexture) {
			so_ui_texture_free(self->textTexture);
			self->textTexture = 0;
		}
		widget_get_do_paint((struct widget_t*) self);
		widget_get_do_layout((struct widget_t*) self);
	}
	return self;
}

struct font_t* label_get_font(struct label_t* self) {
	return self->font;
}

struct label_t* label_set_font(struct label_t* self, struct font_t* font) {
	if (self->font != font) {
		self->font = font;
		if (self->textTexture) {
			so_ui_texture_free(self->textTexture);
			self->textTexture = 0;
		}
		widget_get_do_paint((struct widget_t*) self);
		widget_get_do_layout((struct widget_t*) self);
	}
	return self;
}

struct label_t* label_set_forecolor(struct label_t* self, unsigned int color) {
	if (self->fore_color != color) {
		self->fore_color = color;
		if (self->textTexture) {
			so_ui_texture_free(self->textTexture);
			self->textTexture = 0;
		}
		widget_get_do_paint((struct widget_t*) self);
	}
	return self;
}

unsigned int label_get_forecolor(struct label_t* self) {
	return self->fore_color;
}

struct label_t* label_set_font_size(struct label_t* self, unsigned int color) {
	if (self->font_size != color) {
		self->font_size = color;
		if (self->textTexture) {
			so_ui_texture_free(self->textTexture);
			self->textTexture = 0;
		}
		widget_get_do_layout((struct widget_t*) self);
		widget_get_do_paint((struct widget_t*) self);
	}
	return self;
}

unsigned int label_get_font_size(struct label_t* self) {
	return self->font_size;
}

struct label_t* label_set_wrap(struct label_t* self, unsigned int color) {
	if (self->wrap != color) {
		self->wrap = color;
		if (self->textTexture) {
			so_ui_texture_free(self->textTexture);
			self->textTexture = 0;
		}
		widget_get_do_layout((struct widget_t*) self);
		widget_get_do_paint((struct widget_t*) self);
	}
	return self;
}

unsigned int label_get_wrap(struct label_t* self) {
	return self->wrap;
}

void label_layout(struct label_t* self, struct frame_t* frame) {
	int width = 0, height = 0;
	if (self->width == 0 || self->height == 0) {
		SDL_Texture* texture = label_get_texture(self, frame);
		so_ui_texture_query(texture, 0, 0, &width, &height);
		self->width = self->pref_width ? self->pref_width : width + self->padding_left + self->padding_right + self->margin_left + self->margin_right + self->border_size * 2;
		self->height = self->pref_height ? self->pref_height : height + self->padding_top + self->padding_bottom + self->margin_top + self->margin_bottom + self->border_size * 2;
	}
	self->needToLayout = 0;
}

void label_paint(struct label_t* self, struct frame_t* frame) {
	SDL_Rect destRect;
	if (self->text) {
		SDL_Renderer* ren = frame_get_renderer(frame);
		widget_get_rect_margin((struct widget_t*) self, &destRect);
		SDL_Texture* texture = label_get_texture(self, frame);
		so_ui_texture_query(texture, NULL, NULL, &destRect.w, &destRect.h);
		SDL_Rect srcRect = { 0, 0, destRect.w, destRect.h };
		if (self->padding_left + self->margin_left + destRect.w + self->margin_right + self->padding_right > self->width) {
			srcRect.w = destRect.w = self->width - self->padding_left - self->margin_left - self->margin_right - self->padding_right;
		}
		if (self->padding_top + self->margin_top + destRect.h + self->margin_bottom + self->padding_bottom > self->height) {
			srcRect.h = destRect.h = self->height - self->padding_top - self->margin_top - self->margin_bottom - self->padding_bottom;
		}
		so_ui_render_copy(ren, texture, &srcRect, &destRect);
	}
	self->needToPaint = 0;
}

struct text_t* text_new() {
	struct text_t* self = (struct text_t*) calloc(1, sizeof(struct text_t));
	if (!self) {
		return 0;
	}
	self->classid = WIDGET_TEXT_CLASSID;
	self->needToLayout = 1;
	self->needToPaint = 1;
	self->background_color = color_set(255, 255, 255, 0);
//	self->forecolor_color = color_set(0, 0, 0, 0);
	return self;
}

void text_free(struct text_t* self) {
	if (self->text) {
		free(self->text);
	}
	free(self);
}

const char* text_get_text(struct text_t* self) {
	return self->text;
}

void text_set_text(struct text_t* self, char* text) {
	self->text = text;
}

struct font_t* text_get_font(struct text_t* self) {
	return self->font;
}

void text_set_font(struct text_t* self, struct font_t* font) {
	self->font = font;
}

void text_layout(struct text_t* self, struct frame_t* frame) {
	self->needToLayout = 0;
}

void text_paint(struct text_t* self, struct frame_t* frame) {
	self->needToPaint = 0;
}

struct area_t* area_new() {
	struct area_t* self = (struct area_t*) calloc(1, sizeof(struct area_t));
	if (!self) {
		return 0;
	}
	self->classid = WIDGET_AREA_CLASSID;
	self->needToLayout = 1;
	self->needToPaint = 1;
	self->background_color = color_set(255, 255, 255, 0);
	return self;
}

const char* area_get_text(struct area_t* self) {
	return self->text;
}

void area_set_text(struct area_t* self, char* text) {
	self->text = text;
}

struct font_t* area_get_font(struct area_t* self) {
	return self->font;
}

void area_set_font(struct area_t* self, struct font_t* font) {
	self->font = font;
}

void area_layout(struct area_t* self, struct frame_t* frame) {
	self->needToLayout = 0;
}

void area_paint(struct area_t* self, struct frame_t* frame) {
	self->needToPaint = 0;
}

void area_free(struct area_t* self) {
	if (self->text) {
		free(self->text);
	}
	free(self);
}

struct button_t* button_new() {
	struct button_t* self = (struct button_t*) calloc(1, sizeof(struct button_t));
	if (!self) {
		return 0;
	}
	self->classid = WIDGET_BUTTON_CLASSID;
	self->needToLayout = 1;
	self->needToPaint = 1;
	self->background_color = color_set(255, 255, 255, 0);
	return self;
}

const char* button_get_text(struct button_t* self) {
	return self->text;
}

void button_set_text(struct button_t* self, char* text) {
	self->text = text;
}

struct font_t* button_get_font(struct button_t* self) {
	return self->font;
}

void button_set_font(struct button_t* self, struct font_t* font) {
	self->font = font;
}

void button_layout(struct button_t* self, struct frame_t* frame) {
	self->needToLayout = 0;
}

void button_paint(struct button_t* self, struct frame_t* frame) {
	self->needToPaint = 0;
}

void button_free(struct button_t* self) {
	if (self->text) {
		free(self->text);
	}
	free(self);
}

static SDL_Texture* image_get_texture(struct image_t* self, struct frame_t* frame) {
	if (self->path && !self->texture) {
		SDL_Surface* surface = IMG_Load(self->path);
		if (!surface) {
			return 0;
		}
		SDL_Renderer* ren = frame_get_renderer(frame);
		SDL_Texture* texture = so_ui_texture_new(ren, surface);
		if (!texture) {
			so_ui_surface_free(surface);
			return 0;
		}
		so_ui_surface_free(surface);
		self->texture = texture;
	}
	return self->texture;
}

struct image_t* image_new() {
	struct image_t* self = (struct image_t*) calloc(1, sizeof(struct image_t));
	if (!self) {
		return 0;
	}
	self->classid = WIDGET_IMAGE_CLASSID;
	self->needToLayout = 1;
	self->needToPaint = 1;
	self->background_color = color_set(255, 255, 255, 255);
	return self;
}

void image_free(struct image_t* self) {
	if (self->path) {
		free(self->path);
	}
	free(self);
}

const char* image_get_path(struct image_t* self) {
	return self->path;
}

void image_set_path(struct image_t* self, char* path) {
	if (self->path != path) {
		self->path = path;
		if (self->texture) {
			so_ui_texture_free(self->texture);
			self->texture = 0;
		}
		widget_get_do_paint((struct widget_t*) self);
		widget_get_do_layout((struct widget_t*) self);
	}
}

void image_set_clip(struct image_t* self, int x, int y, int w, int h) {
	self->clip_rect.x = x;
	self->clip_rect.y = y;
	self->clip_rect.w = w;
	self->clip_rect.h = h;
}

void image_layout(struct image_t* self, struct frame_t* frame) {
	int width = 0, height = 0;
	if (self->width == 0 || self->height == 0) {
		SDL_Texture* texture = image_get_texture(self, frame);
		if (self->clip_rect.w > 0 || self->clip_rect.h > 0) {
			width = self->clip_rect.w;
			height = self->clip_rect.h;
		} else {
			so_ui_texture_query(texture, 0, 0, &width, &height);
		}
		self->width = self->pref_width ? self->pref_width : width + self->padding_left + self->padding_right + self->margin_left + self->margin_right;
		self->height = self->pref_height ? self->pref_height : height + self->padding_top + self->padding_bottom + self->margin_top + self->margin_bottom;
	}
	self->needToLayout = 0;
}

void image_paint(struct image_t* self, struct frame_t* frame) {
	if (self->path) {
		SDL_Rect srcRect = self->clip_rect, destRect;
		SDL_Renderer* ren = frame_get_renderer(frame);
		SDL_Texture* texture = image_get_texture(self, frame);
		widget_get_rect_margin((struct widget_t*) self, &destRect);
		if (srcRect.w <= 0 || srcRect.h <= 0) {
			so_ui_texture_query(texture, NULL, NULL, &srcRect.w, &srcRect.h);
		}
		so_ui_render_copy(ren, texture, &srcRect, &destRect);
	}
	self->needToPaint = 0;
}

struct border_t* border_new() {
	struct border_t* self = (struct border_t*) calloc(1, sizeof(struct border_t));
	if (!self) {
		return 0;
	}
	self->children = list_new_full(5);
	if (!self->children) {
		free(self);
		return 0;
	}
	self->classid = WIDGET_BORDER_CLASSID;
	self->needToLayout = 1;
	self->needToPaint = 1;
	self->background_color = color_set(255, 255, 255, 255);
	return self;
}

void border_add_child(struct border_t* self, struct widget_t* widget, unsigned char position) {
	if (position > 4) {
		return;
	}
	list_set(self->children, position, widget);
	widget->parent = (struct panel_t*) self;
}

struct widget_t* border_get_child(struct border_t* self, unsigned int position) {
	if (position < 0 || position > 4) {
		return 0;
	}
	return (struct widget_t*) list_get(self->children, position);
}

unsigned int border_size_child(struct border_t* self) {
	return 5;
}

void border_remove_child(struct border_t* self, struct widget_t* widget) {
	const void** it = list_iterator(self->children);
	const void** ait = it;
	while (*ait) {
		if (*ait == widget) {
			list_set(self->children, ait - it, 0);
			break;
		}
		ait++;
	}
}

void border_layout(struct border_t* self, struct frame_t* frame) {
	struct widget_t* upWidget = list_get(self->children, BORDER_UP);
	struct widget_t* downWidget = list_get(self->children, BORDER_DOWN);
	struct widget_t* centerWidget = list_get(self->children, BORDER_CENTER);
	struct widget_t* leftWidget = list_get(self->children, BORDER_LEFT);
	struct widget_t* rightWidget = list_get(self->children, BORDER_RIGHT);
	unsigned width = self->width;
	unsigned height = self->height;
	if (upWidget) {
		widget_layout(upWidget, frame);
		upWidget->width = self->width;
		upWidget->height = upWidget->pref_height > 0 ? upWidget->pref_height : upWidget->height > 0 ? upWidget->height : 10;
		height -= upWidget->height;
		upWidget->x = 0;
		upWidget->y = 0;
	}
	if (downWidget) {
		widget_layout(downWidget, frame);
		downWidget->width = self->width;
		downWidget->height = downWidget->pref_height > 0 ? downWidget->pref_height : downWidget->height > 0 ? downWidget->height : 10;
		height -= downWidget->height;
		downWidget->x = 0;
		downWidget->y = upWidget ? upWidget->height + height : height;
	}
	if (leftWidget) {
		widget_layout(leftWidget, frame);
		leftWidget->width = leftWidget->pref_width > 0 ? leftWidget->pref_width : leftWidget->height > 0 ? leftWidget->height : 10;
		leftWidget->height = self->height;
		width -= leftWidget->width;
		if (upWidget) {
			leftWidget->height -= upWidget->height;
		}
		if (downWidget) {
			leftWidget->height -= downWidget->height;
		}
		leftWidget->x = 0;
		leftWidget->y = upWidget ? upWidget->height : 0;
	}
	if (rightWidget) {
		widget_layout(rightWidget, frame);
		rightWidget->width = rightWidget->pref_width > 0 ? rightWidget->pref_width : rightWidget->height > 0 ? rightWidget->height : 10;
		rightWidget->height = self->height;
		if (upWidget) {
			rightWidget->height -= upWidget->height;
		}
		if (downWidget) {
			rightWidget->height -= downWidget->height;
		}
		width -= rightWidget->width;
		rightWidget->x = leftWidget ? leftWidget->width + width : width;
		rightWidget->y = upWidget ? upWidget->height : 0;
	}
	if (centerWidget) {
		centerWidget->width = width;
		centerWidget->height = height;
		widget_layout(centerWidget, frame);
		centerWidget->width = width;
		centerWidget->height = height;
		centerWidget->x = leftWidget ? leftWidget->width : 0;
		centerWidget->y = upWidget ? upWidget->height : 0;
	}
	self->needToLayout = 0;
}

void border_paint(struct border_t* self, struct frame_t* frame) {
	SDL_Renderer* ren = frame_get_renderer(frame);
	SDL_Rect clip;
	so_ui_get_clip(ren, &clip);
	int x_abs = widget_get_x_abs((struct widget_t*) self);
	int y_abs = widget_get_y_abs((struct widget_t*) self);
	struct widget_t* upWidget = list_get(self->children, BORDER_UP);
	struct widget_t* downWidget = list_get(self->children, BORDER_DOWN);
	struct widget_t* centerWidget = list_get(self->children, BORDER_CENTER);
	struct widget_t* leftWidget = list_get(self->children, BORDER_LEFT);
	struct widget_t* rightWidget = list_get(self->children, BORDER_RIGHT);
	if (upWidget) {
		SDL_Rect clipRect = { x_abs + upWidget->x, y_abs + upWidget->y, upWidget->width, upWidget->height };
		so_ui_set_clip(ren, &clipRect);
		widget_paint(upWidget, frame);
	}
	if (downWidget) {
		SDL_Rect clipRect = { x_abs + downWidget->x, y_abs + downWidget->y, downWidget->width, downWidget->height };
		so_ui_set_clip(ren, &clipRect);
		widget_paint(downWidget, frame);
	}
	if (leftWidget) {
		SDL_Rect clipRect = { x_abs + leftWidget->x, y_abs + leftWidget->y, leftWidget->width, leftWidget->height };
		so_ui_set_clip(ren, &clipRect);
		widget_paint(leftWidget, frame);
	}
	if (rightWidget) {
		SDL_Rect clipRect = { x_abs + rightWidget->x, y_abs + rightWidget->y, rightWidget->width, rightWidget->height };
		so_ui_set_clip(ren, &clipRect);
		widget_paint(rightWidget, frame);
	}
	if (centerWidget) {
		SDL_Rect clipRect = { x_abs + centerWidget->x, y_abs + centerWidget->y, centerWidget->width, centerWidget->height };
		so_ui_set_clip(ren, &clipRect);
		widget_paint(centerWidget, frame);
	}
	so_ui_set_clip(ren, &clip);
	self->needToPaint = 0;
}

unsigned char border_native_mouse_wheel(struct border_t* self, int x, int y, SDL_Event* e) {
	int n;
	for (n = 0; n < 5; n++) {
		struct widget_t* widget = list_get(self->children, n);
		if (widget && x >= widget->x && x <= widget->x + widget->width && y >= widget->y && y <= widget->y + widget->height) {
			return widget_native_mouse_wheel(widget, x, y, e);
		}
	}
	return 0;
}

void border_free(struct border_t* self) {
	list_free(self->children, widget_free);
	free(self);
}

struct scroll_t* scroll_new(struct widget_t* widget) {
	struct scroll_t* self = (struct scroll_t*) calloc(1, sizeof(struct scroll_t));
	if (!self) {
		return 0;
	}
	self->classid = WIDGET_SCROLL_CLASSID;
	self->needToLayout = 1;
	self->needToPaint = 1;
	self->background_color = color_set(255, 255, 255, 255);
	scroll_set_widget(self, widget);
	return self;
}

void scroll_set_widget(struct scroll_t* self, struct widget_t* widget) {
	self->widget = widget;
	if (widget) {
		widget->parent = (struct panel_t*) self;
	}
}

struct widget_t* scroll_get_child(struct scroll_t* self) {
	return self->widget;
}

unsigned int scroll_size_child(struct scroll_t* self) {
	return 1;
}

void scroll_remove_child(struct scroll_t* self, struct widget_t* widget) {
	self->widget = 0;
}

void scroll_layout(struct scroll_t* self, struct frame_t* frame) {
	if (self->widget) {
		self->width = self->pref_width ? self->pref_width : self->width;
		self->height = self->pref_height ? self->pref_height : self->height;
		widget_layout(self->widget, frame);
	}
	self->needToLayout = 0;
}

void scroll_paint(struct scroll_t* self, struct frame_t* frame) {
	SDL_Rect self_margin_rect;
	if (self->widget) {
		struct widget_t* widget = self->widget;
		SDL_Renderer* ren = frame_get_renderer(frame);
		widget_get_rect_margin((struct widget_t*) self, &self_margin_rect);
		so_ui_set_clip(ren, &self_margin_rect);
		{
			int x = widget->x;
			int y = widget->y;
			widget->x -= self->view_x;
			widget->y -= self->view_y;
			widget_paint(widget, frame);
			widget->x = x;
			widget->y = y;
		}
		int innerSpaceBar = 1;
		int barWidth = 5;
		int barHeight = 5;
		SDL_Rect cornerRect = { 0, 0, 0, 0 };
		if (widget->width > self->width && widget->height > self->height) {
			widget_get_rect_padding((struct widget_t*) self, &cornerRect);
			cornerRect.x = cornerRect.w - innerSpaceBar - barWidth;
			cornerRect.y = cornerRect.h - innerSpaceBar - barHeight;
			cornerRect.h = barHeight;
			cornerRect.w = barWidth;
		}
		SDL_Rect verticalRect = { 0, 0, 0, 0 };
		if (widget->height > self->height) {
			widget_get_rect_padding((struct widget_t*) self, &verticalRect);
			double div = (double) widget->height / verticalRect.h;
			verticalRect.x = verticalRect.w - innerSpaceBar - barWidth;
			verticalRect.y = verticalRect.y + innerSpaceBar + ceil((double) self->view_y / div);
			verticalRect.h = self->height / div - 2 * innerSpaceBar;
			verticalRect.w = barWidth;
		}
		SDL_Rect horizontalRect = { 0, 0, 0, 0 };
		if (widget->width > self->width) {
			widget_get_rect_padding((struct widget_t*) self, &horizontalRect);
			double div = (double) widget->width / horizontalRect.w;
			horizontalRect.x = horizontalRect.x + innerSpaceBar + ceil((double) self->view_x / div);
			horizontalRect.y = horizontalRect.h - innerSpaceBar - barWidth;
			horizontalRect.w = self->width / div - 2 * innerSpaceBar;
			horizontalRect.h = barHeight;
		}
		if (horizontalRect.w > 0) {
			if (verticalRect.w > 0) {
				horizontalRect.w -= barWidth;
			}
		}
		if (verticalRect.w > 0) {
			if (horizontalRect.w > 0) {
				verticalRect.h -= barHeight;
			}
		}
		if (cornerRect.w > 0) {
			if (horizontalRect.x + horizontalRect.w == cornerRect.x) {
				horizontalRect.w--;
			}
			if (verticalRect.y + verticalRect.h == cornerRect.y) {
				verticalRect.h--;
			}
		}
		if (horizontalRect.w > 0 && verticalRect.w > 0) {
			so_ui_render_blend(ren, SDL_BLENDMODE_BLEND);
			so_ui_set_color_uint(ren, 0x3200000);
			so_ui_render_fill_rect(ren, &cornerRect);
			so_ui_render_blend(ren, SDL_BLENDMODE_NONE);
		}
		if (verticalRect.w > 0) {
			so_ui_render_blend(ren, SDL_BLENDMODE_BLEND);
			so_ui_set_color_uint(ren, 0x64000000);
			so_ui_render_fill_rect(ren, &verticalRect);
			so_ui_render_blend(ren, SDL_BLENDMODE_NONE);
		}
		if (horizontalRect.w > 0) {
			so_ui_render_blend(ren, SDL_BLENDMODE_BLEND);
			so_ui_set_color_uint(ren, 0x64000000);
			so_ui_render_fill_rect(ren, &horizontalRect);
			so_ui_render_blend(ren, SDL_BLENDMODE_NONE);
		}
	}
	self->needToPaint = 0;
}

unsigned char scroll_native_mouse_wheel(struct scroll_t* self, int x, int y, SDL_Event* e) {
	if (self->widget) {
		int wheel_x = e ? e->wheel.x : 0;
		int wheel_y = e ? e->wheel.y : 0;
		unsigned char xflag = 0, yflag = 0;
		struct widget_t* widget = self->widget;
		if (wheel_x != 0) {
			if (wheel_x > 0) {
				if (self->view_x <= 0) {
					xflag = 1;
				}
			} else {
				if (widget->width <= self->width || self->view_x >= self->width - widget->width) {
					xflag = 1;
				}
			}
		}
		if (wheel_y != 0) {
			if (wheel_y > 0) {
				if (self->view_y <= 0) {
					yflag = 1;
				}
			} else {
				if (widget->height <= self->height || self->view_y >= self->height - widget->height) {
					yflag = 1;
				}
			}
		}
		if (xflag) {
			widget_native_mouse_wheel(widget, x, y, e);
		} else {
			self->view_x = (self->view_x - wheel_x);
			if (self->view_x < 0) {
				self->view_x = 0;
			}
			if (self->view_x > widget->width - self->width) {
				self->view_x = widget->width - self->width;
			}
		}
		if (yflag) {
			widget_native_mouse_wheel(widget, x, y, e);
		} else {
			self->view_y = (self->view_y - wheel_y);
			if (self->view_y < 0) {
				self->view_y = 0;
			}
			if (self->view_y > widget->height - self->height) {
				self->view_y = widget->height - self->height;
			}
		}
		widget_get_do_paint((struct widget_t*) self);
	}
	return 0;
}

void scroll_free(struct scroll_t* self) {
	if (self->widget) {
		widget_free(self->widget);
	}
	free(self);
}

struct vertical_t* vertical_new(unsigned int childrenSize) {
	struct vertical_t* self = (struct vertical_t*) calloc(1, sizeof(struct vertical_t));
	if (!self) {
		return 0;
	}
	self->children = list_new(childrenSize);
	if (!self->children) {
		free(self);
		return 0;
	}
	self->classid = WIDGET_VERTICAL_CLASSID;
	self->needToLayout = 1;
	self->needToPaint = 1;
	self->background_color = color_set(255, 255, 255, 255);
	return self;
}

unsigned char vertical_add_child(struct vertical_t* self, struct widget_t* widget) {
	if (list_add(&self->children, widget)) {
		return 1;
	}
	widget->parent = (struct panel_t*) self;
	return 0;
}

struct widget_t* vertical_get_child(struct vertical_t* self, unsigned int index) {
	return list_get(self->children, index);
}

unsigned int vertical_size_child(struct vertical_t* self) {
	return list_size(self->children);
}

void vertical_remove_child(struct vertical_t* self, struct widget_t* widget) {
	const void** it = list_iterator(self->children);
	const void** ait = it;
	while (*ait) {
		if (*ait == widget) {
			list_remove(self->children, ait - it);
			break;
		}
		ait++;
	}
}

void vertical_layout(struct vertical_t* self, struct frame_t* frame) {
	struct widget_t** it = (struct widget_t**) list_iterator(self->children);
	unsigned int x = self->padding_left + self->margin_left;
	unsigned int y = self->padding_top + self->margin_top;
	unsigned int width = self->pref_width ? self->pref_width : self->width;
	unsigned int height = self->pref_height ? self->pref_height : self->height;
	while (*it) {
		struct widget_t* widget = *it++;
		widget_layout(widget, frame);
		widget->x = x;
		widget->y = y;
		if (widget->pref_width) {
			widget->width = widget->pref_width;
		}
		if (widget->pref_height) {
			widget->height = widget->pref_height;
		}
		int widgetWidth = self->padding_left + self->margin_left + widget->width + self->margin_right + self->padding_right;
		if (widgetWidth > width) {
			width = widgetWidth;
		}
		y += widget->height;
		if (*it) {
			y += self->margin_middle;
		}
	}
	self->width = width;
	it = (struct widget_t**) list_iterator(self->children);
	while (*it) {
		struct widget_t* widget = *it++;
		if (!widget->pref_width) {
			widget->width = width - self->padding_left - self->margin_left - self->margin_right - self->padding_right;
		}
	}
	self->height = y + self->margin_bottom + self->padding_bottom;
	if (height > self->height) {
		self->height = height;
	}
	self->needToLayout = 0;
}

void vertical_paint(struct vertical_t* self, struct frame_t* frame) {
	SDL_Rect self_rect, self_margin_rect, child_padding_rect;
	SDL_Renderer* ren = frame_get_renderer(frame);
	so_ui_get_clip(ren, &self_rect);
	widget_get_rect_margin((struct widget_t*) self, &self_margin_rect);
	SDL_IntersectRect(&self_rect, &self_margin_rect, &self_margin_rect);
	struct widget_t** it = (struct widget_t**) list_iterator(self->children);
	while (*it) {
		struct widget_t* widget = *it++;
		widget_get_rect_padding((struct widget_t*) widget, &child_padding_rect);
		if (SDL_HasIntersection(&child_padding_rect, &self_margin_rect)) {
			SDL_IntersectRect(&self_margin_rect, &child_padding_rect, &child_padding_rect);
			so_ui_set_clip(ren, &child_padding_rect);
			widget_paint(widget, frame);
		}
	}
	so_ui_set_clip(ren, &self_rect);
	self->needToPaint = 0;
}

unsigned char vertical_native_mouse_wheel(struct vertical_t* self, int x, int y, SDL_Event* e) {
	struct widget_t** it = (struct widget_t**) list_iterator(self->children);
	while (*it) {
		struct widget_t* widget = *it++;
		unsigned char consumed = widget_native_mouse_wheel(widget, x, y, e);
		if (consumed) {
			return consumed;
		}
	}
	return 0;
}

void vertical_free(struct vertical_t* self) {
	list_free(self->children, widget_free);
	free(self);
}

struct horizontal_t* horizontal_new(unsigned int childrenSize) {
	struct horizontal_t* self = (struct horizontal_t*) calloc(1, sizeof(struct horizontal_t));
	if (!self) {
		return 0;
	}
	self->children = list_new(childrenSize);
	if (!self->children) {
		free(self);
		return 0;
	}
	self->classid = WIDGET_HORIZONTAL_CLASSID;
	self->needToLayout = 1;
	self->needToPaint = 1;
	self->background_color = color_set(255, 255, 255, 255);
	return self;
}

unsigned char horizontal_add_child(struct horizontal_t* self, struct widget_t* widget) {
	if (list_add(&self->children, widget)) {
		return 1;
	}
	widget->parent = (struct panel_t*) self;
	return 0;
}

struct widget_t* horizontal_get_child(struct horizontal_t* self, unsigned int index) {
	return list_get(self->children, index);
}

unsigned int horizontal_size_child(struct horizontal_t* self) {
	return list_size(self->children);
}

void horizontal_remove_child(struct horizontal_t* self, struct widget_t* widget) {
	const void** it = list_iterator(self->children);
	const void** ait = it;
	while (*ait) {
		if (*ait == widget) {
			list_remove(self->children, ait - it);
			break;
		}
		ait++;
	}
}

void horizontal_layout(struct horizontal_t* self, struct frame_t* frame) {
	struct widget_t** it = (struct widget_t**) list_iterator(self->children);
	unsigned int x = self->padding_left + self->margin_left;
	unsigned int y = self->padding_top + self->margin_top;
	unsigned int width = self->pref_width ? self->pref_width : self->width;
	unsigned int height = self->pref_height ? self->pref_height : self->height;
	while (*it) {
		struct widget_t* widget = *it++;
		widget_layout(widget, frame);
		widget->x = x;
		widget->y = y;
		if (widget->pref_width) {
			widget->width = widget->pref_width;
		}
		if (widget->pref_height) {
			widget->height = widget->pref_height;
		}
		int widget_height = self->padding_top + self->margin_top + widget->height + self->margin_bottom + self->padding_bottom;
		if (widget_height > height) {
			height = widget_height;
		}
		x += widget->width;
		if (*it) {
			x += self->margin_middle;
		}
	}
	self->height = height;
	it = (struct widget_t**) list_iterator(self->children);
	while (*it) {
		struct widget_t* widget = *it++;
		if (!widget->pref_height) {
			widget->height = height - self->padding_top - self->margin_top - self->margin_bottom - self->padding_bottom;
		}
	}
	self->width = x + self->margin_right + self->padding_right;
	if (width > self->width) {
		self->width = width;
	}
	self->needToLayout = 0;
//	struct widget_t** it = (struct widget_t**) list_iterator(self->children);
//	unsigned int x = 0;
//	unsigned int height = 0;
//	while (*it) {
//		struct widget_t* widget = *it++;
//		widget_layout(widget, frame);
//		widget->x = x;
//		widget->y = 0;
//		if (widget->pref_width) {
//			widget->width = widget->pref_width;
//		}
//		if (widget->pref_height) {
//			widget->height = widget->pref_height;
//		}
//		if (widget->height > height) {
//			height = widget->height;
//		}
//		x += widget->width;
//	}
//	self->height = height;
//	it = (struct widget_t**) list_iterator(self->children);
//	while (*it) {
//		struct widget_t* widget = *it++;
//		if (!widget->pref_height) {
//			widget->height = height;
//		}
//	}
//	self->width = x;
//	self->needToLayout = 0;
}

void horizontal_paint(struct horizontal_t* self, struct frame_t* frame) {
	SDL_Rect self_rect, self_margin_rect, child_padding_rect;
	SDL_Renderer* ren = frame_get_renderer(frame);
	so_ui_get_clip(ren, &self_rect);
	widget_get_rect_margin((struct widget_t*) self, &self_margin_rect);
	SDL_IntersectRect(&self_rect, &self_margin_rect, &self_margin_rect);
	struct widget_t** it = (struct widget_t**) list_iterator(self->children);
	while (*it) {
		struct widget_t* widget = *it++;
		widget_get_rect_padding((struct widget_t*) widget, &child_padding_rect);
		if (SDL_HasIntersection(&child_padding_rect, &self_margin_rect)) {
			SDL_IntersectRect(&self_margin_rect, &child_padding_rect, &child_padding_rect);
			so_ui_set_clip(ren, &child_padding_rect);
			widget_paint(widget, frame);
		}
	}
	so_ui_set_clip(ren, &self_rect);
	self->needToPaint = 0;
}

unsigned char horizontal_native_mouse_wheel(struct horizontal_t* self, int x, int y, SDL_Event* e) {
	// TODO
	return 0;
}

void horizontal_free(struct horizontal_t* self) {
	list_free(self->children, widget_free);
	free(self);
}

struct gridbag_t* gridbag_new(unsigned int childrenSize) {
	struct gridbag_t* self = (struct gridbag_t*) calloc(1, sizeof(struct gridbag_t));
	if (!self) {
		return 0;
	}
	self->children = list_new(childrenSize);
	if (!self->children) {
		free(self);
		return 0;
	}
	self->classid = WIDGET_GRIDBAG_CLASSID;
	self->needToLayout = 1;
	self->needToPaint = 1;
	self->background_color = color_set(255, 255, 255, 255);
	return self;
}

unsigned char gridbag_add_child(struct gridbag_t* self, struct widget_t* widget) {
	if (list_add(&self->children, widget)) {
		return 1;
	}
	widget->parent = (struct panel_t*) self;
	return 0;
}

struct widget_t* gridbag_get_child(struct gridbag_t* self, unsigned int index) {
	return list_get(self->children, index);
}

unsigned int gridbag_size_child(struct gridbag_t* self) {
	return list_size(self->children);
}

void gridbag_remove_child(struct gridbag_t* self, struct widget_t* widget) {
	const void** it = list_iterator(self->children);
	const void** ait = it;
	while (*ait) {
		if (*ait == widget) {
			list_remove(self->children, ait - it);
			break;
		}
		ait++;
	}
}

void gridbag_layout(struct gridbag_t* self, struct frame_t* frame) {
	self->needToLayout = 0;
}

void gridbag_paint(struct gridbag_t* self, struct frame_t* frame) {
	SDL_Renderer* ren = frame_get_renderer(frame);
	int x_abs = widget_get_x_abs((struct widget_t*) self);
	int y_abs = widget_get_y_abs((struct widget_t*) self);
	struct widget_t** it = (struct widget_t**) list_iterator(self->children);
	while (*it) {
		struct widget_t* widget = *it++;
		SDL_Rect clipRect = { x_abs + widget->x, y_abs + widget->y, widget->width, widget->height };
		so_ui_set_clip(ren, &clipRect);
		widget_paint(widget, frame);
	}
	self->needToPaint = 0;
}

unsigned char gridbag_native_mouse_wheel(struct gridbag_t* self, int x, int y, SDL_Event* e) {
	// TODO
	return 0;
}

void gridbag_free(struct gridbag_t* self) {
	list_free(self->children, widget_free);
	free(self);
}

struct grid_t* grid_new(unsigned int gridxs, unsigned int gridys) {
	struct grid_t* self = (struct grid_t*) calloc(1, sizeof(struct grid_t));
	if (!self) {
		return 0;
	}
	self->children = list_new_full(gridxs * gridys);
	if (!self->children) {
		free(self);
		return 0;
	}
	self->classid = WIDGET_GRID_CLASSID;
	self->needToLayout = 1;
	self->needToPaint = 1;
	self->gridxs = gridxs;
	self->gridys = gridys;
	self->background_color = color_set(255, 255, 255, 255);
	return self;
}

void grid_add_child(struct grid_t* self, struct widget_t* widget) {
	unsigned int size = list_size(self->children);
	if (size >= self->gridxs * self->gridys) {
		return;
	}
	list_set(self->children, size, widget);
	widget->parent = (struct panel_t*) self;
}

struct widget_t* grid_get_child(struct grid_t* self, unsigned int index) {
	return list_get(self->children, index);
}

unsigned int grid_size_child(struct grid_t* self) {
	return self->gridxs * self->gridys;
}

void grid_remove_child(struct grid_t* self, struct widget_t* widget) {
	const void** it = list_iterator(self->children);
	const void** ait = it;
	while (*ait) {
		if (*ait == widget) {
			list_set(self->children, ait - it, 0);
			break;
		}
		ait++;
	}
}

void grid_layout(struct grid_t* self, struct frame_t* frame) {
	self->needToLayout = 0;
}

void grid_paint(struct grid_t* self, struct frame_t* frame) {
	SDL_Renderer* ren = frame_get_renderer(frame);
	int x_abs = widget_get_x_abs((struct widget_t*) self);
	int y_abs = widget_get_y_abs((struct widget_t*) self);
	struct widget_t** it = (struct widget_t**) list_iterator(self->children);
	while (*it) {
		struct widget_t* widget = *it++;
		SDL_Rect clipRect = { x_abs + widget->x, y_abs + widget->y, widget->width, widget->height };
		so_ui_set_clip(ren, &clipRect);
		widget_paint(widget, frame);
	}
	self->needToPaint = 0;
}

unsigned char grid_native_mouse_wheel(struct grid_t* self, int x, int y, SDL_Event* e) {
	// TODO
	return 0;
}

void grid_free(struct grid_t* self) {
	list_free(self->children, widget_free);
	free(self);
}
