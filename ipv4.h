#pragma once

#include <stdint.h>

class IPV4 {
private:
	uint32_t dummy[3];
	uint32_t sip;
	uint32_t dip;
public:
	IPV4() {}
	int is_sip(uint32_t ip);
	int is_dip(uint32_t ip);
};
