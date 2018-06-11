//Hubert Janicki, numer albumu: 281771
#ifndef DRAWING_H_INCLUDED
#define DRAWING_H_INCLUDED

#include <Graph.h>
#include <GUI.h>
#include "figures.h"

class myWindow;

using namespace Graph_lib;

void cb_close(Address addr1, Address pWnd);

struct myButton : Button
{
    Address spinner_addr;
    myButton(Point xy, int w, int h, const string& label, Callback cb, Address addr)
        : Button(xy,w,h,label,cb), spinner_addr(addr)
        {}

    void attach(Graph_lib::Window& wnd);
};

class Spinner
{
    myWindow* pWnd = nullptr;
    myButton btn_lh, btn_rh;
    Out_box infoBox;

    int minVal, maxVal, curVal;

    const static int step = 5;
    string boxCapacity = "";

    static void cb_lh(Address pBtn, Address pSpinner);
    static void cb_rh(Address pBtn, Address pSpinner);

    void draw_changed_figure();
    void buildBoxCapacity();
    void changeVal(int &val, int step);

public:
    Spinner(Point loc, int minimum, int maximum, string lhCaption, string rhCaption, int startVal)
        : btn_lh(loc, 20, 20, lhCaption, Spinner::cb_rh, this),
          btn_rh(Point(loc.x + 60, loc.y), 20, 20, rhCaption, Spinner::cb_lh, this),
          infoBox(Point(loc.x + 25, loc.y), 30, 20, ""),
          minVal(minimum), maxVal(maximum), curVal(startVal)
          {}

    void attachTo(myWindow& wnd);
    int getCurVal() { return curVal; }
};

class myWindow : public Graph_lib::Window
{
    Button btn_close, btn_Anim;

    Spinner scale, moveX, moveY;

    std::pair<FPoint, FPoint> winBox;
    string filePath;

    void switchBtnAnimLabel();

    static void cb_close(Address pWidget, Address pWnd);
    static void cb_timer(Address pWidget, Address pWnd);
    static void timer_callback(Address addr);

    void getShapesToDraw(int scale, int moveX, int moveY, float rotationAngle);
    void draw();

public:
    myWindow(Point loc, int w, int h, string title, std::pair<FPoint, FPoint> wBox, string fPath) :
            Graph_lib::Window(loc, w, h, title),
            btn_close(Point(x_max() - 80, y_max() - 20), 80, 20, "Close", myWindow::cb_close),
            btn_Anim(Point(x_max() - 80, y_max() - 50), 80, 20, "Start", myWindow::cb_timer),
            scale(Point(x_max() - 90,10), 50, 150, "\\/", "/\\", 100),
            moveX(Point(x_max() - 90, 40), -200, 200, "<", ">", 0),
            moveY(Point(x_max() - 90, 70), -150, 150, "\\/", "/\\", 0),
             winBox(wBox), filePath(fPath)
            {
                attach(btn_close);
                attach(btn_Anim);
                scale.attachTo(*this);
                moveX.attachTo(*this);
                moveY.attachTo(*this);
                vector<figure*> temp = getFiguresFromFile(filePath);
                for(auto fig : temp)
                    vecOfFigures.push_back(unique_ptr<figure>(fig));
            }

    float rotationAngle = 0.0f;
	bool animationRunning = false;
	void refreshMap();
    void getShapesToDraw();

	vector<unique_ptr<figure>> vecOfFigures;
	vector<unique_ptr<Graph_lib::Shape>> vecOfShapes;

};

#endif // DRAWING_H_INCLUDED
