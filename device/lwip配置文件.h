#define LWIP_TIMERS_CUSTOM              0
#define MEMCPY(dst,src,len)             memcpy(dst,src,len)
#define SMEMCPY(dst,src,len)            memcpy(dst,src,len)
#define MEMMOVE(dst,src,len)            memmove(dst,src,len)
#define LWIP_MPU_COMPATIBLE             0
#define LWIP_TCPIP_CORE_LOCKING         1
#define LWIP_TCPIP_CORE_LOCKING_INPUT   0
#define SYS_LIGHTWEIGHT_PROT            1
#define MEM_LIBC_MALLOC                 0
#define MEMP_MEM_MALLOC                 0
#define MEMP_MEM_INIT                   0
#define MEM_ALIGNMENT                   1
#define MEM_SIZE                        1600
#define MEMP_OVERFLOW_CHECK             0
#define MEMP_SANITY_CHECK               0
#define MEM_OVERFLOW_CHECK              0
#define MEM_SANITY_CHECK                0
#define MEM_USE_POOLS                   0
#define MEM_USE_POOLS_TRY_BIGGER_POOL   0
#define MEMP_USE_CUSTOM_POOLS           0
#define LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT 0
#define MEMP_NUM_PBUF                   16
#define MEMP_NUM_RAW_PCB                4
#define MEMP_NUM_UDP_PCB                4
#define MEMP_NUM_TCP_PCB                5
#define MEMP_NUM_TCP_PCB_LISTEN         8
#define MEMP_NUM_TCP_SEG                16
#define MEMP_NUM_ALTCP_PCB              MEMP_NUM_TCP_PCB
#define MEMP_NUM_REASSDATA              5
#define MEMP_NUM_FRAG_PBUF              15
#define MEMP_NUM_ARP_QUEUE              30
#define MEMP_NUM_IGMP_GROUP             8
/**
 * The number of sys timeouts used by the core stack (not apps)
 * The default number of timeouts is calculated here for all enabled modules.
 */
#define LWIP_NUM_SYS_TIMEOUT_INTERNAL   (LWIP_TCP + IP_REASSEMBLY + LWIP_ARP + (2*LWIP_DHCP) + LWIP_AUTOIP + LWIP_IGMP + LWIP_DNS + PPP_NUM_TIMEOUTS + (LWIP_IPV6 * (1 + LWIP_IPV6_REASS + LWIP_IPV6_MLD)))
/**
 * MEMP_NUM_SYS_TIMEOUT: the number of simultaneously active timeouts.
 * The default number of timeouts is calculated here for all enabled modules.
 * The formula expects settings to be either '0' or '1'.
 */
#define MEMP_NUM_SYS_TIMEOUT            LWIP_NUM_SYS_TIMEOUT_INTERNAL
/**
 * MEMP_NUM_NETBUF: the number of struct netbufs.
 * (only needed if you use the sequential API, like api_lib.c)
 */
#define MEMP_NUM_NETBUF                 2
/**
 * MEMP_NUM_NETCONN: the number of struct netconns.
 * (only needed if you use the sequential API, like api_lib.c)
 */
#define MEMP_NUM_NETCONN                4
/**
 * MEMP_NUM_SELECT_CB: the number of struct lwip_select_cb.
 * (Only needed if you have LWIP_MPU_COMPATIBLE==1 and use the socket API.
 * In that case, you need one per thread calling lwip_select.)
 */
#define MEMP_NUM_SELECT_CB              4
/**
 * MEMP_NUM_TCPIP_MSG_API: the number of struct tcpip_msg, which are used
 * for callback/timeout API communication.
 * (only needed if you use tcpip.c)
 */
#define MEMP_NUM_TCPIP_MSG_API          8
/**
 * MEMP_NUM_TCPIP_MSG_INPKT: the number of struct tcpip_msg, which are used
 * for incoming packets.
 * (only needed if you use tcpip.c)
 */
#define MEMP_NUM_TCPIP_MSG_INPKT        8
/**
 * MEMP_NUM_NETDB: the number of concurrently running lwip_addrinfo() calls
 * (before freeing the corresponding memory using lwip_freeaddrinfo()).
 */
#define MEMP_NUM_NETDB                  1
/**
 * MEMP_NUM_LOCALHOSTLIST: the number of host entries in the local host list
 * if DNS_LOCAL_HOSTLIST_IS_DYNAMIC==1.
 */
#define MEMP_NUM_LOCALHOSTLIST          1
/**
 * PBUF_POOL_SIZE: the number of buffers in the pbuf pool.
 */
#define PBUF_POOL_SIZE                  16
/** MEMP_NUM_API_MSG: the number of concurrently active calls to various
 * socket, netconn, and tcpip functions
 */
#define MEMP_NUM_API_MSG                MEMP_NUM_TCPIP_MSG_API
/** MEMP_NUM_DNS_API_MSG: the number of concurrently active calls to netconn_gethostbyname
 */
#define MEMP_NUM_DNS_API_MSG            MEMP_NUM_TCPIP_MSG_API
/** MEMP_NUM_SOCKET_SETGETSOCKOPT_DATA: the number of concurrently active calls
 * to getsockopt/setsockopt
 */
