// import visualization libraries {
#include "algorithm-visualizer.h"
// }

#include <vector>
#include <string>
#include <limits>

using namespace std;

int main() {
    // define tracer variables
    GraphTracer tracer = GraphTracer("Graph").weighted();
    LogTracer logger = LogTracer("Console");
    Layout::setRoot(VerticalLayout({tracer, logger}));
    tracer.log(logger);

    // create random weighted graph
    const int N = 5;
    const double ratio = 1.0; // fully connected
    Randomize::Graph<int> rg(N, ratio);
    rg.weighted(true);
    vector<int> G(N * N);
    rg.fill(G.data());

    // convert adjacency matrix to json and set tracer
    nlohmann::json jG = nlohmann::json::array();
    for (int i = 0; i < N; i++) {
        nlohmann::json row = nlohmann::json::array();
        for (int j = 0; j < N; j++) {
            row.push_back(G[i * N + j]);
        }
        jG.push_back(row);
    }
    tracer.set(jG);
    Tracer::delay();

    // Floyd--Warshall
    const double MAX_VALUE = numeric_limits<double>::infinity();
    vector<vector<double>> S(N, vector<double>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) S[i][j] = 0;
            else if (G[i * N + j] > 0) S[i][j] = G[i * N + j];
            else S[i][j] = MAX_VALUE;
        }
    }

    logger.println("finding the shortest paths from and to all nodes");

    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            if (k == i) continue;
            // visualize
            tracer.visit(k, i);
            Tracer::delay();
            for (int j = 0; j < N; j++) {
                if (i == j || j == k) continue;
                // visualize
                tracer.visit(j, k);
                Tracer::delay();
                if (S[i][j] > S[i][k] + S[k][j]) {
                    // visualize
                    tracer.visit(j, i, S[i][j]);
                    Tracer::delay();
                    S[i][j] = S[i][k] + S[k][j];
                    // visualize
                    tracer.leave(j, i, S[i][j]);
                }
                // visualize
                tracer.leave(j, k);
            }
            // visualize
            tracer.leave(k, i);
            Tracer::delay();
        }
    }

    // logger output
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (S[i][j] == MAX_VALUE) {
                logger.println(string("there is no path from ") + to_string(i) + " to " + to_string(j));
            } else {
                logger.println(string("the shortest path from ") + to_string(i) + " to " + to_string(j) + " is " + to_string(S[i][j]));
            }
        }
    }

    return 0;
}