#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 2
#define RAIO 3
int top_slider = 10;
int top_slider_max = 200;
int raio_slider = 3;
int raio_slider_max = 10;

char TrackbarName[50];

Mat image, border;
int red, blue, green;
int width, height, x, y;
void on_trackbar_cannypoints(int, void *)
{
  vector<int> yrange;
  vector<int> xrange;
  Mat points;
  width = image.size().width;
  height = image.size().height;

  xrange.resize(height / STEP);
  yrange.resize(width / STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for (uint i = 0; i < xrange.size(); i++)
  {
    xrange[i] = xrange[i] * STEP + STEP / 2;
  }

  for (uint i = 0; i < yrange.size(); i++)
  {
    yrange[i] = yrange[i] * STEP + STEP / 2;
  }

  points = Mat(height, width, CV_8UC3, CV_RGB(255, 255, 255));

  random_shuffle(xrange.begin(), xrange.end());

  for (auto i : xrange)
  {
    random_shuffle(yrange.begin(), yrange.end());
    for (auto j : yrange)
    {
      x = i + rand() % (2 * JITTER) - JITTER + 1;
      y = j + rand() % (2 * JITTER) - JITTER + 1;
      blue = image.at<Vec3b>(x, y)[0];
      green = image.at<Vec3b>(x, y)[1];
      red = image.at<Vec3b>(x, y)[2];
      circle(points,
             Point(y, x),
             RAIO,
             CV_RGB(red, green, blue),
             -1,
             LINE_AA);
    }
  }
  imwrite("pontinhismo.jpg", points);
  Canny(image, border, top_slider, 3 * top_slider);
  imwrite("canny.png", border);
  imshow("canny", border);

  for (int j = 0; j < height; j++)
  {
    for (int k = 0; k < width; k++)
    {
      if (border.at<uchar>(j, k) > 0)
      {
        blue = image.at<Vec3b>(j, k)[0];
        green = image.at<Vec3b>(j, k)[1];
        red = image.at<Vec3b>(j, k)[2];
        circle(points, Point(k, j), raio_slider, CV_RGB(red, green, blue), -1, LINE_AA);
      }
    }
  }
  namedWindow("cannypoints", WINDOW_AUTOSIZE);
  imshow("cannypoints", points);
  imwrite("cannypoints.png", points);
}

int main(int argc, char **argv)
{
  image = imread("foto.jpg", IMREAD_COLOR);

  srand(time(0));

  if (!image.data)
  {
    cout << "nao abriu a imagen" << endl;
    exit(0);
  }
  sprintf(TrackbarName, "Threshold inferior", top_slider_max);

  namedWindow("canny", 1);
  createTrackbar(TrackbarName, "canny",
                 &top_slider,
                 top_slider_max,
                 on_trackbar_cannypoints);
  sprintf(TrackbarName, "Raio", raio_slider_max);
  createTrackbar(TrackbarName, "canny",
                 &raio_slider,
                 raio_slider_max,
                 on_trackbar_cannypoints);
  on_trackbar_cannypoints(top_slider, 0);
  waitKey();
  return 0;
}