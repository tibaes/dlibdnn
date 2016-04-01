#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << "<frames_dir path>" << std::endl;
    throw std::runtime_error("Wrong parameters. See usage.");
  }

  cv::VideoCapture cap;
  cap.open(0);
  
  cv::Mat frame;
  bool capturing = true;
  int fid = 1000;

  while (capturing) {
    cap >> frame;
    cv::imshow("Frame", frame);
  
    std::ostringstream s_path;
    s_path << argv[1] << fid++ << ".jpg";
    cv::imwrite(s_path.str(), frame);

    if (cv::waitKey(10) == 'q')
      capturing = false;
  }
  
  std::cout << "Stopping capture." << std::endl;
  return 0;
}
