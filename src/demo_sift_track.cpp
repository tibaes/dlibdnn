#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <stdexcept>
#include <vector>

const float inlier_threshold = 2.5f; // Distance threshold to identify inliers
const float nn_match_ratio = 0.8f;   // Nearest neighbor matching ratio

int main(const int argc, const char **argv) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << "<model path> <target path>";
    throw std::invalid_argument("Wrong usage.");
  }

  cv::Mat img_model = cv::imread(argv[1]);
  cv::Mat img_target = cv::imread(argv[2]);

  cv::Ptr<cv::Feature2D> algorithm = cv::xfeatures2d::SIFT::create();
  std::vector<cv::KeyPoint> keypoits_model, keypoits_target;
  cv::Mat descriptors_model, descriptors_target;
  algorithm->detectAndCompute(img_model, cv::noArray(), keypoits_model,
                              descriptors_model);
  algorithm->detectAndCompute(img_target, cv::noArray(), keypoits_target,
                              descriptors_target);

  auto matcher = cv::BFMatcher::BFMatcher();
  std::vector<std::vector<cv::DMatch>> matches;
  matcher.knnMatch(descriptors_model, descriptors_target, matches, 2);

  /*
  cv::Mat r0, r1;
  cv::drawKeypoints(img_model, keypoits_model, r0);
  cv::drawKeypoints(img_target, keypoits_target, r1);
  cv::imshow("keys0", r0);
  cv::imshow("keys1", r1);
  cv::waitKey(10);
   */

  std::vector<cv::DMatch> good_matches;
  for (size_t i = 0; i < matches.size(); i++) {
    cv::DMatch first = matches[i][0];
    float dist1 = matches[i][0].distance;
    float dist2 = matches[i][1].distance;
    if (dist1 < nn_match_ratio * dist2) {
      good_matches.push_back(first);
    }
  }

  cv::Mat res;
  cv::drawMatches(img_model, keypoits_model, img_target, keypoits_target,
                  good_matches, res);
  cv::imwrite("sift.png", res);

  return 0;
}