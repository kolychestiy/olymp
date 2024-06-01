#include <bits/stdc++.h>
using namespace std;

struct Flow_network {
    int s;
    int t;
    int n;
    vector<vector<int>> g;
    vector<int> cap;
    vector<int> flow;
    vector<int> from;
    vector<int> to;

    Flow_network (int N) : n(N), g(N), s(N - 2), t(N - 1) {}

    void add_edge(int _from, int _to, int _cap){
        g[_from].push_back(to.size());
        cap.push_back(_cap);
        flow.push_back(0);
        from.push_back(_from);
        to.push_back(_to);

        g[_to].push_back(to.size());
        cap.push_back(0);
        flow.push_back(0);
        from.push_back(_to);
        to.push_back(_from);
    }

    int dinic(int k){
        int F = 0;

        vector<int> d(n);
        auto bfs = [&](){
            fill_n(d.begin(), n, -1);
            d[s] = 0;
            queue<int> q({s});
            while (q.size()){
                int cur = q.front();
                q.pop();
                for (int e : g[cur]){
                    if (flow[e] == cap[e]){
                        continue;
                    }
                    int next = to[e];
                    if (d[next] != -1){
                        continue;
                    }

                    q.push(next);
                    d[next] = d[cur] + 1;
                    if (next == t){
                        return;
                    }
                }
            }
        };

        vector<int> ptr(n);
        function<int(int, int)> dfs = [&](int cur, int f){
            if (cur == t){
                return f;
            }

            int F = 0;
            for (int& i = ptr[cur]; i < g[cur].size(); i++){
                int e = g[cur][i];
                if (flow[e] == cap[e]){
                    continue;
                }
                int next = to[e];
                if (d[next] - 1 != d[cur]){
                    continue;
                }

                int z = dfs(next, min(f, cap[e] - flow[e]));
                if (!z){
                    continue;
                }

                f -= z;
                flow[e] += z;
                flow[e ^ 1] -= z;
                F += z;
                if (!f){
                    return F;
                }
            }

            return F;
        };

        while (F < k){
            bfs();
            if (d[t] == -1){
                break;
            }

            fill_n(ptr.begin(), n, 0);
            F += dfs(s, k - F);
        }

        return F;
    }

    struct Ans_edge{
        int from, to, flow;
    };

    vector<Ans_edge> get_flow(){
        vector<Ans_edge> ans;
        for (int i = 0; i < to.size(); i += 2){
            if (flow[i]){
                ans.push_back({from[i], to[i], flow[i]});
            }
        }
        return ans;
    }
};

struct Flow_cost_network : Flow_network {
    vector<int> cost;
    int INF;

    Flow_cost_network (int N, int inf = INT_MAX) : Flow_network(N), INF(inf) {}

    void add_edge(int _from, int _to, int _cap, int _cost) {
        cost.push_back(_cost);
        cost.push_back(-_cost);
        Flow_network::add_edge(_from, _to, _cap);
    }

    int solve_levit(int k){
        int F = 0;
        while (F < k){
            queue<int> qf, qs({s});
            vector<int> d(n, INF);
            vector<int> tp(n, 2);
            vector<int> le(n);
            d[s] = 0;
            tp[s] = 1;

            while (qf.size() || qs.size()){
                int cur;
                if (qf.size()){
                    cur = qf.front();
                    qf.pop();
                }else{
                    cur = qs.front();
                    qs.pop();
                }

                for (int e : g[cur]){
                    if (flow[e] == cap[e]){
                        continue;
                    }
                    int next = to[e];
                    int nd = d[cur] + cost[e];
                    if (d[next] <= nd){
                        continue;
                    }

                    if (tp[next] == 2){
                        qs.push(next);
                    }else
                    if (tp[next] == 0){
                        qf.push(next);
                    }
                    d[next] = nd;
                    tp[next] = 1;
                    le[next] = e;
                }

                tp[cur] = 0;
            }

            if (d[t] == INF){
                return F;
            }

            int cur = t;
            int f = INF;
            while (cur != s){
                int e = le[cur];
                f = min(f, cap[e] - flow[e]);
                cur = from[e];
            }

            cur = t;
            while (cur != s){
                int e = le[cur];
                flow[e] += f;
                flow[e ^ 1] -= f;
                cur = from[e];
            }

            F += f;
        }

        return F;
    }

    struct Ans_edge : Flow_network::Ans_edge {
        int cost;
    };

    vector<Ans_edge> get_flow(){
        vector<Ans_edge> ans;
        for (int i = 0; i < to.size(); i += 2){
            if (flow[i]){
                ans.push_back({from[i], to[i], flow[i], cost[i]});
            }
        }
        return ans;
    }
};

main(){}
