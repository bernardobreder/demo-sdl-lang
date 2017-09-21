#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "so.h"
#include "gui.h"
#include "widget.h"
#include "uil.h"
#include "io.h"
#include "test.h"

#undef main

int main(int argc, char *argv[]) {
//	test();
	if (so_init(0)) {
		return 1;
	}
	if (gui_init()) {
		return 1;
	}
	struct frame_t* frame = frame_new(600, 400);
	if (!frame) {
		so_free();
		gui_free();
		return 1;
	}
	unsigned char* page = file_read("ui.txt");
	struct widget_t* widget = uil_eval(page);
	frame_set_widget(frame, widget);
	if (gui_add_frame(frame)) {
		gui_free();
		return 1;
	}
	gui_event_loop_wait();
	gui_free();
	so_free();
	return 0;
}
