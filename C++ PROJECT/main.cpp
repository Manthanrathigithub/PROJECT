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
struct Edge
{
    int dest;
    double weight; // distance in km
    double time;   // time in minutes

    Edge(int d, double w, double t = 0) : dest(d), weight(w), time(t ? t : w * 2) {}
};

// Structure to represent a location
struct Location
{
    string name;
    double x, y; // Coordinates (for display purposes)

    Location(string n = "", double px = 0, double py = 0) : name(n), x(px), y(py) {}
};

// Structure for the Graph
struct CityGraph
{
    int V; // Number of vertices
    vector<vector<Edge>> adjList;
    vector<Location> locations;
    unordered_map<string, int> nameToId;

    CityGraph(int vertices) : V(vertices), adjList(vertices), locations(vertices) {}

    void addLocation(int id, string name, double x = 0, double y = 0)
    {
        locations[id] = Location(name, x, y);
        nameToId[name] = id;
    }

    void addRoad(int src, int dest, double distance, double time = 0)
    {
        adjList[src].push_back(Edge(dest, distance, time));
        adjList[dest].push_back(Edge(src, distance, time));
    }

    int getLocationId(const string &name)
    {
        if (nameToId.find(name) != nameToId.end())
            return nameToId[name];
        return -1;
    }

    void displayLocations()
    {
        cout << "\n"
             << string(60, '=') << "\n";
        cout << "AVAILABLE LOCATIONS\n";
        cout << string(60, '=') << "\n";
        for (int i = 0; i < V; i++)
        {
            cout << "  [" << setw(2) << i << "] " << locations[i].name << "\n";
        }
        cout << string(60, '=') << "\n";
    }

