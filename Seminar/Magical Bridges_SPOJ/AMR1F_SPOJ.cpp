#include <bits/stdc++.h>

using namespace std;

const int INF = 0x3f3f3f3f;

const int N = 110;

int n, V, E;
int g[N<<2][N<<2];
map<int,int> nodes[N];
vector<int> floors[N];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int t;
  cin >> t;

  while (t--) {
    int f;
    cin >> V >> f >> E;

    n = 0;
    memset(g, 0x3f, sizeof g);
    for (int i = 0; i < V; i++) {
      nodes[i].clear();
      floors[i].clear();
      nodes[i][0] = n++;
      floors[i].push_back(0);
    }

    for (int i = 0; i < V; i++) {
      int u = nodes[i][0];
      int v = nodes[(i+1) % V][0];

      g[u][v] = g[v][u] = 1;
    }

    for (int i = 0; i < E; i++) {
      int bi, fi, bj, fj, tc;
      cin >> bi >> fi >> bj >> fj >> tc;
      bi--, fi--, bj--, fj--;

      if (!nodes[bi].count(fi)) {
        nodes[bi][fi] = n++;
        floors[bi].push_back(fi);
      }
      if (!nodes[bj].count(fj)) {
        nodes[bj][fj] = n++;
        floors[bj].push_back(fj);
      }

      int u = nodes[bi][fi];
      int v = nodes[bj][fj];

      g[u][v] = g[v][u] = tc;
    }

    for (int i = 0; i < V; i++) {
      vector<int> & fl = floors[i];
      for (int j = 0; j < fl.size(); j++) {
        for (int k = j+1; k < fl.size(); k++) {
          int u = nodes[i][fl[j]];
          int v = nodes[i][fl[k]];
          g[u][v] = g[v][u] = abs(fl[j]-fl[k]);
        }
      }
    }

    for (int i = 0; i < n; i++)
      g[i][i] = 0;

    for (int k = 0; k < n; k++)
      for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
          g[i][j] = min(g[i][j], g[i][k] + g[k][j]);

    int Q;
    cin >> Q;

    map<int,int>::iterator it;

    while (Q--) {
      int qbi, qfi, qbj, qfj;
      cin >> qbi >> qfi >> qbj >> qfj;
      qbi--, qfi--, qbj--, qfj--;

      it = nodes[qbi].lower_bound(qfi);
      if (it == nodes[qbi].end())
        it--;

      int up = it->first;
      int lo = -1;

      if (it != nodes[qbi].begin()) {
        it--;
        lo = it->first;
      }

      int di1 = abs(qfi-up);
      int di2 = (lo < 0)? INF : abs(qfi-lo);
      up = nodes[qbi][up];
      lo = (lo < 0)? 0 : nodes[qbi][lo];

      it = nodes[qbj].lower_bound(qfj);
      if (it == nodes[qbj].end())
        it--;

      int up2 = it->first;
      int lo2 = -1;

      if (it != nodes[qbj].begin()) {
        it--;
        lo2 = it->first;
      }

      int dj1 = abs(qfj-up2);
      int dj2 = (lo2 < 0)? INF : abs(qfj-lo2);
      up2 = nodes[qbj][up2];
      lo2 = (lo2 < 0)? 0 : nodes[qbj][lo2];

      int dist = INF;
      dist = min(dist, di1 + dj1 + g[up][up2]); // up-up
      dist = min(dist, di1 + dj2 + g[up][lo2]); // up-lo
      dist = min(dist, di2 + dj1 + g[lo][up2]); // lo-up
      dist = min(dist, di2 + dj2 + g[lo][lo2]); // lo-lo

      if (qbi == qbj)
        dist = min(dist, (int)abs(qfi-qfj));

      cout << dist << '\n';
    }
  }

  return 0;
}