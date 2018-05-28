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
    pWnd -> rotationAngle = 0.40f;
    pWnd -> refreshMap();
    if (pWnd -> animationRunning)
    {
        Fl::repeat_timeout(0.05, timer_callback, pWnd);
    }
}
void myWindow::refreshMap()
{
    for (unsigned int i = 0; i < vecOfShapes.size(); i++)
    {
        this -> detach(*vecOfShapes[i]);
        delete vecOfShapes[i];
    }

    vecOfShapes = getShapesToDraw(winBox, vecOfFigures, scale.getCurVal(), moveX.getCurVal(), moveY.getCurVal(), rotationAngle);

    if (vecOfShapes.size() != 0)
    {
        for (unsigned int i = 0; i < vecOfShapes.size(); i++)
        {
            attach(*vecOfShapes[i]);
        }
    }

    redraw();
}

