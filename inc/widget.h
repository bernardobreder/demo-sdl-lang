/*
 * widget.h
 *
 *  Created on: 12/07/2013
 *      Author: bernardobreder
 */

#ifndef WIDGET_H_
#define WIDGET_H_

#include <SDL2/SDL.h>
#include "struct.h"
#include "list.h"
#include "gui.h"

#define BORDER_CENTER 0

#define BORDER_UP 1

#define BORDER_DOWN 2

#define BORDER_LEFT 3

#define BORDER_RIGHT 4

#define WIDGET_LABEL_CLASSID 1
#define WIDGET_TEXT_CLASSID 2
#define WIDGET_AREA_CLASSID 3
#define WIDGET_BUTTON_CLASSID 4
#define WIDGET_IMAGE_CLASSID 5
#define WIDGET_SCROLL_CLASSID 6
#define WIDGET_BORDER_CLASSID 7
#define WIDGET_VERTICAL_CLASSID 8
#define WIDGET_HORIZONTAL_CLASSID 9
#define WIDGET_GRIDBAG_CLASSID 10
#define WIDGET_GRID_CLASSID 11

unsigned char widget_get_need_to_layout(struct widget_t* self);

unsigned char widget_get_need_to_paint(struct widget_t* self);

void widget_set_need_to_paint(struct widget_t* self, unsigned char flag);

void widget_set_need_to_layout(struct widget_t* self, unsigned char flag);

void widget_get_do_layout(struct widget_t* self);

void widget_get_do_paint(struct widget_t* self);

const char* widget_get_id(struct widget_t* self);

struct widget_t* widget_set_id(struct widget_t* self, char* id);

int widget_get_x(struct widget_t* self);

int widget_get_x_abs(struct widget_t* self);

void widget_set_x(struct widget_t* self, int value);

int widget_get_y(struct widget_t* self);

int widget_get_y_abs(struct widget_t* self);

void widget_set_y(struct widget_t* self, int value);

unsigned int widget_get_width(struct widget_t* self);

void widget_set_width(struct widget_t* self, int value);

unsigned int widget_get_height(struct widget_t* self);

void widget_set_height(struct widget_t* self, int value);

unsigned int widget_get_pref_width(struct widget_t* self);

void widget_set_pref_width(struct widget_t* self, int value);

unsigned int widget_get_pref_height(struct widget_t* self);

void widget_set_pref_height(struct widget_t* self, int value);

struct widget_t* widget_set_margin(struct widget_t* self, int* left, int* top, int* right, int* bottom, int* middle);

void widget_get_margin(struct widget_t* self, int* left, int* top, int* right, int* bottom, int* middle);

struct widget_t* widget_set_padding(struct widget_t* self, int* left, int* top, int* right, int* bottom);

void widget_get_padding(struct widget_t* self, int* left, int* top, int* right, int* bottom);

struct widget_t* widget_set_bgcolor(struct widget_t* self, unsigned int color);

unsigned int widget_get_bgcolor(struct widget_t* self);

struct widget_t* widget_set_border_color(struct widget_t* self, unsigned int color) ;

unsigned int widget_get_border_color(struct widget_t* self) ;

struct widget_t* widget_set_border_size(struct widget_t* self, unsigned int value) ;

unsigned int widget_get_border_size(struct widget_t* self);

void widget_layout(struct widget_t* self, struct frame_t* frame);

void widget_paint(struct widget_t* self, struct frame_t* frame);

struct panel_t* widget_get_parent(struct widget_t* self);

void* widget_get_key_press(struct widget_t* self);

void* widget_get_key_release(struct widget_t* self);

void* widget_get_mouse_press(struct widget_t* self);

void* widget_get_mouse_release(struct widget_t* self);

void* widget_get_mouse_dragin(struct widget_t* self);

void* widget_get_mouse_dragout(struct widget_t* self);

