#include <stdio.h>
#include <stdlib.h>
#include "context.h"
#include "client.h"
#include "wl_compositor.h"
#include "xdg_wm_base.h"
#include "wl_shell.h"
#include "wl_seat.h"
#include "wl_output.h"
#include "zxdg_output_manager_v1.h"
#include "zxdg_output_v1.h"
#include "xdg-output-unstable-v1-server-protocol.h"
#include "wl_data_device_manager.h"
#include "surface.h"
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

static void bind_wl_shell(struct wl_client* client, void* data, uint32_t version, uint32_t id)
{
	struct client* new_client = find_client((struct context*)data, client);

	create_wl_shell(client, &new_client->wl_shell_impl, version, id);
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

	new_client->output = create_wl_output(client, &new_client->wl_output_impl, version, id);
	// wl_output_send_geometry(new_client->output, 0, 0, 530, 300, WL_OUTPUT_SUBPIXEL_UNKNOWN, "DEL", "DELL U2414H", WL_OUTPUT_TRANSFORM_NORMAL);
	// wl_output_send_scale(new_client->output, 1);
	wl_output_send_mode(new_client->output, WL_OUTPUT_MODE_CURRENT|WL_OUTPUT_MODE_PREFERRED, 1280, 720, 60000);
}


static void bind_xdg_output_manager(struct wl_client* client, void* data, uint32_t version, uint32_t id)
{
	struct client* new_client = find_client((struct context*)data, client);
	new_client->xdg_output = create_zxdg_output_manager_v1(client, &new_client->xdg_output_manager_impl, version, id);

}


static void bind_wl_data_device_manager(struct wl_client* client, void* data, uint32_t version, uint32_t id)
{
	struct client* new_client = find_client((struct context*)data, client);

	struct wl_resource* data_device_manager = create_wl_data_device_manager(client, &new_client->wl_output_impl, version, id);
}

static void get_xdg_output(struct wl_client *client, struct wl_resource *resource, uint32_t id, struct wl_resource *output)
{
	struct wl_resource* xdg_output = create_zxdg_output_v1(client, NULL, zxdg_output_v1_interface.version, id);

	zxdg_output_v1_send_name(xdg_output, "xdg-output");
	zxdg_output_v1_send_logical_position(xdg_output, 0, 0);
	zxdg_output_v1_send_logical_size(xdg_output, 1280, 720);
	zxdg_output_v1_send_done(xdg_output);
}

struct context* context_create(void)
{
	struct context* context = NULL;

	context = calloc(sizeof(struct context), 1);

	context->display = wl_display_create();
	wl_display_add_socket_auto(context->display);

	wl_global_create(context->display, &wl_compositor_interface, wl_compositor_interface.version, context, bind_wl_compositor);
	wl_global_create(context->display, &wl_seat_interface, wl_seat_interface.version, context, bind_wl_seat);
	// wl_global_create(context->display, &zxdg_output_manager_v1_interface, zxdg_output_manager_v1_interface.version, context, bind_xdg_output_manager);
	wl_global_create(context->display, &wl_output_interface, wl_output_interface.version, context, bind_wl_output);
	wl_global_create(context->display, &wl_data_device_manager_interface, wl_data_device_manager_interface.version, context, bind_wl_data_device_manager);

	wl_global_create(context->display, &xdg_wm_base_interface, xdg_wm_base_interface.version, context, bind_xdg_wm_base);
	// wl_global_create(context->display, &wl_shell_interface, wl_shell_interface.version, context, bind_wl_shell);

	wl_display_init_shm(context->display);
	// wl_display_add_shm_format(context->display, WL_SHM_FORMAT_ABGR8888);
	// wl_display_add_shm_format(context->display, WL_SHM_FORMAT_XBGR8888);

	setup_surface_impl(context);
	setup_seat_impl(context);

	context->xdg_output_impl.xdg_output_manager_impl.get_xdg_output = get_xdg_output;

	return context;
}

