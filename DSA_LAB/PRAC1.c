#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

// Structure to represent an edge
struct Edge {
    int dest;
    double weight;    // distance in km
    double time;      // time in minutes
    
    Edge(int d, double w, double t = 0) : dest(d), weight(w), time(t ? t : w * 2) {}
};

// Structure to represent a location
struct Location {
    string name;
    double x, y;  // Coordinates (for display purposes)
    
    Location(string n = "", double px = 0, double py = 0) : name(n), x(px), y(py) {}
};

// Structure for the Graph (using struct instead of class)
struct CityGraph {
    int V;  // Number of vertices
    vector<vector<Edge>> adjList;
    vector<Location> locations;
    unordered_map<string, int> nameToId;
    
    // Constructor
    CityGraph(int vertices) : V(vertices), adjList(vertices), locations(vertices) {}
    
    // Add a location (node)
    void addLocation(int id, string name, double x = 0, double y = 0) {
        locations[id] = Location(name, x, y);
        nameToId[name] = id;
    }
    
    // Add bidirectional road (edge)
    void addRoad(int src, int dest, double distance, double time = 0) {
        adjList[src].push_back(Edge(dest, distance, time));
        adjList[dest].push_back(Edge(src, distance, time));
    }
    
    // Get location ID by name
    int getLocationId(const string& name) {
        if (nameToId.find(name) != nameToId.end())
            return nameToId[name];
        return -1;
    }
    
    // Display all locations
    void displayLocations() {
        cout << "\n" << string(60, '=') << "\n";
        cout << "📍 AVAILABLE LOCATIONS\n";
        cout << string(60, '=') << "\n";
        for (int i = 0; i < V; i++) {
            cout << "  [" << setw(2) << i << "] " << locations[i].name << "\n";
        }
        cout << string(60, '=') << "\n";
    }
    