void* widget_get_mouse_motion(struct widget_t* self);

void* widget_get_mouse_wheel(struct widget_t* self);

void* widget_get_focus_got(struct widget_t* self);

void* widget_get_focus_lost(struct widget_t* self);

void widget_set_key_press(struct widget_t* self, void (*)(struct widget_t* self, SDL_Event* event));

void widget_set_key_release(struct widget_t* self, void (*)(struct widget_t* self, SDL_Event* event));

void widget_set_mouse_press(struct widget_t* self, void (*)(struct widget_t* self, SDL_Event* event));

void widget_set_mouse_release(struct widget_t* self, void (*)(struct widget_t* self, SDL_Event* event));

void widget_set_mouse_dragin(struct widget_t* self, void (*)(struct widget_t* self, SDL_Event* event));

void widget_set_mouse_dragout(struct widget_t* self, void (*)(struct widget_t* self, SDL_Event* event));

void widget_set_mouse_motion(struct widget_t* self, void (*)(struct widget_t* self, SDL_Event* event));

void widget_set_mouse_focus_got(struct widget_t* self, void (*)(struct widget_t* self, SDL_Event* event));

void widget_set_mouse_focus_lost(struct widget_t* self, void (*)(struct widget_t* self, SDL_Event* event));

unsigned char widget_native_mouse_wheel(struct widget_t* self, int x, int y, SDL_Event* e);

void widget_free(struct widget_t* self);

struct label_t* label_new();

void label_free(struct label_t* self);

const char* label_get_text(struct label_t* self);

struct label_t* label_set_text(struct label_t* self, char* text);

struct font_t* label_get_font(struct label_t* self);

struct label_t* label_set_font(struct label_t* self, struct font_t* font);

struct label_t* label_set_forecolor(struct label_t* self, unsigned int color);

unsigned int label_get_forecolor(struct label_t* self);

struct label_t* label_set_font_size(struct label_t* self, unsigned int color);

unsigned int label_get_font_size(struct label_t* self);

void label_layout(struct label_t* self, struct frame_t* frame);

void label_paint(struct label_t* self, struct frame_t* frame);

struct text_t* text_new();

void text_free(struct text_t* self);

const char* text_get_text(struct text_t* self);

void text_set_text(struct text_t* self, char* text);

struct font_t* text_get_font(struct text_t* self);

void text_set_font(struct text_t* self, struct font_t* font);

void text_layout(struct text_t* self, struct frame_t* frame);

void text_paint(struct text_t* self, struct frame_t* frame);

struct area_t* area_new();

const char* area_get_text(struct area_t* self);

void area_set_text(struct area_t* self, char* text);

struct font_t* area_get_font(struct area_t* self);

void area_set_font(struct area_t* self, struct font_t* font);

void area_layout(struct area_t* self, struct frame_t* frame);

void area_paint(struct area_t* self, struct frame_t* frame);

void area_free(struct area_t* self);

struct button_t* button_new();

const char* button_get_text(struct button_t* self);

void button_set_text(struct button_t* self, char* text);

struct font_t* button_get_font(struct button_t* self);

void button_set_font(struct button_t* self, struct font_t* font);

void button_layout(struct button_t* self, struct frame_t* frame);

void button_paint(struct button_t* self, struct frame_t* frame);

void button_free(struct button_t* self);

struct image_t* image_new();

const char* image_get_path(struct image_t* self);

void image_set_path(struct image_t* self, char* path);

void image_set_clip(struct image_t* self, int x, int y, int w, int h);

void image_layout(struct image_t* self, struct frame_t* frame);

void image_paint(struct image_t* self, struct frame_t* frame);

void image_free(struct image_t* self);

//
// Scroll
//

struct scroll_t* scroll_new();

void scroll_add_child(struct scroll_t* self, struct widget_t* widget);

void scroll_set_widget(struct scroll_t* self, struct widget_t* widget);

