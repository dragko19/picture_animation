#include "drawing.h"
#include "figures.h"
#include "matrix.h"
#include <iostream>

using namespace std;

int main()
{
    /*Graph_lib::Window wnd(Point(600, 600), 600, 400, "Hubert Janicki (281771)");

    Button btn_close(Point(wnd.x_max() - 80,wnd.y_max() - 20), 80, 20, "Close", cb_close);
    wnd.attach(btn_close);
    Spinner spnScale(Point(wnd.x_max() - 90,10), 50, 150, "\\/", "/\\", 95);
    spnScale.attachTo(wnd);

    Spinner spnLR(Point(wnd.x_max() - 90, 40), -200, 200, "<", ">", 0);
    spnLR.attachTo(wnd);

    Spinner spnUD(Point(wnd.x_max() - 90, 70), -150, 150, "\\/", "/\\", 0);
    spnUD.attachTo(wnd);*/
//Matrix tests

    Matrix<int> matr_test;
    cout << Matrix<int>::scaleMx(2,3) << "\n";
    cout << Matrix<int>::translateMx(2,3) << "\n";
    cout << Matrix<double>::rotateMx(45) << "\n";


//******************************************

    /*ifstream ifs("house.txt");
    if (!ifs.good()) // dobrze sprawdziæ przed czytaniem
    {
        cout << "Cannot find input file.\n";
        return 1;
    }
    vector<figure*> figs;
    try
    {
        figure *fig;
        while ((fig = get_figure(ifs)) != nullptr)
        {
            cout << *fig << endl;
            figs.push_back(fig);
        }
    }
    catch (exception& ex)
    {
        cout << ex.what() << endl;
    }
    catch (...)
    {
        cout << "figure: unexpected exception in >>\n";
    }
    ifs.close();


    // przygotowanie do wyswietlenia
    pair<FPoint, FPoint> egg_box = map_bbox(figs);
    pair<FPoint, FPoint> win_box = { {20.0f, 20.0f}, {500.f, 380.f} };
    pair<FPoint, FPoint> trafo = get_transformation(egg_box, win_box);


    for (auto pf : figs)
        wnd.attach(*(pf->get_shape(trafo.first, trafo.second)));

    Graph_lib::gui_main();*/
}
