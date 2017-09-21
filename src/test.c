/*
 * test.c
 *
 *  Created on: 22/07/2013
 *      Author: bernardobreder
 */

#include <assert.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "so.h"
#include "gui.h"
#include "widget.h"
#include "uil.h"
#include "io.h"

#define assert_not_error(v) assert(!v)
#define TIMEOUT 1000

static void robot_close() {
	SDL_Event e;
	e.type = SDL_QUIT;
	so_ui_push_event(&e);
}

static void robot_move_mouse(struct scroll_t* scroll, int x, int y) {
	int x_abs = widget_get_x_abs((struct widget_t*) scroll);
	int y_abs = widget_get_x_abs((struct widget_t*) scroll);
	so_ui_mouse_set_xy(x_abs, y_abs);
	SDL_Event e;
	e.type = SDL_MOUSEWHEEL;
	e.wheel.x = x;
	e.wheel.y = y;
	e.wheel.windowID = so_ui_window_get_id(0);
	so_ui_push_event(&e);
	assert_not_error(gui_event_loop());
}

#define expected_robot_video(frame,log) assert_not_error(robot_video(frame,log));

static error_t robot_video(struct frame_t* frame, char* log) {
	char** logs;
	so_debug(0, 0, &logs);
	char** it = logs;
	while (*it) {
		char* ilog = *(it++);
		if (!strcmp(ilog, log)) {
			free(logs);
			return 0;
		}
	}
	if (logs) {
		free(logs);
	}
	printf("expected '%s' and found:\n", log);
	it = logs;
	while (*it) {
		char* ilog = *(it++);
		printf("\t%s\n", ilog);
	}
	return 1;
}

static struct widget_t* robot_find(struct frame_t* frame, unsigned char classid, const char* id) {
	if (frame->widget) {
		struct list_t* list = list_new(10);
		assert_not_error(list_add(&list, frame->widget));
		while (list_size(list) > 0) {
			struct widget_t* widget = list_remove(list, list_size(list) - 1);
			if (widget->classid == classid && !strcmp(widget->id, id)) {
				list_free(list, 0);
				return widget;
			}
			switch (widget->classid) {
			case WIDGET_SCROLL_CLASSID : {
				struct scroll_t* scroll = (struct scroll_t*) widget;
				assert_not_error(list_add(&list, scroll->widget));
				break;
			}
			case WIDGET_BORDER_CLASSID :
			case WIDGET_VERTICAL_CLASSID :
			case WIDGET_HORIZONTAL_CLASSID :
			case WIDGET_GRIDBAG_CLASSID :
			case WIDGET_GRID_CLASSID : {
				struct panel_t* panel = (struct panel_t*) widget;
				struct widget_t** it = (struct widget_t**) list_iterator(panel->children);
				while (*it) {
					struct widget_t* child = (struct widget_t*) *(it++);
					assert_not_error(list_add(&list, child));
				}
				break;
			}
			}
		}
	}
	assert(0);
	return 0;
}

static void test_ui_thread(void (*callback)(struct frame_t*), char* uil) {
	assert(!so_init(1));
	assert_not_error(gui_init());
	struct frame_t* frame = frame_new(800, 600);
	assert(frame);
	struct widget_t* widget = uil_eval((unsigned char*) strdup(uil));
	assert(widget);
	frame_set_widget(frame, widget);
	assert_not_error(gui_add_frame(frame));
	assert_not_error(gui_event_loop());
	callback(frame);
	assert_not_error(gui_event_loop());
	robot_close();
	assert(gui_event_loop());
	gui_free();
	so_free();
}

static void test_ui_quit(struct frame_t* frame) {
	printf("test_ui_quit...\n");
}

static void test_ui_label_find(struct frame_t* frame) {
	printf("test_ui_label_find...\n");
	robot_find(frame, WIDGET_LABEL_CLASSID, "abc");
}

static void test_ui_vertical_find(struct frame_t* frame) {
	printf("test_ui_vertical_find...\n");
	robot_find(frame, WIDGET_VERTICAL_CLASSID, "a");
	robot_find(frame, WIDGET_LABEL_CLASSID, "b");
	robot_find(frame, WIDGET_LABEL_CLASSID, "c");
	robot_find(frame, WIDGET_LABEL_CLASSID, "d");
}

static void test_ui_both_find(struct frame_t* frame) {
	printf("test_ui_both_find...\n");
	robot_find(frame, WIDGET_SCROLL_CLASSID, "a");
	robot_find(frame, WIDGET_BORDER_CLASSID, "b");
	robot_find(frame, WIDGET_VERTICAL_CLASSID, "c");
	robot_find(frame, WIDGET_HORIZONTAL_CLASSID, "d");
//	robot_find(frame, WIDGET_GRIDBAG_CLASSID, "e");
//	robot_find(frame, WIDGET_GRID_CLASSID, "f");
	robot_find(frame, WIDGET_LABEL_CLASSID, "g");
	robot_find(frame, WIDGET_TEXT_CLASSID, "h");
	robot_find(frame, WIDGET_AREA_CLASSID, "i");
	robot_find(frame, WIDGET_BUTTON_CLASSID, "j");
	robot_find(frame, WIDGET_IMAGE_CLASSID, "k");
}

