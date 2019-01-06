#include <iostream>
#include <vector>
#include <queue>
//#include <cmath>

/*
 * Про алгоримтм Диница.
 * Сеть устроена таким образом, количество исходяших
 * из истока и входящих в сток ребер в точности равна |V|.
 * между полезными темами и бесполезными ребра имеют
 * бесконечную емкость, откуда следует, что они никода не
 * насыщяются. Следовательно, в процессе поиска очередного
 * потока насыщается только одно ребро, либо исходящее из
 * истока, либо входящее в сток. Отсюда асимптотика поиска
 * блокирующего потока сводится к O(|E|), а, значит,
 * асимптотика работы алгоритка для нашей сети составляет
 * O(|V||E|) <= O(|V|^3)
 */

using uint32 = unsigned int;
using TFlow = int;

const TFlow INF = INT32_MAX;

struct Edge {
    uint32 start, finish;
    TFlow capacity, flow;

    Edge(uint32 start, uint32 finish, TFlow capacity, TFlow flow) :
            start(start),
            finish(finish),
            capacity(capacity),
            flow(flow) {}

};

struct Vertex {
    std::vector<uint32> edges;
    TFlow extraFlow, inFlow, outFlow;
    uint32 height, nextEdge;
    int level;
    bool isBlocked;


    Vertex() :
            level(-1),
            isBlocked(false) {}
};

class Network {

protected:

    std::vector<Edge> edges_;
    std::vector<Vertex> graph_;
    uint32 vertexNumber_, source_, sink_;

public:

    virtual ~Network() = default;

    explicit Network(uint32 n) :
            vertexNumber_(n),
            source_(0),
            sink_(n - 1) {
        graph_.assign(n, Vertex());
    }

    void addEdgeLocal(uint32 start, uint32 finish, TFlow capacity) {
        graph_.at(start).edges.emplace_back(edges_.size());
        edges_.emplace_back(start, finish, capacity, 0);
        graph_.at(finish).edges.emplace_back(edges_.size());
        edges_.emplace_back(finish, start, 0, 0);
    }

    void setSource(uint32 source) {
        source_ = source;
    }

    void setSink(uint32 sink) {
        sink_ = sink;
    }

    uint32 getSource() const {
        return source_;
    }

    uint32 getSink() const {
        return sink_;
    }

    uint32 getVertNum() const {
        return vertexNumber_;
    }

    void clearFlows() {
        for (auto& edge : edges_)
            edge.flow = 0;
    }

    std::vector<Vertex>& getGraph() {
        return graph_;
    }

    std::vector<Edge>& getEdges() {
        return edges_;
    }
};


struct MaxFlow {

    TFlow maxFlow;

public:

    explicit MaxFlow(TFlow maxF) : maxFlow(maxF) {}
};

class FlowFinder {

protected:

    Network& network_;
    TFlow maxFlow_;

    virtual TFlow findMaxFlow() = 0;

public:

    virtual ~FlowFinder() = default;

    explicit FlowFinder(Network& network) : network_(network), maxFlow_(0) {}

    MaxFlow getMaxFlow() {
        return MaxFlow(findMaxFlow());
    }

};

class PushRelabel : public FlowFinder {

    std::vector<uint32> list_;

    void push(uint32 u, uint32 v) {
        uint32 edgeID = 0;
        for (uint32 i = 0; i < network_.getGraph().at(u).edges.size(); ++i)
            if (network_.getEdges().at(network_.getGraph().at(u).edges.at(i)).finish == v) {
                edgeID = network_.getGraph().at(u).edges.at(i);
                break;
            }
        TFlow send = std::min(network_.getGraph().at(u).extraFlow,
                              network_.getEdges().at(edgeID).capacity - network_.getEdges().at(edgeID).flow);
        network_.getEdges().at(edgeID).flow += send;
        network_.getEdges().at(edgeID ^ 1).flow = -network_.getEdges().at(edgeID).flow;
        network_.getGraph().at(u).extraFlow -= send;
        network_.getGraph().at(v).extraFlow += send;
    }

    void relabel(uint32 u) {
        uint32 minHeight = INF;
        for (uint32 i = 0; i < network_.getGraph().at(u).edges.size(); ++i) {
            uint32 edgeID = network_.getGraph().at(u).edges.at(i);
            if (network_.getEdges().at(edgeID).capacity -
                network_.getEdges().at(edgeID).flow > 0) {
                minHeight = std::min(minHeight, network_.getGraph().at(network_.getEdges().at(edgeID).finish).height);
            }
        }
        network_.getGraph().at(u).height = minHeight + 1;
    };

    void discharge(uint32 u) {
        while (network_.getGraph().at(u).extraFlow > 0) {
            if (network_.getGraph().at(u).nextEdge < network_.getGraph().at(u).edges.size()) {
                uint32 edgeID = network_.getGraph().at(u).edges.at(network_.getGraph().at(u).nextEdge);
                uint32 v = network_.getEdges().at(edgeID).finish;
                if ((network_.getEdges().at(edgeID).capacity - network_.getEdges().at(edgeID).flow) > 0 &&
                    network_.getGraph().at(u).height == network_.getGraph().at(v).height + 1) {
                    push(u, v);
                } else
                    ++network_.getGraph().at(u).nextEdge;
            } else {
                relabel(u);
                network_.getGraph().at(u).nextEdge = 0;
            }
        }
    }

    void moveToFront(uint32 iter) {
        uint32 tmp = list_.at(iter);
        for (uint32 i = iter; i > 0; --i) {
            list_.at(i) = list_.at(i - 1);
        }
        list_.at(0) = tmp;
    }

public:

