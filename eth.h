#pragma once

#include <arpa/inet.h>
#include <stdint.h>
#include "mac.h"

#pragma pack(1)
class ETHERNET {
private:
	MAC dmac;
	MAC smac;
	uint16_t type;
public:
	ETHERNET() {}
	ETHERNET(MAC _dmac, MAC _smac, uint16_t _type): 
		dmac(_dmac), smac(_smac), type(htons(_type)) {}
	
	void set_dmac(MAC mac);
	void set_smac(MAC mac);
	
	int is_dmac(MAC mac);
	int is_smac(MAC mac);
	int is_arp();
	int is_ipv4();

	MAC get_dmac();
	MAC get_smac();

	void print();
};
