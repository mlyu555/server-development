// 实现UDP的用户协议栈

// 以太网协议头
struct ethhdr {
    unsigned char src_mac[6];
    unsigned char dst_mac[6];
    unsigned short type;
};

struct ethtail
{
    unsigned int checksum;
};


struct ip_header_s {
    unsigned char version:4,
                    header_len:4;

    unsigned char tos;
    unsigned short total_len;

    unsigned short id;
    unsigned short flag:3,
                    pos:13;

    unsigned char ttl;
    unsigned char protocol;
    unsigned short checksum;

    unsigned int sip;
    unsigned int dip;
};

struct udp_header_s
{
    unsigned short sport;
    unsigned short dport;
    unsigned short length;
    unsigned short checksum;
};

struct tcpheader {
    unsigned short sport;
    unsigned short dport;
    unsigned int seqno;
    unsigned int ackno;

    unsigned short header_len:4,
                remain:4,
                flag:8;

    unsigned short wndsize;
    unsigned short checksum;
    unsigned short urgptr;
};

struct udp_packet_s {
    struct eth_header_s ethhdr;
    struct ip_header_s iphdr;
    struct udp_header_s udphdr;

    // playload 有效负载，即有效数据
    unsigned char playload[0];  // 为什么使用柔性数组，而不使用指针或普通数组(用户数据长度不确定但可计算——从udphdr中; 指针造成内存不连续，首部和包体不连续)
};
