#pragma once

// HEADER FILES
#include <iostream>
#include <stack>
#include <queue>
#include <functional>
#include <unordered_set>
#include <map>
#include <limits>
#include "resources.hpp"
#include <SDL2/SDL.h>

extern bool disable;
using namespace std;

#define CHECK_ERROR(condition, err_msg)        \
    {                                          \
        if (!disable)                          \
            std::cerr << err_msg << std::endl; \
        if (condition)                         \
            return;                            \
    }

using VertexId = int;
using Weight = double;

constexpr Weight INF = std::numeric_limits<Weight>::infinity();
constexpr int NO_PRED = -1;
constexpr int NO_VERTEX = -1;

struct EdgeNode
{
    VertexId from;
    VertexId to;
    Weight weight;
};

struct VertexAttribute
{
    // Dijsktra Attribute
    Weight distance = 0.0;
    bool visited = false;
    VertexId pred = NO_PRED;
};

template <typename T>
using EdgeList = std::map<VertexId, EdgeNode>;

template <typename VertexType>
struct VertexNode
{
    VertexId id = 0;
    VertexType data;
    VertexAttribute attributes;
    EdgeList<EdgeNode> edgeList;

    VertexNode() : data(VertexType()) {}
    VertexNode(VertexId id, VertexType data) : id(id), data(data) {}
};

template <typename T>
using VertexMap = std::map<VertexId, VertexNode<T>>;

template <typename T>
struct Graph;

template <typename Node>
struct Compare
{
    Graph<Node> &graph;

    Compare(Graph<Node> &graph) : graph(graph) {}

    bool operator()(VertexId const u, VertexId const v)
    {
        return graph.getDistance(u) > graph.getDistance(v);
    }
};

template <class Node>
struct Graph
{
    VertexMap<Node> vertexMap;
    VertexId sourceVertex = 0;
    VertexId currVertex = 0;
    VertexId targetVertex = nrVertices - 1;
    size_t nrVertices;

    Graph() = default;

    Weight &getDistance(VertexId id)
    {
        return vertexMap[id].attributes.distance;
    }

    Node &getVertex(VertexId id)
    {
        return vertexMap[id].data;
    }

    VertexId getSourceVertex()
    {
        return sourceVertex;
    }

    VertexId getTargetVertex()
    {
        return targetVertex;
    }

    void setSourceVertex(VertexId id)
    {
        sourceVertex = id;
    }

    void setTargetVertex(VertexId id)
    {
        targetVertex = id;
    }

    bool &isVisited(VertexId id)
    {
        return vertexMap[id].attributes.visited;
    }

    VertexId &getPredecessor(VertexId id)
    {
        return vertexMap[id].attributes.pred;
    }

    Weight &getEdgeCost(VertexId u, VertexId v)
    {
        return vertexMap[u].edgeList[v].weight;
    }

    void addVertex(VertexId id, const Node &vertexData)
    {
        CHECK_ERROR(vertexMap.find(id) != vertexMap.end(),
                    "Vertex already exists");

        VertexNode<Node> newVertex(id, vertexData);
        vertexMap.insert(std::make_pair(id, newVertex));
    }

    void removeVertex(VertexId vertexId)
    {
        CHECK_ERROR(vertexMap.find(vertexId) == vertexMap.end(),
                    "Vertex does not exist");

        EdgeList<Node> edgeList = vertexMap[vertexId].edgeList;

        for (auto edgeNode : edgeList)
            removeEdge(edgeNode.second.from, edgeNode.second.to);

        edgeList.erase(vertexId);
    }

    void addEdge(VertexId from, VertexId to, Weight w = 1.0)
    {
        CHECK_ERROR(vertexMap.find(from) == vertexMap.end(),
                    "Source Vertex does not exits");

        CHECK_ERROR(vertexMap.find(to) == vertexMap.end(),
                    "target Vertex does not exits");

        EdgeList<Node> &edgeListFrom = vertexMap[from].edgeList;
        EdgeList<Node> &edgeListTo = vertexMap[to].edgeList;

        CHECK_ERROR(edgeListFrom.find(to) != edgeListFrom.end(),
                    "Edge already exists");

        edgeListFrom[to] = {from, to, w};
        edgeListTo[from] = {to, from, w};
    }