#define MEMP_NUM_SOCKET_SETGETSOCKOPT_DATA MEMP_NUM_TCPIP_MSG_API
/** MEMP_NUM_NETIFAPI_MSG: the number of concurrently active calls to the
 * netifapi functions
 */
#define MEMP_NUM_NETIFAPI_MSG           MEMP_NUM_TCPIP_MSG_API
/**
 * @}
 */
/*
   ---------------------------------
   ---------- ARP options ----------
   ---------------------------------
*/
/**
 * @defgroup lwip_opts_arp ARP
 * @ingroup lwip_opts_ipv4
 * @{
 */
/**
 * LWIP_ARP==1: Enable ARP functionality.
 */
#define LWIP_ARP                        1
/**
 * ARP_TABLE_SIZE: Number of active MAC-IP address pairs cached.
 */
#define ARP_TABLE_SIZE                  10
/** the time an ARP entry stays valid after its last update,
 *  for ARP_TMR_INTERVAL = 1000, this is
 *  (60 * 5) seconds = 5 minutes.
 */
#define ARP_MAXAGE                      300
/**
 * ARP_QUEUEING==1: Multiple outgoing packets are queued during hardware address
 * resolution. By default, only the most recent packet is queued per IP address.
 * This is sufficient for most protocols and mainly reduces TCP connection
 * startup time. Set this to 1 if you know your application sends more than one
 * packet in a row to an IP address that is not in the ARP cache.
 */
#define ARP_QUEUEING                    0
/** The maximum number of packets which may be queued for each
 *  unresolved address by other network layers. Defaults to 3, 0 means disabled.
 *  Old packets are dropped, new packets are queued.
 */
#define ARP_QUEUE_LEN                   3
/**
 * ETHARP_SUPPORT_VLAN==1: support receiving and sending ethernet packets with
 * VLAN header. See the description of LWIP_HOOK_VLAN_CHECK and
 * LWIP_HOOK_VLAN_SET hooks to check/set VLAN headers.
 * Additionally, you can define ETHARP_VLAN_CHECK to an u16_t VLAN ID to check.
 * If ETHARP_VLAN_CHECK is defined, only VLAN-traffic for this VLAN is accepted.
 * If ETHARP_VLAN_CHECK is not defined, all traffic is accepted.
 * Alternatively, define a function/define ETHARP_VLAN_CHECK_FN(eth_hdr, vlan)
 * that returns 1 to accept a packet or 0 to drop a packet.
 */
#define ETHARP_SUPPORT_VLAN             0
/** LWIP_ETHERNET==1: enable ethernet support even though ARP might be disabled
 */
#define LWIP_ETHERNET                   LWIP_ARP
/** ETH_PAD_SIZE: number of bytes added before the ethernet header to ensure
 * alignment of payload after that header. Since the header is 14 bytes long,
 * without this padding e.g. addresses in the IP header will not be aligned
 * on a 32-bit boundary, so setting this to 2 can speed up 32-bit-platforms.
 */
#define ETH_PAD_SIZE                    0
/** ETHARP_SUPPORT_STATIC_ENTRIES==1: enable code to support static ARP table
 * entries (using etharp_add_static_entry/etharp_remove_static_entry).
 */
#define ETHARP_SUPPORT_STATIC_ENTRIES   0
/** ETHARP_TABLE_MATCH_NETIF==1: Match netif for ARP table entries.
 * If disabled, duplicate IP address on multiple netifs are not supported
 * (but this should only occur for AutoIP).
 */
#define ETHARP_TABLE_MATCH_NETIF        !LWIP_SINGLE_NETIF
/**
 * @}
 */
/*
   --------------------------------
   ---------- IP options ----------
   --------------------------------
*/
/**
 * @defgroup lwip_opts_ipv4 IPv4
 * @ingroup lwip_opts
 * @{
 */
/**
 * LWIP_IPV4==1: Enable IPv4
 */
#define LWIP_IPV4                       1
/**
 * IP_FORWARD==1: Enables the ability to forward IP packets across network
 * interfaces. If you are going to run lwIP on a device with only one network
 * interface, define this to 0.
 */
#define IP_FORWARD                      0
/**
 * IP_REASSEMBLY==1: Reassemble incoming fragmented IP packets. Note that
 * this option does not affect outgoing packet sizes, which can be controlled
 * via IP_FRAG.
 */
#define IP_REASSEMBLY                   1
/**
 * IP_FRAG==1: Fragment outgoing IP packets if their size exceeds MTU. Note
 * that this option does not affect incoming packet sizes, which can be
 * controlled via IP_REASSEMBLY.
 */
#define IP_FRAG                         1

#undef IP_FORWARD
#define IP_FORWARD                      0
#undef IP_REASSEMBLY
#define IP_REASSEMBLY                   0
#undef IP_FRAG
#define IP_FRAG                         0
/**
 * IP_OPTIONS_ALLOWED: Defines the behavior for IP options.
 *      IP_OPTIONS_ALLOWED==0: All packets with IP options are dropped.
 *      IP_OPTIONS_ALLOWED==1: IP options are allowed (but not parsed).
 */
#define IP_OPTIONS_ALLOWED              1
/**
 * IP_REASS_MAXAGE: Maximum time (in multiples of IP_TMR_INTERVAL - so seconds, normally)
 * a fragmented IP packet waits for all fragments to arrive. If not all fragments arrived
 * in this time, the whole packet is discarded.
 */
