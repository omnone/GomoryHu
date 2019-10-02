// All pairs minimum cut
// Bourantas Konstantinos 

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
#include <LEDA/graph/templates/max_flow.h>
#include <LEDA/numbers/integer.h>
#include <LEDA/graph/min_cut.h>

#include <iostream>
#include <ctime>
#include <fstream>

#include "setup.h"

using namespace leda;
using std::endl;

//==================================================================================================================================
//helper function for printing the generated graph
void print_graph(const graph &G, edge_array<int> capacity)
{

    node_array<int> A(G);

    edge e;
    node v;

    int i = 0;

    forall_nodes(v, G) A[v] = i++;

    // write graph to file
    std::cout << "digraph G{"
              << "\n";

    forall_edges(e, G)
    {
        std::cout << A[G.source(e)] << "->" << A[G.target(e)] << " \n";
    }

    std::cout << "}"
              << "\n";

    forall_edges(e, G)
    {
        G.print_edge(e);
        std::cout << "has capacity " << capacity[e] << "\n";
    }
}

//==================================================================================================================================
//Main function
int main()
{
    graph G;

    std::cout << "\n==============================\nFind all pairs min-cut:\n==============================\n"
              << endl;

    //Graph Construction

    //Generate a random graph
    //----------------------------------------------------

    // for (int k = 0; k < 4; k++)
    // {
    int num_nodes = 100; //total number of nodes
    int num_edges = 200; //total number of edges
    node v[num_nodes];

    node temp_node;
    int i = 0;

    random_simple_undirected_graph(G, num_nodes, num_edges);
    //random_graph(G, num_nodes, num_edges);

    forall_nodes(temp_node, G)
    {
        v[i] = temp_node;

        //G.print_node(v[i]);

        i++;
    }

    //Example graph
    //----------------------------------------------------
    // int num_nodes = 6;

    // node v[num_nodes];

    // for (int i = 0; i < num_nodes; i++)
    // {
    //     v[i] = G.new_node();
    // }

    // G.new_edge(v[0], v[1]);
    // G.new_edge(v[0], v[2]);
    // G.new_edge(v[1], v[3]);
    // G.new_edge(v[1], v[2]);
    // G.new_edge(v[1], v[4]);
    // G.new_edge(v[2], v[4]);
    // G.new_edge(v[3], v[4]);
    // G.new_edge(v[3], v[5]);
    // G.new_edge(v[4], v[5]);

    list<edge> residual_edges;

    G.make_bidirected(residual_edges);

    //End of graph construction/////////////////////////////////////////

    edge_array<int> capacity(G, 0); //array to store edges capacities

    node_array<int> color(G, 0);

    //the maximum capacity for the edges
    int max_capacity = 0;

    //std::cout << "Enter the maximum edge capacity: ";
    //std::cin >> max_capacity;
    //std::cin.ignore();
    max_capacity = 50;

    //set capacities for all edges
    capacity = set_capacities(G, max_capacity, residual_edges, 2);

    //find and save the residual edge for every edge
    edge_array<edge> rev_edge = save_rev_edge(capacity, G);

    //Time this---------------------------------------------------------------------------------------------
    //find all pairs mincut with leda
    double time_elapsed_leda = 0;

    clock_t begin = clock();

    integer max_flow_leda[num_nodes * num_nodes];
    find_mincut_for_all_pairs_leda(num_nodes, max_flow_leda, color, v, rev_edge, capacity, G);

    clock_t end = clock();
    time_elapsed_leda = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Time elapsed for leda: " << time_elapsed_leda << "s.\n";

    //---------------------------------------------------------------------------------------------

    //Time this---------------------------------------------------------------------------------------------
    //gomory hu tree construction

    double time_elapsed_gomoryhu = 0;
    edge_array<int> new_capacity(G, 0);

    //find all pairs mincut with gomoryhu tree
    begin = clock();

    edge_array<edge> new_rev_edge = create_gomory_hu_tree(color, v, rev_edge, capacity, new_capacity, G, num_nodes);
    int max_flow_mine[num_nodes * num_nodes];

    find_mincut_for_all_pairs(num_nodes, max_flow_mine, color, v, new_rev_edge, new_capacity, G);

    end = clock();
    time_elapsed_gomoryhu = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Time elapsed for gomoryhu: " << time_elapsed_gomoryhu << "s.\n";
    //---------------------------------------------------------------------------------------------

    //check if the results are correct
    all_pair_mincut_checker(num_nodes, max_flow_mine, max_flow_leda);

    //Write results to a file
    //std::ofstream ofs;
    //ofs.open("test.txt", std::ofstream::out | std::ofstream::app);

    //ofs << "\nNodes: " << num_nodes << "\n";
    //ofs << "leda: " << time_elapsed_leda << "\n"
    //   << "gomoryhu: " << time_elapsed_gomoryhu;

    //ofs.close();
    // }

    return 0;
}
