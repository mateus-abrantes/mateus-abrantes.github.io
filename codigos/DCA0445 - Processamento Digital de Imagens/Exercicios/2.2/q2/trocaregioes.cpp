#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char **)
{
    cv::Mat image, image_quadrantes;
    cv::Mat quadrante1, quadrante2, quadrante3, quadrante4;
    // Dimencoes da imagem
    int width, height;

    image = cv::imread("biel.png", cv::IMREAD_GRAYSCALE);
    if (!image.data)
    {
        std::cout << "nao abriu biel.png" << std::endl;
    }
    // Copiando a imagem original para uma nova matriz que sera manipulada
    image_quadrantes = image.clone();
    width = image.cols;
    height = image.rows;
    cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);
    // Fatiando os quadrantes das imagens
    quadrante1 = image(cv::Rect(0, 0, (width / 2), (height / 2)));
    quadrante2 = image(cv::Rect((width / 2), 0, (width / 2), (height / 2)));
    quadrante3 = image(cv::Rect(0, (height / 2), (width / 2), (height / 2)));
    quadrante4 = image(cv::Rect((width / 2), (height / 2), (width / 2), (height / 2)));

    // copiando para imagem auxiliar
    quadrante1.copyTo(image_quadrantes(cv::Rect((width / 2), (height / 2), (width / 2), (height / 2))));
    quadrante2.copyTo(image_quadrantes(cv::Rect(0, (height / 2), (width / 2), (height / 2))));
    quadrante3.copyTo(image_quadrantes(cv::Rect((width / 2), 0, (width / 2), (height / 2))));
    quadrante4.copyTo(image_quadrantes(cv::Rect(0, 0, (width / 2), (height / 2))));

    cv::imwrite("imagem_resultado.png", image_quadrantes);
    cv::imshow("janela", image_quadrantes);
    cv::waitKey();
    return 0;
}