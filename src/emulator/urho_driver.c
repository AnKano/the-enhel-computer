/************************************************************************/
/*			KEGS: Apple //gs Emulator			*/
/*			Copyright 2002 by Kent Dickey			*/
/*									*/
/*		This code is covered by the GNU GPL			*/
/*									*/
/*	The KEGS web page is kegs.sourceforge.net			*/
/*	You may contact the author at: kadickey@alumni.princeton.edu	*/
/************************************************************************/

/* Based on code from Chea Chee Keong from KEGS32, which is available at */
/*  http://www.geocities.com/akilgard/kegs32 */

#define STRICT

#include "defc.h"

extern int Verbose;

extern int g_warp_pointer;
extern int g_screen_depth;
extern int g_force_depth;

int g_screen_mdepth = 0;

extern int g_quit_sim_now;

int g_use_shmem = 0;
int g_has_focus = 0;
int g_auto_repeat_on = -1;

extern Kimage g_mainwin_kimage;

extern word32 g_palette_8to1624[256];
extern word32 g_a2palette_8to1624[256];

extern word32 g_full_refresh_needed;

extern int g_border_sides_refresh_needed;
extern int g_border_special_refresh_needed;
extern int g_status_refresh_needed;

extern int g_lores_colors[];
extern int g_cur_a2_stat;

extern int g_a2vid_palette;

extern int g_installed_full_superhires_colormap;

extern int g_screen_redraw_skip_amt;

extern word32 g_a2_screen_buffer_changed;

extern int g_joystick_type;
extern int g_config_kegs_update_needed;
extern int g_limit_speed;
extern int g_doc_vol;

extern char *g_status_ptrs[MAX_STATUS_LINES];

int g_win_button_states = 0;

int a2_keymap[][2] = {
        // !TODO: implement numpad

        // Special keys
        {0x76, 1073741885}, // F4

        // First keyboard row
        {0x35, 27},         // ESC
        {0x12, 49},         // 1/!
        {0x13, 50},         // 2/@
        {0x14, 51},         // 3/#
        {0x15, 52},         // 4/$
        {0x17, 53},         // 5/%
        {0x16, 54},         // 6/^
        {0x1a, 55},         // 7/&
        {0x1c, 56},         // 8/*
        {0x19, 57},         // 9/(
        {0x1d, 48},         // 0/)
        {0x1b, 45},         // -/_
        {0x18, 61},         // =/+
        {0x33, 8},          // Backspace aka. Delete

        // Second keyboard row
        {0x30, 9},   // Tab
        {0x0c, 113}, // Q
        {0x0d, 119}, // W
        {0x0e, 101}, // E
        {0x0f, 114}, // R
        {0x11, 116}, // T
        {0x10, 121}, // Y
        {0x20, 117}, // U
        {0x22, 105}, // I
        {0x1f, 111}, // O
        {0x23, 112}, // P
        {0x21, 91},  // [
        {0x1e, 93},  // ]
        {0x24, 13},  // Return

        // Third keyboard row
        {0x36, 1073742048}, // (Left) Control
        {0x00, 97},         // A
        {0x01, 115},        // S
        {0x02, 100},        // D
        {0x03, 102},        // F
        {0x05, 103},        // G
        {0x04, 104},        // H
        {0x26, 106},        // J
        {0x28, 107},        // K
        {0x25, 108},        // L
        {0x29, 59},         // ;
        {0x27, 39},         // '
        {0x24, 13},         // Return

        // Forth keyboard row
        {0x38, 1073742049}, // Left Shift
        {0x06, 122},        // Z
        {0x07, 120},        // X
        {0x08, 99},         // C
        {0x09, 118},        // V
        {0x0b, 98},         // B
        {0x2d, 110},        // N
        {0x2e, 109},        // M
        {0x2b, 44},         // ,
        {0x2f, 46},         // .
        {0x2c, 47},         // /
        {0x38, 1073742053}, // Right Shift

        // Fifth keyboard row
        {0x3a, 1073742050},  // Left Alt -> Option
        {0x37, 1073742054},  // Right Alt -> Command
        {0x32, 96},          // Tilda
        {0x31, 32},          // Space
        {0x2a, 92},          // Backslash
        {0x3e, 1073741906},  // Arrow Up
        {0x3b, 1073741904},  // Arrow Left
        {0x3d, 1073741905},  // Arrow Down
        {0x3c, 1073741903},  // Arrow Right
};
int a2_keymap_length = 62;

