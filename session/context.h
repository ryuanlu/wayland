#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <wayland-server.h>
#include "xdg-shell-server-protocol.h"

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


struct context
{
	struct wl_display*	display;
	struct wl_resource*	seat;

	struct wayland_impl	wayland_impl;
	struct xdg_shell_impl	xdg_shell_impl;

	struct wl_compositor_interface*	wl_compositor_impl;
	struct xdg_wm_base_interface*	xdg_wm_base_impl;
	struct wl_seat_interface*	wl_seat_impl;
	struct wl_data_device_manager_interface*	wl_data_device_manager_impl;
	struct wl_output_interface*	wl_output_impl;
	struct wl_region_interface*	wl_region_impl;

};

struct context* context_create(void);

#endif /* __CONTEXT_H__ */