    // Dijkstra's Algorithm - Returns distances and parent array
    pair<vector<double>, vector<int>> dijkstra(int src, bool useTime = false) {
        vector<double> dist(V, numeric_limits<double>::infinity());
        vector<int> parent(V, -1);
        
        // Min-heap priority queue: pair<distance, vertex>
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
        
        dist[src] = 0;
        pq.push({0, src});
        
        while (!pq.empty()) {
            double d = pq.top().first;
            int u = pq.top().second;
            pq.pop();
            
            // Skip if we've already found a better path
            if (d > dist[u]) continue;
            
            // Explore neighbors
            for (const Edge& edge : adjList[u]) {
                int v = edge.dest;
                double weight = useTime ? edge.time : edge.weight;
                
                // Relaxation step
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
        
        return {dist, parent};
    }
    
    // BFS - Find any path (unweighted)
    vector<int> bfs(int src, int dest) {
        vector<bool> visited(V, false);
        vector<int> parent(V, -1);
        queue<int> q;
        
        q.push(src);
        visited[src] = true;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            if (u == dest) {
                vector<int> path;
                int node = dest;
                while (node != -1) {
                    path.push_back(node);
                    node = parent[node];
                }
                reverse(path.begin(), path.end());
                return path;
            }
            
            for (const Edge& edge : adjList[u]) {
                int v = edge.dest;
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        
        return vector<int>();  // No path found
    }
    
    // Reconstruct path from parent array
    vector<int> reconstructPath(int src, int dest, const vector<int>& parent) {
        vector<int> path;
        int node = dest;
        
        while (node != -1) {
            path.push_back(node);
            node = parent[node];
        }
        
        reverse(path.begin(), path.end());
        
        // Check if path is valid
        if (path.empty() || path[0] != src) {
            return vector<int>();
        }
        
        return path;
    }
    
    // Calculate actual time and distance for a path
    pair<double, double> calculatePathMetrics(const vector<int>& path) {
        double totalDist = 0;
        double totalTime = 0;
        
        for (size_t i = 0; i < path.size() - 1; i++) {
            // Find the edge between path[i] and path[i+1]
            for (const Edge& e : adjList[path[i]]) {
                if (e.dest == path[i + 1]) {
                    totalDist += e.weight;
                    totalTime += e.time;
                    break;
                }
            }
        }
        
        return {totalDist, totalTime};
    }
    
    // Display route with beautiful formatting
    void displayRoute(const vector<int>& path, double totalDist, double totalTime, 
                     string algorithm = "Dijkstra's Algorithm") {
        if (path.empty()) {
            cout << "\n❌ No route found!\n";
            return;
        }
        
        cout << "\n" << string(70, '=') << "\n";
        cout << "🗺️  ROUTE FOUND USING " << algorithm << "\n";
        cout << string(70, '=') << "\n\n";
        
        // Display path with distances
        for (size_t i = 0; i < path.size(); i++) {
            cout << "  " << (i + 1) << ". ";
            cout << "📍 " << locations[path[i]].name;
            
            if (i < path.size() - 1) {
                // Find the edge weight
                for (const Edge& e : adjList[path[i]]) {
                    if (e.dest == path[i + 1]) {
                        cout << "\n     ↓ " << fixed << setprecision(1) << e.weight << " km";
                        cout << " (" << e.time << " min)\n";
                        break;
                    }
                }
            } else {
                cout << "\n";
            }
        }
        
        cout << "\n" << string(70, '=') << "\n";
        cout << "📊 ROUTE SUMMARY\n";
        cout << string(70, '=') << "\n";
        cout << "  📏 Total Distance: " << fixed << setprecision(2) << totalDist << " km\n";
        cout << "  ⏱️  Estimated Time: " << totalTime << " minutes\n";
        cout << "  🚗 Number of Stops: " << path.size() << " locations\n";
        cout << string(70, '=') << "\n\n";
    }
    
    // Compare Dijkstra vs BFS
    void compareAlgorithms(int src, int dest) {
        cout << "\n" << string(70, '=') << "\n";
        cout << "📊 ALGORITHM COMPARISON: DIJKSTRA vs BFS\n";
        cout << string(70, '=') << "\n\n";
        
        // Run Dijkstra
        auto [dist, parent] = dijkstra(src, false);
        vector<int> dijkstraPath = reconstructPath(src, dest, parent);
        auto [dijkstraDist, dijkstraTime] = calculatePathMetrics(dijkstraPath);
        
        // Run BFS
        vector<int> bfsPath = bfs(src, dest);
        auto [bfsDist, bfsTime] = calculatePathMetrics(bfsPath);
        
        if (dijkstraPath.empty() || bfsPath.empty()) {
            cout << "❌ No path found between selected locations!\n";
            return;
        }
        
        // Display comparison
        cout << "🔴 DIJKSTRA'S ALGORITHM (Optimal - Shortest Distance):\n";
        cout << "   Distance: " << fixed << setprecision(2) << dijkstraDist << " km\n";
        cout << "   Time: " << dijkstraTime << " min\n";
        cout << "   Stops: " << dijkstraPath.size() << " locations\n";
        cout << "   Path: ";
        for (size_t i = 0; i < dijkstraPath.size(); i++) {
            cout << locations[dijkstraPath[i]].name;
            if (i < dijkstraPath.size() - 1) cout << " → ";
        }
        cout << "\n\n";
        
        cout << "🟣 BFS ALGORITHM (Simple - Any Path):\n";
        cout << "   Distance: " << fixed << setprecision(2) << bfsDist << " km\n";
        cout << "   Time: " << bfsTime << " min\n";
        cout << "   Stops: " << bfsPath.size() << " locations\n";
        cout << "   Path: ";
        for (size_t i = 0; i < bfsPath.size(); i++) {
            cout << locations[bfsPath[i]].name;
            if (i < bfsPath.size() - 1) cout << " → ";
        }
        cout << "\n\n";
        
        // Show savings
        if (dijkstraDist < bfsDist) {
            double savings = ((bfsDist - dijkstraDist) / bfsDist) * 100;
            cout << "✅ DIJKSTRA SAVES: " << fixed << setprecision(1) << savings << "% distance!\n";
            cout << "   (" << (bfsDist - dijkstraDist) << " km shorter)\n";
        } else if (dijkstraDist > bfsDist) {
            cout << "⚠️  BFS found a shorter path (rare case!)\n";
        } else {
            cout << "✅ Both algorithms found the same optimal path!\n";
        }
        
        cout << string(70, '=') << "\n\n";
    }
    
    // Find route with multiple stops
    void findRouteWithStops(const vector<int>& waypoints) {
        cout << "\n" << string(70, '=') << "\n";
        cout << "🛣️  MULTI-STOP ROUTE PLANNING\n";
        cout << string(70, '=') << "\n\n";
        
        double totalDist = 0;
        double totalTime = 0;
        vector<int> fullPath;
        
        for (size_t i = 0; i < waypoints.size() - 1; i++) {
            auto [dist, parent] = dijkstra(waypoints[i], false);
            vector<int> segment = reconstructPath(waypoints[i], waypoints[i + 1], parent);
            
            if (segment.empty()) {
                cout << "❌ Cannot reach from " << locations[waypoints[i]].name 
                     << " to " << locations[waypoints[i + 1]].name << "\n";
                return;
            }
            
            auto [segDist, segTime] = calculatePathMetrics(segment);
            totalDist += segDist;
            totalTime += segTime;
            
            if (fullPath.empty()) {
                fullPath = segment;
            } else {
                fullPath.insert(fullPath.end(), segment.begin() + 1, segment.end());
            }
        }
        
        displayRoute(fullPath, totalDist, totalTime, "Multi-Stop Dijkstra");
    }
};

// Initialize sample city (Ahmedabad themed)
void initializeSampleCity(CityGraph& city) {
    // Add locations with coordinates
    city.addLocation(0, "SG Highway", 0, 0);
    city.addLocation(1, "Maninagar", 5, 2);
    city.addLocation(2, "Paldi", 2, 4);
    city.addLocation(3, "Vastrapur", 1, 6);
    city.addLocation(4, "Satellite", 3, 8);
    city.addLocation(5, "Navrangpura", 4, 5);
    city.addLocation(6, "CG Road", 6, 6);
    city.addLocation(7, "Ashram Road", 7, 3);
    city.addLocation(8, "Sabarmati", 5, 9);
    city.addLocation(9, "Chandkheda", 8, 10);
    
    // Add roads (bidirectional with distance and time)
    city.addRoad(0, 1, 5.2, 12);   // SG Highway <-> Maninagar
    city.addRoad(0, 2, 4.5, 10);   // SG Highway <-> Paldi
    city.addRoad(1, 7, 3.8, 9);    // Maninagar <-> Ashram Road
    city.addRoad(2, 3, 3.2, 8);    // Paldi <-> Vastrapur
    city.addRoad(2, 5, 2.5, 6);    // Paldi <-> Navrangpura
    city.addRoad(3, 4, 2.8, 7);    // Vastrapur <-> Satellite
    city.addRoad(4, 5, 3.5, 8);    // Satellite <-> Navrangpura
    city.addRoad(4, 8, 4.0, 10);   // Satellite <-> Sabarmati
    city.addRoad(5, 6, 2.2, 5);    // Navrangpura <-> CG Road
    city.addRoad(6, 7, 4.1, 10);   // CG Road <-> Ashram Road
    city.addRoad(7, 9, 8.5, 20);   // Ashram Road <-> Chandkheda
    city.addRoad(8, 9, 3.5, 8);    // Sabarmati <-> Chandkheda
}

// Display welcome banner
void displayWelcomeBanner() {
    cout << "\n" << string(70, '=') << "\n";
    cout << "           🌆 CITY ROUTE FINDER SYSTEM 🌆\n";
    cout << "        Dijkstra's Shortest Path Algorithm\n";
    cout << string(70, '=') << "\n\n";
}

// Main menu
int main() {
    CityGraph city(10);
    initializeSampleCity(city);
    
    displayWelcomeBanner();
    
    int choice;
    
    do {
        cout << "\n📋 MAIN MENU\n";
        cout << string(40, '-') << "\n";
        cout << "1. View All Locations\n";
        cout << "2. Find Shortest Route (Distance)\n";
        cout << "3. Find Fastest Route (Time)\n";
        cout << "4. Compare Algorithms (Dijkstra vs BFS)\n";
        cout << "5. Plan Multi-Stop Route\n";
        cout << "0. Exit\n";
        cout << string(40, '-') << "\n";
        cout << "\nEnter your choice: ";
        cin >> choice;
        
        if (choice == 1) {
            city.displayLocations();
        }
        else if (choice >= 2 && choice <= 5) {
            int src, dest;
            city.displayLocations();
            
            cout << "\nEnter source location ID: ";
            cin >> src;
            cout << "Enter destination location ID: ";
            cin >> dest;
            
            if (src < 0 || src >= 10 || dest < 0 || dest >= 10) {
                cout << "\n❌ Invalid location IDs!\n";
                continue;
            }
            
            if (src == dest) {
                cout << "\n❌ Source and destination cannot be the same!\n";
                continue;
            }
            
            if (choice == 2) {
                // Shortest route by distance
                auto [dist, parent] = city.dijkstra(src, false);
                vector<int> path = city.reconstructPath(src, dest, parent);
                auto [totalDist, totalTime] = city.calculatePathMetrics(path);
                city.displayRoute(path, totalDist, totalTime, "Dijkstra's Algorithm (Distance)");
            }
            else if (choice == 3) {
                // Fastest route by time
                auto [dist, parent] = city.dijkstra(src, true);
                vector<int> path = city.reconstructPath(src, dest, parent);
                auto [totalDist, totalTime] = city.calculatePathMetrics(path);
                city.displayRoute(path, totalDist, totalTime, "Dijkstra's Algorithm (Time)");
            }
            else if (choice == 4) {
                // Compare algorithms
                city.compareAlgorithms(src, dest);
            }
            else if (choice == 5) {
                // Multi-stop route
                int numStops;
                cout << "\nEnter total number of stops (including source and destination): ";
                cin >> numStops;
                
                if (numStops < 2 || numStops > 10) {
                    cout << "❌ Invalid number of stops!\n";
                    continue;
                }
                
                vector<int> waypoints(numStops);
                waypoints[0] = src;
                waypoints[numStops - 1] = dest;
                
                for (int i = 1; i < numStops - 1; i++) {
                    cout << "Enter stop " << i << " location ID: ";
                    cin >> waypoints[i];
                    
                    if (waypoints[i] < 0 || waypoints[i] >= 10) {
                        cout << "❌ Invalid location ID!\n";
                        i--;
                    }
                }
                
                city.findRouteWithStops(waypoints);
            }
        }
        else if (choice != 0) {
            cout << "\n❌ Invalid choice! Please try again.\n";
        }
        
    } while (choice != 0);
    
    cout << "\n" << string(70, '=') << "\n";
    cout << "    👋 Thank you for using City Route Finder!\n";
    cout << "         Drive safely! 🚗💨\n";
    cout << string(70, '=') << "\n\n";
    
    return 0;
}