unsigned char **ex_image;
int **ex_dimx, **ex_dimy, **ex_depth;

unsigned int *audio_buffer_count;
unsigned int *audio_mutex;
byte **audio_buffer;

int run(int argc, char **argv,
        unsigned char **image, int **dimx, int **dimy, int **depth,
        unsigned int *ext_audio_count, byte **ext_audio_buffer, unsigned int *ext_audio_mutex) {
    ex_image = image;
    ex_dimy = dimy;
    ex_dimx = dimx;
    ex_depth = depth;

    g_screen_depth = 24;
    g_screen_mdepth = 32;

    audio_buffer_count = ext_audio_count;
    audio_buffer = ext_audio_buffer;
    audio_mutex = ext_audio_mutex;
    *audio_mutex = 1;

    return kegsmain(argc, argv);
}


struct KeyboardEvent {
    int value;
    int isUp;
    struct KeyboardEvent *_nextVal;
};
extern struct KeyboardEvent *keyboardEmulator;
extern struct KeyboardEvent *keyboardLastEvent;

struct MouseEvent {
    int mouseX;
    int mouseY;
    int clicked;
};
extern struct MouseEvent *mouseInput;

extern int turnOffState;

void check_input_events() {
    if (turnOffState == 1) {
        exit(0);
    }

    // mouse events
    int buttons = (mouseInput->clicked ? 0x1 : 0x0);

    update_mouse(mouseInput->mouseX, mouseInput->mouseY, buttons, 7);

    // keyboard events
    while (keyboardEmulator != NULL) {
        int i;
        for (i = 0; i <= a2_keymap_length; i++) {
            if (a2_keymap[i][1] == keyboardEmulator->value)
                break;
            if (i == a2_keymap_length)
                return;
        }
        adb_physical_key_update(a2_keymap[i][0], keyboardEmulator->isUp);

        struct KeyboardEvent *_prev = keyboardEmulator;
        keyboardEmulator = keyboardEmulator->_nextVal;
        free(_prev);
    }
}

void x_get_kimage(Kimage *kimage_ptr) {
    byte *ptr;
    int width;
    int height;
    int depth, mdepth;
    int size;

    width = kimage_ptr->width_req;
    height = kimage_ptr->height;
    depth = kimage_ptr->depth;
    mdepth = kimage_ptr->mdepth;

    size = 0;
    if (depth == g_screen_depth) {
        size = (width * height * mdepth) >> 3;
        kimage_ptr->data_ptr = (byte *) malloc(size);
        kimage_ptr->dev_handle = kimage_ptr->data_ptr;
    } else {
        size = (width * height * mdepth) >> 3;
        ptr = (byte *) malloc(size);

        if (ptr == 0) {
            printf("malloc for data failed, mdepth: %d\n", mdepth);
            exit(2);
        }

        kimage_ptr->data_ptr = ptr;
        kimage_ptr->dev_handle = (void *) -1;
    }
}

void dev_video_init() {
    int extra_size;
    int lores_col;
    int i;

    video_get_kimages();

    if (g_screen_depth != 8) {
        video_get_kimage(&g_mainwin_kimage, 0, g_screen_depth, g_screen_mdepth);
    }

    for (i = 0; i < 256; i++) {
        lores_col = g_lores_colors[i & 0xf];
        video_update_color_raw(i, lores_col);
        g_a2palette_8to1624[i] = g_palette_8to1624[i];
    }

    g_installed_full_superhires_colormap = 1;

    fflush(stdout);
}

void x_push_kimage(Kimage *kimage_ptr, int destx, int desty, int srcx, int srcy, int width, int height) {
    *ex_dimx = &kimage_ptr->width_req;
    *ex_dimy = &kimage_ptr->height;
    *ex_depth = &kimage_ptr->mdepth;
    *ex_image = kimage_ptr->data_ptr;
}

void x_push_done() {}

void x_redraw_status_lines() {}

void x_auto_repeat_on(int must) {}

void x_auto_repeat_off(int must) {}

void x_hide_pointer(int do_hide) {}

void x_full_screen(int do_full) {}

void x_dialog_create_kegs_conf(const char *str) {}

int x_show_alert(int is_fatal, const char *str) { return 0; }

void x_update_color(int col_num, int red, int green, int blue, word32 rgb) {}

void x_update_physical_colormap() {}

void show_xcolor_array() {}

void xdriver_end() {}