#include "eth.h"
#include "mac.h"

void ETHERNET::set_dmac(MAC mac) { dmac = mac; }

void ETHERNET::set_smac(MAC mac) { smac = mac; }

int ETHERNET::is_dmac(MAC mac) { return dmac == mac; }

int ETHERNET::is_smac(MAC mac) { return smac == mac; }

int ETHERNET::is_arp() { return ntohs(type) == 0x0806; }

int ETHERNET::is_ipv4() { return ntohs(type) == 0x0800; }

MAC ETHERNET::get_dmac() { return dmac; }

MAC ETHERNET::get_smac() { return smac; }

void ETHERNET::print() {
	dmac.print();
	smac.print();
}
