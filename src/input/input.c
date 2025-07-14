#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libudev.h>

#include "vldwm/input.h"

static int open_restricted(const char *path, int flags, void *user_data) {
    return open(path, flags);
}

static void close_restricted(int fd, void *user_data) {
    close(fd);
}

static const struct libinput_interface interface = {
    .open_restricted = open_restricted,
    .close_restricted = close_restricted,
};

int vldwm_input_init(struct vldwm_input *input) {
    if (!input) return -1;
    
    memset(input, 0, sizeof(*input));
    
    // Initialize udev
    input->udev = udev_new();
    if (!input->udev) {
        fprintf(stderr, "Failed to create udev context\n");
        return -1;
    }
    
    // Initialize libinput
    input->libinput = libinput_udev_create_context(&interface, NULL, input->udev);
    if (!input->libinput) {
        fprintf(stderr, "Failed to create libinput context\n");
        goto error;
    }
    
    // Assign seat
    if (libinput_udev_assign_seat(input->libinput, "seat0") != 0) {
        fprintf(stderr, "Failed to assign seat\n");
        goto error;
    }
    
    input->input_fd = libinput_get_fd(input->libinput);
    
    // Initialize XKB
    input->xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (!input->xkb_context) {
        fprintf(stderr, "Failed to create XKB context\n");
        goto error;
    }
    
    input->xkb_keymap = xkb_keymap_new_from_names(input->xkb_context, NULL, XKB_KEYMAP_COMPILE_NO_FLAGS);
    if (!input->xkb_keymap) {
        fprintf(stderr, "Failed to create XKB keymap\n");
        goto error;
    }
    
    input->xkb_state = xkb_state_new(input->xkb_keymap);
    if (!input->xkb_state) {
        fprintf(stderr, "Failed to create XKB state\n");
        goto error;
    }
    
    printf("Input system initialized successfully\n");
    return 0;
    
error:
    vldwm_input_cleanup(input);
    return -1;
}

void vldwm_input_cleanup(struct vldwm_input *input) {
    if (!input) return;
    
    if (input->xkb_state) {
        xkb_state_unref(input->xkb_state);
    }
    
    if (input->xkb_keymap) {
        xkb_keymap_unref(input->xkb_keymap);
    }
    
    if (input->xkb_context) {
        xkb_context_unref(input->xkb_context);
    }
    
    if (input->libinput) {
        libinput_unref(input->libinput);
    }
    
    if (input->udev) {
        udev_unref(input->udev);
    }
    
    memset(input, 0, sizeof(*input));
}

void vldwm_input_process_events(struct vldwm_input *input) {
    if (!input || !input->libinput) return;
    
    libinput_dispatch(input->libinput);
    
    struct libinput_event *event;
    while ((event = libinput_get_event(input->libinput))) {
        enum libinput_event_type type = libinput_event_get_type(event);
        
        switch (type) {
            case LIBINPUT_EVENT_POINTER_MOTION: {
                struct libinput_event_pointer *pointer_event = libinput_event_get_pointer_event(event);
                double dx = libinput_event_pointer_get_dx(pointer_event);
                double dy = libinput_event_pointer_get_dy(pointer_event);
                
                input->pointer_x += dx;
                input->pointer_y += dy;
                
                // Clamp to screen bounds (assuming 1920x1080 for now)
                if (input->pointer_x < 0) input->pointer_x = 0;
                if (input->pointer_y < 0) input->pointer_y = 0;
                if (input->pointer_x > 1920) input->pointer_x = 1920;
                if (input->pointer_y > 1080) input->pointer_y = 1080;
                
                printf("Pointer: %.2f, %.2f\n", input->pointer_x, input->pointer_y);
                break;
            }
            
            case LIBINPUT_EVENT_KEYBOARD_KEY: {
                struct libinput_event_keyboard *keyboard_event = libinput_event_get_keyboard_event(event);
                uint32_t key = libinput_event_keyboard_get_key(keyboard_event);
                enum libinput_key_state state = libinput_event_keyboard_get_key_state(keyboard_event);
                
                xkb_keysym_t keysym = xkb_state_key_get_one_sym(input->xkb_state, key + 8);
                
                printf("Key %s: %d (keysym: %d)\n", 
                       state == LIBINPUT_KEY_STATE_PRESSED ? "pressed" : "released",
                       key, keysym);
                break;
            }
            
            default:
                break;
        }
        
        libinput_event_destroy(event);
    }
}
