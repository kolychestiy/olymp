    int n;
    cin >> n;
    vector a(n, vector<int> (n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> a[i][j];
        }
    }

    vector dp(n, vector<int> (1 << n, INF));
    for (int i = 0; i < n; i++) {
        dp[i][1 << i] = 0; // база
    }

    // for len_mask
    // for mask : masks(len_masks)
    for (int mask = 1; mask < (1 << n); mask++) { 
        for (int i = 0; i < n; i++) { // порядок циклов важен
            // dp[i][mask] = INF;
            for (int j = 0; j < n; j++) {
                if ((1 << j) & mask) continue; // уже посещали
                dp[j][mask | (1 << j)] = min(dp[j][mask | (1 << j)], dp[i][mask] + a[i][j]);
                // ans[j][...] = i
            }
        }
    }

    int ans = 1e9;
    for (int i = 0; i < n; i++) {
        ans = min(ans, dp[i][(1 << n) - 1]);
    }

    cout << ans;