struct widget_t* scroll_get_child(struct scroll_t* self);

unsigned int scroll_size_child(struct scroll_t* self);

void scroll_remove_child(struct scroll_t* self, struct widget_t*);

void scroll_layout(struct scroll_t* self, struct frame_t* frame);

void scroll_paint(struct scroll_t* self, struct frame_t* frame);

unsigned char scroll_native_mouse_wheel(struct scroll_t* self, int x, int y, SDL_Event* e);

void scroll_free(struct scroll_t* self);

//
// Border
//

struct border_t* border_new();

void border_add_child(struct border_t* self, struct widget_t* widget, unsigned char position);

struct widget_t* border_get_child(struct border_t* self, unsigned int position);

unsigned int border_size_child(struct border_t* self);

void border_remove_child(struct border_t* self, struct widget_t*);

void border_layout(struct border_t* self, struct frame_t* frame);

void border_paint(struct border_t* self, struct frame_t* frame);

unsigned char border_native_mouse_wheel(struct border_t* self, int x, int y, SDL_Event* e);

void border_free(struct border_t* self);

struct vertical_t* vertical_new(unsigned int childrenSize);

unsigned char vertical_add_child(struct vertical_t* self, struct widget_t* widget);

struct widget_t* vertical_get_child(struct vertical_t* self, unsigned int index);

unsigned int vertical_size_child(struct vertical_t* self);

void vertical_remove_child(struct vertical_t* self, struct widget_t*);

void vertical_layout(struct vertical_t* self, struct frame_t* frame);

void vertical_paint(struct vertical_t* self, struct frame_t* frame);

unsigned char vertical_native_mouse_wheel(struct vertical_t* self, int x, int y, SDL_Event* e);

void vertical_free(struct vertical_t* self);

struct horizontal_t* horizontal_new(unsigned int childrenSize);

unsigned char horizontal_add_child(struct horizontal_t* self, struct widget_t* widget);

struct widget_t* horizontal_get_child(struct horizontal_t* self, unsigned int index);

unsigned int horizontal_size_child(struct horizontal_t* self);

void horizontal_remove_child(struct horizontal_t* self, struct widget_t* widget);

void horizontal_layout(struct horizontal_t* self, struct frame_t* frame);

void horizontal_paint(struct horizontal_t* self, struct frame_t* frame);

unsigned char horizontal_native_mouse_wheel(struct horizontal_t* self, int x, int y, SDL_Event* e);

void horizontal_free(struct horizontal_t* self);

struct gridbag_t* gridbag_new(unsigned int childrenSize);

unsigned char gridbag_add_child(struct gridbag_t* self, struct widget_t* widget);

struct widget_t* gridbag_get_child(struct gridbag_t* self, unsigned int index);

unsigned int gridbag_size_child(struct gridbag_t* self);

void gridbag_remove_child(struct gridbag_t* self, struct widget_t* widget);

void gridbag_layout(struct gridbag_t* self, struct frame_t* frame);

void gridbag_paint(struct gridbag_t* self, struct frame_t* frame);

unsigned char gridbag_native_mouse_wheel(struct gridbag_t* self, int x, int y, SDL_Event* e);

void gridbag_free(struct gridbag_t* self);

struct grid_t* grid_new(unsigned int gridxs, unsigned int gridys);

void grid_add_child(struct grid_t* self, struct widget_t* widget);

struct widget_t* grid_get_child(struct grid_t* self, unsigned int index);

unsigned int grid_size_child(struct grid_t* self);

void grid_remove_child(struct grid_t* self, struct widget_t* widget);

void grid_layout(struct grid_t* self, struct frame_t* frame);

void grid_paint(struct grid_t* self, struct frame_t* frame);

unsigned char grid_native_mouse_wheel(struct grid_t* self, int x, int y, SDL_Event* e);

void grid_free(struct grid_t* self);

#endif /* WIDGET_H_ */
