/* datatypes.h 
 * Austin Johnson  
 * agjohns
 * ECE 2230 Fall 2016 
 * MP3
 * 
 * Propose: The data type that is stored in the list ADT is defined here.  We
 *          define a single mapping that allows the list ADT to be defined in
 *          terms of a generic data_t.
 *
 * Assumptions: 
 *     For MP2 there are no changes to this file.  For all other MP's
 *     this file must be updated
 *
 *     data_t: The type of data that we want to store in the list
 *
 * Bugs: No bugs from the tests I have ran against it. 
 */

typedef struct socket_tag {
    int socket_id;       /* local id for socket structure*/
    unsigned int dest_ip_addr; /* IP address of destination host */
    unsigned int src_ip_addr;  /* IP address this host or -1 if default address */
    int dest_port_num;   /* Port number at destination host */
    int src_port_num;    /* Port number at host or -1 if supplied by OS */
    int protocol;        /* 1 for TCP, 2 for UDP, 3 for SSL, or 4 for RTP */
    int address_family;  /* 4 for IPv4 or 6 for IPv6 */
    int state;           /* 1 for EST, 2 for CLOSE_WAIT, 3 for CLOSED */
    float data_rate;     /* data rate of local link in Mbps */
    int time_received;   /* time in seconds that information last updated */
} socket_t;

/* the list ADT works on socket data of this type */
typedef socket_t data_t;

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
