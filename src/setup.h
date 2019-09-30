// All pairs minimum cut
// Bourantas Konstantinos 6145
// bourantas@ceid.upatras.gr

//In this program we try to solve all pairs min cut problem by using Gusfield's
//algorithm for implementing a Gomory-Hu tree. We also use Edmond's-Karp algorithm to calculate
//max-flow between two nodes.

//==================================================================================================================================
#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_pq.h>
#include <LEDA/core/string.h>
#include <LEDA/core/stack.h>
#include <LEDA/graph/node_list.h>
#include <LEDA/system/basic.h>
#include <iostream>
#include <LEDA/graph/templates/max_flow.h>
#include <LEDA/numbers/integer.h>
#include <LEDA/graph/min_cut.h>

using namespace leda;

//==================================================================================================================================
// BFS Function
int bfs(node_array<int> &color, node s, node t, node_array<edge> &pred, edge_array<int> &flow, edge_array<int> &capacity, const graph &G);
//==================================================================================================================================
//check if max flow is correct
bool is_max_flow(int s, int t, node v[], int calculated_max_flow, edge_array<int> &capacity, const graph &G);
//==================================================================================================================================
//Implentation of Edmonds–Karp algorithm for finding min cut - max flow between two nodes
int find_max_flow(node_array<int> &color, int index_s, int index_t, node v[], edge_array<edge> &rev_edge, edge_array<int> &capacity, const graph &G);
//==================================================================================================================================
//find the min-cut between a pair of nodes
int find_min_cut(int cut_tree[][10], int s, int t, node_array<int> &color, node v[], edge_array<edge> &rev_edge, edge_array<int> &capacity, const graph &G);
//==================================================================================================================================
//we save for every edge its reverse
edge_array<edge> save_rev_edge(edge_array<int> &capacity, const graph &G);
//==================================================================================================================================
//Helper function for adding the desired capacities values
edge_array<int> set_capacities(const graph &G, int max_capacity, list<edge> residual_edges, int option);
//==================================================================================================================================
//Gomory Hu tree construction
edge_array<edge> create_gomory_hu_tree(node_array<int> &color, node v[], edge_array<edge> &rev_edge, edge_array<int> &capacity, edge_array<int> &new_capacity, graph &G, int num_nodes);
//==================================================================================================================================
//check if calculated max flow for all pairs are the same with the leda's results
void all_pair_mincut_checker(int num_nodes, int calculated_mincuts[], integer ledas_mincuts[]);
//==================================================================================================================================
void find_mincut_for_all_pairs(int num_nodes, int max_flow_mine[], node_array<int> &color, node v[], edge_array<edge> &rev_edge, edge_array<int> &capacity, const graph &G);
//==================================================================================================================================
void find_mincut_for_all_pairs_leda(int num_nodes, integer max_flow_leda[], node_array<int> &color, node v[], edge_array<edge> &rev_edge, edge_array<int> &capacity, const graph &G);
//==================================================================================================================================
