//
// Created by anshu on 9/19/2020.
//

#ifndef KEGS_PORT_PROTOS_GODOT_H
#define KEGS_PORT_PROTOS_GODOT_H

#endif //KEGS_PORT_PROTOS_GODOT_H

#include "defc.h"

int run(int argc, char **argv, unsigned char** image, int** dimx, int** dimy, int** depth,
        unsigned int* ext_audio_count, byte** ext_audio_buffer, unsigned int* ext_audio_mutex);
void check_input_events(void);
void x_update_color(int col_num, int red, int green, int blue, word32 rgb);
void x_update_physical_colormap(void);
void show_xcolor_array(void);
void xdriver_end(void);
void x_get_kimage(Kimage *kimage_ptr);
void dev_video_init(void);
void x_redraw_status_lines(void);
void x_push_kimage(Kimage *kimage_ptr, int destx, int desty, int srcx, int srcy, int width, int height);
void x_push_done(void);
void x_auto_repeat_on(int must);
void x_auto_repeat_off(int must);
void x_hide_pointer(int do_hide);
