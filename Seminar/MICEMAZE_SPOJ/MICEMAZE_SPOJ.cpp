#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;
#define mp make_pair
const int INF = 1e9;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector< vector<ii> >vvii;
int cost[5000];

//#include <fstream>
//ofstream cout("out.txt");

struct option {
	bool operator()(const ii& a, const ii& b)const {
		return a.second < b.second;
	}
};


void dijkstra(vvii adj, int s) {
	priority_queue<ii, vector<ii>, option>pq;
	cost[s] = 0;
	pq.push(mp(s, 0));
	while (!pq.empty()) {
		ii top = pq.top();
		pq.pop();
		int u = top.first, w = top.second;
		for (int i = 0; i < adj[u].size(); ++i) {
			ii neighbor = adj[u][i];
			if (w + neighbor.second < cost[neighbor.first]) {
				cost[neighbor.first] = w + neighbor.second;
				pq.push(mp(neighbor.first, cost[neighbor.first]));
			}
		}
	}
}


void displayCost(int n) {
	for (int i = 0; i < n; ++i) cout  << cost[i] << ' ';
	cout  << endl;
}


int main() {
	int n, e, t, m;
	vvii adj;
	cin >> n >> e >> t >> m;
	adj.resize(n);
	for (int i = 0; i < n; ++i) cost[i] = INF;
	for (int i = 0; i < m; ++i) {
		int a, b, w;
		cin >> a >> b >> w;
		adj[b - 1].push_back(mp(a - 1, w));
	}
	dijkstra(adj, e-1);
	int cnt = 0;
	for (int i = 0; i < n; ++i)
		if (cost[i] <= t) ++cnt;
	cout << cnt;
	return 0;
}