/*
 * frame.h
 *
 *  Created on: 12/07/2013
 *      Author: bernardobreder
 */

#ifndef GUI_H_
#define GUI_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"
#include "widget.h"
#include "list.h"

error_t gui_init();

error_t gui_add_frame(struct frame_t* frame);

error_t gui_event_loop_wait();

error_t gui_event_loop();

void gui_free();

struct frame_t* frame_new();

void frame_set_widget(struct frame_t* self, struct widget_t* widget);

SDL_Renderer* frame_get_renderer(struct frame_t* self);

void frame_free(struct frame_t* self);

struct font_t* font_new();

struct font_t* font_get_default(unsigned int size);

TTF_Font* font_get_data(struct font_t* self);

void font_free(struct font_t* self);

#endif /* GUI_H_ */