#define IP_REASS_MAXAGE                 15
/**
 * IP_REASS_MAX_PBUFS: Total maximum amount of pbufs waiting to be reassembled.
 * Since the received pbufs are enqueued, be sure to configure
 * PBUF_POOL_SIZE > IP_REASS_MAX_PBUFS so that the stack is still able to receive
 * packets even if the maximum amount of fragments is enqueued for reassembly!
 * When IPv4 *and* IPv6 are enabled, this even changes to
 * (PBUF_POOL_SIZE > 2 * IP_REASS_MAX_PBUFS)!
 */
#define IP_REASS_MAX_PBUFS              10
/**
 * IP_DEFAULT_TTL: Default value for Time-To-Live used by transport layers.
 */
#define IP_DEFAULT_TTL                  255
/**
 * IP_SOF_BROADCAST=1: Use the SOF_BROADCAST field to enable broadcast
 * filter per pcb on udp and raw send operations. To enable broadcast filter
 * on recv operations, you also have to set IP_SOF_BROADCAST_RECV=1.
 */
#define IP_SOF_BROADCAST                0
/**
 * IP_SOF_BROADCAST_RECV (requires IP_SOF_BROADCAST=1) enable the broadcast
 * filter on recv operations.
 */
#define IP_SOF_BROADCAST_RECV           0
/**
 * IP_FORWARD_ALLOW_TX_ON_RX_NETIF==1: allow ip_forward() to send packets back
 * out on the netif where it was received. This should only be used for
 * wireless networks.
 * ATTENTION: When this is 1, make sure your netif driver correctly marks incoming
 * link-layer-broadcast/multicast packets as such using the corresponding pbuf flags!
 */
#define IP_FORWARD_ALLOW_TX_ON_RX_NETIF 0
/**
 * @}
 */
/*
   ----------------------------------
   ---------- ICMP options ----------
   ----------------------------------
*/
/**
 * @defgroup lwip_opts_icmp ICMP
 * @ingroup lwip_opts_ipv4
 * @{
 */
/**
 * LWIP_ICMP==1: Enable ICMP module inside the IP stack.
 * Be careful, disable that make your product non-compliant to RFC1122
 */
#define LWIP_ICMP                       1
/**
 * ICMP_TTL: Default value for Time-To-Live used by ICMP packets.
 */
#define ICMP_TTL                        IP_DEFAULT_TTL
/**
 * LWIP_BROADCAST_PING==1: respond to broadcast pings (default is unicast only)
 */
#define LWIP_BROADCAST_PING             0
/**
 * LWIP_MULTICAST_PING==1: respond to multicast pings (default is unicast only)
 */
#define LWIP_MULTICAST_PING             0
/**
 * @}
 */
/*
   ---------------------------------
   ---------- RAW options ----------
   ---------------------------------
*/
/**
 * @defgroup lwip_opts_raw RAW
 * @ingroup lwip_opts_callback
 * @{
 */
/**
 * LWIP_RAW==1: Enable application layer to hook into the IP layer itself.
 */
#define LWIP_RAW                        0
/**
 * LWIP_RAW==1: Enable application layer to hook into the IP layer itself.
 */
#define RAW_TTL                         IP_DEFAULT_TTL
/**
 * @}
 */
/*
   ----------------------------------
   ---------- DHCP options ----------
   ----------------------------------
*/
/**
 * @defgroup lwip_opts_dhcp DHCP
 * @ingroup lwip_opts_ipv4
 * @{
 */
/**
 * LWIP_DHCP==1: Enable DHCP module.
 */
#define LWIP_DHCP                       0

#undef LWIP_DHCP
#define LWIP_DHCP                       0
/**
 * DHCP_DOES_ARP_CHECK==1: Do an ARP check on the offered address.
 */
#define DHCP_DOES_ARP_CHECK             (LWIP_DHCP && LWIP_ARP)
/**
 * LWIP_DHCP_BOOTP_FILE==1: Store offered_si_addr and boot_file_name.
 */
#define LWIP_DHCP_BOOTP_FILE            0
/**
 * LWIP_DHCP_GETS_NTP==1: Request NTP servers with discover/select. For each
 * response packet, an callback is called, which has to be provided by the port:
 * void dhcp_set_ntp_servers(u8_t num_ntp_servers, ip_addr_t* ntp_server_addrs);
*/
#define LWIP_DHCP_GET_NTP_SRV           0
/**
 * The maximum of NTP servers requested
 */
#define LWIP_DHCP_MAX_NTP_SERVERS       1
/**
 * LWIP_DHCP_MAX_DNS_SERVERS > 0: Request DNS servers with discover/select.
 * DNS servers received in the response are passed to DNS via @ref dns_setserver()
 * (up to the maximum limit defined here).
 */
#define LWIP_DHCP_MAX_DNS_SERVERS       DNS_MAX_SERVERS
/**
 * @}
 */
/*
   ------------------------------------
   ---------- AUTOIP options ----------
   ------------------------------------
*/
/**
 * @defgroup lwip_opts_autoip AUTOIP
 * @ingroup lwip_opts_ipv4
 * @{
 */
/**
 * LWIP_AUTOIP==1: Enable AUTOIP module.
 */
