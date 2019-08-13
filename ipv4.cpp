#include "ipv4.h"

int IPV4::is_sip(uint32_t ip) { return sip == ip; }

int IPV4::is_dip(uint32_t ip) { return dip == ip; }
