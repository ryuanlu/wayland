#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <wayland-server.h>

struct client
{
	struct context*		context;
	struct wl_client*	client;
	struct wl_resource*	seat;

	struct wl_listener	destroy;

	struct wl_compositor_interface*			wl_compositor_impl;
	struct xdg_wm_base_interface*			xdg_wm_base_impl;
	struct wl_seat_interface*			wl_seat_impl;
	struct wl_data_device_manager_interface*	wl_data_device_manager_impl;
	struct wl_output_interface*			wl_output_impl;
	struct wl_region_interface*			wl_region_impl;
};


struct client* find_client(struct context* context, struct wl_client* client);


#endif /* __CLIENT_H__ */