    pair<vector<double>, vector<int>> dijkstra(int src, bool useTime = false)
    {
        vector<double> dist(V, numeric_limits<double>::infinity());
        vector<int> parent(V, -1);
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

        dist[src] = 0;
        pq.push(make_pair(0.0, src));

        while (!pq.empty())
        {
            double d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (d > dist[u])
                continue;

            for (size_t i = 0; i < adjList[u].size(); i++)
            {
                const Edge &edge = adjList[u][i];
                int v = edge.dest;
                double weight = useTime ? edge.time : edge.weight;

                if (dist[u] + weight < dist[v])
                {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        return make_pair(dist, parent);
    }

    vector<int> reconstructPath(int src, int dest, const vector<int> &parent)
    {
        vector<int> path;
        int node = dest;

        while (node != -1)
        {
            path.push_back(node);
            node = parent[node];
        }

        reverse(path.begin(), path.end());

        if (path.empty() || path[0] != src)
            return vector<int>();
        return path;
    }

    pair<double, double> calculatePathMetrics(const vector<int> &path)
    {
        double totalDist = 0;
        double totalTime = 0;

        for (size_t i = 0; i < path.size() - 1; i++)
        {
            for (size_t j = 0; j < adjList[path[i]].size(); j++)
            {
                const Edge &e = adjList[path[i]][j];
                if (e.dest == path[i + 1])
                {
                    totalDist += e.weight;
                    totalTime += e.time;
                    break;
                }
            }
        }

        return make_pair(totalDist, totalTime);
    }

    void displayRoute(const vector<int> &path, double totalDist, double totalTime,
                      string algorithm = "Dijkstra")
    {
        if (path.empty())
        {
            cout << "\nNo route found!\n";
            return;
        }

        cout << "\n"
             << string(70, '=') << "\n";
        cout << "ROUTE FOUND USING " << algorithm << "\n";
        cout << string(70, '=') << "\n\n";

        for (size_t i = 0; i < path.size(); i++)
        {
            cout << "  " << (i + 1) << ". " << locations[path[i]].name;

            if (i < path.size() - 1)
            {
                for (size_t j = 0; j < adjList[path[i]].size(); j++)
                {
                    const Edge &e = adjList[path[i]][j];
                    if (e.dest == path[i + 1])
                    {
                        cout << "\n     ↓ " << fixed << setprecision(1) << e.weight << " km";
                        cout << " (" << e.time << " min)\n";
                        break;
                    }
                }
            }
            else
                cout << "\n";
        }

        cout << "\n"
             << string(70, '=') << "\n";
        cout << "ROUTE SUMMARY\n";
        cout << string(70, '=') << "\n";
        cout << "  Total Distance: " << fixed << setprecision(2) << totalDist << " km\n";
        cout << "  Estimated Time: " << totalTime << " minutes\n";
        cout << "  Number of Stops: " << path.size() << " locations\n";
        cout << string(70, '=') << "\n\n";
    }
};

// Sample city initialization
void initializeSampleCity(CityGraph &city)
{
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

    city.addRoad(0, 1, 5.2, 12);
    city.addRoad(0, 2, 4.5, 10);
    city.addRoad(1, 7, 3.8, 9);
    city.addRoad(2, 3, 3.2, 8);
    city.addRoad(2, 5, 2.5, 6);
    city.addRoad(3, 4, 2.8, 7);
    city.addRoad(4, 5, 3.5, 8);
    city.addRoad(4, 8, 4.0, 10);
    city.addRoad(5, 6, 2.2, 5);
    city.addRoad(6, 7, 4.1, 10);
    city.addRoad(7, 9, 8.5, 20);
    city.addRoad(8, 9, 3.5, 8);
}

// Welcome banner
void displayWelcomeBanner()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "CITY ROUTE FINDER SYSTEM\n";
    cout << "Dijkstra's Shortest Path Algorithm\n";
    cout << string(70, '=') << "\n\n";
}

int main()
{
    CityGraph city(10);
    initializeSampleCity(city);

    displayWelcomeBanner();

    int choice;

    do
    {
        cout << "\nMAIN MENU\n";
        cout << string(40, '-') << "\n";
        cout << "1. View All Locations\n";
        cout << "2. Find Shortest Route (Distance)\n";
        cout << "3. Find Fastest Route (Time)\n";
        cout << "0. Exit\n";
        cout << string(40, '-') << "\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            city.displayLocations();
        }
        else if (choice == 2 || choice == 3)
        {
            int src, dest;
            city.displayLocations();

            cout << "\nEnter source location ID: ";
            cin >> src;
            cout << "Enter destination location ID: ";
            cin >> dest;

            if (src < 0 || src >= 10 || dest < 0 || dest >= 10)
            {
                cout << "\nInvalid location IDs!\n";
                continue;
            }
            if (src == dest)
            {
                cout << "\nSource and destination cannot be the same!\n";
                continue;
            }

            if (choice == 2)
            {
                pair<vector<double>, vector<int>> result = city.dijkstra(src, false);
                vector<double> dist = result.first;
                vector<int> parent = result.second;
                vector<int> path = city.reconstructPath(src, dest, parent);
                pair<double, double> metrics = city.calculatePathMetrics(path);
                double totalDist = metrics.first;
                double totalTime = metrics.second;
                city.displayRoute(path, totalDist, totalTime, "Dijkstra (Distance)");
            }
            else if (choice == 3)
            {
                pair<vector<double>, vector<int>> result = city.dijkstra(src, true);
                vector<double> dist = result.first;
                vector<int> parent = result.second;
                vector<int> path = city.reconstructPath(src, dest, parent);
                pair<double, double> metrics = city.calculatePathMetrics(path);
                double totalDist = metrics.first;
                double totalTime = metrics.second;
                city.displayRoute(path, totalDist, totalTime, "Dijkstra (Time)");
            }
        }
        else if (choice != 0)
        {
            cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 0);

    cout << "\n"
         << string(70, '=') << "\n";
    cout << "Thank you for using City Route Finder!\n";

    cout << string(70, '=') << "\n\n";

    return 0;
}