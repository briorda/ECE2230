/* graph.c               
 * Austin Johnson 
 * agjohns       
 * ECE 2230 Fall 2016
 * MP6
 *
 * Purpose: The purpose of this file is to provide functions
 *          for lab6.c in order to construct different types
 *          of graphs and perform different types of operations
 *
 *
 * Assumptions: Graph Type 1 = Weakly-Connected with 7 Vertices
 *		Graph Type 2 = Strongly-Connected with N Vertices
 *		Graph Type 3 = Random with N Vertices & R Average Neighbors
 *
 *		Graph Operation 1 = Dijkstra's Alogrithm
 *		Graph Operation 2 = Network Diameter
 *		Graph Operation 3 = Multiple Link-Disjoint Paths
 *
 *   
 * Bugs: No bugs from the tests that I have ran against it. 
 *
 *
 */

#define TRUE 1
#define FALSE 0

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "graph.h"

/*
    This function creates a graph, storing the number of vertices in the graph and
    creating an array of pointers that each point to an adjacency list that represent
    the paths that connect to a node in the graph. It returns the constructed graph.
*/
graph_t *graph_construct (int NumVertices) {
	graph_t *graph = (graph_t *) malloc(sizeof(graph_t));
	graph->NumVertices = NumVertices;
	graph->adj_list = (adj_list_t *) malloc(NumVertices * sizeof(adj_list_t));
	int i;	
	for (i = 0; i < NumVertices; i++) { 
		graph->adj_list[i].head = NULL; //initialize each adjacency list head as NULL
	}
	return graph;
}

/*
    This function frees the adjaceny list within the graph structure and 
    then frees the graph. It returns void. 
*/
void graph_destruct(graph_t *graph) {
    int i;
    for (i = 0; i < graph->NumVertices; i++) {
	    adj_list_node_t *N = graph->adj_list[i].head;
	    adj_list_node_t *temp = NULL;

	    while (N != NULL) {
		temp = N;
		N = N->next;
		free(temp);
	    }

	    free(N);
   }
   free(graph->adj_list);
   free(graph);
}

/*
    This function inserts a vertex into an adjacency list. The adj_list pointer
    points to the source vertex's list. It inserts the destination vertex into
    that list and puts a weighted edge between the source and destination. It 
    returns void.  
*/
void adj_list_insert(adj_list_t *adj_list, int dest, int weight) { 

    //malloc space for new node and initialize values
    adj_list_node_t *new_node = (adj_list_node_t *) malloc(sizeof(adj_list_node_t)); 
    new_node->next = NULL;
    new_node->vertex = dest;
    new_node->weight = weight; 

    //empty list
    if (adj_list->head == NULL) {
        adj_list->head = new_node;
        adj_list->NumEdges = 1;
    }  

    //insert at the head of the list
    else {
        new_node->next = adj_list->head; 
        adj_list->head = new_node;
        adj_list->NumEdges += 1;
    }
    assert(adj_list != NULL);
}

/*
    This function removes a vertex from an adjacency list. The adj_list pointer
    points to the source vertex's list. It removes the destination vertex from
    that list and returns void.  
*/
void adj_list_remove(adj_list_t *adj_list, int vertex) {

	if (adj_list->head == NULL) return;
	else {
		adj_list_node_t *N = adj_list->head;

		//if only one item in the list
		if (N->next == NULL) adj_list->head = NULL;

		//if node is the head
		else if (adj_list->head->vertex == vertex) adj_list->head = N->next;   
        	else {
			//find node to remove
			adj_list_node_t *prev = N;
			N = N->next;
			while (N != NULL) {
			    if (N->vertex == vertex) break;
			    prev = N;
			    N = N->next;
			}		
			prev->next = N->next;
		}
		free(N);	
	}
}

