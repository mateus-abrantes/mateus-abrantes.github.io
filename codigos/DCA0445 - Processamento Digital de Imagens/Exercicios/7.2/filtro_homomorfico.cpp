#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#define RADIUS 20
// troca os quadrantes da imagem da DFT
void deslocaDFT(cv::Mat &image)
{
    cv::Mat tmp, A, B, C, D;

    // se a imagem tiver tamanho impar, recorta a regiao para
    // evitar cÃ³pias de tamanho desigual
    image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));
    int cx = image.cols / 2;
    int cy = image.rows / 2;

    // reorganiza os quadrantes da transformada
    // A B   ->  D C
    // C D       B A
    A = image(cv::Rect(0, 0, cx, cy));
    B = image(cv::Rect(cx, 0, cx, cy));
    C = image(cv::Rect(0, cy, cx, cy));
    D = image(cv::Rect(cx, cy, cx, cy));

    // A <-> D
    A.copyTo(tmp);
    D.copyTo(A);
    tmp.copyTo(D);

    // C <-> B
    C.copyTo(tmp);
    B.copyTo(C);
    tmp.copyTo(B);
}

//Variaveis globais
int yH, yL, c, D0;
int yL_slider = 10;
const int yL_slider_max = 100;
int yH_slider = 60;
const int yH_slider_max = 100;
int c_slider = 10;
const int c_slider_max = 100;
int D0_slider = 10;
const int D0_max = 100;

cv::Mat image, padded, tmp, exibir;
std::vector<cv::Mat> planos;

// valores ideais dos tamanhos da imagem
// para calculo da DFT
int dft_M, dft_N;

// Filtro
cv::Mat filtro_homomorfico(void)
{
    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();
    cv::Mat_<float> realInput, zeros;

    cv::Mat imaginaryInput, complexImage, filtro;

    // parte imaginaria da matriz complexa (preenchida com zeros)
    zeros = cv::Mat_<float>::zeros(padded.size());

    // cria a compoente real
    realInput = cv::Mat_<float>(padded);

    // Preenchendo com 1
    realInput += cv::Scalar::all(1);

    //Calculando o log da matriz
    cv::log(realInput, realInput);

    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // prepara a matriz complexa para ser preenchida
    complexImage = cv::Mat(padded.size(), CV_32FC2, cv::Scalar(0));

    // combina o array de matrizes em uma unica
    // componente complexa
    cv::merge(planos, complexImage);

    // calcula o dft
    cv::dft(complexImage, complexImage);
    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    cv::normalize(complexImage, complexImage, 0, 1, cv::NORM_MINMAX);
    // a funÃ§Ã£o de transferencia (filtro de frequencia) deve ter o
    // mesmo tamanho e tipo da matriz complexa
    filtro = complexImage.clone();

    // cria uma matriz temporÃ¡ria para criar as componentes real
    // e imaginaria do filtro ideal
    tmp = cv::Mat(dft_M, dft_N, CV_32F);

    // prepara o filtro homomórfico
    for (int i = 0; i < dft_M; i++)
    {
        for (int j = 0; j < dft_N; j++)
        {
            tmp.at<float>(i, j) = (yH - yL) * (1 - exp(-c * (((i - dft_M / 2) * (i - dft_M / 2) + (j - dft_N / 2) * (j - dft_N / 2)) / (D0 * D0)))) + yL;
        }
    }

    // ambas em uma matriz multicanal complexa
    cv::Mat comps[] = {tmp, tmp};
    normalize(tmp, exibir, 0, 1, cv::NORM_MINMAX);
    cv::imshow("Filtro Homomórfico", exibir);

    merge(comps, 2, filtro);

    // aplica o filtro homomorfico
    mulSpectrums(complexImage, filtro, complexImage, 0);

    // troca novamente os quadrantes
    deslocaDFT(complexImage);

    // calcula a DFT inversa
    cv::idft(complexImage, complexImage);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    cv::split(complexImage, planos);

    // Calcula a exponencial da parte real (função inversa do log)
    exp(planos[0], planos[0]);

    // normaliza a parte real para exibicao
    cv::normalize(planos[0], planos[0], 0, 1, cv::NORM_MINMAX);
    return planos[0];
}

void on_trackbar(int, void *)
{
    cv::Mat imagem_filtrada;
    yL = (double)yL_slider / 10.0;
    yH = (double)yH_slider / 10.0;
    c = (double)c_slider;
    D0 = (double)D0_slider;
    if (D0 < 1.0)
    {
        D0 = 1.0;
    }
    imagem_filtrada = filtro_homomorfico();
    cv::imshow("Imagem Filtrada", imagem_filtrada);
    normalize(imagem_filtrada, imagem_filtrada, 0, 255, cv::NORM_MINMAX); 
    cv::imwrite("imagem_filtrada.png", imagem_filtrada);
}

int main(int, char **)
{
    cv::Mat image = cv::imread("foto.jpg", cv::IMREAD_GRAYSCALE);
    cv::namedWindow("Imagem Filtrada", 1);
    if (!image.data)
        return -1;

    // Mostra a imagem original
    cv::imshow("Imagem Original", image);
    cv::imwrite("imagem_original.png", image);

    // identifica os tamanhos otimos para
    // calculo do FFT
    dft_M = cv::getOptimalDFTSize(image.rows);
    dft_N = cv::getOptimalDFTSize(image.cols);

    // realiza o padding da imagem
    cv::copyMakeBorder(image, padded, 0, dft_M - image.rows, 0,
                       dft_N - image.cols, cv::BORDER_CONSTANT,
                       cv::Scalar::all(0));

    char TrackbarName[50];

    std::sprintf(TrackbarName, "yL x %d", yL_slider_max);
    cv::createTrackbar(TrackbarName, "Imagem Filtrada", &yL_slider, yL_slider_max, on_trackbar);

    std::sprintf(TrackbarName, "yH x %d", yH_slider_max);
    cv::createTrackbar(TrackbarName, "Imagem Filtrada", &yH_slider, yH_slider_max, on_trackbar);

    std::sprintf(TrackbarName, "c x %d", c_slider_max);
    cv::createTrackbar(TrackbarName, "Imagem Filtrada", &c_slider, c_slider_max, on_trackbar);

    std::sprintf(TrackbarName, "D0 x %d", D0_max);
    cv::createTrackbar(TrackbarName, "Imagem Filtrada", &D0_slider, D0_max, on_trackbar);

    on_trackbar(0, 0);
    cv::waitKey();
    return 0;
}