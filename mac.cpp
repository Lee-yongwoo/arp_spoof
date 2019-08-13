#include "mac.h"

void MAC::operator=(const MAC &m) { memcpy(addr, m.addr, 6); }

int MAC::operator==(const MAC &m) { return !memcmp(addr, m.addr, 6); }

void MAC::print() {
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n", addr[0], addr[1], addr[2], addr[3], addr[4] ,addr[5]);
}
