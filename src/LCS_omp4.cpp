
//  C


#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <omp.h>

#include "config.h"

using namespace std;



void backtrack_substring(vector<vector<int>>& dp, string &s1, string &s2, int n, int l){
    int i = n, j = n;
    string shared;

    shared.resize(l);
    while (l > 0) {
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


void fill_vector(vector<vector<int>>& dp, string &s1, string &s2, int n){
    vector<vector<int>> last_match(ALPHABET_SIZE, vector<int>(n+2, 0));

    #pragma omp parallel for
    for (int letter = 0; letter < ALPHABET_SIZE; letter++) {
        for (int j = 1; j <= n+1; j++) {
            if (s2[j-2] == letter+'a')
                last_match[letter][j] = j-1;
            else
                last_match[letter][j] = last_match[letter][j-1];
        }
    }

    #pragma omp parallel
    {
        for (int i = 1; i <= n; i++) {
            #pragma omp parallel for num_threads(4)
            for (int j = 1; j <=n; j++) {
                if (s1[i-1] == s2[j-1])
                    dp[i][j] = dp[i-1][j-1]+1;
                else {
                    int last = last_match[s1[i-1]-'a'][j];
                    if (last == 0)
                        dp[i][j] = dp[i-1][j];
                    else
                        dp[i][j] = (dp[i-1][j]>dp[i-1][last-1]+1) ? dp[i-1][j] : dp[i-1][last-1]+1;
                }
            }
        }
    }
}



void solve_set(vector<vector<int>>& longest_substring, int n) {
    string S1, S2;

    cin>>S1>>S2;

    fill_vector(longest_substring, S1, S2, n);
    cout<<longest_substring[n][n]<<"\n";

    backtrack_substring(longest_substring, S1, S2, n, longest_substring[n][n]);
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

    vector<vector<int>> longest_substring(size_of_strings+1, vector<int>(size_of_strings+1, 0));

    while (z--)
        solve_set(longest_substring, size_of_strings);

    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    cout << ms_int.count() << "ms\n";
    
    return 0;
}
