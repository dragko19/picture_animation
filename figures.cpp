//Hubert Janicki, nr albumu: 281771
#include "figures.h"
#include "matrix.h"

//class FPoint defeinitions

	FPoint::operator Graph_lib::Point() const
	{
        return Graph_lib::Point(int(this->x), int(this->y));
    }

    std::ostream& operator<<(std::ostream& os, const FPoint& p)
	{
		return os << '(' << p.x << ',' << p.y << ')';
	}

    std::istream& operator>>(std::istream& is, FPoint& p)
	{
		char l, m, r;
		is >> l >> p.x >> m >> p.y >> r;
		if (l != '(' || m != ',' || r != ')')
			throw std::runtime_error("Invalid point.");
		return is;
	}

	bool operator<(const FPoint& lhp, const FPoint& rhp)
	{
	    return (lhp.x < rhp.x) && (lhp.y < rhp.y);
	}

//class figure definitions

    std::ostream& operator<<(std::ostream& os, const figure& f)
    {
        os << f.get_id() << '(';
        for (int i = 0; i < (int)f.fdef.size(); ++i)
            os << f.fdef[i];
        return os << ')';
    }

    std::pair<FPoint,FPoint> figure::bbox() const
    {
        return min_max(fdef);
    }

//class Rect definitions

    Graph_lib::Shape* Rect::get_shape(const FPoint& scale /*= {1.0f,1.0f}*/, const FPoint& trans /*= {0.0f,0.0f}*/) const
    {
        std::vector<FPoint> fdef_new = coord_scaling(fdef, scale, trans);

        if (!(fdef_new[0] < fdef_new[1]))
        {
            FPoint temp = fdef_new[0];
            fdef_new[0] = fdef_new[1];
            fdef_new[1] = temp;
        }

        std::pair<FPoint,FPoint> res = min_max(fdef_new);

        return new Graph_lib::Rectangle(res.first, res.second);
    }

//class Circ definitions

    Graph_lib::Shape* Circ::get_shape(const FPoint& scale /*= {1.0f,1.0f}*/, const FPoint& trans /*= {0.0f,0.0f}*/) const
    {
        std::vector<FPoint> fdef_new = coord_scaling(fdef, scale, trans);

        return new Graph_lib::Circle(fdef_new[0],(int)radius(fdef_new[0],fdef_new[1]));
    }

    std::pair<FPoint,FPoint> Circ::bbox() const
    {
        FPoint pmin, pmax;
		double length = radius(fdef[0],fdef[1]);
		pmax.x = fdef[0].x + length;
		pmax.y = fdef[0].y + length;
		pmin.x = fdef[0].x - length;
		pmin.y = fdef[0].y - length;

		return std::make_pair(pmin,pmax);
    }

//class Line definitions

    Graph_lib::Shape* Line::get_shape(const FPoint& scale /*= {1.0f,1.0f}*/, const FPoint& trans /*= {0.0f,0.0f}*/) const
    {
        std::vector<FPoint> fdef_new = coord_scaling(fdef, scale, trans);

        Graph_lib::Shape* ply = new Graph_lib::Open_polyline;
        for(int i = 0; i < (int)fdef_new.size(); i++)
            static_cast<Graph_lib::Open_polyline*>(ply)->add(Graph_lib::Point((int)fdef_new[i].x,(int)fdef_new[i].y));

        return ply;
    }


