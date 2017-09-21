/*
 * so.h
 *
 *  Created on: 22/07/2013
 *      Author: bernardobreder
 */

#ifndef SO_H_
#define SO_H_

#include "struct.h"

error_t so_init(unsigned char debug);

void so_free();

SDL_Window* so_ui_window_new(char* title, int x, int y, int width, int height, int flag);

void so_ui_window_free(SDL_Window* self);

unsigned int so_ui_window_get_id(SDL_Window* self);

void so_ui_surface_free(SDL_Surface* self);

SDL_Texture* so_ui_texture_new(SDL_Renderer * renderer, SDL_Surface* surface);

void so_ui_texture_free(SDL_Texture * texture);

int so_ui_texture_query(SDL_Texture* texture, unsigned int* format, int* access, int* w, int* h);

SDL_Renderer* so_ui_renderer_new(SDL_Window * win, int index, unsigned int flags);

void so_ui_renderer_free(SDL_Renderer* self);

void so_ui_renderer_get_size(SDL_Renderer* ren, int* width, int* height);

void so_ui_set_color(SDL_Renderer* ren, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void so_ui_set_color_uint(SDL_Renderer* ren, unsigned int rgba);

void so_ui_push_color_rgba(SDL_Renderer* ren, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void so_ui_push_color_uint(SDL_Renderer* ren, unsigned int rgba);

void so_ui_pop_color(SDL_Renderer* ren);

void so_ui_get_clip(SDL_Renderer* renderer, SDL_Rect* rect);

int so_ui_set_clip(SDL_Renderer * renderer, const SDL_Rect * rect);

void so_ui_clear(SDL_Renderer* ren);

void so_ui_present(SDL_Renderer* ren);

int so_ui_poll_event(SDL_Event* e);

int so_ui_push_event(SDL_Event* e);

int so_ui_wait_event(SDL_Event* e);

unsigned int so_ui_mouse_get_xy(int* x, int* y);

void so_ui_mouse_set_xy(int x, int y);

int so_ui_render_blend(SDL_Renderer* renderer, SDL_BlendMode blendMode);

int so_ui_render_fill_rect(SDL_Renderer* renderer, const SDL_Rect* rect);

int so_ui_render_draw_rect(SDL_Renderer * renderer, const SDL_Rect * rect,unsigned int size) ;

int so_ui_render_copy(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect);

void widget_get_rect_margin(struct widget_t* self, SDL_Rect* padding_clip);

void widget_get_rect_padding(struct widget_t* self, SDL_Rect* padding_clip);

void so_debug(char** state, char*** so_logs, char*** video_logs);

#endif /* SO_H_ */
