#include<iostream>
#include<time.h>
#include<vector>
#include<set>
#include<utility>

using namespace std;

// dataset could be an image pixels values
// populate it before calling KMeans on it
vector<vector<int>> dataset;
vector<vector<pair<int, int>>> clusters;

set<int> initialCenters(int k){
    int rows = dataset.size();
    int cols = dataset[0].size();
    set<int> centers;
    while(centers.size() < k){
        int r = rand() % rows;
        int c = rand() % cols;
        int data = dataset[r][c];
        centers.insert(data);
    }
    return centers;
}

set<int> calculateCenters(){
    int n = clusters.size();
    set<int> centers;
    for(int i = 0; i<n; i++){
        long long center = 0;
        for(auto it: clusters[i]){
            int r = it.first;
            int c = it.second;
            int data = dataset[r][c];
            center += data;
        }
        if (clusters[i].size() != 0) {
            center /= clusters[i].size();
            centers.insert(center);
        }
    }

    int rows = dataset.size();
    int cols = dataset[0].size();
    while(centers.size() < n){
        int r = rand() % rows;
        int c = rand() % cols;
        int data = dataset[r][c];
        centers.insert(data);
    }

    return centers;
}

void iterations(set<int> centers){
    int rows = dataset.size();
    int cols = dataset[0].size();
    for(int r = 0; r<rows; r++){
        for(int c = 0; c<cols; c++){
            int minD = INT32_MAX;
            int closest = 0;
            int i = 0;
            for(int center: centers){
                int data = dataset[r][c];
                int d = abs(data - center);
                if(d < minD){
                    minD = d;
                    closest = i;
                }
                i++;
            }
            clusters[closest].push_back(make_pair(r, c));
        }
    }
}

void KMeans(int k, int maxIter){
    set<int> prev_centers;
    set<int> centers = initialCenters(k);

    int count = 0;
    while(prev_centers != centers && count < maxIter){
        clusters.clear();
        clusters.assign(k, vector<pair<int, int>>());
        prev_centers = centers;
        iterations(centers);
        centers = calculateCenters();
        count++;
    }
}