/*
    This function adds a weighted edge between a source vertex and a destination vertex. 
    It uses pointer arithmetic to get the source vertex's list and sends it into the 
    helper function adj_list_insert to add the edge between the vertices. It returns void. 
*/
void graph_add_edge (graph_t *graph, int link_src, int link_dest, int link_weight) {

	//if weight of the edge is INT_MAX --> no connection
	if (link_weight == INT_MAX) return;

	//get the adjacency list of the source
	adj_list_t *source_list = graph->adj_list + link_src;

	//insert destination into the source list
	adj_list_insert (source_list, link_dest, link_weight);
}

/*
    This function removes a weighted edge from a source vertex and a destination vertex. 
    It uses pointer arithmetic to get the source vertex's list and sends it into the 
    helper function adj_list_remove to remove the edge between the vertices. It returns void. 
*/
void graph_remove_edge (graph_t *graph, int link_src, int vertex) {
	
	//get the adjacency list of the source
	adj_list_t *source_list = graph->adj_list + link_src;

	//delete vertex from the source list
	adj_list_remove (source_list, vertex);
}

/*
    This function uses Depth-First Traversal to check if a graph is connected.
    This funciton only applies to GraphType 3 as it is an undirected graph.
    It returns TRUE (1) if the graph is connected and FALSE (0) otherwise.  
*/
int connected_graph (graph_t *graph) {
	//DFS traversal 
	adj_list_node_t *node = NULL;
	int i;
	int *visited = (int *) malloc(graph->NumVertices * sizeof(int));;	
	for (i = 0; i < graph->NumVertices; i++)
		visited[i] = FALSE;
	for (i = 0; i < graph->NumVertices; i++) {
		node = graph->adj_list[i].head;
		while (node != NULL) {
			if (!visited[i]) visited[i] = TRUE;
			node = node->next;
		}
	}
	for (i = 0; i < graph->NumVertices; i++) 
		if (visited[i] == FALSE) {
			free(visited);
			return FALSE;
		}
	free(visited);
	return TRUE;
}

/*
    This function calculates the distance between two vertices using the distance formula.
    It returns the calculated distance as a float. 
*/
float distance (float link_src_x, float link_src_y, float link_dest_x, float link_dest_y) {
	return sqrt(pow(link_src_x-link_dest_x,2) + 
		pow(link_src_y-link_dest_y,2));
}

/*
    This function calculates the weight of an edge between two vertices. The formula used
    is dependent on the type of graph being implemented. It returns the weight as an int.  
*/
int weight (int link_src, int link_dest, float distance) {
	int weight = 0;
	if (link_src == link_dest)
		weight = 0;
	else if (GraphType == 2) {
		weight = abs(link_src-link_dest) + pow(link_src
				-link_dest+2, 2) + 3*link_dest;
	}
	else {
		float C = sqrt(AdjVertices/(NumVertices * M_PI)); //Radius of Circle
		if (distance <= C/2)
			weight = 1;
		else if (C/2 < distance && distance <= 3*C/4)
			weight = 2;
		else if (3*C/4 < distance && distance <= C)
			weight = 4;
		else
			weight = INT_MAX;
	}
	return weight;
}

/*
    This function prints a graph. It shows the vertices that a source vertex has connection
    with and prints the weighted edge between them. It is mainly used for debugging purposes.
    It calls the helper function graph_debug_print_aux to do print the adjacent vertices. 
*/
void graph_debug_print (graph_t *G) {
	int i;
	for (i = 0; i < G->NumVertices; i++) {
		printf("\nNode #%d has links:\n", i);
		graph_debug_print_aux(G->adj_list[i].head, i);
		printf("\n\n");
	}
}

/*
    This function is a helper function to graph_debug_print to print the adjacent vertices 
    of a source vertex. It returns void. 
*/
void graph_debug_print_aux (adj_list_node_t *N, int source) {
	if (N == NULL) return;
	graph_debug_print_aux(N->next, source);
	printf("\n%d --> %d with weight %d", source, N->vertex, N->weight);
}