#define LWIP_AUTOIP                     0

#undef LWIP_AUTOIP
#define LWIP_AUTOIP                     0
/**
 * LWIP_DHCP_AUTOIP_COOP==1: Allow DHCP and AUTOIP to be both enabled on
 * the same interface at the same time.
 */
#define LWIP_DHCP_AUTOIP_COOP           0
/**
 * LWIP_DHCP_AUTOIP_COOP_TRIES: Set to the number of DHCP DISCOVER probes
 * that should be sent before falling back on AUTOIP (the DHCP client keeps
 * running in this case). This can be set as low as 1 to get an AutoIP address
 * very  quickly, but you should be prepared to handle a changing IP address
 * when DHCP overrides AutoIP.
 */
#define LWIP_DHCP_AUTOIP_COOP_TRIES     9
/**
 * @}
 */
/*
   ----------------------------------
   ----- SNMP MIB2 support      -----
   ----------------------------------
*/
/**
 * @defgroup lwip_opts_mib2 SNMP MIB2 callbacks
 * @ingroup lwip_opts_infrastructure
 * @{
 */
/**
 * LWIP_MIB2_CALLBACKS==1: Turn on SNMP MIB2 callbacks.
 * Turn this on to get callbacks needed to implement MIB2.
 * Usually MIB2_STATS should be enabled, too.
 */
#define LWIP_MIB2_CALLBACKS             0
/**
 * @}
 */
/*
   ----------------------------------
   -------- Multicast options -------
   ----------------------------------
*/
/**
 * @defgroup lwip_opts_multicast Multicast
 * @ingroup lwip_opts_infrastructure
 * @{
 */
/**
 * LWIP_MULTICAST_TX_OPTIONS==1: Enable multicast TX support like the socket options
 * IP_MULTICAST_TTL/IP_MULTICAST_IF/IP_MULTICAST_LOOP, as well as (currently only)
 * core support for the corresponding IPv6 options.
 */
#define LWIP_MULTICAST_TX_OPTIONS       ((LWIP_IGMP || LWIP_IPV6_MLD) && (LWIP_UDP || LWIP_RAW))
/**
 * @}
 */
/*
   ----------------------------------
   ---------- IGMP options ----------
   ----------------------------------
*/
/**
 * @defgroup lwip_opts_igmp IGMP
 * @ingroup lwip_opts_ipv4
 * @{
 */
/**
 * LWIP_IGMP==1: Turn on IGMP module.
 */
#define LWIP_IGMP                       0
#undef LWIP_IGMP
#define LWIP_IGMP                       0
/**
 * @}
 */
/*
   ----------------------------------
   ---------- DNS options -----------
   ----------------------------------
*/
/**
 * @defgroup lwip_opts_dns DNS
 * @ingroup lwip_opts_callback
 * @{
 */
/**
 * LWIP_DNS==1: Turn on DNS module. UDP must be available for DNS
 * transport.
 */
#define LWIP_DNS                        0

#define DNS_TABLE_SIZE                  4

#define DNS_MAX_NAME_LENGTH             256
/** The maximum of DNS servers
 * The first server can be initialized automatically by defining
 * DNS_SERVER_ADDRESS(ipaddr), where 'ipaddr' is an 'ip_addr_t*'
 */
#define DNS_MAX_SERVERS                 2

#define DNS_MAX_RETRIES                 4

#define DNS_DOES_NAME_CHECK             1
/** LWIP_DNS_SECURE: controls the security level of the DNS implementation
 * Use all DNS security features by default.
 * This is overridable but should only be needed by very small targets
 * or when using against non standard DNS servers. */
#define LWIP_DNS_SECURE (LWIP_DNS_SECURE_RAND_XID | LWIP_DNS_SECURE_NO_MULTIPLE_OUTSTANDING | LWIP_DNS_SECURE_RAND_SRC_PORT)

#define LWIP_DNS_SECURE_RAND_XID                1
#define LWIP_DNS_SECURE_NO_MULTIPLE_OUTSTANDING 2
#define LWIP_DNS_SECURE_RAND_SRC_PORT           4
/** DNS_LOCAL_HOSTLIST: Implements a local host-to-address list. If enabled, you have to define an initializer:
 *  \#define DNS_LOCAL_HOSTLIST_INIT {DNS_LOCAL_HOSTLIST_ELEM("host_ip4", IPADDR4_INIT_BYTES(1,2,3,4)), \
 *                                    DNS_LOCAL_HOSTLIST_ELEM("host_ip6", IPADDR6_INIT_HOST(123, 234, 345, 456)}
 *
 *  Instead, you can also use an external function:
 *  \#define DNS_LOOKUP_LOCAL_EXTERN(x) extern err_t my_lookup_function(const char *name, ip_addr_t *addr, u8_t dns_addrtype)
 *  that looks up the IP address and returns ERR_OK if found (LWIP_DNS_ADDRTYPE_xxx is passed in dns_addrtype).
 */
#define DNS_LOCAL_HOSTLIST              0
/** If this is turned on, the local host-list can be dynamically changed
 *  at runtime. */
#define DNS_LOCAL_HOSTLIST_IS_DYNAMIC   0
/** Set this to 1 to enable querying ".local" names via mDNS
 *  using a One-Shot Multicast DNS Query */
