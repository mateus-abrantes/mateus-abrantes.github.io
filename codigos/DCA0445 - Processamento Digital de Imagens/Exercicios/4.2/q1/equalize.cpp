#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{

    cv::Mat image, imagem_equalizada, hist, hist_equalizado;
    cv::VideoCapture cap;
    int width, height;
    int nbins = 64;
    float range[] = {0, 255};
    const float *histrange = {range};
    bool uniform = true;
    bool acummulate = false;
    int key;

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
    // Matriz do histogrma imagem original
    cv::Mat histImg(histh, histw, CV_8UC1, Scalar(0, 0, 0));
    // Matriz do histogrma imagem equalizada
    cv::Mat histImg_equalizada(histh, histw, CV_8UC1, Scalar(0, 0, 0));

    while (true)
    {

        cap >> image;
        // imagem cinza
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
        // equalizando a imagem e colocando na matriz da imagem_equalizada
        cv::equalizeHist(image, imagem_equalizada);
        // histograma da imagem original
        cv::calcHist(&image, 1, 0, Mat(), hist, 1,
                     &nbins, &histrange,
                     uniform, acummulate);
        // histograma da imagem equalizada
        cv::calcHist(&imagem_equalizada, 1, 0, Mat(), hist_equalizado, 1,
                     &nbins, &histrange,
                     uniform, acummulate);
        // normalizando os histogramas
        cv::normalize(hist, hist, 0, histImg.rows, cv::NORM_MINMAX, -1, cv::Mat());
        cv::normalize(hist_equalizado, hist_equalizado, 0, histImg_equalizada.rows, cv::NORM_MINMAX, -1, cv::Mat());

        // zerando as matrizes
        histImg.setTo(cv::Scalar(0));
        histImg_equalizada.setTo(cv::Scalar(0));

        for (int i = 0; i < nbins; i++)
        {
            // Desenhando os histogramas
            cv::line(histImg,
                     cv::Point(i, histh),
                     cv::Point(i, histh - cvRound(hist.at<float>(i))),
                     cv::Scalar(255, 255, 255), 1, 8, 0);
            cv::line(histImg_equalizada,
                     cv::Point(i, histh),
                     cv::Point(i, histh - cvRound(hist_equalizado.at<float>(i))),
                     cv::Scalar(255, 255, 255), 1, 8, 0);
        }
        // copiando para a imagem da camera os histogramas
        histImg.copyTo(image(cv::Rect(0, 0, nbins, histh)));
        histImg_equalizada.copyTo(imagem_equalizada(cv::Rect(0, 0, nbins, histh)));

        cv::imshow("Imagem Original", image);
        cv::imshow("Imagem Equalizada", imagem_equalizada);
        cv::imwrite("imagem_original.png", image);
        cv::imwrite("imagem_equalizada.png", imagem_equalizada);
        key = cv::waitKey(30);
        if (key == 27)
            break;
    }
    return 0;
}