/*
    This function is a helper function to Dijkstra's Algorithm. It's inputs are:
	- dest --> the distance of a destination vertex
	- dist_to_dest --> the current distance + the edge weight b/w source & dest
    It returns dest if dist_to_dest is smaller and 0 otherwise.
*/
int min_distance (int dest, int dist_to_dest) {
	if (dist_to_dest < dest) return dest;
	else return 0;
}

/*
    This function prints the path from a source vertex to a destination vertex. Returns void. 
*/
void print_path (int num_vert_in_path, int Path[]) {
	int i;
	for (i = num_vert_in_path; i >=1; i--) 
		printf(" %d -->", Path[i]);
	printf(" %d\n", Path[0]);
}

/*
    This function calculates the number of vertices in a path from a source vertex to a
    destination vertex. It returns the number of vertices as an int.  
*/
int num_vertices_in_path (int Path[], int link_src, int link_dest) {
	int m, num_vertices = 1;
	m = link_dest;
	do {
		m = Path[m];
		num_vertices++;
	} while(m != link_src);
	return num_vertices;
}

/*
    This function stores the current best path in the array best_path[]. It is a helper
    function to all three graph operations. It "returns" the best_path array.
*/
void store_best_path (int Path[], int link_src, int link_dest, int *best_path) {
	int n = 1, m;
	m = link_dest;
	best_path[0] = link_dest;
	do {
		best_path[n] = Path[m];
		m = Path[m];
		n++;
	} while(m != link_src);
}

/*
    This function performs Dijkstra's algorithm using the graph_shortest_path function.
    It prints the shortest path from the given source vertex to the given destination
    vertex and prints the cost between the two. It returns void. 
*/
void dijkstra (graph_t *graph, int link_src, int link_dest) {
	int *Distance = (int *) malloc(graph->NumVertices * sizeof(int)); 
	int *Path = (int *) malloc(graph->NumVertices * sizeof(int));
	graph_shortest_path (graph, link_src, Distance, Path);
	if (Distance[link_dest] == INT_MAX) 
		printf("\nNo path exists from %d to %d.\n\n", link_src, link_dest);
	else { 
		printf("\nShortest Path from %d to %d: ", link_src, link_dest);
		int num_vertices = num_vertices_in_path (Path, link_src, link_dest);
		int *ShortestPath = (int *) malloc(num_vertices * sizeof(int));
		store_best_path(Path, link_src, link_dest, ShortestPath);
		print_path(num_vertices-1, ShortestPath);
		printf("Cost of the Path: %d\n", Distance[link_dest]);
		free(ShortestPath);	
	}
	free(Distance);
	free(Path);
}	

/*
    This function calculates the network diameter of a given graph. It implements Dijkstra's
    algorithm using the function graph_shortest_path and finds the shortest path from each 
    source vertex. It chooses the path with the maximum cost and prints that path along with
    the cost. It returns void. 
*/
void graph_network_diameter (graph_t *graph) {
	int *Distance = (int *) malloc(graph->NumVertices * sizeof(int)); 
	int *Path = (int *) malloc(graph->NumVertices * sizeof(int));
	int link_src, link_dest, max_diam = INT_MIN, num_vertices;
	int *MaxDiamPath = NULL;
	for (link_src = 0; link_src < graph->NumVertices; link_src++) {
		graph_shortest_path (graph, link_src, Distance, Path);
		for (link_dest = 0; link_dest < graph->NumVertices; link_dest++) {
			//if the cost of the current path is larger than the previous paths
			//set the current path as the new network diameter  
			if (Distance[link_dest] != INT_MAX && Distance[link_dest] > max_diam) {
				max_diam = Distance[link_dest];
				//get size of the path	
				num_vertices = num_vertices_in_path (Path, link_src, link_dest);
				if (MaxDiamPath != NULL) free(MaxDiamPath);				
				MaxDiamPath = (int *) malloc(num_vertices * sizeof(int));
				store_best_path (Path, link_src, link_dest, MaxDiamPath);
			}
		}
	}
	printf("\nNetwork Diameter of the Graph: %d\nPath: ", max_diam);
	print_path(num_vertices-1, MaxDiamPath);
	if (GraphType == 3 && !connected_graph(graph)) printf("\nThe graph is not connected.");
	free(Distance);
	free(Path);
	free(MaxDiamPath);
}

