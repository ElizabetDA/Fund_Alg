#pragma once

#include <cstddef>
#include <cstdint>
#include <netinet/in.h>

struct tcp_traffic_pkg {
	in_port_t  src_port;
	in_addr_t  dst_addr;
	in_port_t  dst_port;
	size_t     sz;
	enum class Event { CONNECT, SEND, RECV, DISCONNECT, STOP } ev;
};