#define LWIP_DNS_SUPPORT_MDNS_QUERIES   0
/**
 * @}
 */
/*
   ---------------------------------
   ---------- UDP options ----------
   ---------------------------------
*/
/**
 * @defgroup lwip_opts_udp UDP
 * @ingroup lwip_opts_callback
 * @{
 */
/**
 * LWIP_UDP==1: Turn on UDP.
 */
#define LWIP_UDP                        1
/**
 * LWIP_UDPLITE==1: Turn on UDP-Lite. (Requires LWIP_UDP)
 */
#define LWIP_UDPLITE                    0
/**
 * UDP_TTL: Default Time-To-Live value.
 */
#define UDP_TTL                         IP_DEFAULT_TTL
/**
 * LWIP_NETBUF_RECVINFO==1: append destination addr and port to every netbuf.
 */
#define LWIP_NETBUF_RECVINFO            0
/**
 * @}
 */
/*
   ---------------------------------
   ---------- TCP options ----------
   ---------------------------------
*/
/**
 * @defgroup lwip_opts_tcp TCP
 * @ingroup lwip_opts_callback
 * @{
 */
/**
 * LWIP_TCP==1: Turn on TCP.
 */
#define LWIP_TCP                        1
/**
 * TCP_TTL: Default Time-To-Live value.
 */
#define TCP_TTL                         IP_DEFAULT_TTL
/**
 * TCP_WND: The size of a TCP window.  This must be at least
 * (2 * TCP_MSS) for things to work well.
 * ATTENTION: when using TCP_RCV_SCALE, TCP_WND is the total size
 * with scaling applied. Maximum window value in the TCP header
 * will be TCP_WND >> TCP_RCV_SCALE
 */
#define TCP_WND                         (4 * TCP_MSS)
/**
 * TCP_MAXRTX: Maximum number of retransmissions of data segments.
 */
#define TCP_MAXRTX                      12
/**
 * TCP_SYNMAXRTX: Maximum number of retransmissions of SYN segments.
 */
#define TCP_SYNMAXRTX                   6
/**
 * TCP_QUEUE_OOSEQ==1: TCP will queue segments that arrive out of order.
 * Define to 0 if your device is low on memory.
 */
#define TCP_QUEUE_OOSEQ                 LWIP_TCP
/**
 * LWIP_TCP_SACK_OUT==1: TCP will support sending selective acknowledgements (SACKs).
 */
#define LWIP_TCP_SACK_OUT               0
/**
 * LWIP_TCP_MAX_SACK_NUM: The maximum number of SACK values to include in TCP segments.
 * Must be at least 1, but is only used if LWIP_TCP_SACK_OUT is enabled.
 * NOTE: Even though we never send more than 3 or 4 SACK ranges in a single segment
 * (depending on other options), setting this option to values greater than 4 is not pointless.
 * This is basically the max number of SACK ranges we want to keep track of.
 * As new data is delivered, some of the SACK ranges may be removed or merged.
 * In that case some of those older SACK ranges may be used again.
 * The amount of memory used to store SACK ranges is LWIP_TCP_MAX_SACK_NUM * 8 bytes for each TCP PCB.
 */
#define LWIP_TCP_MAX_SACK_NUM           4
/**
 * TCP_MSS: TCP Maximum segment size. (default is 536, a conservative default,
 * you might want to increase this.)
 * For the receive side, this MSS is advertised to the remote side
 * when opening a connection. For the transmit size, this MSS sets
 * an upper limit on the MSS advertised by the remote host.
 */
#define TCP_MSS                         536
/**
 * TCP_CALCULATE_EFF_SEND_MSS: "The maximum size of a segment that TCP really
 * sends, the 'effective send MSS,' MUST be the smaller of the send MSS (which
 * reflects the available reassembly buffer size at the remote host) and the
 * largest size permitted by the IP layer" (RFC 1122)
 * Setting this to 1 enables code that checks TCP_MSS against the MTU of the
 * netif used for a connection and limits the MSS if it would be too big otherwise.
 */
#define TCP_CALCULATE_EFF_SEND_MSS      1
/**
 * TCP_SND_BUF: TCP sender buffer space (bytes).
 * To achieve good performance, this should be at least 2 * TCP_MSS.
 */
#define TCP_SND_BUF                     (2 * TCP_MSS)
/**
 * TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
 * as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work.
 */
#define TCP_SND_QUEUELEN                ((4 * (TCP_SND_BUF) + (TCP_MSS - 1))/(TCP_MSS))
/**
 * TCP_SNDLOWAT: TCP writable space (bytes). This must be less than
 * TCP_SND_BUF. It is the amount of space which must be available in the
 * TCP snd_buf for select to return writable (combined with TCP_SNDQUEUELOWAT).
 */
#define TCP_SNDLOWAT                    LWIP_MIN(LWIP_MAX(((TCP_SND_BUF)/2), (2 * TCP_MSS) + 1), (TCP_SND_BUF) - 1)
/**
 * TCP_SNDQUEUELOWAT: TCP writable bufs (pbuf count). This must be less
 * than TCP_SND_QUEUELEN. If the number of pbufs queued on a pcb drops below
 * this number, select returns writable (combined with TCP_SNDLOWAT).
 */
