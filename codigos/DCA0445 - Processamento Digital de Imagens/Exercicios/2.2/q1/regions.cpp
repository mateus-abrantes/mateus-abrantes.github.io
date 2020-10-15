#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Estrutura para ponto
struct ponto
{
  int x;
  int y;
};

void limites(int &variavel, int limite)
{
  while (variavel > limite)
  {
    std::cout << "Valor invalido digite novamente:";
    std::cin >> variavel;
  }
}

int main(int, char **)
{
  cv::Mat image;
  cv::Vec3b val;
  // Definindo as variaveis para os pontos de escolha
  struct ponto P1;
  struct ponto P2;
  int x1, x2, y1, y2;

  image = cv::imread("biel.png", cv::IMREAD_GRAYSCALE);
  if (!image.data)
    std::cout << "nao abriu biel.png" << std::endl;

  cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);
  std::cout << "Digite o valor da coordenada x do primeiro ponto entre 0 e " << image.rows << ":";
  std::cin >> P1.x;
  limites(P1.x, image.rows);
  std::cout << "Digite o valor da coordenada y do primeiro ponto entre 0 e " << image.cols << ":";
  std::cin >> P1.y;
  limites(P1.y, image.cols);
  std::cout << "Digite o valor da coordenada x do segundo ponto entre 0 e " << image.rows << ":";
  std::cin >> P2.x;
  limites(P2.x, image.rows);
  std::cout << "Digite o valor da coordenada y do segundo ponto entre 0 e " << image.cols << ":";
  std::cin >> P2.y;
  limites(P2.y, image.cols);
  std::cout << "As coordenadas dos pontos sao: P1(" << P1.x << "," << P1.y << ") , P1(" << P1.x << "," << P1.y << ").";

  // Pegando as coordenadas x e y maiores e menores
  // Coordenada x
  if (P1.x > P2.x)
  {
    x1 = P2.x;
    x2 = P1.x;
  }
  else
  {
    x1 = P1.x;
    x2 = P2.x;
  }
  // Coordenada y
  if (P1.y > P2.y)
  {
    y1 = P2.y;
    y2 = P1.y;
  }
  else
  {
    y1 = P1.y;
    y2 = P2.y;
  }

  for (int i = x1; i < x2; i++)
  {
    for (int j = y1; j < y2; j++)
    {
      image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
    }
  }
  cv::imwrite("imagem_resultado.png", image);
  cv::imshow("janela", image);
  cv::waitKey();
  return 0;
}
