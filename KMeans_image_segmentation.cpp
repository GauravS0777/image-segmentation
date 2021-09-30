#include<iostream>
#include<string>
#include<vector>
#include<time.h>
#include<set>
#include<utility>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat image;
vector<vector<pair<int, int>>> clusters;

set<int> initialCenters(int k) {
    set<int> centers;
    int rows = image.rows;
    int cols = image.cols;
    while (centers.size() < k) {
        int r = rand() % rows;
        int c = rand() % cols;
        int pixel = image.at<uchar>(r, c);
        centers.insert(pixel);
    }
    return centers;
}

set<int> calculateCenters() {
    int n = clusters.size();
    set<int> centers;
    for (int i = 0; i < n; i++) {
        long long center = 0;
        for (auto it : clusters[i]) {
            int r = it.first;
            int c = it.second;
            int pixel = image.at<uchar>(r, c);
            center += pixel;
        }
        if (clusters[i].size() != 0) {
            center /= clusters[i].size();
            centers.insert(center);
        }
    }

    int rows = image.rows;
    int cols = image.cols;
    while (centers.size() < n) {
        int r = rand() % rows;
        int c = rand() % cols;
        int pixel = image.at<uchar>(r, c);
        centers.insert(pixel);
    }

    return centers;
}

void iterations(set<int> centers) {
    int rows = image.rows;
    int cols = image.cols;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int minD = INT32_MAX;
            int closest = -1;
            int i = 0;
            for (int center : centers) {
                int d = abs(image.at<uchar>(r, c) - center);
                if (d < minD) {
                    minD = d;
                    closest = i;
                }
                i++;
            }
            clusters[closest].push_back(make_pair(r, c));
        }
    }
}

Mat KMeans(int k, int maxIter = 100) {
    set<int> prev_centers;
    set<int> centers = initialCenters(k);
    cout << "\n\nFollowing are the initial centers:\n";
    for (int center : centers) {
        cout << center << "\n";
    }

    int count = 0;
    while (prev_centers != centers && count < maxIter) {
        clusters.clear();
        clusters.assign(k, vector<pair<int, int>>());
        prev_centers = centers;
        iterations(centers);
        centers = calculateCenters();
        count++;
    }
    cout << "\nNumber of iterations: " << count << "\n";

    cout << "\nFollowing are the final centers:\n";
    for (int center : centers) {
        cout << center << "\n";
    }

    Mat segmentedImage = image;
    int flag = 0;
    cout << "\nFinal clusters sizes:\n";
    for (vector<pair<int, int>> cluster : clusters) {
        cout << cluster.size() << "\n";
        for (pair<int, int> point : cluster) {
            int r = point.first;
            int c = point.second;
            if (flag == 0)
                image.at<uchar>(r, c) = 0;
            else
                image.at<uchar>(r, c) = 255;
        }
        flag++;
    }
    cout << "\n\n";
    return segmentedImage;
}

int main() {
    srand(time(0));

    // Read original image
    image = imread("Add path here", IMREAD_GRAYSCALE);
    if (image.empty()) {
        cout << "Image not found!" << endl;
        cin.get();
        return -1;
    }

    // Display original image
    String windowName = "Original image";
    namedWindow(windowName, WINDOW_NORMAL);
    imshow(windowName, image);
    waitKey(0);
    destroyWindow(windowName);

    Mat segmentedImage = KMeans(2);

    // Display segmented image
    windowName = "Segmented image";
    namedWindow(windowName, WINDOW_NORMAL);
    imshow(windowName, segmentedImage);
    waitKey(0);
    destroyWindow(windowName);
    return 0;
}