#define TCP_SNDQUEUELOWAT               LWIP_MAX(((TCP_SND_QUEUELEN)/2), 5)
/**
 * TCP_OOSEQ_MAX_BYTES: The default maximum number of bytes queued on ooseq per
 * pcb if TCP_OOSEQ_BYTES_LIMIT is not defined. Default is 0 (no limit).
 * Only valid for TCP_QUEUE_OOSEQ==1.
 */
#define TCP_OOSEQ_MAX_BYTES             0
/**
 * TCP_OOSEQ_BYTES_LIMIT(pcb): Return the maximum number of bytes to be queued
 * on ooseq per pcb, given the pcb. Only valid for TCP_QUEUE_OOSEQ==1 &&
 * TCP_OOSEQ_MAX_BYTES==1.
 * Use this to override TCP_OOSEQ_MAX_BYTES to a dynamic value per pcb.
 */
#define TCP_OOSEQ_BYTES_LIMIT(pcb)      TCP_OOSEQ_MAX_BYTES
/**
 * TCP_OOSEQ_MAX_PBUFS: The default maximum number of pbufs queued on ooseq per
 * pcb if TCP_OOSEQ_BYTES_LIMIT is not defined. Default is 0 (no limit).
 * Only valid for TCP_QUEUE_OOSEQ==1.
 */
#define TCP_OOSEQ_MAX_PBUFS             0
/**
 * TCP_OOSEQ_PBUFS_LIMIT(pcb): Return the maximum number of pbufs to be queued
 * on ooseq per pcb, given the pcb.  Only valid for TCP_QUEUE_OOSEQ==1 &&
 * TCP_OOSEQ_MAX_PBUFS==1.
 * Use this to override TCP_OOSEQ_MAX_PBUFS to a dynamic value per pcb.
 */
#define TCP_OOSEQ_PBUFS_LIMIT(pcb)      TCP_OOSEQ_MAX_PBUFS
/**
 * TCP_LISTEN_BACKLOG: Enable the backlog option for tcp listen pcb.
 */
#define TCP_LISTEN_BACKLOG              0
/**
 * The maximum allowed backlog for TCP listen netconns.
 * This backlog is used unless another is explicitly specified.
 * 0xff is the maximum (u8_t).
 */
#define TCP_DEFAULT_LISTEN_BACKLOG      0xff
/**
 * TCP_OVERSIZE: The maximum number of bytes that tcp_write may
 * allocate ahead of time in an attempt to create shorter pbuf chains
 * for transmission. The meaningful range is 0 to TCP_MSS. Some
 * suggested values are:
 *
 * 0:         Disable oversized allocation. Each tcp_write() allocates a new
              pbuf (old behaviour).
 * 1:         Allocate size-aligned pbufs with minimal excess. Use this if your
 *            scatter-gather DMA requires aligned fragments.
 * 128:       Limit the pbuf/memory overhead to 20%.
 * TCP_MSS:   Try to create unfragmented TCP packets.
 * TCP_MSS/4: Try to create 4 fragments or less per TCP packet.
 */
#define TCP_OVERSIZE                    TCP_MSS
/**
 * LWIP_TCP_TIMESTAMPS==1: support the TCP timestamp option.
 * The timestamp option is currently only used to help remote hosts, it is not
 * really used locally. Therefore, it is only enabled when a TS option is
 * received in the initial SYN packet from a remote host.
 */
#define LWIP_TCP_TIMESTAMPS             0
/**
 * TCP_WND_UPDATE_THRESHOLD: difference in window to trigger an
 * explicit window update
 */
#define TCP_WND_UPDATE_THRESHOLD        LWIP_MIN((TCP_WND / 4), (TCP_MSS * 4))
/**
 * LWIP_EVENT_API and LWIP_CALLBACK_API: Only one of these should be set to 1.
 *     LWIP_EVENT_API==1: The user defines lwip_tcp_event() to receive all
 *         events (accept, sent, etc) that happen in the system.
 *     LWIP_CALLBACK_API==1: The PCB callback function is called directly
 *         for the event. This is the default.
 */
#define LWIP_EVENT_API                  0
#define LWIP_CALLBACK_API               1

/**
 * LWIP_WND_SCALE and TCP_RCV_SCALE:
 * Set LWIP_WND_SCALE to 1 to enable window scaling.
 * Set TCP_RCV_SCALE to the desired scaling factor (shift count in the
 * range of [0..14]).
 * When LWIP_WND_SCALE is enabled but TCP_RCV_SCALE is 0, we can use a large
 * send window while having a small receive window only.
 */
#define LWIP_WND_SCALE                  0
#define TCP_RCV_SCALE                   0
/**
 * LWIP_TCP_PCB_NUM_EXT_ARGS:
 * When this is > 0, every tcp pcb (including listen pcb) includes a number of
 * additional argument entries in an array (see tcp_ext_arg_alloc_id)
 */
#define LWIP_TCP_PCB_NUM_EXT_ARGS       0
/** LWIP_ALTCP==1: enable the altcp API.
 * altcp is an abstraction layer that prevents applications linking against the
 * tcp.h functions but provides the same functionality. It is used to e.g. add
 * SSL/TLS or proxy-connect support to an application written for the tcp callback
 * API without that application knowing the protocol details.
 *
 * With LWIP_ALTCP==0, applications written against the altcp API can still be
 * compiled but are directly linked against the tcp.h callback API and then
 * cannot use layered protocols.
 *
 * See @ref altcp_api
 */
