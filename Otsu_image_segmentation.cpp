#include<iostream>
#include<string>
#include<vector>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void plotHistogram(vector<int> histogram) {
    int image_w = 512 * 2;
    int image_h = 800;
    int binW = image_w / 256.0;
    int pad = 60;
    Mat image(image_h + 2 * pad, image_w + 2 * pad, CV_8UC1, Scalar(255));
    float maxFreq = *max_element(histogram.begin(), histogram.end());

    // Put text on the image
    putText(image, "Histogram of pixels", Point(image_w/3, pad/2), 
            FONT_HERSHEY_DUPLEX, 1, Scalar(0));

    vector<int> height(256, 0);
    for (int i = 0; i <= 255; i++)
        height[i] = (histogram[i] / maxFreq) * image_h;

    // draw bins
    int thickness = 2;
    for (int i = 0; i <= 255; i++) {
        int x1, y1, x2, y2;
        x1 = x2 = binW * i + pad;
        y1 = image_h + pad;
        y2 = y1 - height[i];
        line(image, Point(x1, y1), Point(x2, y2), Scalar(0), thickness);
    }

    String windowName = "Histogram";
    namedWindow(windowName, WINDOW_NORMAL);
    imshow(windowName, image);
    waitKey(0);
    destroyWindow(windowName);
}

int calculateThreshold(Mat image) {
    vector<int> histogram(256, 0);
    for (int r = 0; r < image.rows; r++) {
        for (int c = 0; c < image.cols; c++) {
            int pixel = image.at<uchar>(r, c);
            histogram[pixel]++;
        }
    }
    plotHistogram(histogram);

    int size = image.rows * image.cols;
    long long sum = 0;
    for (int i = 0; i <= 255; i++)
        sum += i * histogram[i];
    int w1, w2;
    w1 = w2 = 0;
    long double currSum = 0;
    int T = 0;
    long double varMax = 0;

    for (int t = 0; t <= 255; t++) {
        w1 += histogram[t];
        if (w1 == 0)
            continue;
        else if (w2 == size)
            break;

        w2 = size - w1;
        currSum += t * histogram[t];
        long double u1, u2;
        u1 = currSum / w1;
        u2 = (sum - currSum) / w2;

        long double var = w1 * w2 * (u1 - u2) * (u1 - u2);
        if (var > varMax) {
            varMax = var;
            T = t;
        }
    }
    return T;
}

Mat applyBinarization(Mat image, int T) {
    for (int r = 0; r < image.rows; r++) {
        for (int c = 0; c < image.cols; c++) {
            int pixel = image.at<uchar>(r, c);
            if (pixel <= T)
                image.at<uchar>(r, c) = 0;
            else
                image.at<uchar>(r, c) = 255;
        }
    }
    return image;
}

int main() {
    // Read the image
    Mat image = imread("ADD IMAGE PATH HERE", IMREAD_GRAYSCALE);
    if (image.empty()) {
        cout << "Image not found!" << endl;
        cin.get();
        return -1;
    }

    // Display the original image
    String windowName = "Original image";
    namedWindow(windowName, WINDOW_NORMAL);
    imshow(windowName, image);
    waitKey(0);
    destroyWindow(windowName);

    // Calculate threshold value
    int T = calculateThreshold(image);
    cout << "Threshold value = " << T << "\n";
    // Apply binarization
    Mat segmentedImage = applyBinarization(image, T);

    // Display segmented image
    windowName = "Segmented image";
    namedWindow(windowName, WINDOW_NORMAL);
    imshow(windowName, segmentedImage);
    waitKey(0);
    destroyWindow(windowName);

    return 0;
}