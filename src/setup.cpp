// All pairs minimum cut
// Bourantas Konstantinos 6145
// bourantas@ceid.upatras.gr

//In this program we try to solve all pairs min cut problem by using Gusfield's
//algorithm for implementing a Gomory-Hu tree. We also use Edmond's-Karp algorithm to calculate
//max-flow between two nodes.

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
// Returns true if there is a path from source 's' to sink 't' and fills pred[] to store the path
int bfs(node_array<int> &visited, node s, node t, node_array<edge> &pred, edge_array<int> &flow, edge_array<int> &capacity, const graph &G)
{

    // create an empty queue Q
    list<node> Q;
    node v;

    //mark all nodes as unvisited
    forall_nodes(v, G) visited[v] = 0;

    edge e;

    // insert s onto Q
    Q.append(s);

    while (!Q.empty())
    {
        node u = Q.pop();
        //mark u as visited
        visited[u] = 1;

        edge e;

        // Search all adjacent edges of node u
        forall_adj_edges(e, u)
        {

            v = G.target(e);

            // if v is not visited
            if ((visited[v] == 0) && (capacity[e] - flow[e] > 0))
            {

                //mark v as visited
                visited[v] = 1;

                // save pred of node
                pred[v] = e;

                // insert v onto Q
                Q.push(v);
            }
        }
    }

    //if nodes are unvisited it means that are at the other side of the cut
    //There still is an augmenting path left to follow
    return (visited[t] == 1);
}
//==================================================================================================================================

// bool check_maxflow_with_leda(int s, int t, node v[], int calculated_max_flow, edge_array<int> &capacity, const graph &G)
// {
//     std::cout << "Nodes: " << s << "->" << t << "\n";
//     edge_array<int> flow(G, 0);

//     integer max_flow = MAX_FLOW_T(G, v[s], v[t], capacity, flow);

//     if (calculated_max_flow - max_flow == 0)
//         std::cout << "\n\033[32m[+]Right calculation!\033[0m\n";
//     else
//         std::cout << "\033[1;31m[-]Wrong calculation!\033[0m\n\n";

//     std::cout << "Calculated max flow: " << calculated_max_flow;
//     std::cout << "\nActual max flow: " << max_flow;

//     return (calculated_max_flow == max_flow);
// }

//==================================================================================================================================
//Implentation of Edmondsï¿½Karp algorithm for finding min cut - max flow between two nodes
int find_max_flow(node_array<int> &visited, int index_s, int index_t, node v[], edge_array<edge> &rev_edge, edge_array<int> &capacity, const graph &G)
{
    edge_array<int> flow(G, 0);

    edge e;
    node w;

    //here we save the augmenting path
    node_array<edge> pred(G);

    // source
    node s = v[index_s];
    // sink
    node t = v[index_t];

    int max_flow = 0;

    //while augmenting path exists
    while (bfs(visited, s, t, pred, flow, capacity, G))
    {

        int bottleneck = INT_MAX;
        w = t;

        // Find the bottleneck value from augmenting path
        while (pred[w] != nil)
        {

            bottleneck = std::min(bottleneck, capacity[pred[w]] - flow[pred[w]]);
            w = G.source(pred[w]);
        }

        w = t;

        //Increment the flow
        while (pred[w] != nil)
        {
            //add calculated flow to the edge
            flow[pred[w]] = flow[pred[w]] + bottleneck;

            // if w isnt the source node
            if (rev_edge[pred[w]] != nil)
            {
                //substract flow from its reverse edge
                flow[rev_edge[pred[w]]] = flow[rev_edge[pred[w]]] - bottleneck;
            }
            else //error in augmenting path
            {
                std::cout << "Cant find rev_edge of ";
                G.print_edge(pred[w]);
            }

            w = G.source(pred[w]);
        }

        //add bottleneck to max flow
        max_flow += bottleneck;
    }

    return max_flow;
}

//==================================================================================================================================

//find the min-cut between a pair of nodes
int find_min_cut(int s, int t, node_array<int> &visited, node v[], edge_array<edge> &rev_edge, edge_array<int> &capacity, const graph &G)
{

    int min_cut = std::max(find_max_flow(visited, s, t, v, rev_edge, capacity, G), find_max_flow(visited, t, s, v, rev_edge, capacity, G));
    //printf("[%d] - [%d] has min-cut: %d\n", s, t, min_cut);

    return min_cut;
}

//==================================================================================================================================
//find for every edge its reverse
edge_array<edge> save_rev_edge(edge_array<int> &capacity, const graph &G)
{

    edge e;
    int count_edges = 0;
    edge_array<edge> rev_edge(G);
    node source_node, cur_node;

    forall_edges(e, G)
    {

        cur_node = target(e);
        source_node = source(e);

        edge e2;
        forall_edges(e2, G)
        {
            //edge e2 is the reverse of e
            if (source(e2) == cur_node && target(e2) == source_node)
            {
                //G.print_edge(e2);
                rev_edge[e] = e2;
                //in order for the algorithm to work we need to save the same capacity of the edge to its reverse edge.
                capacity[e2] = capacity[e];
                break;
            }
        }
    }
    return rev_edge;
}
//==================================================================================================================================
//Helper function for assign capacity values to edges
edge_array<int> set_capacities(const graph &G, int max_capacity, list<edge> rev_edges, int option)
{

    edge_array<int> capacity(G, 0);
    int count_edges = 0;

    option = 2;

    //Assign random values to the edges capacities
    srand(time(0));
    edge e;
    forall_edges(e, G)
    {
        //if edge is not the reverse
        if (rev_edges.search(e) == nil)
        {
            if (option == 2)
            {
                capacity[e] = (rand() % (max_capacity)) + 1;
            }
            else
            {
                std::cout << "\nEnter capacity for ";
                G.print_edge(e);
                std::cin >> capacity[e];
            }
        }
        else
            capacity[e] = 0;

        //std::cout << "[*]Assigning capacity " << capacity[e] << " to edge: ";
        //G.print_edge(e);
        //std::cout << "\n";
    }

    return capacity;
}

