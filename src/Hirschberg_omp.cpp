//  I

#include <iostream>
#include <vector>
#include <string>
#include <atomic>
#include <chrono>
#include <omp.h>

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

vector<int> fill_vector_normal(string &s1, string &s2, int beg1, int beg2, int n, int m) {
    int last;
    vector<int> dp(m+2, 0);

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

    return dp;
}

// cannot be parallelized
vector<int> fill_vector_reversed(string &s1, string &s2, int beg1, int beg2, int n, int m) {
    int last;
    vector<int> dp(m+2, 0);

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

    return dp;
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


void fill_Hirschberg(string & result, string &s1, string &s2,int beg1, int beg2, int n, int m, int depth = 0) {
    int k;

    if (n <= 30 || m <= 30) {
        result = backtrack_substring_small(s1, s2, beg1, beg2, n, m);
        return;
    }

    int half = (n+1)/2;
    vector<int> all1;
    vector<int> all2;
   
    
    #pragma omp task shared(s1, s2, all1, all2, beg1, beg2, half, m)
    {
        #pragma omp task shared(all1)
        {
            all1 = fill_vector_normal(s1, s2, beg1, beg2, half, m);
        }
        
        all2 = fill_vector_reversed(s1, s2, beg1+half, beg2, n-half, m);
        
        #pragma omp taskwait
    }
    


    k = find_k(all1, all2, m);
    string o1, o2;
    
    #pragma omp task shared(o1, o2, s1, s2)
    {
        #pragma omp task shared(o1, s1, s2)
        {
            fill_Hirschberg(o1, s1, s2, beg1, beg2, half, k, depth+1);
        }
        fill_Hirschberg(o2, s1, s2, beg1+half, beg2+k, n/2, m-k, depth+1);
        
        #pragma omp taskwait
    }
        
    
    result = o1 + o2;
}



void solve_set(int n){
    string S1, S2;

    cin>>S1>>S2;

    string ans;
    fill_Hirschberg(ans, S1, S2, 0, 0, n, n, 0);
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
    cout<<omp_get_max_threads()<<"\n";

    return 0;
}