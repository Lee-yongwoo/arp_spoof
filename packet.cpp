#include "packet.h"
#include "mac.h"

void PACKET::set_dmac(MAC mac) { eth.set_dmac(mac); }

void PACKET::set_smac(MAC mac) { eth.set_smac(mac); }

int PACKET::is_dmac(MAC mac) { return eth.is_dmac(mac); }

int PACKET::is_smac(MAC mac) { return eth.is_smac(mac); }

int PACKET::is_arp_request() { return eth.is_arp() && arp.is_request(); }

int PACKET::is_arp_reply() { return eth.is_arp() && arp.is_reply(); }

int PACKET::is_ipv4() { return eth.is_ipv4(); }

int PACKET::is_sender_mac(MAC mac) { return arp.is_sender_mac(mac); }

int PACKET::is_target_mac(MAC mac) { return arp.is_target_mac(mac); }

int PACKET::is_sender_ip(uint32_t ip) { return arp.is_sender_ip(ip); }

int PACKET::is_target_ip(uint32_t ip) { return arp.is_target_ip(ip); }

MAC PACKET::get_dmac() { return eth.get_dmac(); }

MAC PACKET::get_smac() { return eth.get_smac(); }

MAC PACKET::get_sender_mac() { return arp.get_sender_mac(); }

MAC PACKET::get_target_mac() { return arp.get_target_mac(); }

uint32_t PACKET::get_sender_ip() { return arp.get_sender_ip(); }

uint32_t PACKET::get_target_ip() { return arp.get_target_ip(); }

void PACKET::print() {
	printf("--ETH--\n");
	eth.print();
	printf("--ARP--\n");
	arp.print();
	printf("-------\n");
}
