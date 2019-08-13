#pragma once

#include <stdint.h>
#include "eth.h"
#include "arp.h"

static const uint16_t ARP_TYPE = 0x0806;
static const uint16_t REQUEST = 1;
static const uint16_t REPLY = 2;

class PACKET {
public:
	ETHERNET eth;
	ARP arp;
//public:
	PACKET() {}
	
	PACKET(MAC sender_mac, uint32_t sender_ip, MAC target_mac, uint32_t target_ip) {
		eth = ETHERNET(MAC(1), target_mac, ARP_TYPE);
		arp = ARP(REQUEST, target_mac, target_ip, sender_mac, sender_ip);
	}
	
	PACKET(MAC dmac, MAC smac, MAC target_mac, uint32_t target_ip, MAC sender_mac, uint32_t sender_ip) {
		eth = ETHERNET(dmac, smac, ARP_TYPE);
		arp = ARP(REPLY, target_mac, target_ip, sender_mac, sender_ip);
	}

	void set_dmac(MAC mac);
	void set_smac(MAC mac);

	int is_arp_request();
	int is_arp_reply();
	int is_ipv4();
	
	int is_dmac(MAC mac);
	int is_smac(MAC mac);
	int is_sender_mac(MAC mac);
	int is_target_mac(MAC mac);
	int is_sender_ip(uint32_t ip);
	int is_target_ip(uint32_t ip);

	MAC get_dmac();
	MAC get_smac();
	
	MAC get_sender_mac();
	MAC get_target_mac();
	uint32_t get_sender_ip();
	uint32_t get_target_ip();

	void print();
};