#define LWIP_ALTCP                      0
/** LWIP_ALTCP_TLS==1: enable TLS support for altcp API.
 * This needs a port of the functions in altcp_tls.h to a TLS library.
 * A port to ARM mbedtls is provided with lwIP, see apps/altcp_tls/ directory
 * and LWIP_ALTCP_TLS_MBEDTLS option.
 */
#define LWIP_ALTCP_TLS                  0
/**
 * @}
 */
/*
   ----------------------------------
   ---------- Pbuf options ----------
   ----------------------------------
*/
/**
 * @defgroup lwip_opts_pbuf PBUF
 * @ingroup lwip_opts
 * @{
 */
/**
 * PBUF_LINK_HLEN: the number of bytes that should be allocated for a
 * link level header. The default is 14, the standard value for
 * Ethernet.
 */
#define PBUF_LINK_HLEN                  (18 + ETH_PAD_SIZE)
#define PBUF_LINK_HLEN                  (14 + ETH_PAD_SIZE)
/**
 * PBUF_LINK_ENCAPSULATION_HLEN: the number of bytes that should be allocated
 * for an additional encapsulation header before ethernet headers (e.g. 802.11)
 */
#define PBUF_LINK_ENCAPSULATION_HLEN    0
/**
 * PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. The default is
 * designed to accommodate single full size TCP frame in one pbuf, including
 * TCP_MSS, IP header, and link header.
 */
#define PBUF_POOL_BUFSIZE               LWIP_MEM_ALIGN_SIZE(TCP_MSS+40+PBUF_LINK_ENCAPSULATION_HLEN+PBUF_LINK_HLEN)
/**
 * LWIP_PBUF_REF_T: Refcount type in pbuf.
 * Default width of u8_t can be increased if 255 refs are not enough for you.
 */
#define LWIP_PBUF_REF_T                 u8_t
#define LWIP_SINGLE_NETIF               0
#define LWIP_NETIF_HOSTNAME             0
#define LWIP_NETIF_API                  0
#define LWIP_NETIF_STATUS_CALLBACK      0
#define LWIP_NETIF_EXT_STATUS_CALLBACK  0
#define LWIP_NETIF_LINK_CALLBACK        0
#define LWIP_NETIF_REMOVE_CALLBACK      0
#define LWIP_NETIF_HWADDRHINT           0
#define LWIP_NETIF_TX_SINGLE_PBUF       0
#define LWIP_NUM_NETIF_CLIENT_DATA      0
#define LWIP_HAVE_LOOPIF                (LWIP_NETIF_LOOPBACK && !LWIP_SINGLE_NETIF)
#define LWIP_LOOPIF_MULTICAST           0
#define LWIP_NETIF_LOOPBACK             0
#define LWIP_LOOPBACK_MAX_PBUFS         0
#define LWIP_NETIF_LOOPBACK_MULTITHREADING    (!NO_SYS)
#define TCPIP_THREAD_NAME               "tcpip_thread"
#define TCPIP_THREAD_STACKSIZE          0
#define TCPIP_THREAD_PRIO               1
#define TCPIP_MBOX_SIZE                 0
#define LWIP_TCPIP_THREAD_ALIVE()
#define SLIPIF_THREAD_NAME              "slipif_loop"
#define SLIPIF_THREAD_STACKSIZE         0
#define SLIPIF_THREAD_PRIO              1
#define DEFAULT_THREAD_NAME             "lwIP"
#define DEFAULT_THREAD_STACKSIZE        0
#define DEFAULT_THREAD_PRIO             1
#define DEFAULT_RAW_RECVMBOX_SIZE       0
/**
 * DEFAULT_UDP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_UDP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#define DEFAULT_UDP_RECVMBOX_SIZE       0
/**
 * DEFAULT_TCP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_TCP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#define DEFAULT_TCP_RECVMBOX_SIZE       0
/**
 * DEFAULT_ACCEPTMBOX_SIZE: The mailbox size for the incoming connections.
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when the acceptmbox is created.
 */
#define DEFAULT_ACCEPTMBOX_SIZE         0
/**
 * @}
 */
