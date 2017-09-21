#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mixer.h>

#include "struct.h"
#include "so.h"
#include "list.h"

static unsigned char debug;

static struct list_t* windows = 0;

static SDL_mutex* lock = 0;

static struct list_t* events = 0;

static struct list_t* so_log = 0;

static struct list_t* back_buffer = 0;

static struct list_t* front_buffer = 0;

static SDL_Rect clipRect;

static int color = 0;

static int mouse_x = 0, mouse_y = 0;

error_t so_init(unsigned char debugFlag) {
	debug = debugFlag;
	if (debug) {
		windows = list_new(1);
		lock = SDL_CreateMutex();
		events = list_new(100);
		so_log = list_new(100);
		back_buffer = list_new(100);
		front_buffer = list_new(100);
	} else {
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
			return 1;
		}
		if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP) < 0) {
			printf("IMG_Init: %s\n", IMG_GetError());
			return 3;
		}
		if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MOD) < 0) {
			printf("Mix_Init: %s\n", Mix_GetError());
			return 4;
		}
	}
	if (TTF_Init() < 0) {
		fprintf(stderr, "Unable to init TTF: %s\n", TTF_GetError());
		return 2;
	}
	return 0;
}

void so_free() {
	if (debug) {
		SDL_mutexP(lock);
		list_free(so_log, free);
		list_free(back_buffer, free);
		list_free(front_buffer, free);
		list_free(events, free);
		list_free(windows, 0);
		SDL_mutexV(lock);
		SDL_DestroyMutex(lock);
	} else {
		SDL_Quit();
	}
}

SDL_Window* so_ui_window_new(char* title, int x, int y, int width, int height, int flags) {
	if (debug) {
		SDL_mutexP(lock);
		list_add(&so_log, strdup("window created"));
		SDL_mutexV(lock);
		return (SDL_Window*) title;
	} else {
		return SDL_CreateWindow(title, x, y, width, height, flags);
	}
}

void so_ui_window_free(SDL_Window* self) {
	if (debug) {
		SDL_mutexP(lock);
		list_add(&so_log, strdup("window freed"));
		SDL_mutexV(lock);
	} else {
		SDL_DestroyWindow(self);
	}
}

unsigned int so_ui_window_get_id(SDL_Window* self) {
	if (debug) {
		SDL_mutexP(lock);
		SDL_mutexV(lock);
		return 1;
	} else {
		return SDL_GetWindowID(self);
	}
}

void so_ui_surface_free(SDL_Surface* self) {
	if (debug) {
		SDL_mutexP(lock);
		SDL_FreeSurface(self);
		list_add(&so_log, strdup("surface freed"));
		SDL_mutexV(lock);
	} else {
		SDL_FreeSurface(self);
	}
}

struct SDL_FakeTexture {
	int w;
	int h;
};

SDL_Texture* so_ui_texture_new(SDL_Renderer * renderer, SDL_Surface* surface) {
	if (debug) {
		SDL_mutexP(lock);
		struct SDL_FakeTexture* texture = (struct SDL_FakeTexture*) calloc(1, sizeof(struct SDL_FakeTexture));
		texture->w = surface->w;
		texture->h = surface->h;
		list_add(&so_log, strdup("texture created"));
		SDL_mutexV(lock);
		return (SDL_Texture*) texture;
	} else {
		return SDL_CreateTextureFromSurface(renderer, surface);
	}
}

void so_ui_texture_free(SDL_Texture * texture) {
	if (debug) {
		SDL_mutexP(lock);
		list_add(&so_log, strdup("texture freed"));
		struct SDL_FakeTexture* fake_texture = (struct SDL_FakeTexture*) texture;
		free(fake_texture);
		SDL_mutexV(lock);
	} else {
		SDL_DestroyTexture(texture);
	}
}

