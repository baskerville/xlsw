#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_ewmh.h>
#include "helpers.h"
#include "xlsw.h"

int main(int argc, char *argv[])
{
    recursive = false;
    char opt;

    while ((opt = getopt(argc, argv, "hvr")) != -1) {
        switch (opt) {
            case 'h':
                printf("xlsw [-h|-v|-r] [WID ...]\n");
                return EXIT_SUCCESS;
                break;
            case 'v':
                printf("%s\n", VERSION);
                return EXIT_SUCCESS;
                break;
            case 'r':
                recursive = true;
                break;
        }
    }

    int num = argc - optind;
    char **args = argv + optind;

    setup();

    if (num > 0) {
        char *end;
        for (int i = 0; i < num; i++) {
            errno = 0;
            long int wid = strtol(args[i], &end, 0);
            if (errno != 0 || *end != '\0')
                warn("Invalid window ID: '%s'\n", args[i]);
            else
                list_subwindows(wid, 0);
        }
    } else {
        list_subwindows(root, 0);
    }

    xcb_ewmh_connection_wipe(ewmh);
    free(ewmh);
    xcb_disconnect(dpy);
    return EXIT_SUCCESS;
}

void setup(void)
{
    dpy = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(dpy))
        err("Can't open display.\n");
    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;
    if (screen == NULL)
        err("Can't acquire screen.\n");
    root = screen->root;
    ewmh = malloc(sizeof(xcb_ewmh_connection_t));
    if (xcb_ewmh_init_atoms_replies(ewmh, xcb_ewmh_init_atoms(dpy, ewmh), NULL) == 0)
        err("Can't initialize EWMH atoms.\n");
}

void list_subwindows(xcb_window_t parent, int depth)
{
    xcb_query_tree_reply_t *qtr = xcb_query_tree_reply(dpy, xcb_query_tree(dpy, parent), NULL);
    if (qtr == NULL) {
        warn("Failed to get the window subtree for 0x%X\n", parent);
        return;
    }
    int len = xcb_query_tree_children_length(qtr);
    xcb_window_t *wins = xcb_query_tree_children(qtr);
    for (int i = 0; i < len; i++) {
        print_info(wins[i], depth);
        if (recursive)
            list_subwindows(wins[i], depth + 2);
    }
    free(qtr);
}

void print_info(xcb_window_t win, int depth)
{
    char class_name[MAXLEN] = {0}, instance_name[MAXLEN] = {0}, class_instance[MAXLEN] = {0}, title[MAXLEN] = {0};
    xcb_icccm_get_wm_class_reply_t icr;
    if (xcb_icccm_get_wm_class_reply(dpy, xcb_icccm_get_wm_class(dpy, win), &icr, NULL) == 1) {
        strncpy(class_name, icr.class_name, sizeof(class_name));
        strncpy(instance_name, icr.instance_name, sizeof(instance_name));
        xcb_icccm_get_wm_class_reply_wipe(&icr);
    } else {
        strncpy(class_name, MISSING_VALUE, sizeof(class_name));
        strncpy(instance_name, MISSING_VALUE, sizeof(instance_name));
    }
    if (strncmp(class_name, instance_name, sizeof(class_name)) == 0)
        strncpy(class_instance, class_name, sizeof(class_instance));
    else
        snprintf(class_instance, sizeof(class_instance), "%s/%s", class_name, instance_name);
    if (!get_window_title(win, title, sizeof(title)))
        strncpy(title, MISSING_VALUE, sizeof(title));
    xcb_get_window_attributes_reply_t *war = xcb_get_window_attributes_reply(dpy, xcb_get_window_attributes(dpy, win), NULL);
    uint8_t override_redirect = 0, map_state = 0;
    uint16_t window_class = XCB_WINDOW_CLASS_INPUT_OUTPUT;
    if (war != NULL) {
        override_redirect = war->override_redirect;
        map_state = war->map_state;
        window_class = war->_class;
        free(war);
    }
    printf("%*s0x%07X  %c%c%c  %-11s  %s\n", depth, "", win, (map_state == XCB_MAP_STATE_VIEWABLE ? '-' : 'u'), (window_class == XCB_WINDOW_CLASS_INPUT_OUTPUT ? '-' : 'i'), (override_redirect ? 'o' : '-'), class_instance, title);
}

bool get_window_title(xcb_window_t win, char *title, size_t len) {
    xcb_ewmh_get_utf8_strings_reply_t ewmh_txt_prop;
    xcb_icccm_get_text_property_reply_t icccm_txt_prop;
    ewmh_txt_prop.strings = icccm_txt_prop.name = NULL;
    title[0] = '\0';
    if (xcb_ewmh_get_wm_name_reply(ewmh, xcb_ewmh_get_wm_name(ewmh, win), &ewmh_txt_prop, NULL) == 1 || xcb_icccm_get_wm_name_reply(dpy, xcb_icccm_get_wm_name(dpy, win), &icccm_txt_prop, NULL) == 1) {
        char *src = NULL;
        size_t title_len = 0;
        if (ewmh_txt_prop.strings != NULL) {
            src = ewmh_txt_prop.strings;
            title_len = MIN(len, ewmh_txt_prop.strings_len);
        } else if (icccm_txt_prop.name != NULL) {
            src = icccm_txt_prop.name;
            title_len = MIN(len, icccm_txt_prop.name_len);
        }
        if (src != NULL) {
            strncpy(title, src, title_len);
            title[title_len] = '\0';
            return true;
        }
    }
    return false;
}
