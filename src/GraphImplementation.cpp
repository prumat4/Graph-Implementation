#include "GraphImplementation.hpp"

Graph::Graph(unsigned int _numVertices) : numVertices(_numVertices) {
    adjMatrix.reserve(numVertices + int(numVertices * 0.2));
    adjMatrix = std::vector<std::vector<int>>(numVertices, std::vector<int>(numVertices, 0));

    for(auto& row : adjMatrix)
        row.reserve(numVertices + int(numVertices * 0.2));
}

Graph::Graph(const Graph& graph) {
    numVertices = graph.numVertices;
    adjMatrix = graph.adjMatrix;
}

int Graph::getNumVertices() const {
    return numVertices;
}

std::vector<std::vector<int>> Graph::getAdjMatrix() const {
    return adjMatrix;
}

void Graph::removeEdge(int from, int to) {
    if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
        adjMatrix.at(from).at(to) = adjMatrix.at(to).at(from) = 0;
    }
}

bool Graph::containsEdge(int from, int to) const {
    if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
        return adjMatrix.at(from).at(to) != 0;
    }
    return false;
}

void Graph::removeVertex(int vertex) {
    if(vertex >= 0 && vertex <= numVertices) {
        adjMatrix.erase(adjMatrix.begin() + vertex);
        for(auto& row : adjMatrix)
            row.erase(row.begin() + vertex);
    }

    numVertices--;
} 

void Graph::addVertex() {
    adjMatrix.push_back(std::vector<int>(numVertices, 0));
    numVertices++;

    for(auto& row : adjMatrix) {
        row.push_back(0);
    }
}

void Graph::printAdjMatrix() const {
    for (const auto& row : adjMatrix) {
        for (const auto& edge : row) {
            std::cout << edge << "\t";
        }
        std::cout << std::endl;
    }
}

float Graph::calculateProbability(const float constant) {
    return (std::log(numVertices)  * constant) / numVertices;
}

UndirectedGraph::UndirectedGraph(unsigned int _numVertices) : Graph(_numVertices) {}

bool UndirectedGraph::isDirected() const {
    return false;
}

void UndirectedGraph::addEdge(const int from, const int to) {
    if (from == to)
        return;

    if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
        adjMatrix.at(from).at(to) = adjMatrix.at(to).at(from) = 1;
    }
}

void UndirectedGraph::ErdosRenyiModelGeneration(const float constant) {
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    float probability = calculateProbability(constant);

    for(int i = 0; i < numVertices; i++) {
        for(int j = i + 1; j < numVertices; j++){
            float randomValue = distribution(gen); 
            if(randomValue <= probability)  
                adjMatrix.at(i).at(j) = adjMatrix.at(j).at(i) = 1;
            else 
                adjMatrix.at(i).at(j) = adjMatrix.at(j).at(i) = 0;
        }
    }
}

std::vector<std::vector<int>> UndirectedGraph::getAdjList() const {
    std::vector<std::vector<int>> adjList(numVertices);

    for(int i = 0; i < numVertices; i++) {
        for(int j = 0; j < numVertices; j++) {
            if (adjMatrix.at(i).at(j) != 0) {
                adjList.at(i).push_back(j);
                adjList.at(j).push_back(i);
            }
        }
    }
    
    return adjList;
}

UndirectedWeightedGraph::UndirectedWeightedGraph(unsigned int _numVertices) : UndirectedGraph(_numVertices) {}

void UndirectedWeightedGraph::addEdge(const int from, const int to, const int weight) {
    if (from == to)
        return;

    if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
        adjMatrix.at(from).at(to) = adjMatrix.at(to).at(from) = weight;
    }
}

void UndirectedWeightedGraph::ErdosRenyiModelGeneration(const float constant) {
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_real_distribution<double> probabilityDistribution(0.0, 1.0);
    std::uniform_real_distribution<double> weightDistribution(MIN_WEIGHT, MAX_WEIGHT);
    float probability = calculateProbability(constant);

    for(int i = 0; i < numVertices; i++) {
        for(int j = i + 1; j < numVertices; j++){
            float randomValue = probabilityDistribution(gen); 
            if(randomValue <= probability)  
                adjMatrix.at(i).at(j) = adjMatrix.at(j).at(i) = weightDistribution(gen);
            else 
                adjMatrix.at(i).at(j) = adjMatrix.at(j).at(i) = 0;
        }
    }
}

DirectedGraph::DirectedGraph(unsigned int _numVertices) : Graph::Graph(_numVertices) {}

bool DirectedGraph::isDirected() const {
    return true;
}

void DirectedGraph::addEdge(const int from, const int to) {
    if (from == to)
        return;

    if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
        adjMatrix.at(from).at(to) = 1;
    }
}

void DirectedGraph::ErdosRenyiModelGeneration(const float constant) {
   std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    float probability = calculateProbability(constant);

    for(int i = 0; i < numVertices; i++) {
        for(int j = 0; j < numVertices; j++){
            float randomValue = distribution(gen); 
            if(randomValue <= probability && i != j)  
                adjMatrix.at(i).at(j) = 1;
            else 
                adjMatrix.at(i).at(j) = 0;
        }
    }
}

