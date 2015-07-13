#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define PCKT_LEN 8192

struct ipheader {
	unsigned char	  iph_ihl:5, iph_ver:4;
	unsigned char	  iph_tos;
	unsigned short int iph_len;
	unsigned short int iph_ident;
	unsigned char	  iph_flag;
	unsigned short int iph_offset;
	unsigned char	  iph_ttl;
	unsigned char	  iph_protocol;
	unsigned short int iph_chksum;
	unsigned int	   iph_sourceip;
	unsigned int	   iph_destip;
};

struct udpheader {
	unsigned short int udph_srcport;
	unsigned short int udph_destport;
	unsigned short int udph_len;
	unsigned short int udph_chksum;
};


int main()
{
	int sd;
	char buffer[PCKT_LEN];
	// Our own headers' structures
	struct ipheader *ip = (struct ipheader *) buffer;
		struct udpheader *udp = (struct udpheader *) (buffer + sizeof(struct ipheader));
	char message[]= "hello world!\n";
	memcpy((buffer + sizeof(struct ipheader)+sizeof(struct udpheader)), message, sizeof(message));
		// Source and destination addresses: IP and port
	struct sockaddr_in addr;
	int one = 1;
	const int *val = &one;
	memset(buffer, 0, PCKT_LEN);
	sd = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
	if(sd < 0)
	{
		perror("socket() error");
		exit(-1);
	}

	else
		printf("socket() is OK.\n");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	ip->iph_ihl = 5;
	ip->iph_ver = 4;
	ip->iph_tos = 16; // Low delay
	ip->iph_len = sizeof(struct ipheader) + sizeof(struct udpheader);
	ip->iph_ident = htons(54321);
	ip->iph_ttl = 64; // hops
	ip->iph_protocol = 17; // UDP
	// Source IP address, can use spoofed address here!!!
	ip->iph_sourceip = inet_addr("123.123.123.123");
	// The destination IP address
	ip->iph_destip = htonl(INADDR_LOOPBACK);
	// Fabricate the UDP header. Source port number, redundant
	udp->udph_srcport = htons(111);
	// Destination port number
	udp->udph_destport = htons(3425);
	udp->udph_len = htons(sizeof(struct udpheader));
	ip->iph_chksum = 0;
	if(setsockopt(sd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
	{
		perror("setsockopt() error");
		exit(1);
	}
	if(sendto(sd, buffer, ip->iph_len, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("sendto() error");
		exit(1);
	}
	else
		printf("send is OK.\n");
	close(sd);
	return 0;
}
