#ifndef _XLSW_H
#define _XLSW_H

#define MISSING_VALUE  "NA"

xcb_connection_t *dpy;
xcb_window_t root;
xcb_ewmh_connection_t *ewmh;
bool recursive;

void setup(void);
void list_subwindows(xcb_window_t, int);
void print_info(xcb_window_t, int);
bool get_window_title(xcb_window_t, char *, size_t);

#endif
