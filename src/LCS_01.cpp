//  LCS_01


#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "config.h"

using namespace std;

void backtrack_substring(vector<vector<int>>& dp, string &s1, string &s2, int n, int m, int l){
    int i = n, j = m;
    string shared;

    shared.resize(l);
    while (i > 0 && j > 0) {
        if (s1[i-1] == s2[j-1]){
            shared[--l] = s1[i-1];
            i--;
            j--;
        }
        else
            (dp[i][j] == dp[i][j-1]) ? j-- : i--;
    }
    cout<<shared<<"\n";
}


void fill_vector(vector<vector<int>>& dp, string &s1, string &s2, int n, int m){
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= m; j++){
            if (s1[i-1] == s2[j-1])
                dp[i][j] = dp[i-1][j-1]+1;
            else
                dp[i][j] = (dp[i][j-1]>dp[i-1][j]) ? dp[i][j-1] : dp[i-1][j];
        }
    }
}



void solve_set(int n, vector<vector<int>>& longest_substring){
    string S1, S2;

    cin>>S1>>S2;

    fill_vector(longest_substring, S1, S2, n, n);

    cout<<longest_substring[n][n]<<"\n";

    backtrack_substring(longest_substring, S1, S2, n, n, longest_substring[n][n]);
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

    vector<vector<int>> longest_substring(n+1, vector<int>(n+1, 0));

    while (z--) 
        solve_set(n, longest_substring);

    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    cout << ms_int.count() << "ms\n";
    
    return 0;
}