int so_ui_texture_query(SDL_Texture* texture, unsigned int* format, int* access, int* w, int* h) {
	if (debug) {
		SDL_mutexP(lock);
		struct SDL_FakeTexture* fakeTexture = (struct SDL_FakeTexture*) texture;
		if (w) {
			if (texture) {
				*w = fakeTexture->w;
			} else {
				*w = 0;
			}
		}
		if (h) {
			if (texture) {
				*h = fakeTexture->h;
			} else {
				*h = 0;
			}
		}
		SDL_mutexV(lock);
		return 0;
	} else {
		return SDL_QueryTexture(texture, format, access, w, h);
	}
}

SDL_Renderer* so_ui_renderer_new(SDL_Window * win, int index, unsigned int flags) {
	if (debug) {
		SDL_mutexP(lock);
		list_add(&so_log, strdup("renderer created"));
		SDL_mutexV(lock);
		return (SDL_Renderer*) win;
	} else {
		return SDL_CreateRenderer(win, index, flags);
	}
}

void so_ui_renderer_free(SDL_Renderer* self) {
	if (debug) {
		SDL_mutexP(lock);
		list_add(&so_log, strdup("renderer freed"));
		SDL_mutexV(lock);
	} else {
		SDL_DestroyRenderer(self);
	}
}

void so_ui_renderer_get_size(SDL_Renderer* ren, int* width, int* height) {
	if (debug) {
		SDL_mutexP(lock);
		if (width) {
			*width = 600;
		}
		if (height) {
			*height = 400;
		}
		SDL_mutexV(lock);
	} else {
		SDL_GetRendererOutputSize(ren, width, height);
	}
}

void so_ui_set_color_uint(SDL_Renderer* ren, unsigned int rgba) {
	color = rgba;
	if (debug) {
	} else {
		unsigned char r = color_get_red(rgba);
		unsigned char g = color_get_green(rgba);
		unsigned char b = color_get_blue(rgba);
		unsigned char a = color_get_alpha(rgba);
		SDL_SetRenderDrawColor(ren, r, g, b, 0xFF - a);
	}
}

void so_ui_get_color_uint(SDL_Renderer* ren, unsigned int* rgba) {
	if (rgba) {
		*rgba = color;
	}
}

void so_ui_get_clip(SDL_Renderer* renderer, SDL_Rect* rect) {
	if (debug) {
		SDL_mutexP(lock);
		if (rect) {
			*rect = clipRect;
		}
		SDL_mutexV(lock);
	} else {
		SDL_RenderGetClipRect(renderer, rect);
	}
}

int so_ui_set_clip(SDL_Renderer * renderer, const SDL_Rect * rect) {
	if (debug) {
		SDL_mutexP(lock);
		if (rect) {
			clipRect = *rect;
		} else {
			clipRect.x = 0;
			clipRect.y = 0;
			clipRect.w = 0;
			clipRect.h = 0;
		}
		char buffer[100];
		sprintf(buffer, "clip seted to x:%d y:%d w:%d h:%d", clipRect.x, clipRect.y, clipRect.w, clipRect.h);
		list_add(&so_log, strdup(buffer));
		SDL_mutexV(lock);
		return 0;
	} else {
		return SDL_RenderSetClipRect(renderer, rect);
	}
}

void so_ui_clear(SDL_Renderer* ren) {
	if (debug) {
		SDL_mutexP(lock);
		list_clear(back_buffer, free);
		SDL_mutexV(lock);
	} else {
		SDL_RenderClear(ren);
	}
}

void so_ui_present(SDL_Renderer* ren) {
	if (debug) {
		SDL_mutexP(lock);
		list_clear(front_buffer, free);
		char** it = (char**) list_iterator(back_buffer);
		while (*it) {
			char* log = (char*) *(it++);
			list_add(&front_buffer, log);
		}
		list_clear(back_buffer, 0);
		SDL_mutexV(lock);
	} else {
		SDL_RenderPresent(ren);
	}
}

int so_ui_poll_event(SDL_Event* e) {
	if (debug) {
		SDL_Event* pe = 0;
		SDL_mutexP(lock);
		if (list_size(events) > 0) {
			pe = list_remove(events, 0);
		}
		SDL_mutexV(lock);
		if (pe) {
			e->type = pe->type;
			switch (pe->type) {
			case SDL_MOUSEWHEEL : {
				e->wheel = pe->wheel;
				break;
			}
			}
			free(pe);
			return 1;
		} else {
			return 0;
		}
	} else {
		return SDL_PollEvent(e);
	}
}

