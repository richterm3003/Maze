
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

vector<vector<int>> maze;
vector<int> start= {2,2};
vector<int> goal= {4,0};

struct Edge {
    pair<int,int> destination;
    int cost;
};

stack<pair<int,int>> path;

map<pair<int, int>, vector<Edge>> graph;
map<pair<int, int>, bool> visited;
map<pair<int, int>, bool> mapped;

int rows, cols;
int firstCol = 2;
int firstRow = 2;
int goalCol = 0;
int goalRow = 4;
pair<int, int> originalPair = {firstRow, firstCol};
int startRow;
int startCol;
int curCol;
int curRow;
int leftValue;
int rightValue;
int upValue;
int downValue;
int smallestCost;
pair<int,int> smallestEdge = {0, 0};
pair<int,int> current;

map<pair<int,int>, int> djistrka(pair<int,int> src, map<pair<int,int>, pair<int,int>>& previous) {

    map<pair<int,int>, int> distance;
    using State = pair<int,pair<int,int>>;
    priority_queue<State, vector<State>, greater<State>> pq;


    distance[src] = 0;
    pq.push({0, src});

    while(!pq.empty()) {
        auto[d,u] = pq.top();
        pq.pop();

        if(d > distance[u]) continue;

        for(const auto& edge : graph[u]) {
            pair<int,int> v = edge.destination;
            int w = edge.cost;

            if (!distance.count(v)) {
                distance[v] = INT_MAX;
            }

            if (d+w < distance[v]) {
                distance[v] = d+w;
                previous[v] = u;
                pq.push({distance[v], v});
            }
        }
    }
    return distance;
}

void checkDirections(int row, int col) {
    startRow = row;
    startCol = col;
    curRow = row;
    curCol = col;
    leftValue = 0;
    rightValue = 0;
    upValue = 0;
    downValue = 0;
    current = {row, col};
    

    while (curCol - 1 >= 0 && maze[curRow][curCol - 1] == 0) {
        curCol--;
        leftValue++;
    }
    if(leftValue > 0) {
        graph[{startRow, startCol}].push_back({{curRow, curCol}, leftValue});
    }
    
     curRow = row;
     curCol = col;
    while (curCol + 1 < cols && maze[curRow][curCol + 1] == 0) {
        curCol++;
        rightValue++;
    }
    if(rightValue > 0) {
        graph[{startRow, startCol}].push_back({{curRow, curCol}, rightValue});
    }
     curRow = row;
     curCol = col;

    while (curRow - 1 >= 0 && maze[curRow - 1][curCol] == 0) {
        curRow--;
        upValue++;
    }
    if(upValue > 0) {
        graph[{startRow, startCol}].push_back({{curRow, curCol}, upValue}); 
    }
     curRow = row;
     curCol = col;

    while (curRow + 1 < rows && maze[curRow + 1][curCol] == 0) {
        curRow++;
        downValue++;
    }
    if(downValue > 0) {
        graph[{startRow, startCol}].push_back({{curRow, curCol}, downValue});
    }

}


int main() {

cout << "Enter number of rows and columns for the maze (Rows Cols): ";
cin >> rows >> cols;

maze.resize(rows, vector<int>(cols));

cout << "Enter the maze values (0 for open space, 1 for wall):" << endl;
for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
        cin >> maze[i][j];
    }
}



cout << "starting coordinates, (Row Col)" << endl;
cin >> firstRow >> firstCol;
cout << "goal coordinates, (Row Col)" << endl;
cin >> goalRow >> goalCol;


    pair<int, int> startPosition = {firstRow, firstCol};
    pair<int, int> goalPosition = {goalRow, goalCol};


    path.push(startPosition);
    visited[startPosition] = true;

    while (!path.empty()) {
        pair<int, int> current = path.top();

        if (!mapped[current]) {
            checkDirections(current.first, current.second);
            mapped[current] = true;      
        }

        bool foundNew = false;
        for (const auto& edge : graph[current]) {
            if (!visited[edge.destination]) {
                visited[edge.destination] = true;
                path.push(edge.destination);
                foundNew = true;
                break;
            }
   
        }
 if (!foundNew) {
        path.pop();
    }
}
for(const auto& edge: graph) {
    cout << "From (" << edge.first.first << ", " << edge.first.second << ") to: ";
    for(const auto& e : edge.second) {
        cout << "(" << e.destination.first << ", " << e.destination.second << ") with cost " << e.cost << "; ";
    }
    cout << endl;
}

map<pair<int,int>, pair<int,int>> previous;

map<pair<int,int>, int> distances = djistrka(startPosition, previous);

if (!distances.count(goalPosition) || distances[goalPosition] == INT_MAX) {
    cout << "Goal is unreachable \n";
return -1;
}

    vector<pair<int,int>> shortestPath;

    pair<int,int> current = goalPosition;

    shortestPath.push_back(current);

    while (current != startPosition) {

        if (!previous.count(current)) {
            cout << "Could not remake path.\n";
            return -1;
        }

        current = previous[current];

        shortestPath.push_back(current);
    }

    reverse(shortestPath.begin(), shortestPath.end());

    cout << "Start: (" << startPosition.first << ", " << startPosition.second << ")\n";

    cout << "Goal: (" << goalPosition.first << ", " << goalPosition.second << ")\n";

    cout << "\nRoute:\n";

    for (const auto& position : shortestPath) {

        cout << "(" << position.first << ", " << position.second << ")";

        if (position != goalPosition) {
            cout << " -> ";
        }
    }

    cout << "\n\nTotal Cost: " << distances[goalPosition] << endl;
return 0;

}
