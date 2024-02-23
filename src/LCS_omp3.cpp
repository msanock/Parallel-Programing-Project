

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "config.h"

using namespace std;

void backtrack_substring(vector<vector<int>>& dp, string &s1, string &s2, int n, int l){
    int diagonal = 2*n, index_diagonal = 0, i = n, j = n;
    string shared;

    shared.resize(l);
    while (l>0 && diagonal > n+1) {
        if (s1[i-1] == s2[j-1]){
            shared[--l] = s1[i-1];
            diagonal -= 2;
            index_diagonal++;
            i--;
            j--;
        }
        else {
            if (dp[diagonal][index_diagonal] == dp[diagonal-1][index_diagonal+1]) {
                j--;
                diagonal--;
                index_diagonal++;
            } else {
                i--;
                diagonal--;
            }
        }
    }
    if (diagonal == n+1 && l > 0) {
        if (s1[i-1] == s2[j-1]) {
            shared[--l] = s1[i-1];
            diagonal -= 2;
            i--;
            j--;
        }
        else {
            if (dp[diagonal][index_diagonal] == dp[diagonal-1][index_diagonal+1]) {
                j--;
                diagonal--;
                index_diagonal++;
            } else {
                i--;
                diagonal--;
            }
        }
    }
    while (l > 0) {
        if (s1[i-1] == s2[j-1]) {
            shared[--l] = s1[i-1];
            diagonal -= 2;
            index_diagonal--;
            i--;
            j--;
        }
        else {
            if (dp[diagonal][index_diagonal] == dp[diagonal-1][index_diagonal]) {
                j--;
                diagonal--;
            } else {
                i--;
                diagonal--;
                index_diagonal--;
            }
        }
    }
    
    cout<<shared<<"\n";
}


void fill_vector(vector<vector<int>>& dp, string &s1, string &s2, int n){
    #pragma omp parallel
    for (int diagonal = 2; diagonal <= n; diagonal++) {
        int size_of_diagonal = diagonal-1;
        #pragma omp parallel for
        for (int i = 1; i <= size_of_diagonal; i++) {
            int j = diagonal - i;
            if (s1[i-1] == s2[j-1])
                dp[diagonal][i] = dp[diagonal-2][i-1]+1;
            else
                dp[diagonal][i] = (dp[diagonal-1][i] > dp[diagonal-1][i-1]) ? dp[diagonal-1][i] : dp[diagonal-1][i-1];
        }

    }

    int diagonal = n+1;
    int row = diagonal-n;
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int j = diagonal - i - row;
        if (s1[i+row-1] == s2[j-1])
            dp[diagonal][i] = dp[diagonal-2][i]+1;
        else
            dp[diagonal][i] = (dp[diagonal-1][i+1] > dp[diagonal-1][i]) ? dp[diagonal-1][i+1] : dp[diagonal-1][i];

    }

    #pragma omp parallel
    for (int diagonal = n+2; diagonal <= 2*n+1; diagonal++) {
        int size_of_diagonal = 2*n+1-diagonal;
        int row = diagonal-n;
        #pragma omp parallel for
        for (int i = 0; i < size_of_diagonal; i++) {
            int j = diagonal - i - row;
            if (s1[i+row-1] == s2[j-1])
                dp[diagonal][i] = dp[diagonal-2][i+1]+1;
            else
                dp[diagonal][i] = (dp[diagonal-1][i+1] > dp[diagonal-1][i]) ? dp[diagonal-1][i+1] : dp[diagonal-1][i];
        }
    }
}

void solve_set(vector<vector<int>>& longest_substring, int n) {
    string S1, S2;

    cin>>S1>>S2;

    fill_vector(longest_substring, S1, S2, n);
    cout<<longest_substring[2*n][0]<<"\n";

    backtrack_substring(longest_substring, S1, S2, n, longest_substring[2*n][0]);
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

    int z, size_of_strings;
    cin>>z>>size_of_strings;

    vector<vector<int>> longest_substring_diagonals(2*size_of_strings+1);
    for (int i = 0; i <= size_of_strings; i++)
        longest_substring_diagonals[i].resize(i+1, 0);
    for (int i = size_of_strings+1; i < 2*(size_of_strings+1); i++)
        longest_substring_diagonals[i].resize(2*size_of_strings+1-i, 0);

    while (z--)
        solve_set(longest_substring_diagonals, size_of_strings);

    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    cout << ms_int.count() << "ms\n";
    
    return 0;
}