int so_ui_wait_event(SDL_Event* e) {
	if (debug) {
		SDL_Event* pe = 0;
		for (;;) {
			SDL_mutexP(lock);
			if (list_size(events) > 0) {
				pe = list_remove(events, 0);
			}
			SDL_mutexV(lock);
			if (pe) {
				break;
			}
			SDL_Delay(10);
		}
		e->type = pe->type;
		switch (pe->type) {
		}
		free(pe);
		return 1;
	} else {
		return SDL_WaitEvent(e);
	}
}

int so_ui_push_event(SDL_Event* e) {
	if (debug) {
		SDL_mutexP(lock);
		SDL_Event* pe = (SDL_Event*) calloc(1, sizeof(SDL_Event));
		pe->type = e->type;
		switch (e->type) {
		case SDL_MOUSEWHEEL : {
			pe->wheel = e->wheel;
			break;
		}
		}
		list_add(&events, pe);
		SDL_mutexV(lock);
		return 1;
	} else {
		return SDL_PushEvent(e);
	}
}

void so_ui_mouse_set_xy(int x, int y) {
	if (debug) {
		mouse_x = x;
		mouse_y = y;
	}
}

unsigned int so_ui_mouse_get_xy(int* x, int* y) {
	if (debug) {
		SDL_mutexP(lock);
		if (x) {
			*x = mouse_x;
		}
		if (y) {
			*y = mouse_y;
		}
		SDL_mutexV(lock);
		return 0;
	} else {
		return SDL_GetMouseState(x, y);
	}
}

int so_ui_render_blend(SDL_Renderer * renderer, SDL_BlendMode blendMode) {
	if (debug) {
		SDL_mutexP(lock);
		SDL_mutexV(lock);
		return 0;
	} else {
		return SDL_SetRenderDrawBlendMode(renderer, blendMode);
	}
}

int so_ui_render_fill_rect(SDL_Renderer * renderer, const SDL_Rect * rect) {
	if (debug) {
		SDL_mutexP(lock);
		char buffer[100];
		sprintf(buffer, "fill rect at x:%d y:%d w:%d h:%d r:%d g:%d b:%d a:%d", rect->x, rect->y, rect->w, rect->h, color_get_red(color), color_get_green(color), color_get_blue(color),
		color_get_alpha(color));
		list_add(&back_buffer, strdup(buffer));
		SDL_mutexV(lock);
		return 0;
	} else {
		return SDL_RenderFillRect(renderer, rect);
	}
}

int so_ui_render_draw_rect(SDL_Renderer * renderer, const SDL_Rect * rect, unsigned int size) {
	if (debug) {
		SDL_mutexP(lock);
		char buffer[100];
		sprintf(buffer, "draw rect at x:%d y:%d w:%d h:%d s:%d r:%d g:%d b:%d a:%d", rect->x, rect->y, rect->w, rect->h, size, color_get_red(color), color_get_green(color), color_get_blue(color),
		color_get_alpha(color));
		list_add(&back_buffer, strdup(buffer));
		SDL_mutexV(lock);
		return 0;
	} else {
		SDL_Rect rects[size];
		rects[0] = *rect;
		int n;
		for (n = 1; n < size; n++) {
			rects[n].x = rect->x + n;
			rects[n].y = rect->y + n;
			rects[n].w = rect->w - 2 * n;
			rects[n].h = rect->h - 2 * n;
		}
		return SDL_RenderDrawRects(renderer, rects, size);
	}
}

