//
//  main.cpp
//  optimal-exercises
//
//  Created by mndx on 09/03/2023.
//

#include <iostream>
#include <vector>

using namespace std;

int maxPointsGreedy(vector<int> & D, vector<int> & P, vector<int> & S) {
    
    int n = (int) D.size();
    
    vector<bool> vis(n, false);
    
    int res = 0;
    
    // Keep choosing the assignment which gives the most points with the latest deadline.
    for(int i = n - 1; i >= 0; i--) {
        int points = 0;
        int index = -1;
        for(int j = 0; j < n; j++) {
            if(D[j] >= i && !vis[j]) {
                if(P[j] >= points) {
                    index = j;
                    points = P[j];
                }
            }
        }
        
        vis[index] = true;
        res = res + points;
        S[i] = index;
    }
    
    return res;
}

int maxPointsBackTrack(vector<int> & D, vector<int> & P, int d, vector<bool> & vis) {
    
    int n = (int) D.size();
    
    if(d >= n)
        return 0;
    
    int res = 0;
    
    // Try making all assignments on any day and pick the sequence which gives the most points.
    for(int k = 0; k < n; k++) {
        if(!vis[k]) {
            vis[k] = true;
            if(D[k] >= d)
                res = max(res, P[k] + maxPointsBackTrack(D, P, d + 1, vis));
            vis[k] = false;
        }
    }
    
    return res;
}

bool checkStatus(vector<int> & D, int d, vector<bool> & vis) {
    
    int n = (int) D.size();
    
    // Check if all deadlines have passed. Return false otherwise
    for(int k = 0; k < n; k++) {
        if(!vis[k]) {
            if(D[k] >= d) {
                return false;
            }
        }
    }
    
    return true;
}

void maxPointsBackTrackSchedule(vector<int> & D, vector<int> & P, int d, vector<bool> & vis, vector<int> & C, vector<int> & S, int & out) {
    
    int n = (int) D.size();
    
    bool allDone = checkStatus(D, d, vis);
    
    if(d >= n || allDone) {
        int sum = 0;
        for(auto e : C) {
            if(e != -1)
                sum = sum + P[e];
        }
        if(sum >= out) {
            out = sum;
            S = C;
        }
        return;
    }
    
    // Try making all assignments on any day and pick the sequence which gives the most points.
    for(int k = 0; k < n; k++) {
        if(!vis[k]) {
            vis[k] = true;
            if(D[k] >= d) {
                int tmp = C[d];
                C[d] = k;
                maxPointsBackTrackSchedule(D, P, d + 1, vis, C, S, out);
                C[d] = tmp;
            }
            vis[k] = false;
        }
    }
}

int checkSum(vector<int> & S, vector<int> & P) {
    
    int sum = 0;
    
    for(auto e : S) {
        if(e != -1)
            sum = sum + P[e];
    }
    
    return sum;
}

int main(int argc, const char * argv[]) {
    
    int n = 10; // Number of days and assignments
    
    vector<int> D(n, 0);
    vector<int> P(n, 0);
    vector<int> SG(n, -1);
    vector<int> SB(n, -1);
    vector<int> C(n, -1);
    vector<bool> vis(n, false);
    vector<bool> visBS(n, false);
    
    // Initialize deadlines and points
    srand((unsigned) time(NULL));
    
    for(int i = 0; i < n; i++) {
        D[i] = rand() % n;
        P[i] = rand() % (2 * n);
    }
    
    // Compute maximum obtainable points
    int val1 = maxPointsGreedy(D, P, SG);
    int val2 = maxPointsBackTrack(D, P, 0, vis);
    int val3 = 0;
    
    maxPointsBackTrackSchedule(D, P, 0, visBS, C, SB, val3);
    
    // Verify results
    int sum1 = checkSum(SG, P);
    int sum2 = checkSum(SB, P);
    
    // Print results
    cout << "max points greedy: " << val1 << endl;
    cout << "max points backtrack: " << val2 << endl;
    cout << "max points backtrack2: " << val3 << endl;
    cout << "sum verification greedy: " << sum1 << endl;
    cout << "sum verification backtracking: " << sum2 << endl;
    
    return 0;
}
