#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include <opencv2/imgcodecs.hpp>
#include <ctime>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    VideoCapture cap(0);

    Mat img;
    Mat grey;
    Mat sobelx;
    Mat draw;

    timespec start, finish;

    double minVal, maxVal, takenTime = 0, result = 0;

    while (true) {
        double fps = cap.get(CAP_PROP_FPS);
        cout << "Frames per second using cap.get() : " << fps << endl;

        clock_gettime(CLOCK_MONOTONIC, &start);
        cap.read(img);
        clock_gettime(CLOCK_MONOTONIC, &finish);

        takenTime = (finish.tv_sec - start.tv_sec) * 1e9;
        takenTime = (takenTime + (finish.tv_nsec - start.tv_nsec)) * 1e-9;

        result = 1.0 / takenTime;

        cout << "Frames per second using clock_gettime() for read: " << result << endl;

        clock_gettime(CLOCK_MONOTONIC, &start);
        cvtColor(img, grey, COLOR_BGR2GRAY);
        Sobel(grey, sobelx, CV_32F, 1, 0);
        minMaxLoc(sobelx, &minVal, &maxVal);
        sobelx.convertTo(draw, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
        clock_gettime(CLOCK_MONOTONIC, &finish);

        takenTime = (finish.tv_sec - start.tv_sec) * 1e9;
        takenTime = (takenTime + (finish.tv_nsec - start.tv_nsec)) * 1e-9;

        result = 1.0 / takenTime;

        cout << "Frames per second using clock_gettime() for filter: " << result << endl;

        clock_gettime(CLOCK_MONOTONIC, &start);
        imshow("Image", draw);
        clock_gettime(CLOCK_MONOTONIC, &finish);

        takenTime = (finish.tv_sec - start.tv_sec) * 1e9;
        takenTime = (takenTime + (finish.tv_nsec - start.tv_nsec)) * 1e-9;

        result = 1.0 / takenTime;

        cout << "Frames per second using clock_gettime() for show: " << result << endl;

        char c = waitKey(1);
        if (c == 'z') break;
    }

    return 0;
}