int so_ui_render_copy(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect) {
	if (debug) {
		SDL_mutexP(lock);
		char buffer[100];
		int sx = srcrect ? srcrect->x : 0;
		int sy = srcrect ? srcrect->y : 0;
		int sw = srcrect ? srcrect->w : 0;
		int sh = srcrect ? srcrect->h : 0;
		int dx = dstrect ? dstrect->x : 0;
		int dy = dstrect ? dstrect->y : 0;
		int dw = dstrect ? dstrect->w : 0;
		int dh = dstrect ? dstrect->h : 0;
		sprintf(buffer, "copy texture from x:%d y:%d w:%d h:%d to x:%d y:%d w:%d h:%d", sx, sy, sw, sh, dx, dy, dw, dh);
		list_add(&back_buffer, strdup(buffer));
		SDL_mutexV(lock);
		return 0;
	} else {
		return SDL_RenderCopy(renderer, texture, srcrect, dstrect);
	}
}

void widget_get_rect_margin(struct widget_t* self, SDL_Rect* rect) {
	int x_abs = 0, y_abs = 0;
	int width = self->pref_width ? self->pref_width : self->width;
	int height = self->pref_height ? self->pref_height : self->height;
	struct widget_t* parent = (struct widget_t*) self;
	while (parent) {
		x_abs += parent->x;
		y_abs += parent->y;
		parent = (struct widget_t*) parent->parent;
	}
	rect->x = x_abs + self->padding_left + self->border_size + self->margin_left;
	rect->y = y_abs + self->padding_top + self->border_size + self->margin_top;
	rect->w = width - self->padding_left - self->padding_right - self->margin_left - self->margin_right - self->border_size;
	rect->h = height - self->padding_top - self->padding_bottom - self->margin_top - self->margin_bottom - self->border_size;
}

void widget_get_rect_padding(struct widget_t* self, SDL_Rect* rect) {
	int x_abs = 0, y_abs = 0;
	int width = self->pref_width ? self->pref_width : self->width;
	int height = self->pref_height ? self->pref_height : self->height;
	struct widget_t* parent = (struct widget_t*) self;
	while (parent) {
		x_abs += parent->x;
		y_abs += parent->y;
		parent = (struct widget_t*) parent->parent;
	}
	rect->x = x_abs + self->padding_left;
	rect->y = y_abs + self->padding_top;
	rect->w = width - self->padding_left - self->padding_right;
	rect->h = height - self->padding_top - self->padding_bottom;
}

void so_debug(char** state, char*** so_logs, char*** video_logs) {
	if (debug) {
		char buffer[16 * 1024];
		if (state) {

			sprintf(buffer, "color { r:%d g:%d b:%d a:%d }\n", color_get_red(color), color_get_green(color), color_get_blue(color), color_get_alpha(color));
			sprintf(buffer + strlen(buffer), "clip { x:%d y:%d w:%d h:%d }\n", clipRect.x, clipRect.y, clipRect.w, clipRect.h);
			sprintf(buffer + strlen(buffer), "\n");
			{
				sprintf(buffer + strlen(buffer), "so_log {\n");
				char** it = (char**) list_iterator(so_log);
				while (*it) {
					char* chars = *(it++);
					sprintf(buffer + strlen(buffer), "\t%s\n", chars);
				}
				sprintf(buffer + strlen(buffer), "}\n\n");
			}
			{
				sprintf(buffer + strlen(buffer), "front_buffer {\n");
				char** it = (char**) list_iterator(front_buffer);
				while (*it) {
					char* chars = *(it++);
					sprintf(buffer + strlen(buffer), "\t%s\n", chars);
				}
				sprintf(buffer + strlen(buffer), "}\n\n");
			}
			*state = strdup(buffer);
		}
		if (so_logs) {
			char** aux = (char**) malloc((list_size(so_log) + 1) * sizeof(char*));
			if (aux) {
				*so_logs = aux;
				aux[list_size(so_log)] = 0;
				char** it = (char**) list_iterator(so_log);
				while (*it) {
					char* chars = *(it++);
					*(aux++) = chars;
				}
			}
		}
		if (video_logs) {
			char** aux = (char**) malloc((list_size(front_buffer) + 1) * sizeof(char*));
			if (aux) {
				*video_logs = aux;
				aux[list_size(front_buffer)] = 0;
				char** it = (char**) list_iterator(front_buffer);
				while (*it) {
					char* chars = *(it++);
					*(aux++) = chars;
				}
			}
		}
	}
}

