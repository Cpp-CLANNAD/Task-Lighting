#include <ege.h>
#include <cmath>
#include <vector>
#include <fstream>

using namespace ege;

struct Point{
    double x, y;
    Point(const double xv = 0.0, const double yv = 0.0) : x(xv), y(yv)
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
    const bool operator==(const Point &rhp) const{
        return x == rhp.x && y == rhp.y;
    }
    const bool operator!=(const Point &rhp) const{
        return x != rhp.x || y != rhp.y;
    }
};

double minDiff;
double MaxTime, LightingTime;
bool DrawPoint, ShowStep;

void drawLighting(const Point &beg, const Point &end, const double diff, PIMAGE img)
{
    if(diff < minDiff){
        line(beg.x, beg.y, end.x, end.y, img);
        return;
    }
    auto mid = (beg + end) / 2;
    mid.x += (randomf() - 0.5) * diff;
    mid.y += (randomf() - 0.5) * diff;
    drawLighting(beg, mid, diff / 2, img);
    drawLighting(mid, end, diff / 2, img);
}

const Point findNearPoint(const Point &center, std::vector<Point> &pvec, std::vector<Point> &invalidvec)
{
    Point rlt = center;
    if (invalidvec.size() == pvec.size()) {
        return rlt;
    }
    for (auto &point : pvec){
        if (point != center){
            bool invalid = false;
            for (auto &tmp : invalidvec){
                if (tmp == point){
                    invalid = true;
                    break;
                }
            }
            if (!invalid){
                if (rlt != center){
                    if (center.distance(rlt) <= center.distance(point))
                        continue;
                }
                rlt = point;
            }
        }
    }
    return rlt;
}

void mainloop()
{
    std::vector<Point> pvec, paintedVec;
    PIMAGE img = newimage(640, 480);
    setbkcolor(getbkcolor(), img);
    setcolor(getcolor(), img);
    Point beg, end;
    int mx, my;
    mousepos(&mx, &my);
    pvec.push_back(Point(mx, my));
    bool isShow = false;
    color_t color = getcolor();
    double leftTime, lastTime, lightingTime = -1.0;
    for (; is_run(); delay_fps(60)){
        while (mousemsg()){
            if (isShow){
                while (mousemsg())
                    getmouse();
                break;
            }
            mouse_msg msg = getmouse();
            if (msg.is_right() && pvec.size() >= 2){
                isShow = true;
                lastTime = fclock();
                leftTime = MaxTime;
                beg = pvec[0];
                paintedVec.clear();
                end = findNearPoint(beg, pvec, paintedVec);
                paintedVec.push_back(beg);
                paintedVec.push_back(end);
            }
            if (msg.is_left()){
                Point tmp(msg.x, msg.y);
                bool hasPoint = false;
                for (auto i = 0u; i < pvec.size() - 1; ++i){
                    if (pvec[pvec.size() - 1] == pvec[i]){
                        hasPoint = true;
                        break;
                    }
                }
                if (!hasPoint)
                    pvec.push_back(tmp);
            }
            pvec[pvec.size() - 1].x = msg.x;
            pvec[pvec.size() - 1].y = msg.y;
        }
        if (kbhit()){
            int key = getch();
            if(key == 'r'){
                isShow = false;
                pvec.clear();
                pvec.push_back(Point());
            }
        }

        //paint
        cleardevice();
        if (!isShow || DrawPoint) {
            for (auto &point : pvec){
                putpixel(point.x, point.y, EGERGB(255, 255, 255));
            }
        }
        if (isShow){
            if (lightingTime < 0.0) {
                cleardevice(img);
                drawLighting(ShowStep ? end - (end - beg) * leftTime / MaxTime : beg, end, 100, img);
                lightingTime = LightingTime;
            }
            putimage_transparent(NULL, img, 0, 0, getbkcolor());
            lightingTime -= (fclock() - lastTime);
            leftTime -= (fclock() - lastTime);
            lastTime = fclock();
            if (leftTime<0.0){
                if (paintedVec.size() == pvec.size()){
                    isShow = false;
                    paintedVec.clear();
                    continue;
                }
                leftTime = MaxTime;
                beg = end;
                end = findNearPoint(beg, pvec, paintedVec);
                paintedVec.push_back(end);
            }
        }
    }
    delimage(img);
}

int main()
{
    initgraph(640, 480, INIT_RENDERMANUAL | INIT_NOFORCEEXIT);
    randomize();
    setbkcolor(EGERGB(0, 0, 0));
    setcolor(EGERGB(0, 0, 208));
    std::ifstream fin("config.ini", std::fstream::in);
    fin >> minDiff >> MaxTime >> LightingTime >> DrawPoint >> ShowStep;
    fin.close();
    mainloop();
    closegraph();
}
