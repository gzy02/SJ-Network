#pragma once
#define MAC_SIZE 18
#define IP_SIZE 16

extern int get_ip_by_domain(const char *domain, char *ip); // ����������ȡip
extern int get_local_mac(const char *eth_inf, char *mac);  // ��ȡ����mac
extern int get_local_ip(const char *eth_inf, char *ip);    // ��ȡ����ip
