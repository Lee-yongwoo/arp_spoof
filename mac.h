#include <stdio.h>
#include <stdint.h>
#include <string.h>

#pragma once

class MAC {
private:
	uint8_t addr[6];
public:
	MAC() { memset(addr, 0x00, 6); }
	MAC(int x) { memset(addr, 0xff, 6); }
	MAC(uint8_t *_addr) { memcpy(addr, _addr, 6); }
	void print();
	void operator=(const MAC &m);
	int operator==(const MAC &m);
};
