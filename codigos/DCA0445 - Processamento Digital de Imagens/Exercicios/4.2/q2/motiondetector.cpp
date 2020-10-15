#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{

    cv::Mat image, hist, hist_anterior;
    cv::VideoCapture cap;
    int width, height;
    int nbins = 64;
    float range[] = {0, 255};
    const float *histrange = {range};
    bool uniform = true;
    bool acummulate = false;
    int key, cont = 0;
    float diferenca;

    cap.open(0);

    if (!cap.isOpened())
    {
        std::cout << "cameras indisponiveis";
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "largura = " << width << std::endl;
    std::cout << "altura  = " << height << std::endl;

    int histw = nbins, histh = nbins / 2;
    // Matriz do histogrma imagem
    cv::Mat histImg(histh, histw, CV_8UC1, Scalar(0, 0, 0));
    // Matriz do histogrma imagem anterior
    cv::Mat histImg_anterior(histh, histw, CV_8UC1, Scalar(0, 0, 0));
    while (true)
    {
        cont++;
        cap >> image;
        // imagem cinza
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
        // histograma da imagem
        cv::calcHist(&image, 1, 0, Mat(), hist, 1,
                     &nbins, &histrange,
                     uniform, acummulate);
        diferenca = 1;
        // normalizando os histogramas
        cv::normalize(hist, hist, 0, histImg.rows, cv::NORM_MINMAX, -1, cv::Mat());
        cv::normalize(hist_anterior, hist_anterior, 0, histImg.rows, cv::NORM_MINMAX, -1, cv::Mat());

        // zerando as matrizes
        histImg.setTo(cv::Scalar(0));
        for (int i = 0; i < nbins; i++)
        {
            // Desenhando os histogramas
            cv::line(histImg,
                     cv::Point(i, histh),
                     cv::Point(i, histh - cvRound(hist.at<float>(i))),
                     cv::Scalar(255, 255, 255), 1, 8, 0);
        }
        // copiando para a imagem da camera os histogramas
        histImg.copyTo(image(cv::Rect(0, 0, nbins, histh)));
        if (cont == 1)
        {
            // histograma da imagem anterior
            cv::calcHist(&image, 1, 0, Mat(), hist_anterior, 1,
                         &nbins, &histrange,
                         uniform, acummulate);
        }
        else if (cont == 10)
        {
            cont = 0;
            diferenca = cv::compareHist(hist, hist_anterior, HISTCMP_CORREL);
            std::cout << diferenca << std::endl;
        }

        if (diferenca < 0.99)
        {
            cv::putText(image, "Movimento detectado!", cv::Point(70, 30),
                        FONT_HERSHEY_SIMPLEX, .7,
                        Scalar(0, 0, 0, 0), 2);
            cv::imwrite("imagem_movimento.png", image);
        }

        cv::imwrite("imagem_original.png", image);
        cv::imshow("Imagem", image);
        key = cv::waitKey(30);
        if (key == 27)
            break;
    }
    return 0;
}