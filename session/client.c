#include <stdio.h>
#include <stdlib.h>
#include <wayland-server.h>
#include "context.h"
#include "client.h"


static enum wl_iterator_result iterator(struct wl_resource* resource, void* data)
{
	// fprintf(stderr, "%s:%d\n", wl_resource_get_class(resource), wl_resource_get_id(resource));
	return resource ? WL_ITERATOR_CONTINUE : WL_ITERATOR_STOP;
}


static void client_destroy(struct wl_listener *listener, void *data)
{
	// struct wl_client* client = data;
	struct client* client = wl_container_of(listener, client, destroy);

	wl_client_for_each_resource(client->client, iterator, NULL);

}


struct client* find_client(struct context* context, struct wl_client* client)
{
	int i;

	for(i = 0;i < MAX_CLIENTS;++i)
	{
		if(context->clients[i] && context->clients[i]->client == client)
		{
			// fprintf(stderr, "%s: %p\n", __FUNCTION__, context->clients[i]);
			return context->clients[i];
		}
	}

	for(i = 0;i < MAX_CLIENTS;++i)
	{
		if(!context->clients[i])
		{
			context->clients[i] = calloc(sizeof(struct client), 1);
			context->clients[i]->client = client;
			context->clients[i]->context = context;
			context->clients[i]->wl_compositor_impl = &context->wayland_impl.wl_compositor_impl;
			context->clients[i]->wl_data_device_manager_impl = &context->wayland_impl.wl_data_device_manager_impl;
			context->clients[i]->wl_seat_impl = &context->wayland_impl.wl_seat_impl;
			context->clients[i]->wl_output_impl = &context->wayland_impl.wl_output_impl;
			context->clients[i]->xdg_output_manager_impl = &context->xdg_output_impl.xdg_output_manager_impl;
			context->clients[i]->xdg_output_impl = &context->xdg_output_impl.xdg_output_impl;
			context->clients[i]->xdg_wm_base_impl = &context->xdg_shell_impl.xdg_wm_base_impl;
			context->clients[i]->destroy.notify = client_destroy;
			wl_client_add_destroy_listener(client, &context->clients[i]->destroy);
			return context->clients[i];
		}
	}

	return NULL;
}
