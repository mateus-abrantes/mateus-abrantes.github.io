#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>

double alfa;
int alfa_slider = 0;
int alfa_slider_max = 100;

int top_slider = 0;
int top_slider_max = 100;

int foco_slider = 0;
int foco_slider_max = 100;

cv::Mat image1, image2, frame32f, frameFiltered, imagem_borrada, blended,final;
cv::Mat imageTop;

char TrackbarName[50];

void on_trackbar_blend(int, void *)
{
    alfa = (double)alfa_slider / alfa_slider_max;
    cv::addWeighted(image1, 1 - alfa, final, alfa, 0.0, blended);
    cv::imshow("tiltshift", blended);
    cv::imwrite("tiltshift.png",blended);
}

void on_trackbar_line(int, void *)
{
    image1.copyTo(imageTop);
    imagem_borrada.copyTo(final);
    int width_img = image1.cols;
    int height_img = image1.rows;
    int posy = (foco_slider*height_img)/(100);
    int limit = (top_slider * height_img)/100;
    if((posy+limit) > height_img){
        posy =(height_img-limit);
        limit=(height_img-posy);
    }
    if (limit > 0)
    {
        cv::Mat tmp = image1(cv::Rect(0, posy, width_img, limit));
        tmp.copyTo(final(cv::Rect(0, posy, width_img, limit)));
    }
    on_trackbar_blend(alfa_slider, 0);
}

int main(int argvc, char **argv)
{
    //mascara da media
    float media[] = {1, 1, 1,
                     1, 1, 1,
                     1, 1, 1};
    cv::Mat mask1, mask;
    mask = cv::Mat(3, 3, CV_32F, media);
    cv::scaleAdd(mask, 1 / 9.0, cv::Mat::zeros(3, 3, CV_32F), mask1);
    cv::swap(mask, mask1);
    //lendo a imagem
    image1 = cv::imread("foto.png");
    //copiando a imagem1 para a imagem2
    image1.copyTo(image2);
    //convertendo a imagem2 para float e salvando em frame32f
    image2.convertTo(frame32f, CV_32F);
    //Aplicando o filtro da media
    for (int i = 0; i < 10; i++)
    {
        cv::filter2D(frame32f, frameFiltered, frame32f.depth(), mask,
                     cv::Point(1, 1), 0);
        frame32f = frameFiltered;
    }
    frameFiltered.convertTo(imagem_borrada, CV_8U);
    frameFiltered.convertTo(final, CV_8U);
    image2.copyTo(imageTop);
    
    cv::namedWindow("tiltshift", 1);

    std::sprintf(TrackbarName, "Alpha x %d", alfa_slider_max);
    cv::createTrackbar(TrackbarName, "tiltshift",
                       &alfa_slider,
                       alfa_slider_max,
                       on_trackbar_blend);
    on_trackbar_blend(alfa_slider, 0);

    std::sprintf(TrackbarName, "foco x %d", top_slider_max);
    cv::createTrackbar(TrackbarName, "tiltshift",
                       &top_slider,
                       top_slider_max,
                       on_trackbar_line);
    std::sprintf(TrackbarName, "posicao x %d", foco_slider_max);
    cv::createTrackbar(TrackbarName, "tiltshift",
                       &foco_slider,
                       foco_slider_max,
                       on_trackbar_line);
    on_trackbar_line(top_slider, 0);

    cv::waitKey(0);
    return 0;
}
