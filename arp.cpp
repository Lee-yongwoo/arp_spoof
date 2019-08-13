#include "mac.h"
#include "arp.h"

void print_ip(uint32_t ip) {
	printf("%d.%d.%d.%d\n", ip & 0xff, (ip>>8) & 0xff, (ip>>16) & 0xff, ip>>24);
}

int ARP::is_request() { return ntohs(opcode) == 1; }

int ARP::is_reply() { return ntohs(opcode) == 2; }

int ARP::is_sender_mac(MAC mac) { return sender_mac == mac; }

int ARP::is_target_mac(MAC mac) { return target_mac == mac; }

int ARP::is_sender_ip(uint32_t ip) { return sender_ip == ip; }

int ARP::is_target_ip(uint32_t ip) { return target_ip == ip; }

MAC ARP::get_sender_mac() { return sender_mac; }

MAC ARP::get_target_mac() { return target_mac; }

uint32_t ARP::get_sender_ip() { return sender_ip; }

uint32_t ARP::get_target_ip() { return target_ip; }

void ARP::print() {
	sender_mac.print();
	target_mac.print();
	print_ip(sender_ip);
	print_ip(target_ip);
}
