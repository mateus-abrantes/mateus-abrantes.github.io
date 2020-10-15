#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    cv::Mat image;
    int width, height;
    int nobjects, nburacos;
    cv::Point p;
    image = cv::imread("bolhas.png", cv::IMREAD_GRAYSCALE);

    if (!image.data)
    {
        std::cout << "imagem nao carregou corretamente\n";
        return (-1);
    }
    width = image.cols;
    height = image.rows;
    std::cout << width << "x" << height << std::endl;

    p.x = 0;
    p.y = 0;

    // Removendo os objetos percentes as bordas
    // Bordas das laterais esquerdas e direitas
    for (int i = 0; i < height; i++)
    {
        if (image.at<uchar>(i, 0) == 255)
        {
            p.x = 0;
            p.y = i;
            // preenche o objeto com 0
            cv::floodFill(image, p, 0);
        }

        if (image.at<uchar>(i, width - 1) == 255)
        {
            p.x = width - 1;
            p.y = i;
            // preenche o objeto com 0
            cv::floodFill(image, p, 0);
        }
    }
    cv::imwrite("image_bordas_verticais.png", image);
    // Bordas das partes superior e inferior
    for (int i = 1; i < width - 1; i++)
    {
        if (image.at<uchar>(0, i) == 255)
        {
            p.x = i;
            p.y = 0;
            cv::floodFill(image, p, 0);
        }

        if (image.at<uchar>(height - 1, i) == 255)
        {
            p.x = i;
            p.y = height - 1;
            cv::floodFill(image, p, 0);
        }
    }
    cv::imwrite("image_bordas_horizontais.png", image);

    p.x = 0;
    p.y = 0;
    cv::floodFill(image, p, 120);

    // busca objetos presentes
    nobjects = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (image.at<uchar>(i, j) == 255)
            {
                // achou um objeto
                nobjects++;
                p.x = j;
                p.y = i;
                cv::floodFill(image, p, 50);
            }
        }
    }
    cv::imwrite("image_num_objetos.png", image);

    // numero de buracos
    nburacos = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (image.at<uchar>(i, j) == 0)
            {
                // achou um objeto
                nburacos++;
                p.x = j;
                p.y = i;
                cv::floodFill(image, p, 200);
            }
        }
    }
    std::cout << "Foram encontrados " << nobjects << " objetos" << endl;
    std::cout << "Foram encontrados " << nburacos << " objetos contendo buracos" << endl;
    cv::imshow("image", image);
    cv::imwrite("labeling.png", image);
    cv::waitKey();
    return 0;
}