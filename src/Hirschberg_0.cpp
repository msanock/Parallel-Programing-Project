//  I

#include <iostream>
#include <vector>
#include <string>
#include <atomic>
#include <thread>
#include <chrono>

#include "config.h"

using namespace std;

string backtrack_substring_small(string &s1, string &s2, int beg1, int beg2, int n, int m) {
    vector<vector<int>> dp(n+2, vector<int>(m+2, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i-1+beg1] == s2[j-1+beg2])
                dp[i][j] = dp[i-1][j-1]+1;
            else
                dp[i][j] = (dp[i][j-1]>dp[i-1][j]) ? dp[i][j-1] : dp[i-1][j];
        }
    }

    int l = dp[n][m];
    int i = n, j = m;
    string shared;

    shared.resize(l);
    while (i > 0 && j > 0) {
        if (s1[i-1+beg1] == s2[j-1+beg2]) {
            shared[--l] = s1[i-1+beg1];
            i--;
            j--;
        }
        else
            (dp[i][j] == dp[i][j-1]) ? j-- : i--;
    }
    return shared;
}

void fill_vector_normal(vector<int> &dp, string &s1, string &s2, int beg1, int beg2, int n, int m) {
    int last;
    fill(dp.begin(), dp.begin()+m+2, 0);

    for (int i = 1; i <= n; i++){
        last = dp[0];
        for (int j = 1; j <= m; j++) {
            if (s1[i+beg1-1] == s2[j+beg2-1]) {
                swap(dp[j],last);
                dp[j]++;
            }
            else {
                last = dp[j];
                dp[j] = (dp[j-1]>dp[j]) ? dp[j-1] : dp[j];
            }
        }
    }
}

// cannot be parallelized
void fill_vector_reversed(vector<int> &dp, string &s1, string &s2, int beg1, int beg2, int n, int m) {
    int last;
    fill(dp.begin(), dp.begin()+m+2, 0);

    for (int i = n; i > 0; i--) {
        last = dp[m+1];
        for (int j = m; j > 0; j--) {
            if (s1[i+beg1-1] == s2[j+beg2-1]) {
                swap(dp[j],last);
                dp[j]++;
            }
            else {
                last = dp[j];
                dp[j] = (dp[j+1]>dp[j]) ? dp[j+1] : dp[j];
            }
        }
    }
}



int find_k(vector<int> &C, vector<int> &D, int m) {
    int k = -1;
    int max = -1;

    for (int i = 0; i <= m; i++) {
        if(C[i]+D[i+1] > max) {
            max = C[i]+D[i+1];
            k = i;
        }
    }
    return k;
}


string fill_Hirschberg(vector<vector<int>> &all, string &s1, string &s2,int beg1, int beg2, int n, int m, int depth = 0) {
    int k;

    if (n <= 2 || m <= 2)
        return backtrack_substring_small(s1, s2, beg1, beg2, n, m);

    int half = (n+1)/2;
  
    fill_vector_normal(all[0], s1, s2, beg1, beg2, half, m);
    fill_vector_reversed(all[1], s1, s2, beg1+half, beg2, n-half, m);

    k = find_k(all[0], all[1], m);

    string o1 = fill_Hirschberg(all, s1, s2, beg1, beg2, half, k, depth+1);
    string o2 = fill_Hirschberg(all, s1, s2, beg1+half, beg2+k, n/2, m-k, depth+1);

    return o1+o2;
}



void solve_set(int n){
    string S1, S2;

    cin>>S1>>S2;


    vector<vector<int>> all(2, vector<int>(n+2,0));

    string ans = fill_Hirschberg(all, S1, S2, 0, 0, n, n);
    cout<<ans.length()<<"\n"<<ans<<"\n";
}

int main(int argc, const char * argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin.tie(nullptr);

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    int z, n;
    cin>>z>>n;

    while (z--)
        solve_set(n);

    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    cout << ms_int.count() << "ms\n";

    return 0;
}