.SUFFIXES:
.ONESHELL:
.DEFAULT_GOAL:=all

define define_c_to_o_targets

$(1)_obj_files:=$$(foreach obj,$$($(1)_src_files:.c=.o),$$(obj))
$(1)_dep_files:=$$($(1)_obj_files:.o=.o.d)

$$($(1)_obj_files): %.o: %.c | $$($(1)_extra_deps)
	@printf "\tCC\t$$@\n"
	$$(CC) -MMD -MF $$@.d -c $$< -o $$@ $$($(1)_cflags)

$(1)_obj_clean:
	@rm -f $$($(1)_obj_files) $$($(1)_dep_files)

sinclude $$($(1)_dep_files)

endef


define define_c_targets

$(1)_resource_obj_files:=$$(foreach obj,$$($(1)_resource_files:%=%.o),$$(obj))

$(eval $(call define_c_to_o_targets,$(1)))

$$($(1)_resource_obj_files): %.o: %
	@printf "\tLD\t$$@\n"
	$$(LD) -r -b binary $$< -o $$@

$(1): $$($(1)_obj_files) $$($(1)_extra_obj_files) $$($(1)_resource_obj_files)
	@printf "\tLD\t$$@\n"
	$$(CC) $$^ -o $$@ $$($(1)_ldflags)
	@printf "\tDEBUG\t$$@\n"
	objcopy --only-keep-debug $$@ $$@.debug
	strip --strip-debug $$@
	objcopy --add-gnu-debuglink $$@.debug $$@

$(1)_clean: $(1)_obj_clean
	@printf "\tCLEAN\t$(1)\n"
	rm -f $$($(1)_resource_obj_files) $(1) $(1).debug

endef


define define_protocol_targets

$(1)-protocol.c: /usr/share/wayland-protocols/stable/$(1)/$(1).xml | $(1)-server-protocol.h
	@printf "\tWAYLAND_SCANNER\tPRIVATE_CODE\t$$@\t\n"
	wayland-scanner private-code $$< $$@

$(1)-server-protocol.h: /usr/share/wayland-protocols/stable/$(1)/$(1).xml
	@printf "\tWAYLAND_SCANNER\tSERVER_HEADER\t$$@\t\n"
	wayland-scanner server-header $$< $$@

$(1)_clean:
	@printf "\tCLEAN\t$(1)\n"
	rm -f $(1)-protocol.c $(1)-server-protocol.h
endef


define define_protocol_impl_targets

$(1)_headers:=$$($(1)_src_files:.c=.h)

$$($(1)_src_files): %.c:
	@printf "\tGEN\t$$@\n"
	../extract.sh source $$($(1)_xml_file) $$$$(printf "$$@"|cut -d. -f1)

$$($(1)_headers): %.h:
	@printf "\tGEN\t$$@\n"
	../extract.sh header $$($(1)_xml_file) $$$$(printf "$$@"|cut -d. -f1)

$(1)_clean:
	@printf "\tCLEAN\t$(1)\n"
	rm -f $$($(1)_src_files) $$($(1)_headers)
endef