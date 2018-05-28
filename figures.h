//Hubert Janicki, numer albumu: 281771
#ifndef FIGURE_H_INCLUDED
#define FIGURE_H_INCLUDED
#include <iostream>
#include <stdexcept>
#include <exception>
#include <vector>
#include <string>
#include <algorithm>
#include <graph.h>

class FPoint;
class figure;
class Rect;
class Circ;

std::ostream& operator<<(std::ostream& os, const std::pair<FPoint, FPoint>& p);
FPoint min(const FPoint& lf, const FPoint& rt);
FPoint max(const FPoint& lf, const FPoint& rt);

vector<FPoint> get_points(std::istream& is);
std::string get_id(std::istream& is);

std::string get_id(std::istream& is);
vector<FPoint> get_points(std::istream& is);

figure* get_figure(std::istream& is);

double radius(FPoint,FPoint);

std::pair<FPoint,FPoint> get_transformation(const std::pair<FPoint,FPoint>& obj_bbox, const std::pair<FPoint,FPoint>& disp_bbox);
std::pair<FPoint,FPoint> map_bbox(const std::vector<figure*>& figures);

const std::string window_title();

std::vector<FPoint> coord_scaling(const std::vector<FPoint>& coord, const FPoint& scale, const FPoint& trans);
std::pair<FPoint,FPoint> min_max(const std::vector<FPoint>& vec);
FPoint scale_calculating(const std::pair<FPoint,FPoint>& obj_bbox, const std::pair<FPoint,FPoint>& disp_bbox);
FPoint center_of_box(const std::pair<FPoint,FPoint>& box, const FPoint& scale);
FPoint shift(const FPoint& obj_bbox_center, const FPoint& disp_bbox_center);

vector<figure*> getFiguresFromFile(const string& filePath);
vector<Graph_lib::Shape*> getShapesToDraw(vector<figure*> vF, std::pair<FPoint,FPoint> winBox);
vector<Graph_lib::Shape*> getShapesToDraw(std::pair<FPoint,FPoint> winBox, vector<figure*> vF, int scale = 100, int moveX = 0, int moveY = 0, float rotationAngle = 0.0f);
FPoint setVecBboxFirstPartCoordSys(pair<FPoint, FPoint>& bbox);

class FPoint
{
	public:
	float x, y;
	FPoint(float cx = 0.0f, float cy = 0.0f) : x(cx), y(cy) {}

	friend std::ostream& operator<<(std::ostream& os, const FPoint& p);

	friend std::istream& operator>>(std::istream& is, FPoint& p);

	operator Graph_lib::Point() const;

	friend bool operator<(const FPoint& lhp, const FPoint& rhp);
};


class figure
{
    std::string id;

    protected:
    vector<FPoint> fdef;

    public:

    figure(const vector<FPoint>& fv) : fdef(fv) {}

    virtual ~figure() {}

    static std::string class_id()
    {
        return "Unknown";
    }

    vector<FPoint> getFDef() const { return fdef; }
    void setFDef(int index, const FPoint& val) { fdef[index] = val; }
    void setFDef(const vector<FPoint>& vec) { fdef = vec; }

    virtual std::string get_id() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const figure& f);

    virtual std::pair<FPoint, FPoint> bbox() const;

    virtual Graph_lib::Shape* get_shape(const FPoint& scale = {1.0f,1.0f}, const FPoint& trans = {0.0f,0.0f}) const = 0;

};

class Rect : public figure
{
    Graph_lib::Shape* get_shape(const FPoint& scale = {1.0f,1.0f}, const FPoint& trans = {0.0f,0.0f}) const;

public:

    Rect(const std::vector<FPoint>& fv) : figure(fv)
    {
        if (fdef.size() != 2)
        throw std::runtime_error("Rect: … points.");
    }

    static std::string class_id()
    {
        return "Rect";
    }

    virtual std::string get_id() const
    {
        return Rect::class_id();
    }


};

class Circ : public figure
{
    Graph_lib::Shape* get_shape(const FPoint& scale = {1.0f,1.0f}, const FPoint& trans = {0.0f,0.0f}) const;

public:
    Circ(const vector<FPoint>& fv) : figure(fv)
    {
        if (fdef.size() != 2)
            throw std::runtime_error("Circ: … points.");
    }

    static std::string class_id()
    {
        return "Circ";
    }

    virtual std::string get_id() const
    {
        return Circ::class_id();
    }

    virtual std::pair<FPoint,FPoint> bbox() const;
};

class Line: public figure
{
    Graph_lib::Shape* get_shape(const FPoint& scale = {1.0f,1.0f}, const FPoint& trans = {0.0f,0.0f}) const;

public:
    Line(const vector<FPoint>& fv) : figure(fv)
    {
        if (fdef.size() < 2)
            throw std::runtime_error("Line: … points.");
    }

    static std::string class_id()
    {
        return "Line";
    }

    virtual std::string get_id() const
    {
        return Line::class_id();
    }

};


#endif // FIGURE_H_INCLUDED
