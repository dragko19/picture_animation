//Hubert Janicki, numer albumu: 281771
#include "drawing.h"
#include "matrix.h"
#include <string>
//Callback for close button
void cb_close(Address addr1, Address pWnd)
{
    ((Graph_lib::Window*)pWnd) -> hide();
}

//myButton class functions implementation
void myButton::attach(Graph_lib::Window& wnd)
{
    Button::attach(wnd);
    pw->callback(reinterpret_cast<Fl_Callback*>(do_it), this);
    pw->user_data(spinner_addr); //reference to Spinner object
}

//Spinner class functions implementation
void Spinner::changeVal(int &val, int step)
{
    if (val + step == maxVal || val + step == minVal)
        cout << "You cannot change val\n";
    else
    {
        val += step;
        buildBoxCapacity();
        pWnd -> refreshMap();
    }
}

void Spinner::buildBoxCapacity()
{
    std::ostringstream os;
    os << curVal;
    boxCapacity = os.str();
    infoBox.put(boxCapacity);
}

void Spinner::cb_lh(Address pBtn, Address pSpinner)
{
    ((Spinner*)pSpinner) -> changeVal(((Spinner*)pSpinner) -> curVal,step);
}

void Spinner::cb_rh(Address pBtn, Address pSpinner)
{
    ((Spinner*)pSpinner) -> changeVal(((Spinner*)pSpinner) -> curVal,-step);
}

void Spinner::attachTo(myWindow& wnd)
{
    wnd.attach(btn_lh);
    wnd.attach(btn_rh);
    wnd.attach(infoBox);
    buildBoxCapacity();
    pWnd = &wnd;
}

void Spinner::draw_changed_figure()
{
    /*Matrix<double> matrix;

    matrix = Matrix<double>::scaleMx(curVal, curVal);
    matrix *= Matrix<double>::translateMx(curVal, 0);
    matrix *= Matrix<double>::translateMx(0, curVal);

    cout << matrix << "\n";

    for(unsigned int i = 0; i < pWnd -> vecOfFigures.size(); i++)
    {
        pWnd -> detach(reinterpret_cast<Graph_lib::Shape&>(pWnd -> vecOfFigures[i]));
        for(unsigned int k = 0; k < pWnd -> vecOfFigures[i] -> getFDef().size(); k++)
        {
            pWnd -> vecOfFigures[i] -> setFDef(k, matrix.transform(pWnd -> vecOfFigures[i] -> getFDef()[k]));
            //cout << matrix.transform(point) << "\n";
        }
        pWnd -> attach(reinterpret_cast<Graph_lib::Shape&>(pWnd -> vecOfFigures[i]));
    }

    pWnd -> redraw();*/
}

//myWindow class function implementation
void myWindow::switchBtnAnimLabel()
{
    if (animationRunning)
    {
        btn_Anim.label = "Start";
        animationRunning = false;
    }
    else
    {
        btn_Anim.label = "Stop";
        animationRunning = true;
    }
}

void myWindow::cb_close(Address pWidget, Address pWnd)
{
    reinterpret_cast<myWindow*>(pWnd) -> hide();
}
void myWindow::cb_timer(Address pWidget, Address pWnd)
{
    reinterpret_cast<myWindow*>(pWnd) -> switchBtnAnimLabel();
    reinterpret_cast<myWindow*>(pWnd) -> timer_callback(pWnd);
}
void myWindow::timer_callback(Address addr)
{
    myWindow *pWnd = static_cast<myWindow*>(addr);
    pWnd -> rotationAngle += 0.05f;
    pWnd -> refreshMap();
    if (pWnd -> animationRunning)
    {
        Fl::repeat_timeout(0.25, timer_callback, pWnd);
    }
}
void myWindow::refreshMap()
{
    for (unsigned int i = 0; i < vecOfShapes.size(); i++)
    {
        this -> detach(*vecOfShapes[i]);
    }

    getShapesToDraw(scale.getCurVal(), moveX.getCurVal(), moveY.getCurVal(), rotationAngle);

    if (vecOfShapes.size() != 0)
    {
        for (unsigned int i = 0; i < vecOfShapes.size(); i++)
        {
            attach(*vecOfShapes[i]);
        }
    }

    redraw();
}

void myWindow::draw()
{
    cout << "myWindow::draw()\n";
    Fl_Window::draw();
    fl_push_clip(10, 10, x_max()-100, y_max() - 20);
    for(size_t i = 0; i < vecOfShapes.size(); i++)
        vecOfShapes[i] -> draw();
    fl_pop_clip();
}

void myWindow::getShapesToDraw()
{
    vecOfShapes.clear();
    pair<FPoint, FPoint> picBox = map_bbox(vecOfFigures);
    pair<FPoint, FPoint> trafo = get_transformation(picBox, winBox);

        for (size_t i = 0; i < vecOfFigures.size(); ++i)
        {
            vecOfShapes.push_back((vecOfFigures[i]->get_shape(trafo.first, trafo.second)));
        }
}

void myWindow::getShapesToDraw(int scale, int moveX, int moveY, float rotationAngle)
{
    vecOfShapes.clear();
    vector<unique_ptr<figure>> new_figures;
    std::string id = "Line";

    float centerX = (float)((winBox.second.x + winBox.first.x)/ 2) + moveX;
    float centerY = (float)((winBox.second.y + winBox.first.y)/ 2) - moveY;


    Matrix<float> mx;
    mx *= Matrix<float>::translateMx(-centerX + moveX, -centerY - moveY);
    mx *= Matrix<float>::rotateMx(rotationAngle);
    float sc = static_cast<float>(scale)/100;
    mx *= Matrix<float>::scaleMx(sc, sc);
    mx *= Matrix<float>::translateMx(centerX, centerY);

    pair<FPoint, FPoint> picBox = map_bbox(vecOfFigures);
    pair<FPoint, FPoint> trafo = get_transformation(picBox, winBox);

    for(size_t i = 0; i < vecOfFigures.size(); ++i)
    {
        vector<FPoint> transPoints = vecOfFigures[i] -> getFDef(trafo.first, trafo.second);
        for(int i = 0; i < (int)transPoints.size(); i++)
        {
            transPoints[i] = mx.transform(transPoints[i]);
            cout << mx.transform(transPoints[i]) << "\n";
        }
        new_figures.push_back(get_figure(transPoints, id));
        transPoints.clear();
    }

    for (size_t i = 0; i < new_figures.size(); ++i)
    {
        vecOfShapes.push_back((new_figures[i]->get_shape()));
    }
}

