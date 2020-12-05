#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
  int nClusters = 8;
  Mat rotulos;
  int nRodadas = 1;
  Mat centros;

  Mat img = imread("foto.jpg", IMREAD_COLOR);
  if (!img.data)
  {
    cout << "Imagem nao encontrada" << endl;
    return 0;
  }
  for (int i = 0; i < 10; i++)
  {
    Mat samples(img.rows * img.cols, 3, CV_32F);

    for (int y = 0; y < img.rows; y++)
    {
      for (int x = 0; x < img.cols; x++)
      {
        for (int z = 0; z < 3; z++)
        {
          samples.at<float>(y + x * img.rows, z) = img.at<Vec3b>(y, x)[z];
        }
      }
    }

    kmeans(samples,
           nClusters,
           rotulos,
           TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10000, 0.0001),
           nRodadas,
           KMEANS_RANDOM_CENTERS,
           centros);

    Mat rotulada(img.size(), img.type());
    for (int y = 0; y < img.rows; y++)
    {
      for (int x = 0; x < img.cols; x++)
      {
        int indice = rotulos.at<int>(y + x * img.rows, 0);
        rotulada.at<Vec3b>(y, x)[0] = (uchar)centros.at<float>(indice, 0);
        rotulada.at<Vec3b>(y, x)[1] = (uchar)centros.at<float>(indice, 1);
        rotulada.at<Vec3b>(y, x)[2] = (uchar)centros.at<float>(indice, 2);
      }
    }
    imshow("clustered image", rotulada);
    string indice;
    indice = "foto-kmeans"+to_string(i)+".jpg";
    imwrite(indice, rotulada);
  }
  waitKey(0);
}