/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
 * @defgroup lwip_opts_netconn Netconn
 * @ingroup lwip_opts_threadsafe_apis
 * @{
 */
/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN                    1
/** LWIP_TCPIP_TIMEOUT==1: Enable tcpip_timeout/tcpip_untimeout to create
 * timers running in tcpip_thread from another thread.
 */
#define LWIP_TCPIP_TIMEOUT              0
/** LWIP_NETCONN_SEM_PER_THREAD==1: Use one (thread-local) semaphore per
 * thread calling socket/netconn functions instead of allocating one
 * semaphore per netconn (and per select etc.)
 * ATTENTION: a thread-local semaphore for API calls is needed:
 * - LWIP_NETCONN_THREAD_SEM_GET() returning a sys_sem_t*
 * - LWIP_NETCONN_THREAD_SEM_ALLOC() creating the semaphore
 * - LWIP_NETCONN_THREAD_SEM_FREE() freeing the semaphore
 * The latter 2 can be invoked up by calling netconn_thread_init()/netconn_thread_cleanup().
 * Ports may call these for threads created with sys_thread_new().
 */
#define LWIP_NETCONN_SEM_PER_THREAD     0
/** LWIP_NETCONN_FULLDUPLEX==1: Enable code that allows reading from one thread,
 * writing from a 2nd thread and closing from a 3rd thread at the same time.
 * ATTENTION: This is currently really alpha! Some requirements:
 * - LWIP_NETCONN_SEM_PER_THREAD==1 is required to use one socket/netconn from
 *   multiple threads at once
 * - sys_mbox_free() has to unblock receive tasks waiting on recvmbox/acceptmbox
 *   and prevent a task pending on this during/after deletion
 */
#define LWIP_NETCONN_FULLDUPLEX         0
/**
 * @}
 */
/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
 * @defgroup lwip_opts_socket Sockets
 * @ingroup lwip_opts_threadsafe_apis
 * @{
 */
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET                     1
/**
 * LWIP_COMPAT_SOCKETS==1: Enable BSD-style sockets functions names through defines.
 * LWIP_COMPAT_SOCKETS==2: Same as ==1 but correctly named functions are created.
 * While this helps code completion, it might conflict with existing libraries.
 * (only used if you use sockets.c)
 */
#define LWIP_COMPAT_SOCKETS             1
/**
 * LWIP_POSIX_SOCKETS_IO_NAMES==1: Enable POSIX-style sockets functions names.
 * Disable this option if you use a POSIX operating system that uses the same
 * names (read, write & close). (only used if you use sockets.c)
 */
#define LWIP_POSIX_SOCKETS_IO_NAMES     1
/**
 * LWIP_SOCKET_OFFSET==n: Increases the file descriptor number created by LwIP with n.
 * This can be useful when there are multiple APIs which create file descriptors.
 * When they all start with a different offset and you won't make them overlap you can
 * re implement read/write/close/ioctl/fnctl to send the requested action to the right
 * library (sharing select will need more work though).
 */
#define LWIP_SOCKET_OFFSET              0
/**
 * LWIP_TCP_KEEPALIVE==1: Enable TCP_KEEPIDLE, TCP_KEEPINTVL and TCP_KEEPCNT
 * options processing. Note that TCP_KEEPIDLE and TCP_KEEPINTVL have to be set
 * in seconds. (does not require sockets.c, and will affect tcp.c)
 */
#define LWIP_TCP_KEEPALIVE              0
/**
 * LWIP_SO_SNDTIMEO==1: Enable send timeout for sockets/netconns and
 * SO_SNDTIMEO processing.
 */
#define LWIP_SO_SNDTIMEO                0
/**
 * LWIP_SO_RCVTIMEO==1: Enable receive timeout for sockets/netconns and
 * SO_RCVTIMEO processing.
 */
#define LWIP_SO_RCVTIMEO                0
/**
 * LWIP_SO_SNDRCVTIMEO_NONSTANDARD==1: SO_RCVTIMEO/SO_SNDTIMEO take an int
 * (milliseconds, much like winsock does) instead of a struct timeval (default).
 */
#define LWIP_SO_SNDRCVTIMEO_NONSTANDARD 0
/**
 * LWIP_SO_RCVBUF==1: Enable SO_RCVBUF processing.
 */
#define LWIP_SO_RCVBUF                  0
/**
 * LWIP_SO_LINGER==1: Enable SO_LINGER processing.
 */
#define LWIP_SO_LINGER                  0
/**
 * If LWIP_SO_RCVBUF is used, this is the default value for recv_bufsize.
 */
#define RECV_BUFSIZE_DEFAULT            INT_MAX
/**
 * By default, TCP socket/netconn close waits 20 seconds max to send the FIN
 */
#define LWIP_TCP_CLOSE_TIMEOUT_MS_DEFAULT 20000
/**
 * SO_REUSE==1: Enable SO_REUSEADDR option.
 */
#define SO_REUSE                        0
/**
 * SO_REUSE_RXTOALL==1: Pass a copy of incoming broadcast/multicast packets
 * to all local matches if SO_REUSEADDR is turned on.
 * WARNING: Adds a memcpy for every packet if passing to more than one pcb!
 */
#define SO_REUSE_RXTOALL                0
/**
 * LWIP_FIONREAD_LINUXMODE==0 (default): ioctl/FIONREAD returns the amount of
 * pending data in the network buffer. This is the way windows does it. It's
 * the default for lwIP since it is smaller.
 * LWIP_FIONREAD_LINUXMODE==1: ioctl/FIONREAD returns the size of the next
 * pending datagram in bytes. This is the way linux does it. This code is only
 * here for compatibility.
 */
#define LWIP_FIONREAD_LINUXMODE         0
/**
 * LWIP_SOCKET_SELECT==1 (default): enable select() for sockets (uses a netconn
 * callback to keep track of events).
 * This saves RAM (counters per socket) and code (netconn event callback), which
 * should improve performance a bit).
 */
#define LWIP_SOCKET_SELECT              1
/**
 * LWIP_SOCKET_POLL==1 (default): enable poll() for sockets (including
 * struct pollfd, nfds_t, and constants)
 */
#define LWIP_SOCKET_POLL                1
