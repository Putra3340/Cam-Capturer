#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <thread>
#include <filesystem>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

void captureAndSaveImage(int cameraIndex) {
    VideoCapture cap(cameraIndex);

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        cerr << "Error: Unable to open camera " << cameraIndex << endl;
        return;
    }

    // Capture a frame from the camera
    Mat frame;
    cap >> frame;

    // Check if frame is empty
    if (frame.empty()) {
        cerr << "Error: Unable to capture frame from camera " << cameraIndex << endl;
        return;
    }

    // Get current time
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    // Format filename
    stringstream filename_ss;
    filename_ss << "Captured/" << setfill('0')
        << setw(2) << now->tm_sec << "-"
        << setw(2) << now->tm_min << "-"
        << setw(2) << now->tm_hour << "_"
        << setw(2) << now->tm_mday << "-"
        << setw(2) << now->tm_mon + 1 << "-"
        << setw(2) << now->tm_year + 1900
        << "_Cam" << cameraIndex
        << ".png";
    string filename = filename_ss.str();

    // Create directory if it doesn't exist
    fs::create_directories("captured");

    // Save the frame as a PNG image
    bool success = imwrite(filename, frame);

    // Check if image saved successfully
    if (!success) {
        cerr << "Error: Unable to save image from camera " << cameraIndex << endl;
        return;
    }

    cout << "Image saved successfully as " << filename << endl;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const int numCameras = 6; // Number of cameras
    thread cameraThreads[numCameras];

    // Create threads for each camera
    for (int cameraIndex = 0; cameraIndex < numCameras; cameraIndex++) {
        cameraThreads[cameraIndex] = thread(captureAndSaveImage, cameraIndex);
    }

    // Join threads
    for (int cameraIndex = 0; cameraIndex < numCameras; cameraIndex++) {
        cameraThreads[cameraIndex].join();
    }

    return 0;
}
