#include <iostream>
#include <vector>
const int INF = -1;

std::vector<std::vector<int>> shortestPath(std::vector<std::vector<int>> graph) {
    const int INF = -1;
    std::vector<std::vector<int>> ans = graph;
    int n = graph.size();
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            if (ans[i][k] != INF)
                for (int j = 0; j < n; ++j) {
                    if (ans[k][j] != INF)
                        if (ans[i][j] == INF || (ans[i][k] + ans[k][j] < ans[i][j]))
                            ans[i][j] = ans[i][k] + ans[k][j];
                }
    return ans;
}

int main() {
    using namespace std;
    int n, m;
    cin >> n >> m;
    vector<vector<int>> graph(n);
    for (int i = 0; i < n; ++i) {
        graph[i].resize(n);
        for (int j = 0; j < n; ++j)
            graph[i][j] = INF;
    }
    for (int i = 0; i < n; ++i)
        graph[i][i] = 0;
    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        cin >> graph[x - 1][y - 1];
    }
    vector<vector<int>> ans = shortestPath(graph);
    int queries;
    cin >> queries;
    while (queries--) {
        int x, y;
        cin >> x >> y;
        cout << ans[x - 1][y - 1] << endl;
    }
    return 0;
}