static void test_ui_label_text(struct frame_t* frame) {
	printf("test_ui_label_text...\n");
	robot_find(frame, WIDGET_LABEL_CLASSID, "a");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:0 y:0 w:18 h:47");
}

static void test_ui_image_path(struct frame_t* frame) {
	printf("test_ui_image_path...\n");
	robot_find(frame, WIDGET_IMAGE_CLASSID, "a");
	expected_robot_video(frame, "fill rect at x:0 y:0 w:600 h:400 r:255 g:0 b:0 a:0");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:469 h:159 to x:0 y:0 w:600 h:400");
}

static void test_ui_image_path_size(struct frame_t* frame) {
	printf("test_ui_image_path_size...\n");
	robot_find(frame, WIDGET_IMAGE_CLASSID, "a");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:469 h:159 to x:0 y:0 w:200 h:200");
}

static void test_ui_label_height(struct frame_t* frame) {
	printf("test_ui_label_height...\n");
	robot_find(frame, WIDGET_LABEL_CLASSID, "a");
	expected_robot_video(frame, "fill rect at x:0 y:0 w:600 h:400 r:255 g:0 b:0 a:0");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:0 y:0 w:18 h:47");
}

static void test_ui_label_margin(struct frame_t* frame) {
	printf("test_ui_label_margin...\n");
	robot_find(frame, WIDGET_LABEL_CLASSID, "a");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:10 y:10 w:18 h:47");
}

static void test_ui_label_padding(struct frame_t* frame) {
	printf("test_ui_label_padding...\n");
	robot_find(frame, WIDGET_LABEL_CLASSID, "a");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:10 y:10 w:18 h:47");
}

static void test_ui_label_padding_margin(struct frame_t* frame) {
	printf("test_ui_label_padding_margin...\n");
	robot_find(frame, WIDGET_LABEL_CLASSID, "a");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:15 y:15 w:18 h:47");
}

static void test_ui_vertical_label(struct frame_t* frame) {
	printf("test_ui_vertical_label...\n");
	robot_find(frame, WIDGET_LABEL_CLASSID, "a");
	expected_robot_video(frame, "fill rect at x:0 y:0 w:600 h:400 r:255 g:0 b:0 a:0");
	expected_robot_video(frame, "fill rect at x:0 y:0 w:600 h:47 r:0 g:255 b:0 a:0");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:0 y:0 w:18 h:47");
}

static void test_ui_vertical_label_height(struct frame_t* frame) {
	printf("test_ui_vertical_label_height...\n");
	robot_find(frame, WIDGET_LABEL_CLASSID, "a");
	expected_robot_video(frame, "fill rect at x:0 y:0 w:600 h:400 r:255 g:0 b:0 a:0");
	expected_robot_video(frame, "fill rect at x:0 y:0 w:600 h:400 r:0 g:255 b:0 a:0");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:0 y:0 w:18 h:47");
}

static void test_ui_vertical_label_padding_margin(struct frame_t* frame) {
	printf("test_ui_vertical_label_padding_margin...\n");
	robot_find(frame, WIDGET_LABEL_CLASSID, "a");
	expected_robot_video(frame, "fill rect at x:2 y:2 w:596 h:396 r:255 g:0 b:0 a:0");
	expected_robot_video(frame, "fill rect at x:13 y:13 w:574 h:57 r:0 g:255 b:0 a:0");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:18 y:18 w:18 h:47");
}

static void test_ui_vertical_labels_padding_margin(struct frame_t* frame) {
	printf("test_ui_vertical_labels_padding_margin...\n");
	robot_find(frame, WIDGET_LABEL_CLASSID, "a");
	robot_find(frame, WIDGET_LABEL_CLASSID, "b");
	robot_find(frame, WIDGET_LABEL_CLASSID, "c");
	expected_robot_video(frame, "fill rect at x:2 y:2 w:596 h:398 r:255 g:0 b:0 a:0");
	expected_robot_video(frame, "fill rect at x:13 y:13 w:574 h:230 r:0 g:255 b:0 a:0");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:18 y:18 w:18 h:47");
	expected_robot_video(frame, "fill rect at x:13 y:264 w:574 h:136 r:0 g:255 b:0 a:0");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:18 y:269 w:18 h:47");
}

