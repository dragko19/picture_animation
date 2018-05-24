#include "drawing.h"
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
        int prev_val = val;
        val += step;
        buildBoxCapacity();
        draw_changed_figure(val - prev_val, 0);
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

void Spinner::draw_changed_figure(int dx, int dy)
{
    pWnd -> redraw();
}

void Spinner::attachTo(Graph_lib::Window& wnd)
{
    wnd.attach(btn_lh);
    wnd.attach(btn_rh);
    wnd.attach(infoBox);
    buildBoxCapacity();
    pWnd = &wnd;
}

