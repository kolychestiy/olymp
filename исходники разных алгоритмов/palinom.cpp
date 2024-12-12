#include <bits/stdc++.h>
using namespace std;

#define int long long
#define all(x) x.begin(), x.end()

template<typename T>
ostream& operator << (ostream& out, const vector<T>& a){
    for (const auto& e : a){
        out << e << ' ';
    }
    return out;
}

const int mod = 998244353;

int fpow(int a, int p){
    int ans = 1;
    while (p){
        if (p & 1){
            ans = ans * a % mod;
        }
        a = a * a % mod;
        p >>= 1;
    }
    return ans;
}

vector<int> roots;

vector<int> operator * (vector<int> a, vector<int> b){
    int sz = 1;
    while ((1 << sz) < (int)a.size() + (int)b.size()){
        sz++;
    }

    int SZ = 1 << sz;

    vector<int> rev(SZ);
    for (int i = 0, j = 0; i < SZ; i++){
        rev[i] = j;
        int k = SZ >> 1;
        for (; j & k; k >>= 1){
            j ^= k;
        }
        j ^= k;
    }

    auto fft = [&](vector<int>& a, int br){
        for (int i = 0; i < SZ; i++){
            if (rev[i] < i){
                swap(a[rev[i]], a[i]);
            }
        }

        for (int j = 2; j <= SZ; j <<= 1){
            int root = br;
            for (int i = j; i < SZ; i <<= 1){
                root = root * root % mod;
            }
            int lj = j >> 1;
            vector<int> rt(lj);
            rt[0] = 1;
            for (int i = 1; i < lj; i++){
                rt[i] = rt[i - 1] * root % mod;
            }
 
            for (int i = 0; i < SZ; i += j){
                int z = 0;
                auto il = a.begin() + i;
                auto ir = il + lj;
                auto en = ir;
                auto it = rt.begin();
                for (; il != en; il++, ir++, it++){
                    z = *ir * *it % mod;
                    *ir = *il - z;
                    if (*ir < mod){
                        *ir += mod;
                    }
                    *il += z;
                    if (*il >= mod){
                        *il -= mod;
                    }
                }
            }
        }
    };

    a.resize(SZ);
    b.resize(SZ);

    fft(a, roots[sz]);
    fft(b, roots[sz]);

    for (int i = 0; i < SZ; i++){
        a[i] = a[i] * b[i] % mod;
    }

    fft(a, fpow(roots[sz], mod - 2));
    int rn = fpow(SZ, mod - 2);
    for (auto& e : a){
        e = e * rn % mod;
    }

    while (a.size() > 1 && a.back() == 0){
        a.pop_back();
    }
    return a;
}

vector<int> inverse(const vector<int>& p, vector<int> q, int sz){
    if (sz >= (int)p.size()){
        return q;
    }

    vector<int> p_;
    for (int i = 0; i < 2*sz && i < (int)p.size(); i++){
        p_.push_back(p[i]);
    }

    p_ = p_ * q;
    p_.resize(2*sz);
    for (auto& e : p_){
        e = mod - e;
    }
    p_[0] = (p_[0] + 1) % mod;

    p_ = p_ * q;
    p_.resize(2*sz);
    for (int i = 0; i < (int)q.size(); i++){
        p_[i] = q[i];
    }

    return inverse(p, p_, sz << 1);
}

void init_roots(){
    int mx_sz = 23;
    int j = 1;
    for (; j < mod; j++){
        if (fpow(j, 1 << (mx_sz - 1)) == (mod - 1)){
            break;
        }
    }
    if (j == mod){
        cout << "very big size palinom. not found root!";
        exit(0);
    }

    for (int i = mx_sz; i >= 0; i--) {
        roots.push_back(j);
        j = j * j % mod;
    }

    reverse(all(roots));
}

signed main(){

    init_roots();
    vector<int> a = {1, 1, 1};
    cout << a * a << endl;
    cout << a * inverse(a, {fpow(a[0], mod - 2)}, 1) << endl;

}
