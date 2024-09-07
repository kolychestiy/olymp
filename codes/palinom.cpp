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
    if (p == 0){
        return 1;
    }
    if (p & 1){
        return fpow(a, p - 1) * a % mod;
    }
    return fpow(a*a % mod, p >> 1);
}

vector<int> roots;

vector<int> fft(vector<int> a, int root){
    if (a.size() == 1){
        return a;
    }

    int sz = a.size() >> 1;
    vector<int> p0(sz), p1(sz);
    for (int i = 0; i < sz; i++){
        p0[i] = a[i << 1];
        p1[i] = a[i << 1 | 1];
    }

    int sq = root * root % mod;
    auto r0 = fft(p0, sq);
    auto r1 = fft(p1, sq);

    vector<int> r(a.size());
    int x = 1;
    for (int i = 0; i < (int)r.size(); i++){
        r[i] = (r0[i & (sz - 1)] + x*r1[i & (sz - 1)]) % mod;
        x = x * root % mod;
    }

    return r;
}

vector<int> operator * (vector<int> a, vector<int> b){
    int sz = 1;
    while ((1 << sz) < (int)a.size() + (int)b.size()){
        sz++;
    }

    int SZ = 1 << sz;
    a.resize(SZ);
    b.resize(SZ);

    int root = roots[sz];
    auto ra = fft(a, root);
    auto rb = fft(b, root);

    vector<int> rc(SZ);
    for (int i = 0; i < SZ; i++){
        rc[i] = ra[i] * rb[i] % mod;
    }

    auto c = fft(rc, fpow(root, mod - 2));
    int rn = fpow(SZ, mod - 2);
    for (auto& e : c){
        e = (e * rn) % mod;
    }

    while (c.size() > 1 && c.back() == 0){
        c.pop_back();
    }
    return c;
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
    int mx_sz = 21;
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