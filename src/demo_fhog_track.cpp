// Rafael H. Tibaes !2016
// r@fael.nl http://fael.nl


#include <dlib/svm_threaded.h>
#include <dlib/data_io.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/dir_nav.h>
#include <stdexcept>
#include <iostream>
#include <fstream>

using namespace dlib;
using namespace std;

int main(int argc, char** argv) {
  const string detectorFilename = "../share/knife_detector.svm";
  object_detector< scan_fhog_pyramid< pyramid_down<6> > > detector;
  deserialize(detectorFilename) >> detector;
  // image_window hogwin(draw_fhog(detector), "Learned fHOG detector");

  if (argc != 2) {
    cout << "Call this program like this: " << endl;
    cout << "./demo ../video_frames" << endl;
    throw runtime_error("Invalid arguments.");
  }

  std::vector<file> files = get_files_in_directory_tree(argv[1],
                                                        match_ending(".jpg"));
  std::sort(files.begin(), files.end());
  if (files.size() == 0) {
    throw runtime_error("No images found.");
  }

  array2d<unsigned char> img;
  image_window win;
  bool detected = false;
  drectangle object_roi;

  unsigned long fid;
  for (fid = 0; fid < files.size() && !detected; ++fid) {
    load_image(img, files[fid]);
    win.set_image(img);

    array2d<unsigned char> imgvm, imghm, imgvhm;
    flip_image_up_down(img, imghm);
    flip_image_left_right(img, imgvm);
    flip_image_left_right(imghm, imgvhm);

    std::vector<rectangle> dets = detector(img);
    std::vector<rectangle> detshm = detector(imghm);
    std::vector<rectangle> detsvm = detector(imgvm);
    std::vector<rectangle> detsvhm = detector(imgvhm);

    detected = !(dets.empty() && detshm.empty() && detsvm.empty()
               && detsvhm.empty());
    if (!dets.empty()) {
      object_roi = drectangle(dets.front());
    }
    else if (!detsvm.empty()) {
      rectangle r = detsvm.front();
      object_roi = drectangle(rectangle(r.right(), r.top(),
                                        r.left(), r.bottom()));
    } else if (!detshm.empty()) {
      rectangle r = detshm.front();
      object_roi = drectangle(rectangle(r.left(), r.bottom(),
                                        r.right(), r.top()));
    } else if (!detsvhm.empty()) {
      rectangle r = detsvhm.front();
      object_roi = drectangle(rectangle(r.right(), r.bottom(),
                                        r.left(), r.top()));
    }
  }

  if (detected) {
    cout << "Object found at (" << object_roi << ")" << endl;
    cout << " Press any key to start the tracker." << endl;
    win.clear_overlay();
    win.add_overlay(object_roi);
    cin.get();
  } else {
    throw runtime_error("Object not found, shutting down.");
  }

  correlation_tracker tracker;
  tracker.start_track(img, object_roi);

  for (; fid < files.size(); ++fid) {
    load_image(img, files[fid]);
    tracker.update(img);

    win.set_image(img);
    win.clear_overlay();
    win.add_overlay(tracker.get_position());

    cout << "hit enter to process next frame" << endl;
    cin.get();
  }

  return 0;
}
