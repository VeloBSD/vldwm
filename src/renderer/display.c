#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

#include "vldwm/display.h"

static const char *drm_device_paths[] = {
    "/dev/dri/card0",
    "/dev/dri/card1",
    "/dev/dri/card2",
    NULL
};

int vldwm_display_init(struct vldwm_display *display) {
    if (!display) return -1;
    
    memset(display, 0, sizeof(*display));
    
    // Open DRM device
    for (int i = 0; drm_device_paths[i]; i++) {
        display->drm_fd = open(drm_device_paths[i], O_RDWR | O_CLOEXEC);
        if (display->drm_fd >= 0) {
            printf("Opened DRM device: %s\n", drm_device_paths[i]);
            break;
        }
    }
    
    if (display->drm_fd < 0) {
        perror("Failed to open DRM device");
        return -1;
    }
    
    // Get DRM resources
    display->resources = drmModeGetResources(display->drm_fd);
    if (!display->resources) {
        perror("drmModeGetResources");
        goto error;
    }
    
    // Find connected connector
    for (int i = 0; i < display->resources->count_connectors; i++) {
        display->connector = drmModeGetConnector(display->drm_fd, display->resources->connectors[i]);
        if (display->connector && display->connector->connection == DRM_MODE_CONNECTED) {
            break;
        }
        if (display->connector) {
            drmModeFreeConnector(display->connector);
            display->connector = NULL;
        }
    }
    
    if (!display->connector) {
        fprintf(stderr, "No connected display found\n");
        goto error;
    }
    
    // Get mode
    if (display->connector->count_modes > 0) {
        drmModeModeInfo *mode = &display->connector->modes[0];
        display->width = mode->hdisplay;
        display->height = mode->vdisplay;
        display->refresh_rate = mode->vrefresh;
        printf("Display mode: %dx%d@%dHz\n", display->width, display->height, display->refresh_rate);
    }
    
    // Get encoder
    display->encoder = drmModeGetEncoder(display->drm_fd, display->connector->encoder_id);
    if (!display->encoder) {
        fprintf(stderr, "Failed to get encoder\n");
        goto error;
    }
    
    // Get CRTC
    display->crtc = drmModeGetCrtc(display->drm_fd, display->encoder->crtc_id);
    if (!display->crtc) {
        fprintf(stderr, "Failed to get CRTC\n");
        goto error;
    }
    
    // Initialize GBM
    display->gbm_device = gbm_create_device(display->drm_fd);
    if (!display->gbm_device) {
        fprintf(stderr, "Failed to create GBM device\n");
        goto error;
    }
    
    // Initialize EGL
    display->egl_display = eglGetDisplay(display->gbm_device);
    if (display->egl_display == EGL_NO_DISPLAY) {
        fprintf(stderr, "Failed to get EGL display\n");
        goto error;
    }
    
    if (!eglInitialize(display->egl_display, NULL, NULL)) {
        fprintf(stderr, "Failed to initialize EGL\n");
        goto error;
    }
    
    printf("Display initialized successfully\n");
    return 0;
    
error:
    vldwm_display_cleanup(display);
    return -1;
}

void vldwm_display_cleanup(struct vldwm_display *display) {
    if (!display) return;
    
    if (display->egl_display != EGL_NO_DISPLAY) {
        eglTerminate(display->egl_display);
    }
    
    if (display->gbm_device) {
        gbm_device_destroy(display->gbm_device);
    }
    
    if (display->crtc) {
        drmModeFreeCrtc(display->crtc);
    }
    
    if (display->encoder) {
        drmModeFreeEncoder(display->encoder);
    }
    
    if (display->connector) {
        drmModeFreeConnector(display->connector);
    }
    
    if (display->resources) {
        drmModeFreeResources(display->resources);
    }
    
    if (display->drm_fd >= 0) {
        close(display->drm_fd);
    }
    
    memset(display, 0, sizeof(*display));
}

int vldwm_display_page_flip(struct vldwm_display *display) {
    if (!display) return -1;
    
    // TODO: Implement actual rendering and page flipping
    // For now, just return success
    return 0;
}
