#include <stdio.h>
#include <wayland-server.h>
#include "context.h"
#include "wl_pointer.h"
#include "wl_keyboard.h"
#include "wl_touch.h"


static void get_pointer(struct wl_client* client, struct wl_resource* resource, uint32_t id)
{
	create_wl_pointer(client, NULL, wl_pointer_interface.version, id);
}

static void get_keyboard(struct wl_client* client, struct wl_resource* resource, uint32_t id)
{
	create_wl_keyboard(client, NULL, wl_keyboard_interface.version, id);
}

static void get_touch(struct wl_client* client, struct wl_resource* resource, uint32_t id)
{
	create_wl_touch(client, NULL, wl_touch_interface.version, id);
}

void setup_seat_impl(struct context* context)
{
	context->wayland_impl.wl_seat_impl.get_pointer = get_pointer;
	context->wayland_impl.wl_seat_impl.get_keyboard = get_keyboard;
	context->wayland_impl.wl_seat_impl.get_touch = get_touch;
}