std::vector<std::vector<int>> DirectedGraph::getAdjList() const {
    std::vector<std::vector<int>> adjList(numVertices);

    for(int i = 0; i < numVertices; i++) {
        for(int j = 0; j < numVertices; j++) {
            if (adjMatrix.at(i).at(j) != 0) {
                adjList.at(i).push_back(j);
            }
        }
    }

    return adjList;
}

void DirectedGraph::DFSRecursive(int vertex, int& number, std::vector<bool> &isSelectedVertice, std::vector<int>& enumeration) {
    isSelectedVertice.at(vertex) = true;
    std::vector<bool> isVisitedVertice(numVertices, false);
    
    for(int i = 0; i < numVertices; i++) {
        if(adjMatrix.at(vertex).at(i) == 1 && isVisitedVertice.at(i) == true) {
            std::cout << "Error: There is a cycle in the graph! Exiting...\n";
            return;
        }
    }

    for(int i = 0; i < numVertices; i++) {
        if(adjMatrix.at(vertex).at(i) == 1 && isSelectedVertice.at(i) == false) {
            DFSRecursive(i, number, isSelectedVertice, enumeration);
        }
    }

    enumeration.at(vertex) = number;
    number--;
}

std::vector<int> DirectedGraph::DFSEnumeration() {
    std::vector<int> enumeration(numVertices, 0);
    std::vector<bool> isSelectedVertice(numVertices, false);

    int number = numVertices;

    for(int i = 0; i < numVertices; i++) {
        if(isSelectedVertice.at(i) == false) 
            DFSRecursive(i, number, isSelectedVertice, enumeration);
    }

    return enumeration;
}

bool DirectedGraph::isDag() {
    std::vector<bool> visited(numVertices, false);
    std::vector<bool> stack(numVertices, false);

    for (int i = 0; i < numVertices; i++) {
        if (!visited[i]) {
            if (isCyclic(i, visited, stack)) {
                return false; 
            }
        }
    }

    return true; 
}

bool DirectedGraph::isCyclic(int vertex, std::vector<bool>& visited, std::vector<bool>& stack) {
    visited[vertex] = true;
    stack[vertex] = true;

    for (int i = 0; i < numVertices; i++) {
        if (adjMatrix[vertex][i] == 1) {
            if (!visited[i]) {
                if (isCyclic(i, visited, stack)) {
                    return true;
                }
            } else if (stack[i]) {
                return true; 
            }
        }
    }

    stack[vertex] = false;
    return false;
}

std::vector<int> DirectedGraph::getIndegs() {
    std::vector<int> indegs(numVertices, 0);

    for(int j = 0; j < indegs.size(); j++) {
        int indeg = 0;
        for(int i = 0; i < indegs.size(); i++) {
            indeg += adjMatrix.at(i).at(j);    
        }
        indegs.at(j) = indeg;
    }

    return indegs;
}

void DirectedGraph::correctDifference(std::vector<int>& vec, const int pos) {
    for(int i = 0; i < numVertices; i++) {
        vec.at(i) -= adjMatrix.at(pos).at(i);
    }
}

std::vector<int> DirectedGraph::DemukronsAlgorithm() {
    std::vector<int> enumeration(numVertices, 0);    
    auto indegs = getIndegs();
    
    std::set<int> verticesToRemove;
    std::set<int> vertices;
    for(int i = 0; i < numVertices; i++) 
        vertices.insert(i);

    int number = 1;

    while(!vertices.empty()) {
        for(int i = 0; i < numVertices; i++) {
            if(vertices.contains(i) && indegs.at(i) == 0) {
                enumeration.at(i) = number;
                number++;
                verticesToRemove.insert(i);
                correctDifference(indegs, i);
            }
        }

        for(const auto& ver : verticesToRemove)
            vertices.erase(ver);
    }    

    return enumeration;
}

DirectedWeightedGraph::DirectedWeightedGraph(unsigned int _numVertices) : DirectedGraph(_numVertices) {}

void DirectedWeightedGraph::addEdge(const int from, const int to, const int weight) {
    if (from == to)
        return;

    if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
        adjMatrix.at(from).at(to) = weight;
    }
}

void DirectedWeightedGraph::ErdosRenyiModelGeneration(const float constant) {
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_real_distribution<double> probabilityDistribution(0.0, 1.0);
    std::uniform_real_distribution<double> weightDistribution(MIN_WEIGHT, MAX_WEIGHT);
    float probability = calculateProbability(constant);

    for(int i = 0; i < numVertices; i++) {
        for(int j = 0; j < numVertices; j++){
            float randomValue = probabilityDistribution(gen); 
            if(randomValue <= probability && i != j)  
                adjMatrix.at(i).at(j) = weightDistribution(gen);
            else 
                adjMatrix.at(i).at(j) = 0;
        }
    }
}