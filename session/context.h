#include <wayland-server.h>
#include "xdg-shell-server-protocol.h"

struct wayland_impl
{
	struct wl_compositor_interface	wl_compositor_impl;
	struct wl_surface_interface	wl_surface_impl;
	struct wl_region_interface	wl_region_impl;
};

struct xdg_shell_impl
{
	struct xdg_wm_base_interface	xdg_wm_base_impl;
	struct xdg_surface_interface	xdg_surface_impl;
	struct xdg_toplevel_interface	xdg_toplevel_impl;
	struct xdg_popup_interface	xdg_popup_impl;
};

struct surface
{
	struct wl_resource*	wl_surface;
	struct wl_resource*	xdg_surface;
	struct wl_resource*	xdg_toplevel;
	struct wl_resource*	xdg_popup;

	int	width;
	int	height;
	int	x, y;

	struct wl_surface_interface*	wl_surface_impl;
	struct xdg_surface_interface*	xdg_surface_impl;
	struct xdg_toplevel_interface*	xdg_toplevel_impl;
	struct xdg_popup_interface*	xdg_popup_impl;
};


struct context
{
	struct wl_display*	display;

	struct wayland_impl	wayland_impl;
	struct xdg_shell_impl	xdg_shell_impl;


	struct wl_compositor_interface*	wl_compositor_impl;
	struct xdg_wm_base_interface*	xdg_wm_base_impl;


	struct wl_region_interface*	wl_region_impl;

};

struct context* context_create(void);

