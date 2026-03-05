#include <iostream>
#include <vector>
#include <string>
using namespace std;

int n;
vector<string> g;
vector<int> dp;

int solve(int mask) {
    if (dp[mask] != -1) return dp[mask];

    int res = 0;

    int i;
    for (i = 0; i < n; i++)
        if (!(mask & (1 << i)))
            break;

    if (i == n) return 0;

    res = solve(mask | (1 << i));

    for (int j = i + 1; j < n; j++) {
        if (!(mask & (1 << j)) && g[i][j] == 'Y') {
            res = max(res, 1 + solve(mask | (1 << i) | (1 << j)));
        }
    }

    return dp[mask] = res;
}

int main() {
    cin >> n;

    g.resize(n);
    for (int i = 0; i < n; i++)
        cin >> g[i];

    dp.assign(1 << n, -1);

    int pairs = solve(0);

    cout << pairs * 2;
}
