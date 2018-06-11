//Hubert Janicki, numer albumu: 281771
#include "drawing.h"
#include "matrix.h"
#include <iostream>

using namespace std;

int main()
{
//Matrix tests

    Matrix<int> matr_test;
    cout << Matrix<int>::scaleMx(2,3) << "\n";
    cout << Matrix<int>::translateMx(2,3) << "\n";
    cout << Matrix<double>::rotateMx(45) << "\n";

    Matrix<int> multiply_test;

    multiply_test = Matrix<int>::scaleMx(2,3) * Matrix<int>::translateMx(2,3);
    cout << multiply_test << "\n";

    FPoint multiplied = multiply_test.transform(FPoint(10,20));
    cout << multiplied << "\n";


//******************************************


    myWindow wnd(Point(400, 400), 600, 400, "Hubert Janicki (281771)", { {20.0f, 20.0f}, {500.0f, 380.0f} }, "house.txt");

    wnd.getShapesToDraw();

    // przygotowanie do wyswietlenia

    for(unsigned int i = 0; i < wnd.vecOfShapes.size(); i++)
            wnd.attach(*wnd.vecOfShapes[i]);


    Graph_lib::gui_main();
}
