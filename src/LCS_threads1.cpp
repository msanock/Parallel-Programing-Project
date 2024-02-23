//  C


#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>

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

void f(string &s1, string &s2, vector<vector<int>>& dp, int diagonal, int start, int size_of_diagonal, int number_of_threads, int thread_index) {
    for (int i = start; i-start+thread_index < size_of_diagonal; i+=number_of_threads) {
        int j = diagonal - i;
        if (s1[i-1] == s2[j-1])
            dp[i][j] = dp[i-1][j-1]+1;
        else
        dp[i][j] = (dp[i][j-1]>dp[i-1][j]) ? dp[i][j-1] : dp[i-1][j];
    }
}


void fill_vector(vector<vector<int>>& dp, string &s1, string &s2, int n){
    //parallel version
    thread threads[THREADS];

    for (int diagonal = 2; diagonal < 2*n+1; diagonal++){
        int size_of_diagonal = (diagonal <= n+1) ? diagonal-1 : 2*n+1-diagonal;
        for (int thread_index = 0; thread_index < THREADS; thread_index++) {
            int start = (diagonal <= n+1) ? thread_index+1 : thread_index+diagonal-n;
            threads[thread_index] = thread(f, ref(s1), ref(s2), ref(dp), diagonal, start, size_of_diagonal, THREADS, thread_index);
        }

        for (int thread_index = 0; thread_index < THREADS; thread_index++)
        threads[thread_index].join();
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