//==================================================================================================================================
//Check if the results are the same
void all_pair_mincut_checker(int num_nodes, int calculated_mincuts[], integer ledas_mincuts[])
{

    int wrong_calculations = 0;
    int right_calculations = 0;

    for (int i = 0; i < num_nodes * num_nodes; i++)
    {
        if (calculated_mincuts[i] - ledas_mincuts[i] == 0)
        {
            std::cout << "\n\033[32m[+]Right calculation!\033[0m\n";
            right_calculations++;
        }
        else
        {
            std::cout << "\033[1;31m[-]Wrong calculation!\033[0m\n\n";
            wrong_calculations++;
        }
        std::cout << "Calculated max flow: " << calculated_mincuts[i];
        std::cout << "\nActual max flow: " << ledas_mincuts[i];
    }

    std::cout << "\n========================\nRight Calculations: " << right_calculations << "\nWrong Calculations: " << wrong_calculations;
}

//==================================================================================================================================
//find min cut for all pairs of nodes from gomoryhu cut tree
void find_mincut_for_all_pairs(int num_nodes, int max_flow_from_gh[], node_array<int> &visited, node v[], edge_array<edge> &rev_edge, edge_array<int> &capacity, const graph &G)
{

    int min_cut_found;
    int index = 0;

    for (int node_i = 0; node_i < num_nodes; node_i++)
    {
        for (int node_j = 0; node_j < num_nodes; node_j++)
        {

            //if nodes are the same continue
            if (node_i == node_j)
            {
                max_flow_from_gh[index] = 0;
                index++;
                continue;
            }

            //calculated mincut for the pair
            min_cut_found = find_min_cut(node_i, node_j, visited, v, rev_edge, capacity, G);

            max_flow_from_gh[index] = min_cut_found;
            index++;
        }
    }
}

//==================================================================================================================================
//find all pairs max flow with leda's function/
void find_mincut_for_all_pairs_leda(int num_nodes, integer max_flow_leda[], node_array<int> &visited, node v[], edge_array<edge> &rev_edge, edge_array<int> &capacity, const graph &G)
{

    int index = 0;
    integer max_flow;

    for (int node_i = 0; node_i < num_nodes; node_i++)
    {
        for (int node_j = 0; node_j < num_nodes; node_j++)
        {

            //if nodes are the same continue
            if (node_i == node_j)
            {
                max_flow_leda[index] = 0;
                index++;
                continue;
            }

            edge_array<int> flow(G, 0);

            //calculated mincut for the pair
            max_flow = MAX_FLOW_T(G, v[node_i], v[node_j], capacity, flow);
            max_flow_leda[index] = max_flow;
            index++;
        }
    }
}

//==================================================================================================================================
//Gomory Hu tree construction
edge_array<edge> create_gomory_hu_tree(node_array<int> &visited, node v[], edge_array<edge> &rev_edge, edge_array<int> &capacity, edge_array<int> &new_capacity, graph &G, int num_nodes)
{

    int i, j, s, t, pos, min_cut;

    node source,
        sink;

    int p[num_nodes], f1[num_nodes];
    int cut_tree_capacities[num_nodes][num_nodes];

    for (i = 0; i < num_nodes; i++)
    {
        p[i] = 0;
        f1[i] = 0;
        for (j = 0; j < num_nodes; j++)
        {
            cut_tree_capacities[i][j] = 0;
        }
    }

    //////////////////////////////////////////////////////////
    for (s = 1; s < num_nodes; s++)
    {
        t = p[s];

        min_cut = find_max_flow(visited, s, t, v, rev_edge, capacity, G);

        f1[s] = min_cut;

        //Mark the nodes in the supernode with its representative
        for (i = 0; i < num_nodes; i++)
        {

            if (i != s && p[i] == t && visited[v[i]] == 1)
            {
                p[i] = s;
            }
        }

        //node with index p[t] belongs to the set of nodes on the s side
        if (visited[v[p[t]]] == 1)
        {
            p[s] = p[t];
            p[t] = s;
            f1[s] = f1[t];
            f1[t] = min_cut;
        }

        //Store the final cut tree when s is the last node of the input graph.
        if (s == num_nodes - 1)
        {
            for (i = 1; i <= s; i++)
            {

                cut_tree_capacities[i][p[i]] = f1[i];
                cut_tree_capacities[p[i]][i] = f1[i];
            }
        }
    }

    //reconstruct graph based on the resulted cut tree
    G.del_all_edges();

    for (int node_i = 0; node_i < num_nodes; node_i++)
    {
        for (int node_j = 0; node_j < num_nodes; node_j++)
        {

            if (node_i == node_j)
                continue;

            if (std::max(cut_tree_capacities[node_i][node_j], cut_tree_capacities[node_j][node_i]) > 0)
            {
                G.new_edge(v[node_i], v[node_j]);
            }
        }
    }

    //create id for all nodes
    node_array<int> node_id(G);
    int count = 0;
    node v1;

    forall_nodes(v1, G)
    {
        node_id[v1] = count++;
    }

    edge e1;

    //add the resulted capacities of the cut tree
    forall_edges(e1, G)
    {
        node i = G.source(e1);
        node j = G.target(e1);
        new_capacity[e1] = std::max(cut_tree_capacities[node_id[i]][node_id[j]], cut_tree_capacities[node_id[j]][node_id[i]]);
    }

    edge_array<edge> new_rev_edge;
    return new_rev_edge = save_rev_edge(new_capacity, G);
}
