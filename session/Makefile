include ../build.mk

TARGET:=session

PKG_CONFIG_PACKAGES:=wayland-server

WAYLAND_xml_file:=/usr/share/wayland/wayland.xml
WAYLAND_src_files:=wl_compositor.c wl_data_offer.c wl_data_source.c wl_data_device.c wl_data_device_manager.c wl_surface.c wl_seat.c wl_pointer.c wl_keyboard.c wl_touch.c wl_output.c wl_region.c wl_subcompositor.c wl_subsurface.c

XDG_SHELL_xml_file:=/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml
XDG_SHELL_src_files:=xdg_wm_base.c xdg_surface.c xdg_toplevel.c xdg_popup.c

$(TARGET)_src_files:=main.c context.c surface.c seat.c xdg-shell-protocol.c $(WAYLAND_src_files) $(XDG_SHELL_src_files)
$(TARGET)_cflags:=$(shell pkg-config --cflags $(PKG_CONFIG_PACKAGES))
$(TARGET)_ldflags:=$(shell pkg-config --libs $(PKG_CONFIG_PACKAGES))

$(eval $(call define_protocol_impl_targets,WAYLAND))
$(eval $(call define_protocol_impl_targets,XDG_SHELL))
$(TARGET)_extra_deps:=xdg-shell-server-protocol.h $(WAYLAND_headers) $(XDG_SHELL_headers)

$(eval $(call define_c_targets,$(TARGET)))
$(eval $(call define_protocol_targets,xdg-shell))

all: $(TARGET)

clean: $(TARGET)_clean xdg-shell_clean WAYLAND_clean XDG_SHELL_clean

