#include <stdio.h>
#include <stdlib.h>
#include <wayland-server.h>
#include "wl_surface.h"
#include "xdg_surface.h"
#include "xdg_toplevel.h"
#include "xdg_popup.h"
#include "surface.h"
#include "client.h"


static void create_surface(struct wl_client* client, struct wl_resource* resource, uint32_t id)
{
	struct wl_compositor_interface** wl_compositor_impl = wl_resource_get_user_data(resource);
	struct surface* surface = calloc(sizeof(struct surface), 1);
	struct client* new_client = wl_container_of(wl_compositor_impl, new_client, wl_compositor_impl);
	surface->wl_surface_impl = &new_client->context->wayland_impl.wl_surface_impl;
	surface->wl_surface = create_wl_surface(client, &surface->wl_surface_impl, wl_surface_interface.version, id);
	fprintf(stderr, "New surface: %p\n", surface->wl_surface);
}

static void get_xdg_surface(struct wl_client* client, struct wl_resource* resource, uint32_t id, struct wl_resource* wl_surface)
{
	struct xdg_wm_base_interface** xdg_wm_base_impl = wl_resource_get_user_data(resource);
	struct wl_surface_interface** wl_surface_impl = wl_resource_get_user_data(wl_surface);
	struct surface* surface = wl_container_of(wl_surface_impl, surface, wl_surface_impl);
	struct client* new_client = wl_container_of(xdg_wm_base_impl, new_client, xdg_wm_base_impl);

	surface->xdg_surface_impl = &new_client->context->xdg_shell_impl.xdg_surface_impl;
	surface->xdg_toplevel_impl = &new_client->context->xdg_shell_impl.xdg_toplevel_impl;
	surface->xdg_surface = create_xdg_surface(client, &surface->xdg_surface_impl, xdg_surface_interface.version, id);

}

static void get_toplevel(struct wl_client* client, struct wl_resource* resource, uint32_t id)
{
	struct xdg_surface_interface** xdg_surface_impl = wl_resource_get_user_data(resource);
	struct surface* surface = wl_container_of(xdg_surface_impl, surface, xdg_surface_impl);
	surface->xdg_toplevel = create_xdg_toplevel(client, &surface->xdg_toplevel_impl, xdg_toplevel_interface.version, id);

}

static void get_popup(struct wl_client* client, struct wl_resource* resource, uint32_t id, struct wl_resource* parent, struct wl_resource* positioner)
{
	struct xdg_surface_interface** xdg_surface_impl = wl_resource_get_user_data(resource);
	struct surface* surface = wl_container_of(xdg_surface_impl, surface, xdg_surface_impl);
	surface->xdg_popup = create_xdg_popup(client, &surface->xdg_popup_impl, xdg_popup_interface.version, id);

}

static void attach(struct wl_client *client, struct wl_resource *resource, struct wl_resource *buffer, int32_t x, int32_t y)
{

}

static void destroy_surface(struct wl_client *client, struct wl_resource *resource)
{
	struct wl_surface_interface** wl_surface_impl = wl_resource_get_user_data(resource);
	struct surface* surface = wl_container_of(wl_surface_impl, surface, wl_surface_impl);

	fprintf(stderr, "Destroy surface: %p\n", surface->wl_surface);
}

static void commit(struct wl_client *client, struct wl_resource *resource)
{
	struct wl_surface_interface** wl_surface_impl = wl_resource_get_user_data(resource);
	struct surface* surface = wl_container_of(wl_surface_impl, surface, wl_surface_impl);

	struct wl_array array;
	wl_array_init(&array);
	int32_t *states = wl_array_add (&array, sizeof(int32_t));
	states[0] = XDG_TOPLEVEL_STATE_ACTIVATED;
	xdg_toplevel_send_configure(surface->xdg_toplevel, 1280, 720, &array);
	xdg_surface_send_configure(surface->xdg_surface, 0);
}


static void ack_configure(struct wl_client *client, struct wl_resource *resource, uint32_t serial)
{

}

void setup_surface_impl(struct context* context)
{
	context->wayland_impl.wl_compositor_impl.create_surface = create_surface;
	context->wayland_impl.wl_surface_impl.destroy = destroy_surface;
	context->wayland_impl.wl_surface_impl.commit = commit;

	context->xdg_shell_impl.xdg_wm_base_impl.get_xdg_surface = get_xdg_surface;
	context->xdg_shell_impl.xdg_surface_impl.get_toplevel = get_toplevel;
	context->xdg_shell_impl.xdg_surface_impl.get_popup = get_popup;
	context->xdg_shell_impl.xdg_surface_impl.ack_configure = ack_configure;
}