    void removeEdge(VertexId from, VertexId to)
    {
        CHECK_ERROR(vertexMap.find(from) == vertexMap.end(),
                    "Source Vertex does not exits");

        CHECK_ERROR(vertexMap.find(to) == vertexMap.end(),
                    "target Vertex does not exits");

        EdgeList<Node> &edgeListFrom = vertexMap[from].edgeList;
        EdgeList<Node> &edgeListTo = vertexMap[to].edgeList;

        CHECK_ERROR(edgeListFrom.find(to) == edgeListFrom.end() or
                        edgeListTo.find(from) == edgeListTo.end(),
                    "Edge does not exists");

        edgeListFrom.erase(to);
        edgeListTo.erase(from);
    }

    void show(std::string msg = "")
    {
        if (!msg.empty())
            std::cout << msg << std::endl;

        for (auto vertex : vertexMap)
        {
            std::cout << "[" << vertex.second.id << "]\t<--->\t";

            for (auto edge : vertex.second.edgeList)
                std::cout << "[" << edge.second.to << "]<->";

            std::cout << "[END]" << std::endl;
        }
        std::cout << std::endl;
    }

    void resetVertexAttribute(VertexId sourceId = NO_VERTEX)
    {
        for (auto &vertex : vertexMap)
        {
            vertex.second.attributes.distance = INF;
            vertex.second.attributes.visited = false;
            vertex.second.attributes.pred = NO_PRED;
        }

        if (sourceId != NO_VERTEX)
            vertexMap[sourceId].attributes.distance = 0.0;
    }

    void reconstructPath(VertexId targetId, std::function<void()> renderCallBack)
    {
        std::stack<VertexId> stack;
        stack.push(targetId);

        while (stack.top() != NO_PRED)
        {
            VertexId pred = getPredecessor(stack.top());
            stack.push(pred);
        }

        while (!stack.empty())
        {
            currVertex = stack.top();
            renderCallBack();
            stack.pop();
        }
    }

    void relaxEdge(VertexId u, VertexId v, Weight w)
    {
        if (getDistance(v) > getDistance(u) + w)
        {
            getDistance(v) = getDistance(u) + w;
            getPredecessor(v) = u;
        }

        if (getDistance(u) > getDistance(v) + w)
        {
            getDistance(u) = getDistance(v) + w;
            getPredecessor(u) = v;
        }
    }

    void dijkstra(VertexId sourceId, VertexId targetId)
    {
        resetVertexAttribute(sourceId);

        Compare<Node> compare(*this);

        std::priority_queue<VertexId, std::vector<VertexId>, decltype(compare)> minDistance(compare);

        for (auto vertex : vertexMap)
            minDistance.push(vertex.first);

        while (!minDistance.empty())
        {
            VertexId currId = minDistance.top();
            minDistance.pop();

            for (auto edge : vertexMap[currId].edgeList)
                relaxEdge(edge.second.from, edge.second.to, edge.second.weight);

            if (currId == targetId)
                break;
        }
    }

    void dfs(VertexId sourceId, VertexId targetId, std::function<void()> renderCallBack)
    {

        resetVertexAttribute();
        std::stack<VertexId> stack;
        stack.push(sourceId);

        while (!stack.empty())
        {
            VertexId curr = stack.top();
            stack.pop();

            if (!isVisited(curr))
            {
                isVisited(curr) = true;

                currVertex = curr;
                renderCallBack();

                if (curr == targetId)
                    break;

                for (auto &edge : vertexMap[curr].edgeList)
                {
                    if (!isVisited(edge.second.to))
                    {
                        stack.push(edge.second.to);
                    }
                }
            }
        }
    }

    void bfs(VertexId sourceId, VertexId targetId, std::function<void()> callback)
    {
        std::queue<VertexId> queue;
        resetVertexAttribute();
        queue.push(sourceId);

        while (!queue.empty())
        {
            VertexId curr = queue.front();

            queue.pop();

            if (!isVisited(curr))
            {
                isVisited(curr) = true;

                currVertex = curr;
                callback();

                if (curr == targetId)
                    break;

                for (auto &edge : vertexMap[curr].edgeList)
                {

                    if (!isVisited(edge.second.to))
                    {
                        queue.push(edge.second.to);
                    }
                }
            }
        }
    }
};
