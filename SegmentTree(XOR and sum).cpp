#include "bits/stdc++.h"

using namespace std;

// #pragma GCC optimize ("O3,unroll-loops")
// #pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

const int len = 30;
string empty_string = bitset<len>(0).to_string();
struct segtree {
    array<int, len> empty_array;
    struct node {
        int l = -1, r = -1;
        array<int, len> _1, _0;
        string add = empty_string;
        explicit node() {
            for (int i = 0; i < len; i++) _1[i] = 0, _0[i] = 0;
        }
    };

    vector<node> tree = {node()};
    int n;

    void push(int x) {
        if (tree[x].l != -1) {
            for (int i = 0; i < len; i++) {
                tree[tree[x].l].add[i] = (tree[x].add[i] != tree[tree[x].l].add[i] ? '1' : '0');
            }
        } if (tree[x].r != -1) {
            for (int i = 0; i < len; i++) {
                tree[tree[x].r].add[i] = (tree[x].add[i] != tree[tree[x].r].add[i] ? '1' : '0');
            }
        }

        for (int i = 0; i < len; i++) {
            if (tree[x].add[i] == '1') {
                swap(tree[x]._1[i], tree[x]._0[i]);
            }
        }

        tree[x].add = empty_string;
    }

    void create(int x) {
        if (tree[x].l == -1) {
            tree[x].l = (int)tree.size();
            tree.emplace_back();
        } if (tree[x].r == -1) {
            tree[x].r = (int)tree.size();
            tree.emplace_back();
        }
    }

    explicit segtree(int n) {
        this->n = n;
        for (int i = 0; i < len; i++) {
            empty_array[i] = 0;
        }
    }

    void update(int x, int l, int r, int lx, int rx, string &bin) {
        if (lx >= r || rx <= l) return;
        if (l <= lx && rx <= r) {
            for (int i = 0; i < len; i++) {
                tree[x]._1[i] = bin[i] == '1';
                tree[x]._0[i] = bin[i] == '0';
            }
            tree[x].add = empty_string;
            push(x);
            return;
        }

        int mx = (lx + rx) / 2;
        create(x);
        push(x);

        update(tree[x].l, l, r, lx, mx, bin);
        update(tree[x].r, l, r, mx, rx, bin);

        push(tree[x].l);
        push(tree[x].r);

        for (int i = 0; i < len; i++) {
            tree[x]._1[i] = tree[tree[x].l]._1[i] + tree[tree[x].r]._1[i];
            tree[x]._0[i] = tree[tree[x].l]._0[i] + tree[tree[x].r]._0[i];
        }
    }

    void update(int ind, int x) {
        string bin = bitset<len>(x).to_string();
        update(0, ind, ind + 1, 0, n, bin);
    }

    void xoring(int x, int l, int r, int lx, int rx, string &bin) {
        if (lx >= r || rx <= l) return;
        if (l <= lx && rx <= r) {
            for (int i = 0; i < len; i++) {
                tree[x].add[i] = (bin[i] != tree[x].add[i] ? '1' : '0');
            }
            push(x);
            return;
        }

        int mx = (lx + rx) / 2;
        create(x);
        push(x);

        xoring(tree[x].l, l, r, lx, mx, bin);
        xoring(tree[x].r, l, r, mx, rx, bin);

        push(tree[x].l);
        push(tree[x].r);

        for (int i = 0; i < len; i++) {
            tree[x]._1[i] = tree[tree[x].l]._1[i] + tree[tree[x].r]._1[i];
            tree[x]._0[i] = tree[tree[x].l]._0[i] + tree[tree[x].r]._0[i];
        }
    }

    void xoring(int l, int r, int x) {
        string bin = bitset<len>(x).to_string();
        xoring(0, l, r, 0, n, bin);
    }

    auto get(int x, int l, int r, int lx, int rx) {
        if (lx >= r || rx <= l) return empty_array;
        if (l <= lx && rx <= r) {
            push(x);
            return tree[x]._1;
        }

        int mx = (lx + rx) / 2;
        create(x);
        push(x);

        auto a = get(tree[x].l, l, r, lx, mx);
        auto b = get(tree[x].r, l, r, mx, rx);

        for (int i = 0; i < len; i++) {
            a[i] += b[i];
        }
        return a;
    }

    long long get(int l, int r) {
        auto arr = get(0, l, r, 0, n);
        long long sum = 0;
        for (int i = 0; i < len; i++) {
            sum += (1ll << (len - i - 1)) * arr[i];
        }
        return sum;
    }
};

void solve() {
    int n, q;
    cin >> n >> q;

    vector<int> s(n);
    int maxes = 0;

    for (int i = 0; i < n; i++) {
        cin >> s[i];
        maxes = max(s[i], maxes);
    }

    segtree st(n);
    for (int i = 0; i < n; i++) {
        st.update(i, s[i]);
    }

    while (q--) {
        int com;
        cin >> com;

        if (com == 0) {
            int ind, x;
            cin >> ind >> x;
            ind--;
            st.update(ind, x);
            continue;
        } if (com == 1) {
            int l, r, x;
            cin >> l >> r >> x;
            l--, r--;
            st.xoring(l, r + 1, x);
            continue;
        }

        int l, r;
        cin >> l >> r;

        l--;

        cout << st.get(l, r) << '\n';
    }
}

signed main() {
#ifdef local
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    // cin >> t;

    while (t--) solve();
}
