#ifndef __SURFACE_H__
#define __SURFACE_H__

#include "context.h"

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
	struct wl_region_interface*	wl_region_impl;
	struct xdg_surface_interface*	xdg_surface_impl;
	struct xdg_toplevel_interface*	xdg_toplevel_impl;
	struct xdg_popup_interface*	xdg_popup_impl;
};


void setup_surface_impl(struct context* context);

#endif /* __SURFACE_H__ */