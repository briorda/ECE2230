/* graph.h
 * Austin Johnson
 * agjohns
 * Lab6: Graph
 * ECE 2230, Fall 2016
 */

extern int GraphType;
extern int GraphOperation;
extern int NumVertices;
extern int AdjVertices;

typedef struct adj_list_node_tag {
	int vertex;
	int weight;
	struct adj_list_node_tag *next;
} adj_list_node_t;

typedef struct adj_list_tag {
	adj_list_node_t *head; 
	int NumEdges;
} adj_list_t;

typedef struct graph_tag {
	int NumVertices; 
	adj_list_t *adj_list; //an array of adjacency lists
} graph_t;

//prototype definitions
graph_t *graph_construct (int NumVertices);
void graph_destruct(graph_t *graph);
void adj_list_insert(adj_list_t *list_ptr, int dest, int weight);
void adj_list_remove(adj_list_t *adj_list, int vertex);
int connected_graph (graph_t *graph);
void graph_destruct (graph_t *graph);
void graph_add_edge (graph_t *graph, int link_src, int link_dest, int link_weight);
void graph_remove_edge (graph_t *graph, int link_src, int vertex);
float distance (float link_src_x, float link_src_y, float link_dest_x, float link_dest_y);
int weight (int link_src, int link_dest, float distance);
void graph_debug_print (graph_t *G);
void graph_debug_print_aux (adj_list_node_t *N, int source);
int min_distance (int dest, int dist_to_dest);
void store_best_path (int Path[], int link_src, int link_dest, int *best_path);
void dijkstra (graph_t *graph, int link_src, int link_dest);
void graph_network_diameter (graph_t *graph);
void graph_multiple_paths (graph_t *graph, int link_src, int link_dest);
void store_best_path (int Path[], int link_src, int link_dest, int *best_path);
void print_path (int num_vert_in_path, int Path[]);
void graph_shortest_path (graph_t *graph, int link_src, int Distance[], int Path[]);
