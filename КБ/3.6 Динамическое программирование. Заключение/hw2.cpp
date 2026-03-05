#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    // по классике мы бы хранили либо 
    // списки рёбер vector<vector<int>> g(n);
    // либо матрицу смежности 
    // vector adj(vector<int> adj(n));

    // Но мы будем хранить матрицу смежности 
    // в виде списка битовых масок
    // для каждой вершины храним 1 в бите, если 
    // есть ребро из неё в вершину с таким номером

    // Это позволит нам сократить время работы в n раз
    // Хотя в задаче заходит и без этой оптимизации
    vector<int> adj(n, 0); 

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        adj[u] |= (1 << v);
        adj[v] |= (1 << u);
    }

    int ans = 0;

    int total = 1 << n;

    // задача решается и без ДП, простым перебором
    // все возможных подмножеств вершин
    // его кстати можно реализовать рекурсивно
    // и сократить время работы ещё в n раз
    // и без этих двух оптимизаций с такими 
    // ограниченями можно сдать
    for (int mask = 1; mask < total; mask++) {
        // количество вершин в маске
        int cnt = __builtin_popcount(mask);
        if (cnt <= ans) continue;

        bool ok = true;

        for (int v = 0; v < n; v++) {
            if (mask & (1 << v)) {
                int others = mask ^ (1 << v);
                if ((adj[v] & others) != others) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok) ans = cnt;
    }

    cout << ans;
}