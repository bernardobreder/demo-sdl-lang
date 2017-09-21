/*
 * struct.h
 *
 *  Created on: 14/07/2013
 *      Author: bernardobreder
 */

#ifndef STRUCT_H_
#define STRUCT_H_

#if defined(_MSC_VER)
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_thread.h>
#include <SDL_net.h>
#include <SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mixer.h>
#endif

typedef unsigned char error_t;

#define color_get_alpha(c) ((c&0xFF000000)>>24)
#define color_get_red(c) ((c&0xFF0000)>>16)
#define color_get_green(c) ((c&0xFF00)>>8)
#define color_get_blue(c) (c&0xFF)
#define color_set_alpha(v) ((v&0xFF)<<24)
#define color_set_red(v) ((v&0xFF)<<16)
#define color_set_green(v) ((v&0xFF)<<8)
#define color_set_blue(v) (v&0xFF)
#define color_set(r,g,b,a) color_set_alpha(a)+color_set_red(r)+color_set_green(g)+color_set_blue(b)

struct gui_t {
	struct list_t* frames;
	struct list_t* fonts;
};

struct frame_t {
	struct widget_t* widget;
	SDL_Window* win;
	SDL_Renderer* ren;
	unsigned int winId;
};

struct font_t {
	char* name;
	unsigned int size;
	unsigned int refCounter;
	TTF_Font* data;
};

struct list_t {
	unsigned int size;
	unsigned int max;
	void* data;
};

struct widget_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned int pref_width;
	unsigned int pref_height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void (*key_press)(struct widget_t* self, SDL_Event* event);
	void (*key_release)(struct widget_t* self, SDL_Event* event);
	void (*mouse_press)(struct widget_t* self, SDL_Event* event);
	void (*mouse_release)(struct widget_t* self, SDL_Event* event);
	void (*mouse_motion)(struct widget_t* self, SDL_Event* event);
	void (*mouse_wheel)(struct widget_t* self, SDL_Event* event);
	void (*mouse_dragin)(struct widget_t* self, SDL_Event* event);
	void (*mouse_dragout)(struct widget_t* self, SDL_Event* event);
	void (*focus_got)(struct widget_t* self, SDL_Event* event);
	void (*focus_lost)(struct widget_t* self, SDL_Event* event);
};

struct label_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned int pref_width;
	unsigned int pref_height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void* key_press;
	void* key_release;
	void* mouse_press;
	void* mouse_release;
	void* mouse_motion;
	void* mouse_dragin;
	void* mouse_dragout;
	void* focus_got;
	void* focus_lost;
	char* text;
	struct font_t* font;
	unsigned int fore_color;
	unsigned int font_size;
	unsigned char wrap;
	SDL_Texture* textTexture;
	SDL_Texture** textures;
	unsigned int texture_size;
};

struct text_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned int pref_width;
	unsigned int pref_height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void* key_press;
	void* key_release;
	void* mouse_press;
	void* mouse_release;
	void* mouse_motion;
	void* mouse_dragin;
	void* mouse_dragout;
	void* focus_got;
	void* focus_lost;
	char* text;
	struct font_t* font;
};

struct area_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned int pref_width;
	unsigned int pref_height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void* key_press;
	void* key_release;
	void* mouse_press;
	void* mouse_release;
	void* mouse_motion;
	void* mouse_dragin;
	void* mouse_dragout;
	void* focus_got;
	void* focus_lost;
	char* text;
	struct font_t* font;
};

struct button_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned int pref_width;
	unsigned int pref_height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void* key_press;
	void* key_release;
	void* mouse_press;
	void* mouse_release;
	void* mouse_motion;
	void* mouse_dragin;
	void* mouse_dragout;
	void* focus_got;
	void* focus_lost;
	char* text;
	struct font_t* font;
};

