all : arp_spoof

arp_spoof: main.o mac.o eth.o arp.o ipv4.o packet.o
	g++ -g -o arp_spoof main.o mac.o eth.o arp.o ipv4.o packet.o -lpcap

main.o: mac.h eth.h arp.h ipv4.h packet.h
	g++ -g -c -o main.o main.cpp

mac.o: mac.h
	g++ -g -c -o mac.o mac.cpp 

eth.o: eth.h
	g++ -g -c -o eth.o eth.cpp

arp.o: arp.h
	g++ -g -c -o arp.o arp.cpp

ipv4.o : ipv4.h
	g++ -g -c -o ipv4.o ipv4.cpp

packet.o: packet.h
	g++ -g -c -o packet.o packet.cpp

clean:
	rm -f arp_spoof
	rm -f *.o

