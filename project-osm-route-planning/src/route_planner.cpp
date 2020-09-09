#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.

    // Find node cloest to starting location
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    // Find node closest to end location
    end_node = &m_Model.FindClosestNode(end_x, end_y);

}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {

    // Calcualte distance between current node and end_node
    //std::cout << node->distance(*end_node) << "\n";
    return node->distance(*end_node);

}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {

    // Find all neighbors for the current node
    current_node->FindNeighbors();

    // Iterate over all the neighbors of the current node
    for(auto neighbor : current_node->neighbors) {

        // Set the parent of the neighbor to the current node
        neighbor->parent = current_node;
        // Calculate the G-Value of the neighbor
        neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
        // Calculate the H-value of the neighbor
        neighbor->h_value = CalculateHValue(neighbor);

        // Add the neighbor to the open list
        open_list.push_back(neighbor);

        // Flag the neighbor as visited and calculated
        neighbor->visited = true;

        // Debug
        //std::cout << "Debug: AddNeighbors: " << open_list.size() << " " << neighbor->g_value << " " << neighbor->h_value << "\n";
    }

}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

// Helper function according to Udacity class videos about A*Search algorithm to sort list
bool compare_entries(RouteModel::Node *first, RouteModel::Node *second) {
    
    float compare_first = first->g_value + first->h_value;
    float compare_second = second->g_value + second->h_value;

    return compare_first > compare_second;
}

RouteModel::Node *RoutePlanner::NextNode() {

    // Sort the list according to G and H values
    std::sort(open_list.begin(), open_list.end(), compare_entries);

    // Pointer to the node in the list with the lowest sum
    RouteModel::Node *lowestSum = open_list.back();

    // Debug
    //std::cout << "Debug: NextNode: " << open_list.size() << "\n";

    // Erase that node from the open_list
    //open_list.erase(open_list.begin());
    open_list.pop_back();

    // Debug
     //std::cout << "Debug: NextNode: " << open_list.size() << "\n";

    // Return pointer
    return lowestSum;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.

    // Path starts with first node
    path_found.push_back(*current_node);

    // Loop as long as we have parents in the list
    while(current_node !=start_node) {

        // TotalDistance is the sum of distances between each nodes
        distance += current_node->distance(*current_node->parent);

        // Move on to next node in path
        current_node = current_node->parent;

        // Start the path with the first node
        path_found.push_back(*current_node);

        // Debug
        //std::cout << "Debug Construct Path: " << path_found.size() << " " << distance << "\n";
    }

    // Reverse order of nodes in path
    std::reverse(path_found.begin(), path_found.end());

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.

    // Search starts on first node
    start_node->visited = true;
    open_list.push_back(start_node);

    // Loop over all neighbors
    while (!open_list.empty()) {
        
        // Check next node
        current_node = NextNode();

        // Check if we have reached end node
        if (current_node == end_node) {
            // Construct final path and return it
            m_Model.path = ConstructFinalPath(current_node);

            // Debug
            //std::cout << "Debug AStar Path: " << m_Model.path.size() << "\n";
            return;
        }

        // Add neighbors of node and continue
        AddNeighbors(current_node);
    }

}