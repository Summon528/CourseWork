#include <arpa/inet.h>
#include <assert.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned short checksum(unsigned short *buff, int _16bitword) {
    unsigned long sum;
    for (sum = 0; _16bitword > 0; _16bitword--) sum += htons(*(buff)++);
    do {
        sum = ((sum >> 16) + (sum & 0xFFFF));
    } while (sum & 0xFFFF0000);
    return ~sum;
}

int main(int argc, char *argv[]) {
    unsigned char query[] =
        "\x53\xbe\x01\x20\x00\x01\x00\x00\x00\x00\x00\x01\x03\x69\x73\x63"
        "\x03\x6f\x72\x67\x00\x00\x2e\x00\x01\x00\x00\x29\x10\x00\x00\x00"
        "\x00\x00\x00\x0c\x00\x0a\x00\x08\xf7\xdf\x14\x67\x9a\x31\x51\xa6";
    const size_t q_size = sizeof(query) - 1;

    const size_t PACKET_SZ =
        sizeof(struct iphdr) + sizeof(struct udphdr) + q_size;

    unsigned char *sendbuff = (unsigned char *)malloc(PACKET_SZ);
    memset(sendbuff, 0, PACKET_SZ);
    int total_len = 0;

    // Construct IP header
    struct iphdr *iph = (struct iphdr *)(sendbuff);
    iph->ihl = 5;
    iph->version = 4;
    iph->id = htons(11262);
    iph->ttl = 64;
    iph->protocol = 17;  // UDP
    iph->saddr = inet_addr("172.21.148.169");
    iph->daddr = inet_addr("8.8.8.8");
    total_len += sizeof(struct iphdr);

    // Construct UDP header
    struct udphdr *udph = (struct udphdr *)(sendbuff + sizeof(struct iphdr));
    udph->source = htons(23451);
    udph->dest = htons(53);
    udph->check = 0;
    total_len += sizeof(struct udphdr);

    // Put data
    memcpy(sendbuff + total_len, query, q_size);
    total_len += q_size;

    // Fill rest of the header
    udph->len = htons((total_len - sizeof(struct iphdr)));
    iph->tot_len = htons(total_len);
    iph->check =
        htons(checksum((unsigned short *)sendbuff, (sizeof(struct iphdr) / 2)));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "8.8.8.8", &addr.sin_addr);

    int sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    assert(sd != -1);

    int send_len = sendto(sd, sendbuff, PACKET_SZ, 0, (struct sockaddr *)&addr,
                          sizeof(addr));
    assert(send_len >= 0);
}
