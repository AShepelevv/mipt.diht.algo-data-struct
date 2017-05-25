#ifndef point_h
#define point_h

const double accuracy = 1.0e-8;

class Point {
private:
    
    double x, y, z;
    
public:
    
    ~Point () {}
    
    Point () {};
    
    Point (double a, double b, double c) {
        x = a;
        y = b;
        z = c;
    }
    
    bool operator < (const Point& point) const {
        return sqrt(x * x + y * y + z * z) - sqrt(point.x * point.x + point.y * point.y + point.z * point.z) < -accuracy ;
    }
    
    bool operator > (const Point& point) const {
        return sqrt(x * x + y * y + z * z) - sqrt(point.x * point.x + point.y * point.y + point.z * point.z) > accuracy;
    }
    
    bool operator == (const Point& point) const {
        return !(*this > point) && !(*this < point);
    }
    
    bool operator != (const Point& point) const {
        return !(*this == point);
    }
};

#endif /* point_h */
