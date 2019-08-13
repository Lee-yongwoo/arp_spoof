#pragma once

#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>
#include "mac.h"

#pragma pack(1)
class ARP {
public:
	uint16_t hdw_type;
	uint16_t pro_type;
	uint8_t  hdw_len;
	uint8_t  pro_len;
	uint16_t opcode;
	MAC sender_mac;
	uint32_t sender_ip;
	MAC target_mac;
	uint32_t target_ip;
//public:
	ARP() {}
	ARP(uint16_t _opcode, MAC _sender_mac, uint32_t _sender_ip, MAC _target_mac, uint32_t _target_ip):
		hdw_type(htons(0x0001)),
		pro_type(htons(0x0800)),
		hdw_len(0x06),
		pro_len(0x04),
		opcode(htons(_opcode)),
		sender_mac(_sender_mac),
		sender_ip(_sender_ip),
		target_mac(_target_mac),
		target_ip(_target_ip)
		{}

	int is_request();
	int is_reply();
	
	int is_sender_mac(MAC mac);
	int is_target_mac(MAC mac);
	int is_sender_ip(uint32_t ip);
	int is_target_ip(uint32_t ip);
	
	MAC get_sender_mac();
	MAC get_target_mac();
	uint32_t get_sender_ip();
	uint32_t get_target_ip();
	void print();
};
