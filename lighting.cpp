#include <ege.h>
#include <cmath>
#include <vector>

using namespace ege;

struct Point{
    double x, y;
    Point(const double xv, const double yv) : x(xv), y(yv)
    {
    }
    const double distance(const Point &rhp) const{
        return std::sqrt((x - rhp.x) * (x - rhp.x) + (y - rhp.y) * (y - rhp.y));
    }
    Point operator+(const Point &rhp) const{
        return Point(x + rhp.x, y + rhp.y);
    }
    Point operator-(const Point &rhp) const{
        return Point(x - rhp.x, y - rhp.y);
    }
    Point operator/(const double rhf) const{
        return Point(x / rhf, y / rhf);
    }
    Point operator*(const double rhf) const{
        return Point(x * rhf, y * rhf);
    }
};

Point startPoint(5,5), endPoint(635, 475);
const double minDiff = 5;

void drawLighting(const Point &beg, const Point &end, const double diff)
{
    if(diff < minDiff){
        line(beg.x, beg.y, end.x, end.y);
        return;
    }
    auto mid = (beg + end) / 2;
    mid.x += (randomf() - 0.5) * diff;
    mid.y += (randomf() - 0.5) * diff;
    drawLighting(beg, mid, diff / 2);
    drawLighting(mid, end, diff / 2);
}

void mainloop()
{
    for(; is_run(); delay_fps(20)){
        //paint
        cleardevice();
        drawLighting(startPoint, endPoint, 100);
    }
}

int main()
{
    initgraph(640, 480, INIT_RENDERMANUAL | INIT_NOFORCEEXIT);
    randomize();
    mainloop();
    closegraph();
}