    ~PushRelabel() override = default;

    explicit PushRelabel(Network& network) : FlowFinder(network) {}

    TFlow findMaxFlow() override {

        for (uint32 i = 0; i < network_.getVertNum(); ++i) {
            network_.getGraph().at(i).height = 0;
            network_.getGraph().at(i).nextEdge = 0;
        }

        network_.clearFlows();

        list_.clear();
        for (uint32 i = 0; i < network_.getVertNum(); ++i)
            if (i != network_.getSource() && i != network_.getSink())
                list_.emplace_back(i);

        network_.getGraph().at(network_.getSource()).height = network_.getVertNum();
        network_.getGraph().at(network_.getSource()).extraFlow = INF;

        for (uint32 i = 0; i < network_.getGraph().at(network_.getSource()).edges.size(); ++i)
            push(network_.getSource(),
                 network_.getEdges().at(network_.getGraph().at(network_.getSource()).edges.at(i)).finish);

        uint32 iter = 0;
        while (iter < list_.size()) {
            uint32 cur = list_.at(iter);
            int oldHeight = network_.getGraph().at(cur).height;
            discharge(cur);

            if (network_.getGraph().at(cur).height != oldHeight) {
                moveToFront(iter);
                iter = 0;
            } else {
                ++iter;
            }
        }

        for (auto i : network_.getGraph().at(network_.getSource()).edges)
            maxFlow_ += network_.getEdges().at(i).flow;

        return maxFlow_;
    }
};

class Dinic : public FlowFinder {

    bool bfs(uint32 source, uint32 sink) {
        if (source == sink)
            return false;
        std::queue<uint32> q;
        q.push(source);
        for (auto& i : network_.getGraph())
            i.level = -1;
        network_.getGraph().at(source).level = 0;
        while (!q.empty() && network_.getGraph().at(sink).level == -1) {
            uint32 cur = q.front();
            q.pop();
            for (uint32 i = 0; i < network_.getGraph().at(cur).edges.size(); ++i) {
                uint32 edgeId = network_.getGraph().at(cur).edges.at(i);
                uint32 next = network_.getEdges().at(edgeId).finish;
                if (network_.getGraph().at(next).level == -1 &&
                    network_.getEdges().at(edgeId).flow < network_.getEdges().at(edgeId).capacity) {
                    q.push(next);
                    network_.getGraph().at(next).level = network_.getGraph().at(cur).level + 1;
                }
            }
        }
        return network_.getGraph().at(sink).level != -1;
    }

    TFlow dfs(uint32 v, uint32 sink, TFlow flow) {
        if (!flow)
            return 0;
        if (v == sink)
            return flow;
        for (; network_.getGraph().at(v).nextEdge < network_.getGraph().at(v).edges.size(); ++network_.getGraph().at(
                v).nextEdge) {
            uint32 edgeId = network_.getGraph().at(v).edges.at(network_.getGraph().at(v).nextEdge);
            uint32 next = network_.getEdges().at(edgeId).finish;
            if (network_.getGraph().at(next).level != network_.getGraph().at(v).level + 1)
                continue;
            int pushFlow = dfs(next, sink, std::min(flow, network_.getEdges().at(edgeId).capacity -
                                                          network_.getEdges().at(edgeId).flow));
            if (pushFlow > 0) {
                network_.getEdges().at(edgeId).flow += pushFlow;
                network_.getEdges().at(edgeId ^ 1).flow -= pushFlow;
                return pushFlow;
            }
        }
        return 0;
    }

public:

    ~Dinic() override = default;

    explicit Dinic(Network& network) : FlowFinder(network) {}

    TFlow findMaxFlow() override {
        maxFlow_ = 0;
        while (true) {
            if (!bfs(network_.getSource(), network_.getSink()))
                break;
            for (auto& i : network_.getGraph())
                i.nextEdge = 0;
            while (int pushedFlow = dfs(network_.getSource(), network_.getSink(), INF))
                maxFlow_ += pushedFlow;
        }
        return maxFlow_;
    }
};

template<typename T>
TFlow findMaxFlow(Network& network) {
    T maxFlowFinder(network);
    return maxFlowFinder.getMaxFlow().maxFlow;
}

void solveMatan() {
    uint32 vertexNumber = 0;
    TFlow usefullness = 0;
    std::cin >> vertexNumber;
    Network network(vertexNumber + 2);
    network.setSource(vertexNumber);
    network.setSink(vertexNumber + 1);

    std::vector<int> themesUsefulness(vertexNumber, 0);
    for (uint32 i = 0; i < vertexNumber; ++i)
        std::cin >> themesUsefulness.at(i);
    uint32 dependsNumber = 0;
    for (uint32 i = 0; i < vertexNumber; ++i) {
        std::cin >> dependsNumber;
        uint32 dependTheme = 0;
        for (uint32 j = 0; j < dependsNumber; ++j) {
            std::cin >> dependTheme;
            network.addEdgeLocal(i, dependTheme - 1, INF);
        }
        if (themesUsefulness.at(i) > 0) {
            network.addEdgeLocal(network.getSource(), i, themesUsefulness.at(i));
            usefullness += themesUsefulness.at(i);
        }
        if (themesUsefulness.at(i) < 0)
            network.addEdgeLocal(i, network.getSink(), abs(themesUsefulness.at(i)));
    }

    TFlow maxFlow = findMaxFlow<PushRelabel>(network);

    std::cout << usefullness - maxFlow;
}

int main() {
    solveMatan();
}
