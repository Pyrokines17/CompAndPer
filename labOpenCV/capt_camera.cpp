#include <fstream>
#include <math.h>
#include <opencv2/core.hpp>  
#include <opencv2/imgproc.hpp> 
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp> 
#include <opencv2/imgcodecs.hpp>
#include <ctime>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    VideoCapture cap(0);
    Mat img, blure;

    unsigned int program_time = 0;
    double takenTime = 0, result = 0;
    ofstream out("data_time.csv");
    out << "fps, program time sec., read time %, fps, show time %, fps, blure time %" << endl;
    struct timespec pts, pte, rts, rte, sts, ste, bts, bte;

    while (true) {
        out << cap.get(CAP_PROP_FPS) << ",";

        clock_gettime(CLOCK_MONOTONIC, &pts);

        clock_gettime(CLOCK_MONOTONIC, &rts);
        cap.read(img);
        clock_gettime(CLOCK_MONOTONIC, &rte);

        clock_gettime(CLOCK_MONOTONIC, &sts);
        imshow("Normal", img);
        clock_gettime(CLOCK_MONOTONIC, &ste);

        clock_gettime(CLOCK_MONOTONIC, &bts);
        medianBlur(img, blure, 25);
        clock_gettime(CLOCK_MONOTONIC, &bte);
        imshow("Blure", blure);

        clock_gettime(CLOCK_MONOTONIC, &pte);

        double pt = abs(pte.tv_sec - pts.tv_sec) + abs(pte.tv_nsec - pts.tv_nsec) * 1e-9;
        out << pt << ",";

        double rt = abs(rte.tv_sec - rts.tv_sec) + abs(rte.tv_nsec - rts.tv_nsec) * 1e-9;
        out << rt/pt * 100 << "," << 1.0 / rt << ",";

        double st = abs(ste.tv_sec - sts.tv_sec) + abs(ste.tv_nsec - sts.tv_nsec) * 1e-9;
        out << st/pt * 100 << "," << 1.0 / st << ",";

        double bt =  abs(bte.tv_sec - bts.tv_sec) + abs(bte.tv_nsec - bts.tv_nsec) * 1e-9;
        out << bt/pt * 100 << ";" << endl;

        char c = waitKey(1);
        if (c == 'z') break;
    }

    return 0;
}
