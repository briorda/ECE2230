/* socketlist.h 
 * Austin Johnson
 * agjohns 
 * ECE 2230 Fall 2016 
 * MP1
 *
 * Purpose: The interface definition for MP1.  DO NOT CHANGE THIS FILE
 *
 * Assumptions: structure definitions and public functions as defined for
 * assignmnet.  There are no extensions.
 *
 * Bugs: No bugs from the tests that I have ran against it.
 */

struct socketlist_t {
    int vector_capacity;  // current size of array, initially MINSOCKETS 
    int vector_size;      // current number of records in socket list
    struct socket_info_t **vector_ptr;
};

struct socket_info_t {
    int socket_id;        // local id for socket structure
    unsigned int dest_ip_addr;   // IP address of destination host 
    unsigned int src_ip_addr;    // IP address this host 
    int dest_port_num;    // Port number at destination host 
    int src_port_num;     // Port number at this host or -1 is supplied by OS 
    int protocol;         // 1=TCP, 2=UDP, 3=SSL, or 4=RTP 
    int address_family;   // 4 for IPv4 or 6 for IPv6 
    int state;            // 1=EST, 2=CLOSE_WAIT, 3=CLOSED 
    float data_rate;      // data rate of local link in Mbps 
    int time_received;    // time in seconds that information last updated 
};

//  The sequential list ADT must have the following interface:

struct socketlist_t *socketlist_construct();
void socketlist_destruct(struct socketlist_t *);
int socketlist_add(struct socketlist_t *, int, struct socket_info_t *);
struct socket_info_t *socketlist_replace(struct socketlist_t *, int, struct socket_info_t *);
struct socket_info_t *socketlist_access(struct socketlist_t *, int);
struct socket_info_t *socketlist_remove(struct socketlist_t *, int);
int socketlist_capacity(struct socketlist_t *);
int socketlist_number_entries(struct socketlist_t *);


