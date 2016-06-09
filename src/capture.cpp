#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdexcept>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << "<frames_dir path> [fps]" << std::endl;
    throw std::runtime_error("Wrong parameters. See usage.");
  }

  cv::VideoCapture cap;
  cap.open(0);

  cv::Mat frame;
  int sleep = (argc == 3) ? (int)round(1000.0 / atof(argv[2])) : 30;
  bool capturing = true;
  int fid = 1000;

  std::cout << "Capture Interval: " << sleep << "ms. " << std::endl;
  while (capturing) {
    cap >> frame;
    cv::imshow("Frame", frame);

    std::ostringstream s_path;
    s_path << argv[1] << fid++ << ".jpg";
    cv::imwrite(s_path.str(), frame);

    if (cv::waitKey(sleep) == 'q')
      capturing = false;
  }

  std::cout << "Stopping capture." << std::endl;
  return 0;
}
