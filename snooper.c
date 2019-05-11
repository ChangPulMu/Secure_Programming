/*snooper.c
 *
 * Copyright (c) 2000 Sean Walton and Macmillan Publishers. Use may be in
 * whole or in part in accordance to the General Public License(GPL).
 *
 */

#include <stdio.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <linux/if_ether.h>
#include <stdlib.h>

#define IP_SIZE		4
#define ETH_SIZE	6

typedef enum { eETH_ADDR, eIP_ADDR } EAddress;

typedef unsigned char uchar;

struct ip_packet {
	struct {
		uchar dst_eth[ETH_SIZE];
		uchar src_eth[ETH_SIZE];
		uchar __unknwn[2];
	} hw_header;			/* hardware header */
	uint header_len:4;		/* header length in words in 32bit words*/
	uint version:4;			/* 4-bit version */
	uint serve_type:8;		/* how to service packet */
	uint packet_len :16;		/* total size of packet in bytes */
	uint ID:16;			/* fragment ID */
	uint frag_offset:13;		/* to help reassembly */
	uint more_frags:1;		/* flag for "more frags to follow" */
	uint dont_frag:1;		/* flag to permit fragmentation */
	uint __reserved:1;		/* always zero */
	uint time_to_live:8;		/* maximum router hop count */
	uint protocol:8;		/* ICMP, UDP, TCP */
	uint hdr_chksum:16;		/* ones-comp. checksum of header */
	uchar IPv4_src[IP_SIZE];	/* IP address of originator */
	uchar IPv4_dst[IP_SIZE];	/* IP address of destination */
	uchar options[0];		/* up to 40 bytes */
	uchar data[0];			/* message data up to 64KB */
};

void dump(void* b, int len)
{
	unsigned char *buf = b;
	int i=0;

	printf("----------------capture %d byte------------------\n",len);

	for(i=0;i<len;++i){
		printf("%2x ", *buf);
		buf++;
		if(i%25 == 24)
			printf("\n");
	}
}

void PrintAddr(char* msg, uchar *addr, EAddress is_ip)
{
	int i;
	static struct {
		int len;
		char *fmt;
		char delim;
	} addr_fmt[] = {{ETH_SIZE, "%x", ':'}, {IP_SIZE, "%d", '.'}};

	printf("%s", msg);

	for(i=0;i<addr_fmt[is_ip].len;i++)
	{
		printf(addr_fmt[is_ip].fmt, addr[i]);
		if(i<addr_fmt[is_ip].len-1)
			putchar(addr_fmt[is_ip].delim);
	}
}

char* GetProtocol(int value)
{
	switch (value)
	{
		case IPPROTO_IP: return "IP";
		case IPPROTO_ICMP: return "ICMP";
		case IPPROTO_IGMP: return "IGMP";
		case IPPROTO_IPIP: return "IPIP";
		case IPPROTO_TCP: return "TCP";
		case IPPROTO_EGP: return "EGP";
		case IPPROTO_PUP: return "PUP";
		case IPPROTO_UDP: return "UDP";
		case IPPROTO_RSVP: return "RSVP";
		case IPPROTO_GRE: return "GRE";
		case IPPROTO_IPV6: return "IPV6/4";
		case IPPROTO_PIM: return "PIM";
		case IPPROTO_RAW: return "RAW";
		default: return "???";
	}
}

void DumpPacket(char* buffer, int len)
{
	struct ip_packet *ip=(void*)buffer;

	printf("-----------------------------------------\n");

	dump(buffer, len);

	PrintAddr("\nDestination EtherID=", ip->hw_header.dst_eth, eETH_ADDR);
	PrintAddr(", Source EtherID=", ip->hw_header.src_eth, eETH_ADDR);

	printf("\nIPv%d: header-len=%d, type=%d, packet-size=%d, ID=%d\n",
			ip->version, ip->header_len*4, ip->serve_type,
			ntohs(ip->packet_len), ntohs(ip->ID));
	printf("frag=%c, more=%c, offset=%d, TTL=%d, protocol=%s\n",
			(ip->dont_frag? 'N': 'Y'),
			(ip->more_frags? 'N': 'Y'),
			ip->frag_offset,
			ip->time_to_live, GetProtocol(ip->protocol));
	printf("checksum=%d, ", ntohs(ip->hdr_chksum));
	
	PrintAddr("source=", ip->IPv4_src, eIP_ADDR);
	PrintAddr(", destination=", ip->IPv4_dst, eIP_ADDR);
	
	printf("\n");
	fflush(stdout);
}

void PANIC(char *msg);
#define PANIC(msg) {perror(msg);exit(0);}

int main()
{
	int sd, bytes_read;
	char data[1024];

	sd = socket(PF_INET, SOCK_PACKET, htons(ETH_P_ALL));

	if(sd<0)
		PANIC("Snooper socket");
	do
	{
		bytes_read = recvfrom(sd, data, sizeof(data), 0, 0, 0);
		if(bytes_read>0)
			DumpPacket(data, bytes_read);
	}
	while (bytes_read>0);

	return 0;
}