//other functions definitions

    FPoint min(const FPoint& lf, const FPoint& rt)
    {
        return FPoint(std::min(lf.x, rt.x), std::min(lf.y, rt.y));
    }
    FPoint max(const FPoint& lf, const FPoint& rt)
    {
        return FPoint(std::max(lf.x, rt.x), std::max(lf.y, rt.y));
    }

    //getting figures id from stream
    std::string get_id(std::istream& is)
    {
        std::string retv;
        char ch;
        while (is >> ch) {
            if (isalpha(ch)) // litery dodajemy do identyfikatora
                retv += ch;
            else if (retv.empty()) // pusty identyfikator
                throw std::runtime_error("Indentifier not found.");
            else {
                is.unget(); // nie wiemy co, ale odk³adamy
                return retv;
            }
        }
        return retv;
    }
    //getting coordinates from stream
    vector<FPoint> get_points(std::istream& is)
    {
        char ch;
        is >> ch; // lista zawsze zaczyna siê od '('
        if (ch != '(')
            throw std::runtime_error("Invalid figure.");
        vector<FPoint> vp;
        do {
            FPoint p;
            is >> p; // mo¿e wygenerowaæ wyj¹tek
            vp.push_back(p); // jak nie, to mamy zdrowy punkt
            is >> ch; // sprawdzamy, co czai siê w strumieniu
            if (ch == '(')
            is.unget(); // odk³adamy '(' ¿eby punkt by³ ca³y
        } while (ch != ')');

        return vp;
    }

    //creating * of figures objects
    figure* get_figure(std::istream& is)
    {
        std::string id = get_id(is);
        if (id.length() == 0)
            return nullptr;

        std::vector<FPoint> pts = get_points(is);

        if (id == Rect::class_id())
            return new Rect(pts);
        if (id == Circ::class_id())
            return new Circ(pts);
        if (id == Line::class_id())
            return new Line(pts);

        throw std::runtime_error("Unknown figure id.");
    }

    std::ostream& operator<<(std::ostream& os, const std::pair<FPoint, FPoint>& p)
    {
        return os << '[' << p.first << p.second << ']';
    }

    double radius(FPoint P1, FPoint P2)
    {
        return sqrt(pow((P2.x - P1.x), 2) + pow((P2.y - P1.y), 2));
    }

    //calculating transformation for all figure's points to scale drawing picture
    std::pair<FPoint,FPoint> get_transformation(const std::pair<FPoint,FPoint>& obj_bbox, const std::pair<FPoint,FPoint>& disp_bbox)
    {

        FPoint scale = scale_calculating(obj_bbox, disp_bbox);

        FPoint obj_bbox_center, disp_bbox_center;

        obj_bbox_center = center_of_box(obj_bbox,scale);
        disp_bbox_center = center_of_box(disp_bbox,{1.0f, 1.0f});

        FPoint trans = shift(obj_bbox_center, disp_bbox_center);

        return std::make_pair(scale,trans);
    }

    //finding min and max point of whole picture for drawing
    std::pair<FPoint,FPoint> map_bbox(const std::vector<figure*>& figures)
    {
        std::vector<FPoint> points;
        for(int i = 0; i < (int)figures.size(); i++)
        {
            points.push_back((figures[i]->bbox()).first);
            points.push_back((figures[i]->bbox()).second);
        }

        return min_max(points);
    }

    // name of displayed window
    const std::string window_title()
    {
        return "Hubert Janicki, nr albumu: 281771";
    }

    //scaling figure's coordinates
    std::vector<FPoint> coord_scaling(const std::vector<FPoint>& coord, const FPoint& scale, const FPoint& trans)
    {
        std::vector<FPoint> coord_new(coord.size());
        for(int i = 0; i < (int)coord.size(); i++)
        {
            coord_new[i].x = coord[i].x * scale.x + trans.x;
            coord_new[i].y = coord[i].y * scale.y + trans.y;
        }

        return coord_new;
    }

    // finding min and max point of figure's coordinates
    std::pair<FPoint,FPoint> min_max(const std::vector<FPoint>& vec)
    {
        FPoint pmin = vec[0], pmax = vec[0];
        for (int i = 1; i < (int)vec.size(); ++i)
        {
            pmin = min(pmin, vec[i]);
            pmax = max(pmax, vec[i]);
        }
        return std::make_pair(pmin,pmax);
    }

    //calculating scale for figure drawing
    FPoint scale_calculating(const std::pair<FPoint,FPoint>& obj_bbox, const std::pair<FPoint,FPoint>& disp_bbox)
    {
        float disp_bbox_width = disp_bbox.second.x - disp_bbox.first.x;
        float disp_bbox_height = disp_bbox.second.y - disp_bbox.first.y;
        float obj_bbox_width = obj_bbox.second.x - obj_bbox.first.x;
        float obj_bbox_height = obj_bbox.second.y - obj_bbox.first.y;

        FPoint scale;
        scale.x = (disp_bbox_width/obj_bbox_width) < (disp_bbox_height/obj_bbox_height) ? (disp_bbox_width/obj_bbox_width)
                    : (disp_bbox_height/obj_bbox_height);
        scale.y = -scale.x;

        return scale;
    }

    //finding center point of box
    FPoint center_of_box(const std::pair<FPoint,FPoint>& box, const FPoint& scale)
    {
        FPoint obj_center;
        obj_center.x = (box.second.x*scale.x + box.first.x*scale.x)/2;
        obj_center.y = (box.second.y*scale.y + box.first.y*scale.y)/2;

        return obj_center;
    }

    //finding shift between two boxes
    FPoint shift(const FPoint& obj_bbox_center, const FPoint& disp_bbox_center)
    {
        FPoint trans;
        trans.x = disp_bbox_center.x - obj_bbox_center.x;
        trans.y = disp_bbox_center.y - obj_bbox_center.y;

        return trans;
    }

    vector<figure*> getFiguresFromFile(const string& filePath)
    {
        vector<figure*> figs;
        ifstream ifs(filePath);
        if (!ifs.good()) // dobrze sprawdziæ przed czytaniem
        {
            cout << "Cannot find input file.\n";
        }
        else
        {
            try
            {
                figure *fig;
                while ((fig = get_figure(ifs)) != nullptr)
                {
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
        }
        ifs.close();

        return figs;
    }

    vector<Graph_lib::Shape*> getShapesToDraw(vector<figure*> vF, std::pair<FPoint,FPoint> winBox)
    {
        vector<Graph_lib::Shape*> retVec;

        pair<FPoint, FPoint> picBox = map_bbox(vF);
        pair<FPoint, FPoint> trafo = get_transformation(picBox, winBox);

            for (auto pf : vF)
            {
                retVec.push_back((pf->get_shape(trafo.first, trafo.second)));
            }

        return retVec;
    }

    vector<Graph_lib::Shape*> getShapesToDraw(std::pair<FPoint,FPoint> winBox, vector<figure*> vF, int scale, int moveX, int moveY, float rotationAngle)
    {
        vector<Graph_lib::Shape*> retVec;


        float centerX = (float)((winBox.second.x - winBox.first.x)/ 2) + moveX;
        float centerY = (float)((winBox.second.y - winBox.first.y)/ 2) - moveY;

        Matrix<float> mx;
        mx *= Matrix<float>::translateMx(-centerX + moveX, -centerY + moveY);
        mx *= Matrix<float>::rotateMx(rotationAngle);
        mx *= Matrix<float>::translateMx(centerX, centerY);

        for(auto fig : vF)
        {
            vector<FPoint> transPoints = fig -> getFDef();
            for(int i = 0; i < (int)fig -> getFDef().size(); i++)
            {
                transPoints[i] = mx.transform(transPoints[i]);
                cout << mx.transform(transPoints[i]) << "\n";
            }
            fig -> setFDef(transPoints);
            transPoints.clear();
        }

        pair<FPoint, FPoint> picBox = map_bbox(vF);
        FPoint pic_center = center_of_box(picBox, {1.0f, -1.0f});
        FPoint win_center = center_of_box(winBox, {1.0f, 1.0f});

        FPoint trans = shift(pic_center, win_center);

        for (auto pf : vF)
        {
            retVec.push_back((pf->get_shape({1.0f, -1.0f}, trans)));
        }

        return retVec;

    }







