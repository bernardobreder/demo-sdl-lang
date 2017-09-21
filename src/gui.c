/*
 * gui.c
 *
 *  Created on: 13/07/2013
 *      Author: bernardobreder
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mixer.h>
#include "so.h"
#include "gui.h"
#include "widget.h"
#include "list.h"

static struct gui_t* env = 0;

error_t gui_init() {
	struct gui_t* self = (struct gui_t*) calloc(1, sizeof(struct gui_t));
	if (!self) {
		return 1;
	}
	self->frames = list_new(10);
	self->fonts = list_new(10);
	env = self;
	return 0;
}

void gui_free() {
	if (env) {
		list_free(env->frames, frame_free);
		list_free(env->fonts, font_free);
		free(env);
	}
}

error_t gui_add_frame(struct frame_t* frame) {
	if (list_add(&env->frames, frame)) {
		return 1;
	}
	return 0;
}

error_t gui_event_loop_wait() {
	for (;;) {
		unsigned char quit = gui_event_loop();
		if (quit) {
			break;
		}
		SDL_Delay(1000 / 60);
	}
	return 0;
}
error_t gui_event_loop() {
	unsigned char draging = 0;
	SDL_Event e;
	while (so_ui_poll_event(&e)) {
		switch (e.type) {
		case SDL_WINDOWEVENT : {
//			switch (e.window.event) {
//			case SDL_WINDOWEVENT_RESIZED : {
			struct frame_t** it = (struct frame_t**) list_iterator(env->frames);
			while (*it) {
				struct frame_t* frame = *it;
				if (frame->winId == e.window.windowID) {
					widget_set_need_to_paint(frame->widget, 1);
					widget_set_need_to_layout(frame->widget, 1);
					break;
				}
				it++;
			}
			break;
//			}
//			}
			break;
		}
		case SDL_KEYDOWN : {
			break;
		}
		case SDL_MOUSEWHEEL : {
			struct frame_t** it = (struct frame_t**) list_iterator(env->frames);
			while (*it) {
				struct frame_t* frame = *it;
				if (frame->winId == e.wheel.windowID) {
					if (frame->widget) {
						int x, y;
						so_ui_mouse_get_xy(&x, &y);
						widget_native_mouse_wheel(frame->widget, x, y, &e);
					}
					break;
				}
				it++;
			}
			break;
		}
		case SDL_MOUSEMOTION : {
			if (draging) {
			}
			break;
		}
		case SDL_MOUSEBUTTONDOWN : {
			switch (e.button.button) {
			case 1 : {
				draging = 1;
				break;
			}
			}
			break;
		}
		case SDL_MOUSEBUTTONUP : {
			switch (e.button.button) {
			case 1 : {
				case SDL_RELEASED :
				{
					draging = 0;
					break;
				}
			}
			}
			break;
		}
		case SDL_QUIT : {
			return 1;
		}
		}
	}
	{
		struct frame_t** it = (struct frame_t**) list_iterator(env->frames);
		while (*it) {
			struct frame_t* frame = *it;
			if (frame->widget) {
				struct widget_t* widget = frame->widget;
				unsigned char need_t_layout = widget_get_need_to_layout(widget);
				unsigned char need_to_paint = widget_get_need_to_paint(widget);
				if (need_t_layout || need_to_paint) {
					SDL_Renderer* ren = frame->ren;
					int width = 0, height = 0;
					so_ui_renderer_get_size(ren, &width, &height);
					if (need_t_layout) {
						widget_set_width(widget, width);
						widget_set_height(widget, height);
						widget_layout(widget, frame);
					}
					if (need_to_paint) {
						so_ui_set_clip(frame->ren, 0);
						so_ui_set_color_uint(ren, 0x00ffffff);
						so_ui_clear(ren);
						SDL_Rect clipRect = { 0, 0, width, height };
						so_ui_set_clip(frame->ren, &clipRect);
						widget_paint(widget, frame);
						so_ui_present(ren);
					}
				}
			}
			it++;
		}
	}
	return 0;
}

struct frame_t* frame_new(unsigned int width, unsigned int height) {
	struct frame_t* self = (struct frame_t*) calloc(1, sizeof(struct frame_t));
	if (!self) {
		return 0;
	}
	SDL_Window* win = so_ui_window_new("Hello World!", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!win) {
		printf("Unable to set up video: %s\n", SDL_GetError());
		free(self);
		return 0;
	}
	SDL_Renderer* ren = so_ui_renderer_new(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!ren) {
		printf("Unable to create renderer: %s\n", SDL_GetError());
		free(self);
		so_ui_window_free(win);
		return 0;
	}
	so_ui_set_color_uint(ren, 0x00ffffff);
	so_ui_clear(ren);
	so_ui_present(ren);
	self->winId = so_ui_window_get_id(win);
	self->win = win;
	self->ren = ren;
	return self;
}

void frame_set_widget(struct frame_t* self, struct widget_t* widget) {
	self->widget = widget;
}

SDL_Renderer* frame_get_renderer(struct frame_t* self) {
	return self->ren;
}

void frame_free(struct frame_t* self) {
	if (self->widget) {
		widget_free(self->widget);
	}
	so_ui_renderer_free(self->ren);
	so_ui_window_free(self->win);
	free(self);
}

struct font_t* font_new(char* name, unsigned int size) {
	TTF_Font *font = 0;
	char filename[strlen(name) + 6];
	memset(filename, 0, sizeof(filename));
	strcat(filename, "font/");
	strcat(filename, name);
	font = TTF_OpenFont(filename, size);
	if (!font) {
		printf("%s\n", TTF_GetError());
		return 0;
	}
	struct font_t* self = (struct font_t*) calloc(1, sizeof(struct font_t));
	if (!self) {
		TTF_CloseFont(font);
		return 0;
	}
	self->name = name;
	self->size = size;
	self->data = font;
	return self;
}

void font_free(struct font_t* self) {
	if (self->name) {
		free(self->name);
	}
	if (self->data) {
		TTF_CloseFont(self->data);
	}
	free(self);
}

struct font_t* font_get_default(unsigned int size) {
	char* defaultName = "SourceSansPro-Regular.ttf";
	int defaultSize = size;
	struct font_t** it = (struct font_t**) list_iterator(env->fonts);
	while (*it) {
		struct font_t* font = *it++;
		if (font->size == defaultSize) {
			if (*font->name == *defaultName) {
				if (!strcmp(font->name, defaultName)) {
					return font;
				}
			}
		}
	}
	struct font_t* font = font_new(strdup(defaultName), defaultSize);
	if (!font) {
		return 0;
	}
	if (list_add(&env->fonts, font)) {
		font_free(font);
		return 0;
	}
	return font;
}

TTF_Font* font_get_data(struct font_t* self) {
	return self->data;
}