struct image_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned int pref_width;
	unsigned int pref_height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void* key_press;
	void* key_release;
	void* mouse_press;
	void* mouse_release;
	void* mouse_motion;
	void* mouse_dragin;
	void* mouse_dragout;
	void* focus_got;
	void* focus_lost;
	char* path;
	SDL_Texture* texture;
	SDL_Rect clip_rect;
};

struct panel_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned int pref_width;
	unsigned int pref_height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void* key_press;
	void* key_release;
	void* mouse_press;
	void* mouse_release;
	void* mouse_motion;
	void* mouse_dragin;
	void* mouse_dragout;
	void* focus_got;
	void* focus_lost;
	struct list_t* children;
};

struct scroll_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned int pref_width;
	unsigned int pref_height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void* key_press;
	void* key_release;
	void* mouse_press;
	void* mouse_release;
	void* mouse_motion;
	void* mouse_dragin;
	void* mouse_dragout;
	void* focus_got;
	void* focus_lost;
	struct widget_t* widget;
	int view_x;
	int view_y;
};

struct border_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned int pref_width;
	unsigned int pref_height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void* key_press;
	void* key_release;
	void* mouse_press;
	void* mouse_release;
	void* mouse_motion;
	void* mouse_dragin;
	void* mouse_dragout;
	void* focus_got;
	void* focus_lost;
	struct list_t* children;
};

struct vertical_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned int pref_width;
	unsigned int pref_height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void* key_press;
	void* key_release;
	void* mouse_press;
	void* mouse_release;
	void* mouse_motion;
	void* mouse_dragin;
	void* mouse_dragout;
	void* focus_got;
	void* focus_lost;
	struct list_t* children;
};

struct horizontal_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned int pref_width;
	unsigned int pref_height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void* key_press;
	void* key_release;
	void* mouse_press;
	void* mouse_release;
	void* mouse_motion;
	void* mouse_dragin;
	void* mouse_dragout;
	void* focus_got;
	void* focus_lost;
	struct list_t* children;
};

struct gridbag_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void* key_press;
	void* key_release;
	void* mouse_press;
	void* mouse_release;
	void* mouse_motion;
	void* mouse_dragin;
	void* mouse_dragout;
	void* focus_got;
	void* focus_lost;
	struct list_t* children;
};

struct grid_t {
	unsigned char classid;
	struct panel_t* parent;
	char* id;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	unsigned int pref_width;
	unsigned int pref_height;
	unsigned char needToLayout;
	unsigned char needToPaint;
	int margin_left;
	int margin_top;
	int margin_right;
	int margin_bottom;
	int margin_middle;
	int padding_left;
	int padding_top;
	int padding_right;
	int padding_bottom;
	unsigned int background_color;
	unsigned int border_color;
	unsigned int border_size;
	void* key_press;
	void* key_release;
	void* mouse_press;
	void* mouse_release;
	void* mouse_motion;
	void* mouse_dragin;
	void* mouse_dragout;
	void* focus_got;
	void* focus_lost;
	struct list_t* children;
	unsigned int gridxs;
	unsigned int gridys;
};

#define UIL_EOF 0
#define UIL_FIRST UIL_ID
#define UIL_ID 300
#define UIL_STRING 301
#define UIL_NUMBER 302
#define UIL_HEX 303
#define UIL_GREATER_EQUAL 304
#define UIL_LOWER_EQUAL 305
#define UIL_GREATER_GREATER 306
#define UIL_LOWER_LOWER 307
#define UIL_DIFERENT 308
#define UIL_EQUAL_EQUAL 309
#define UIL_SUM_EQUAL 310
#define UIL_SUB_EQUAL 311
#define UIL_MUL_EQUAL 312
#define UIL_DIV_EQUAL 313
#define UIL_SUM_SUM 314
#define UIL_SUB_SUB 315

struct uil_t {
	unsigned char* text;
	struct lng_token_t* tokens;
	unsigned int tokenSize;
};

struct lng_token_t {
	unsigned int begin;
	unsigned int size;
	unsigned short type;
	unsigned int line;
	unsigned int column;
};

#endif /* STRUCT_H_ */
