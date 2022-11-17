#include <stdio.h>
#include <stdlib.h>
#include "context.h"
#include "client.h"
#include "wl_compositor.h"
#include "xdg_wm_base.h"
#include "surface.h"
#include "wl_seat.h"
#include "wl_output.h"
#include "wl_data_device_manager.h"
#include "seat.h"

static void bind_wl_compositor(struct wl_client* client, void* data, uint32_t version, uint32_t id)
{
	struct client* new_client = find_client((struct context*)data, client);

	create_wl_compositor(client, &new_client->wl_compositor_impl, version, id);
}

static void bind_xdg_wm_base(struct wl_client* client, void* data, uint32_t version, uint32_t id)
{
	struct client* new_client = find_client((struct context*)data, client);

	create_xdg_wm_base(client, &new_client->xdg_wm_base_impl, version, id);
}

static void bind_wl_seat(struct wl_client* client, void* data, uint32_t version, uint32_t id)
{
	struct client* new_client = find_client((struct context*)data, client);

	new_client->seat = create_wl_seat(client, &new_client->wl_seat_impl, version, id);
	wl_seat_send_capabilities(new_client->seat, WL_SEAT_CAPABILITY_POINTER|WL_SEAT_CAPABILITY_KEYBOARD|WL_SEAT_CAPABILITY_TOUCH);

}

static void bind_wl_output(struct wl_client* client, void* data, uint32_t version, uint32_t id)
{
	struct client* new_client = find_client((struct context*)data, client);

	create_wl_output(client, &new_client->wl_output_impl, version, id);
}

static void bind_wl_data_device_manager(struct wl_client* client, void* data, uint32_t version, uint32_t id)
{
	struct client* new_client = find_client((struct context*)data, client);

	create_wl_data_device_manager(client, &new_client->wl_output_impl, version, id);
}

struct context* context_create(void)
{
	struct context* context = NULL;

	context = calloc(sizeof(struct context), 1);

	context->display = wl_display_create();
	wl_display_add_socket_auto(context->display);

	wl_global_create(context->display, &wl_compositor_interface, wl_compositor_interface.version, context, bind_wl_compositor);
	wl_global_create(context->display, &wl_seat_interface, wl_seat_interface.version, context, bind_wl_seat);
	wl_global_create(context->display, &wl_output_interface, wl_output_interface.version, context, bind_wl_output);
	wl_global_create(context->display, &wl_data_device_manager_interface, wl_data_device_manager_interface.version, context, bind_wl_data_device_manager);

	wl_global_create(context->display, &xdg_wm_base_interface, xdg_wm_base_interface.version, context, bind_xdg_wm_base);

	wl_display_init_shm(context->display);

	setup_surface_impl(context);
	setup_seat_impl(context);

	return context;
}