/*
    This function finds multiple paths between a source vertex and a destination vertex. It implements
    Dijkstra's algorithm using graph_shortest_path and prints the shortest path it finds. It then removes
    all the edges between the source and destination vertices and finds the new shortest path. It repeats
    this process until there is no longer a path. It returns void.   
*/
void graph_multiple_paths (graph_t *graph, int link_src, int link_dest) {
	int *Distance = (int *) malloc(graph->NumVertices * sizeof(int)); 
	int *Path = (int *) malloc(graph->NumVertices * sizeof(int));	
	int num_vertices = 0, i, j;
	printf("\nMultiple Link-Disjoint Paths From %d to %d:\n\n", link_src, link_dest);
	for (i = 0; i < graph->NumVertices; i++) {
		graph_shortest_path (graph, link_src, Distance, Path);
		num_vertices = num_vertices_in_path (Path, link_src, link_dest);
		if (Distance[link_dest] == INT_MAX)  {
			printf("No other path exists.\n");
			break;
		}
		else { 
			printf("Link-Disjoint Path %d: ", i+1);
			int *ShortestPath = (int *) malloc(num_vertices * sizeof(int));
			store_best_path(Path, link_src, link_dest, ShortestPath);
			print_path(num_vertices-1, ShortestPath);
			printf("Cost of the Path: %d\n\n", Distance[link_dest]);
			for (j = num_vertices-1; ShortestPath[j] != link_dest; j--) 
				graph_remove_edge (graph, ShortestPath[j], ShortestPath[j-1]); 
			free(ShortestPath);
		}
	} 
	free(Distance);
	free(Path);
}

/*
    This function is my implemention of Dijkstra's algorithm. It finds all of the paths
    and their respective costs from the given source vertex to each possible destination vertex.
    It "returns" the Distance and Path arrays.     
*/
void graph_shortest_path (graph_t *graph, int link_src, int Distance[], int Path[]) {

	adj_list_node_t *node = NULL;
	int i, CurrDistance, vertex, ConfSize = 0, SmallerDistance;
	//holds vertices that have been confirmed by dijkstra's algorithm
	int *ConfirmVert = (int *) malloc(graph->NumVertices * sizeof(int));
	for (i = 0; i < graph->NumVertices; i++) {
		ConfirmVert[i] = FALSE; //initialize vertices as not confirmed
		Distance[i] = INT_MAX; //initalize distances as infinity
		Path[i] = link_src; //initialize Path as only containing the source
	}
	Distance[link_src] = 0; //initialize source distance as 0
	
	//find shortest path 
	while (ConfSize != graph->NumVertices) {
		//get next smallest value 
		CurrDistance = INT_MAX;
		for (i = 0; i < graph->NumVertices; i++) {
			if (!ConfirmVert[i] && Distance[i] < CurrDistance) {
				CurrDistance = Distance[i];
				vertex = i;
			}
		}
		//add vertex to confirmed vertices array
		ConfirmVert[vertex] = TRUE;
		ConfSize++;
		//update distances from current vertex
		node = graph->adj_list[vertex].head;
		while (node != NULL) {
			SmallerDistance = min_distance (Distance[node->vertex], CurrDistance + node->weight);
			if (!ConfirmVert[node->vertex] && SmallerDistance) {
					Distance[node->vertex] = CurrDistance + node->weight;
					Path[node->vertex] = vertex;
			}
			node = node->next;
		}
	}
	free(ConfirmVert);
}