static void test_ui_scroll_vertical_labels_padding_margin(struct frame_t* frame) {
	printf("test_ui_scroll_vertical_labels_padding_margin...\n");
	struct scroll_t* scroll = (struct scroll_t*) robot_find(frame, WIDGET_SCROLL_CLASSID, "s");
	robot_find(frame, WIDGET_LABEL_CLASSID, "a");
	robot_find(frame, WIDGET_LABEL_CLASSID, "b");
	robot_find(frame, WIDGET_LABEL_CLASSID, "c");
	expected_robot_video(frame, "fill rect at x:2 y:2 w:50 h:398 r:255 g:0 b:0 a:0");
	expected_robot_video(frame, "fill rect at x:13 y:13 w:28 h:230 r:0 g:255 b:0 a:0");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:18 y:18 w:18 h:47");
	expected_robot_video(frame, "fill rect at x:13 y:264 w:28 h:136 r:0 g:255 b:0 a:0");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:18 y:269 w:18 h:47");
	expected_robot_video(frame, "fill rect at x:594 y:1 w:5 h:209 r:0 g:0 b:0 a:100");
	robot_move_mouse(scroll, 0, -1000);
	expected_robot_video(frame, "fill rect at x:2 y:0 w:50 h:398 r:255 g:0 b:0 a:0");
	expected_robot_video(frame, "fill rect at x:13 y:0 w:28 h:136 r:0 g:255 b:0 a:0");
	expected_robot_video(frame, "fill rect at x:13 y:157 w:28 h:230 r:0 g:255 b:0 a:0");
	expected_robot_video(frame, "copy texture from x:0 y:0 w:18 h:47 to x:18 y:162 w:18 h:47");
	expected_robot_video(frame, "fill rect at x:594 y:190 w:5 h:209 r:0 g:0 b:0 a:100");
}

static void test_ui() {
	test_ui_thread(test_ui_quit, "label{}");
	test_ui_thread(test_ui_label_find, "label{id:'abc'}");
	test_ui_thread(test_ui_vertical_find, "vertical{ id:'a' content:[label{id:'b'} label{id:'c'} label{id:'d'}]}");
	test_ui_thread(test_ui_both_find, "scroll{id:'a' content:border{ id:'b' center:vertical{id:'c' content:"
			"[horizontal{id:'d' content:[label{id='g'}text{id:'h'}area{id:'i'}button{id='j'}image{id='k'}]}]}}}");
	test_ui_thread(test_ui_label_text, "label{id='a' text:'a'}");
	test_ui_thread(test_ui_image_path, "image{id='a' path='img/tool.png' bgcolor:0xff0000}");
	test_ui_thread(test_ui_image_path_size, "image{id='a' path='img/tool.png' width:200 height:200}");
	test_ui_thread(test_ui_label_height, "label{id='a' text:'a' height: 600 bgcolor: 0xff0000}");
	test_ui_thread(test_ui_label_margin, "label{id='a' text:'a' margin: 10}");
	test_ui_thread(test_ui_label_padding, "label{id='a' text:'a' padding: 10}");
	test_ui_thread(test_ui_label_padding_margin, "label{id='a' text:'a' margin 5 padding 10}");
	test_ui_thread(test_ui_vertical_label, "vertical{bgcolor:0xff0000 content:[label{id='a' text:'a' bgcolor:0x00ff00}]}");
	test_ui_thread(test_ui_vertical_label_height, "vertical{bgcolor:0xff0000 content:[label{id='a' text:'a' bgcolor:0x00ff00 height: 600}]}");
	test_ui_thread(test_ui_vertical_label_padding_margin, "vertical{margin:1 padding:2 bgcolor:0xff0000 content:["
			"label{id='a' text:'a' margin 5 padding 10 bgcolor:0x00ff00}]}");
	test_ui_thread(test_ui_vertical_labels_padding_margin, "vertical{margin:1 padding:2 bgcolor:0xff0000 content:["
			"label{id='a' text:'a' margin 5 padding 10 bgcolor:0x00ff00 height: 250}"
			"label{id='b' text:'a' margin 5 padding 10 bgcolor:0x00ff00 height: 250}"
			"label{id='c' text:'a' margin 5 padding 10 bgcolor:0x00ff00 height: 250}]}");
	test_ui_thread(test_ui_scroll_vertical_labels_padding_margin, "scroll {id='s' content: vertical{margin:1 padding:2 bgcolor:0xff0000 content:["
			"label{id='a' text:'a' margin 5 padding 10 bgcolor:0x00ff00 height: 250}"
			"label{id='b' text:'a' margin 5 padding 10 bgcolor:0x00ff00 height: 250}"
			"label{id='c' text:'a' margin 5 padding 10 bgcolor:0x00ff00 height: 250}]}}");
}

void test() {
	test_ui();
	/*{
		unsigned char* text = file_read("test/main.txt");
		struct node_t* node = lng(text);
	}
	int n;
	long timer = clock();
	printf("start with %ld\n", timer);
	for (n = 0;; n++) {
		unsigned char* text = file_read("test/main.txt");
		lng(text);
		free(text);
		if (n % 64 == 0 && clock() - timer > 5 * 1000 * 1000) {
			break;
		}
	}
	printf("finish with %ld\n", clock());
	printf("diff with %ld\n", clock() - timer);
	printf("%d compile/seg\n", n / 10);*/
}
