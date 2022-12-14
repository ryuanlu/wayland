#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <wayland-server.h>
#include "xdg-shell-server-protocol.h"
#include "xdg-output-unstable-v1-server-protocol.h"
#include "client.h"

#define MAX_CLIENTS	(128)


struct wayland_impl
{
	struct wl_compositor_interface	wl_compositor_impl;
	struct wl_surface_interface	wl_surface_impl;
	struct wl_region_interface	wl_region_impl;
	struct wl_seat_interface	wl_seat_impl;
	struct wl_output_interface	wl_output_impl;
	struct wl_data_device_manager_interface	wl_data_device_manager_impl;

};

struct xdg_shell_impl
{
	struct xdg_wm_base_interface	xdg_wm_base_impl;
	struct xdg_surface_interface	xdg_surface_impl;
	struct xdg_toplevel_interface	xdg_toplevel_impl;
	struct xdg_popup_interface	xdg_popup_impl;
};

struct xdg_output_impl
{
	struct zxdg_output_manager_v1_interface	xdg_output_manager_impl;
	struct zxdg_output_v1_interface		xdg_output_impl;
};

struct context
{
	struct wl_display*	display;
	struct client*		clients[MAX_CLIENTS];

	struct wayland_impl	wayland_impl;
	struct xdg_shell_impl	xdg_shell_impl;
	struct xdg_output_impl	xdg_output_impl;

};

struct context* context_create(void);

#endif /* __CONTEXT_H__ */