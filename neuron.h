#ifndef NUERON_H
#define NUERON_H
#include "activatefunc.h"
#include "matrix.h"
#include <fstream>
#include <vector>
using namespace std;

//для сравнения сетей с разной конфигурацией
struct data_Net
{
    int layers;
    int* size;
};



class Neuron
{
    int layers=0;             //кол-во слоев
    vector<int> size;              //кол-во нейронов на каждом слое
    ActivateFunc* actFunc;
    Matrix* weights;        //матрица весов
    double** bios;          //веса смещения
    double** neurons_val, ** neurons_err; //значения нейронов, ошибки нейронов
    double* neurons_bios_val, * neurons_bios_err;  //значение нейрона смещения

public:
    Neuron(int actFunc);
    Neuron(const Neuron &N);
    //~Neuron();
    void Init();
    void PrintConfig();
    void SetInputs(double* value);
    void WeightsUpdater(double lr);
    void SaveNet();
    void LoadNet();

    double ForwardFeed();
    void BackPropogation(double expect);
    int SearchMaxIndex(double* value);
    void PrintValues();
    double ErrorCounter();

    //void SetLayers(int layers); //установка кол-ва слоев
    void SetSizeLayer(int size); //установка кол-ва нейронов в слое
    int GetCoutLayer(); //получить колво слоев
    int GetCoutSizeLayer(int layer);//получить кол-во нейронов в слое
    //Matrix GetWeights(int i);

    const Neuron &operator=(const Neuron &N);
    const Neuron operator+(const Neuron &N) const;
    const Neuron operator/(const double &x) const;
    const Neuron operator*(const double &x) const;
    Matrix GetWeights(int i);
    void Qwerty();

private:
    void Multi(const Matrix& M, const double* M2, double* c);
    void Multi_T(const Matrix& M, const double* M2, double* c);
    void Summ(double* a, const double* b, int n);



};

#endif // NUERON_H

