#!/bin/sh

XMLFILES='/usr/share/wayland/wayland.xml
/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml
'

get_type ()
{
	case "$1" in
	"int"|"fd")
		printf "int32_t"
	;;
	"uint"|"new_id")
		printf "uint32_t"
	;;
	"string")
		printf "const char*"
	;;
	"object")
		printf "struct wl_resource*"
	;;
	"")
	;;
	esac
}


gen_request_function ()
{
	INTERFACE=$1
	REQUEST=$2
	shift 2

	printf "static void ${REQUEST}(struct wl_client* client, struct wl_resource* resource"

	ARGS=""

	while [ "$#" != "0" ]
	do
		printf ", $(get_type $1) $2"
		ARGS="${ARGS}, $2"
		shift 2
	done

	printf ")\n{\n"

	printf "\tstruct ${INTERFACE}_interface** impl = wl_resource_get_user_data(resource);
\tfprintf(stderr, \"%%s: %%s client = %%p, resource = %%p\\\\n\", __FILE__, __PRETTY_FUNCTION__, client, resource);\n
\tif((*impl)->${REQUEST})
\t\t(*impl)->${REQUEST}(client, resource${ARGS});\n"

	printf "}\n\n"
}


gen_src ()
{
	XMLFILE=$1
	INTERFACE=$2
	(
		printf "#include <stdio.h>\n#include <wayland-server.h>\n"

		[ "$(basename ${XMLFILE})" != "wayland.xml" ] && printf "#include \"$(basename ${XMLFILE} | cut -d. -f1)-server-protocol.h\"\n"

		printf "\n"

		REQUESTS=$(xmllint --xpath "//interface[@name='${INTERFACE}']/request/@name" ${XMLFILE} 2>/dev/null | cut -d\" -f 2)

		for REQUEST in ${REQUESTS}
		do
			ARGS=$(xmllint --xpath "//interface[@name='${INTERFACE}']/request[@name='${REQUEST}']/arg" ${XMLFILE} 2>/dev/null | awk -F\" '{ printf " %s %s", $4, $2 }')
			gen_request_function ${INTERFACE} ${REQUEST} ${ARGS}
		done

		printf "struct wl_resource* create_${INTERFACE}(struct wl_client* client, void* data, uint32_t version, uint32_t id)\n{\n\tstatic struct ${INTERFACE}_interface ${INTERFACE}_implementation =\n\t{\n"

		for REQUEST in ${REQUESTS}
		do
			printf "\t\t${REQUEST},\n"
		done

		printf "\t};\n\n\tstruct wl_resource *resource = wl_resource_create(client, &${INTERFACE}_interface, version ? version : ${INTERFACE}_interface.version, id);\n\twl_resource_set_implementation(resource, &${INTERFACE}_implementation, data, NULL);\n\treturn resource;\n}\n\n"
	) > ${INTERFACE}.c
}


gen_header ()
{
	XMLFILE=$1
	INTERFACE=$2
	(
		HEADER_NAME=__$(printf ${INTERFACE}| tr '[:lower:]' '[:upper:]')_H__
		printf "#ifndef ${HEADER_NAME}\n#define ${HEADER_NAME}\n\n#include <stdint.h>\nstruct wl_resource * create_${INTERFACE}(struct wl_client* client, void* data, uint32_t version, uint32_t id);\n\n#endif /* ${HEADER_NAME} */\n"
	) > ${INTERFACE}.h
}


list_interface ()
{
	XMLFILE=$1
	xmllint --xpath "//interface/@name" ${XMLFILE} | cut -d\" -f 2
}


case "$1" in
"source")
gen_src $2 $3
;;
"header")
gen_header $2 $3
;;
"list")
list_interface $2
;;
"")
;;
esac


