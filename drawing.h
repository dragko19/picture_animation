#ifndef DRAWING_H_INCLUDED
#define DRAWING_H_INCLUDED

#include <Graph.h>
#include <GUI.h>

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
    Graph_lib::Window* pWnd = nullptr;
    myButton btn_lh, btn_rh;
    Out_box infoBox;

    int minVal, maxVal, curVal;

    const static int step = 5;
    string boxCapacity = "";

    static void cb_lh(Address pBtn, Address pSpinner);
    static void cb_rh(Address pBtn, Address pSpinner);

    void changeVal(int &val, int step);

    void draw_changed_figure(int dx, int dy);

    void buildBoxCapacity();

public:
    Spinner(Point loc, int minimum, int maximum, string lhCaption, string rhCaption, int startVal)
        : btn_lh(loc, 20, 20, lhCaption, Spinner::cb_rh, this),
          btn_rh(Point(loc.x + 60, loc.y), 20, 20, rhCaption, Spinner::cb_lh, this),
          infoBox(Point(loc.x + 25, loc.y), 30, 20, ""),
          minVal(minimum), maxVal(maximum), curVal(startVal)
          {}

    void attachTo(Graph_lib::Window& wnd);
};

#endif // DRAWING_H_INCLUDED
