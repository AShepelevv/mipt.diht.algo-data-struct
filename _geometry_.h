#ifndef geometry_h
#define geometry_h

#include <cmath>
#include <vector>
#include <typeinfo>
#include <string>
#include <iostream>
#include <limits>

const double PI = 4.0 * atan(1.0);

const double accuracy = 1.0e-7;

const int sign (double a) {
    if (a > accuracy) {
        return 1;
    } else if (a < -accuracy) {
        return -1;
    } else {
        return 0;
    }
}

template <typename T>
std::pair<T, T> swapElementsInPair (const std::pair<T, T>& p) {
    std::pair<T, T> ans = p;
    std::swap(ans.first, ans.second);
    return ans;
}

const double square (double a) {
    return a * a;
}

class Line;

struct Point {
    
    double x, y;
    
    Point (const double x0 = 0.0, const double y0 = 0.0) {
        x = x0;
        y = y0;
    }
    
    bool operator == (const Point& another) const {
        return std::abs(x - another.x) < accuracy && std::abs(y - another.y) < accuracy;
    }
    
    bool operator != (const Point& another) const {
        return !(*this == another);
    }
    
    const Point& operator += (const Point& another) {
        x += another.x;
        y += another.y;
        return *this;
    }
    
    const Point operator + (const Point& another) const {
        Point ans = *this;
        ans += another;
        return ans;
    }
    
    const Point operator - () const {
        return Point(-x, -y);
    }
    
    const Point& operator -= (const Point& another) {
        x -= another.x;
        y -= another.y;
        return *this;
    }
    
    const Point operator - (const Point& another) const {
        Point ans = *this;
        ans -= another;
        return ans;
    }
    
    //Скалярное произведение
    double operator * (const Point& another) const {
        return double(x * another.x + y * another.y);
    }
    
    const Point operator *= (double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    const Point operator * (double scalar) const {
        Point ans = *this;
        ans *= scalar;
        return ans;
    }
    
    //Псевдоскалярное произведение
    double operator % (const Point& another) const {
        return double(x * another.y - y * another.x);
    }
    
    double modulus () const {
        return sqrt(pow(x, 2) + pow(y, 2));
    }
    
    const double angle () const {
        if (sign(x) == 0 && sign(y) == 0) {
            return 0.0;
        }
        if (sign(x) == 0) {
            return sign(y) > 0 ? PI / 2.0 : -PI / 2.0;
        }
        double angle = atan(y / x);
        return sign(x) < 0 ? angle + PI : angle;
    }
    
    bool propertyToSegment (const Point& a, const Point& b) const;
    
    void rotatePoint (const Point& center, const double angle);
    
    void scalePoint (const Point& center,  const double coefficient);
    
    void reflex (const Line& axis);
};

double length (const Point& a, const Point& b) {
    return sqrt(square(a.x - b.x) + square(a.y - b.y));
}

class Line {
    
private:
    
    double a, b, c; //ax + by + c = 0
    
public:
    
    Line (const Point& p, const Point& q) {
        if (sign(p.x - q.x)) {
            a = (p.y - q.y) / (p.x - q.x);
            b = -1.0;
            c = p.y - p.x * a;
        } else {
            a = 1.0;
            b = 0.0;
            c = -p.x;
        }
    }
    
    Line (const double angleCoefficient = 0.0, const double shift = 0.0) {
        a = angleCoefficient;
        b = -1.0;
        c = shift;
    }
    
    Line (const Point& p, const double angleCoefficient) {
        a = angleCoefficient;
        b = -1.0;
        c = p.y - p.x * a;
    }
    
    bool operator == (const Line& another) {
        return std::abs(a - another.a) < accuracy && std::abs(b - another.b) < accuracy && std::abs(c - another.c) < accuracy;
    }
    
    bool operator != (const Line& another) {
        return !(*this == another);
    }
    
    const double getCofficientK () const {
        return a;
    }
    
    const double getCofficientB () const {
        return c;
    }
    
    const double getAngle () const {
        return !b ? PI / 2.0 : atan(a);
    }
    
    const Point getNormalVector () const {
        if (std::abs(b) < accuracy) {
            return Point(1.0, 0.0);
        } else if (std::abs(a) < accuracy) {
            return Point(0.0, 1.0);
        } else {
            return Point(-1.0, 1.0 / a);
        }
    }
    
    const double getCoefficientD () const {
        Point r = !sign(b) ? Point(-c / a, 1.0) : Point(0.0, -c / b);
        return r * getNormalVector();
    }
    
    const double lineFunction (const Point& point) const {
        return a * point.x + b * point.y + c;
    }
    
    const double getAbscissaOfCross (double y) const {
        return (-b * y - c) / a;
    }
};

bool Point::propertyToSegment (const Point& a, const Point& b) const {
    if (x < std::min(a.x, b.x) || x > std::max(a.x, b.x) || y < std::min(a.y, b.y) || y > std::max(a.y, b.y)) {
        return false;
    }
    Line l = Line (a, b);
    return std::abs(l.lineFunction(*this)) < accuracy;
}

void Point::rotatePoint (const Point& center, const double angle) {
    Point dif = *this - center;
    double rotateAngle = dif.angle() + angle / 180.0 * PI;
    *this = center + Point(dif.modulus() * cos(rotateAngle), dif.modulus() * sin(rotateAngle));
}

void Point::scalePoint (const Point& center,  const double coefficient) {
    *this = center + (*this - center) * coefficient;
}

void Point::reflex (const Line& axis) {
    *this += axis.getNormalVector() * (2 * (axis.getCoefficientD() - *this * axis.getNormalVector()) / pow(axis.getNormalVector().modulus(), 2));
}

template <typename T>
std::string gettype () {
    return typeid(T).name();
}

class Circle;
class Ellipse;

class Shape {
    
public:
    
    virtual ~Shape () {}
    
    virtual const double perimeter () const = 0;
    
    virtual const double area () const = 0;
    
    virtual bool isCongruentTo (const Shape& another) const = 0;
    
    virtual bool isSimilarTo (const Shape& another) const = 0;
    
    virtual bool containsPoint (const Point& point) const = 0;
    
    virtual bool operator == (const Shape& another) const = 0;
    
    virtual bool operator != (const Shape& another) const = 0;
    
    virtual void rotate (const Point& center, const double angle) = 0;
    
    virtual void scale (const Point& center, const double coefficient) = 0;
    
    virtual void reflex (const Point& center) = 0;
    
    virtual void reflex (const Line& axis) = 0;
};

class Ellipse : public Shape {
    
private:
    
    std::pair <Point, Point> focus;
    double summaryLength;
    
public:
    
    virtual ~Ellipse () {}

    Ellipse (const Point& a, const Point& b, const double length) {
        focus = std::make_pair(a, b);
        summaryLength = length;
    }
    
    virtual const std::pair <Point,Point> focuses() const {
        return focus;
    }
    
    virtual const double eccentricity() const {
        return focus.first == focus.second ? 0.0 : length(focus.first, focus.second) / summaryLength;
    }
    
    virtual const double getSemiMajorAxisLength () const {
        return !eccentricity() ? 0.5 * summaryLength : length(focus.first, focus.second) / (2.0 * eccentricity());
    }
    
    virtual const double getSemiMinorAxisLength () const {
        return !eccentricity() ? 0.5 * summaryLength : getSemiMajorAxisLength() * sqrt(1 - square(eccentricity()));
    }
    
    double summaryDistanseToFocuses () const {
        return summaryLength;
    }
    
    virtual const Point center() const {
        return  Point ((focus.first.x + focus.second.x) / 2.0, (focus.first.y + focus.second.y) / 2.0);
    }
    
    const std::pair<Line, Line> directrixes () const {
        Line majorAxis (focus.first, focus.second);
        double angle = atan(majorAxis.getCofficientK());
        double distance = getSemiMajorAxisLength() / eccentricity();
        Point firstPoint(center().x + cos(angle) * distance, center().y + sin(angle) * distance);
        Point secondPoint(center().x - cos(angle) * distance, center().y - sin(angle) * distance);
        Line firstDirectrixe(firstPoint, majorAxis.getAngle() + PI / 2.0);
        Line secondDirectrixe(secondPoint, majorAxis.getAngle() + PI / 2.0);
        return std::pair <Line, Line> (std::make_pair(firstDirectrixe, secondDirectrixe));
    }
    
    const double perimeter () const {
        double ans = 0;
        double e = eccentricity();
        double dx = 1.5e-7;
        for (double x = 0; x < PI / 2; x += dx) {
            ans += 0.5 * (sqrt(1 - square(e) * square(cos(x))) + sqrt(1 - square(e) * square(cos(x + dx)))) * dx;
        }
        ans *= 4 * getSemiMajorAxisLength();
        return ans;
    }
    
    virtual const double area () const {
        return PI * getSemiMinorAxisLength() * getSemiMajorAxisLength();
    }
    
    virtual bool isSimilarTo (const Shape& another) const {
        try {
            Ellipse anotherEllipse = dynamic_cast<const Ellipse&>(another);
            return !sign(eccentricity() - anotherEllipse.eccentricity());
        } catch (std::bad_cast){
            return false;
        }
    }
    
    virtual bool isCongruentTo (const Shape& another) const {
        if (!isSimilarTo(another)) {
            return false;
        }
        try {
            Ellipse anotherEllipse = dynamic_cast<const Ellipse&>(another);
            return (getSemiMajorAxisLength() - anotherEllipse.getSemiMajorAxisLength()) < accuracy;
        } catch (std::bad_cast) {
            return false;
        }
    }
    
    virtual bool operator == (const Shape& another) const {
        if (!isCongruentTo(another)) {
            return false;
        }
        Ellipse anotherEllipse = dynamic_cast<const Ellipse&>(another);
        
        return focuses() == anotherEllipse.focuses() || focuses() == swapElementsInPair<Point>(anotherEllipse.focuses());
    }
    
    virtual void rotate (const Point& center, const double angle) {
        focus.first.rotatePoint(center, angle);
        focus.second.rotatePoint(center, angle);
    }
    
    virtual void scale (const Point& center, const double coefficient) {
        focus.first.scalePoint(center, coefficient);
        focus.second.scalePoint(center, coefficient);
        summaryLength *= std::abs(coefficient);
    }
    
    virtual void reflex (const Point& center) {
        scale(center, -1);
    };
    
    virtual void reflex (const Line& axis) {
        focus.first.reflex(axis);
        focus.second.reflex(axis);
    }
    
    virtual bool containsPoint (const Point& point) const {
        return (length(focus.first, point) + length(focus.second, point)) < summaryLength;
    }
    
    virtual bool operator != (const Shape& another) const {
        return !(*this == another);
    }
};

class Circle : public virtual Ellipse {
    
public:
    
    ~Circle () {}
    
    Circle (const Point& center, const double r) : Ellipse (center, center, 2 * r) {}
    
    const Point center() const {
        return focuses().first;
    }

    const double radius () const {
        return 0.5 * summaryDistanseToFocuses();
    }

    const double area () const {
        return PI * square(radius());
    }
    
    const double perimeter () const {
        return 2.0 * PI * radius();
    }
};

template<class... Args>
void init(std::vector<Point>& v, const Point& newPoint, Args... args) {
    v.push_back(newPoint);
    init(v, args...);
}

template <class... Args>
void init(std::vector<Point>& v, const Point& newPoint) {
    v.push_back(newPoint);
}

class Triangle;

class Polygon : public Shape {
    
protected:
    
    std::vector <Point> vertices;
    
    int verticesNumber (int i) const {
        return (i + verticesCount()) % verticesCount();
    }
    
public:
    
    virtual ~Polygon () {}
    
    Polygon (const std::vector<Point>& verticesArr) {
        for (int i = 0; i < int(verticesArr.size()); ++i) {
            vertices.push_back(verticesArr[i]);
        }
    }
    
    template<class... Args>
    Polygon(Args... args) {
        vertices.clear();
        init(vertices, args...);
    }
    
    const Point& operator [] (int number) const {
        return vertices[number % verticesCount()];
    }
    
    const std::vector<Point> getVertices () const {
        return vertices;
    }
    
    const int verticesCount () const {
        return int(vertices.size());
    }
    
    void printVerticsCordinates () const {
        for (int i = 0; i < verticesCount(); ++i) {
            std::cout << vertices[i].x << " " << vertices[i].y << std::endl;
        }
    }
    
    const bool isConvex () const {
        for (int i = 0; i < verticesCount(); ++i) {
            if (sign(getAngleSin(i)) != sign(getAngleSin((i + 1) % verticesCount()))) {
                return false;
            }
        }
        return true;
    }
    
    const double getAngleSin (int i) const {
        return double((vertices[verticesNumber(i)] - vertices[verticesNumber(i - 1)]) % (vertices[verticesNumber(i + 1)] - vertices[verticesNumber(i)]) / (vertices[verticesNumber(i + 1)] - vertices[i]).modulus() / (vertices[verticesNumber(i)] - vertices[verticesNumber(i - 1)]).modulus());
    }
    
    const double getAngleCos (int i) const {
        return double(-(vertices[verticesNumber(i)] - vertices[verticesNumber(i - 1)]) * (vertices[verticesNumber(i + 1)] - vertices[verticesNumber(i)]) / (vertices[verticesNumber(i + 1)] - vertices[i]).modulus() / (vertices[verticesNumber(i)] - vertices[verticesNumber(i - 1)]).modulus());
    }
    
    virtual const double area () const {
        double area = 0.0;
        for (int i = 0; i < verticesCount(); ++i) {
            area += vertices[i] % vertices[(i + 1) % verticesCount()];
        }
        area /= 2.0;
        return std::abs(area);
    }
    
    virtual const double perimeter () const {
        double perimeter = 0;
        for (int i = 0; i < verticesCount(); ++i) {
            perimeter += length(vertices[i], vertices[(i + 1) % verticesCount()]);
        }
        return perimeter;
    }
    
    void rotate (const Point& center, const double angle) {
        for (int i = 0; i < verticesCount(); ++i) {
            vertices[i].rotatePoint(center, angle);
        }
    }
    
    void scale (const Point& center, const double coeffcient) {
        for (int i = 0; i < verticesCount(); ++i) {
            vertices[i].scalePoint(center, coeffcient);
        }
    }
    
    void reflex (const Point& center) {
        scale(center, -1);
    };
    
    void reflex (const Line& axis) {
        for (int i = 0; i < verticesCount(); ++i) {
            vertices[i].reflex(axis);
        }
    }
    
    virtual bool isSimilarTo (const Shape& another) const;
    
    bool operator == (const Shape& another) const {
        try {
            Polygon anotherPolygon = dynamic_cast<const Polygon&>(another);
            for (int sign = -1; sign <= 1; sign += 2) {
                for (int i = 0; i < verticesCount(); ++i) {
                    for (int j = 0; j < verticesCount(); ++j) {
                        if (vertices[(i + sign * j + verticesCount()) % verticesCount()] != anotherPolygon.vertices[j]) {
                            break;
                        }
                        if (j == verticesCount() - 1) {
                            return true;
                        }
                    }
                }
            }
            return false;
        } catch (std::bad_cast) {
            return false;
        }
    }
    
    bool operator != (const Shape& another) const {
        return !(*this == another);
    }
    
    bool isCongruentTo (const Shape& another) const {
        if (!isSimilarTo(another)) {
            return false;
        }
        try {
            Polygon anotherPolygon = dynamic_cast<const Polygon&>(another);
            for (int sign = -1; sign <= 1; sign += 2) {
                for (int i = 0; i < verticesCount(); ++i) {
                    for (int j = 0; j < verticesCount(); ++j) {
                        if (std::abs(length(vertices[verticesNumber(j)],
                                             vertices[verticesNumber(j + 1)]) -
                                      length(anotherPolygon.vertices[verticesNumber(i + sign * j)],
                                             anotherPolygon.vertices[verticesNumber(i + sign* (j + 1))])
                                     ) > accuracy) {
                            break;
                        }
                        if (j == verticesCount() - 1) {
                            return true;
                        }
                    }
                }
            }
            return false;
        } catch (std::bad_cast) {
            return false;
        }
    }
    
    bool containsPoint (const Point& point) const {
        double xMin = std::numeric_limits<double>::max();
        double yMin = std::numeric_limits<double>::max();
        double xMax = -std::numeric_limits<double>::max();
        double yMax = -std::numeric_limits<double>::max();
        for (int i = 0; i < verticesCount(); ++i) {
            if (vertices[i].x > xMax) {
                xMax = vertices[i].x;
            }
            if (vertices[i].y > yMax) {
                yMax = vertices[i].y;
            }
            if (vertices[i].x < xMin) {
                xMin = vertices[i].x;
            }
            if (vertices[i].y < yMin) {
                yMin = vertices[i].y;
            }
        }
        if (point.x > xMax || point.x < xMin || point.y > yMax || point.y < yMin) {
            return false;
        }
        for (int i = 0; i < verticesCount(); ++i) {
            if (point.propertyToSegment(vertices[i], vertices[(i + 1) % verticesCount()])) {
                return true;
            }
        }
        Line ray = Line(point, 0.01); // 0.01
        bool onLine = false;
        int signLastVertice = 0;
        std::vector<Point> crossPoints;
        int start = 0;
        for (; start < verticesCount(); ++start) {
            if (!sign(ray.lineFunction(vertices[start]))) {
                break;
            }
        }
        for (int i = start; i < verticesCount() + start; ++i) {
            Point current = vertices[i % verticesCount()];
            Point next = vertices[(i + 1) % verticesCount()];
            if (sign(ray.lineFunction(current)) != sign(ray.lineFunction(next)) && !onLine && sign(ray.lineFunction(next))) {
                crossPoints.push_back(Point(Line(current, next).getAbscissaOfCross(point.y), point.y));
            } else if (!sign(ray.lineFunction(next)) && !onLine) {
                onLine = true;
                crossPoints.push_back(next);
                signLastVertice = sign(ray.lineFunction(current));
            } else if (sign(ray.lineFunction(next)) && onLine) {
                onLine = false;
                if (sign(ray.lineFunction(next)) == signLastVertice) {
                    crossPoints.push_back(current);
                }
            }
        }
        int crossingNumber = 0;
        for (int i = 0; i < int(crossPoints.size()); ++i) {
            if (crossPoints[i].x > point.x) ++crossingNumber;
        }
        return bool(crossingNumber % 2);
    }
    
    
};

class Rectangle : public Polygon {
    
public:
    
    virtual ~Rectangle() {}
    
    Rectangle (const std::vector<Point>& verticesArr) : Polygon (verticesArr) {}
    
    Rectangle (const Point& a, const Point& b, const Point& c, const Point& d) : Polygon(a, b, c, d) {}
    
    Rectangle (const Point& a, const Point& c, double ratio) : Polygon(a, a, c, c) {
        vertices.clear();
        vertices.push_back(a);
        if (ratio > 1) {
            ratio = 1.0 / ratio;
        }
        double angle = atan(ratio);
        Point b = c;
        b.scalePoint(a, sin(angle));
        b.rotatePoint(a, 90 - angle * 180 / PI);
        vertices.push_back(b);
        vertices.push_back(c);
        Point d = a;
        d.scalePoint(c, sin(angle));
        d.rotatePoint(c, 90 - angle * 180 / PI);
        vertices.push_back(d);
    }
    
    const Point center () const {
        return Point((vertices[0].x + vertices[2].x) / 2.0, (vertices[0].y + vertices[2].y) / 2.0);
    }
    
    const std::pair <Line, Line> diagonals () const {
        Line firstDiagonal(vertices[0], vertices[2]);
        Line secondDiagonal(vertices[1], vertices[3]);
        std:: pair<Line, Line> ans = std::make_pair(firstDiagonal, secondDiagonal);
        return ans;
    }
    
    virtual const double area () const {
        return length(vertices[0], vertices[1]) * length(vertices[1], vertices[2]);
    }
    
    virtual const double perimeter () const {
        double perimeter = 0;
        for (int i = 0; i < verticesCount() / 2; ++i) {
            perimeter += length(vertices[i], vertices[(i + 1) % verticesCount()]);
        }
        return perimeter * 2.0;
    }
};

class Square : public Rectangle {
    
public:
    
    ~Square () {}
    
    
    Square (const std::vector<Point>& verticesArr) : Rectangle (verticesArr) {}
    
    Square (const Point& a, const Point& b, const Point& c, const Point& d) : Rectangle(a, b, c, d) {}
    
    Square (const Point& a, const Point& b) : Rectangle (a, b, 1.0) {}
    
    const double area () const {
        return square(length(vertices[0], vertices[1]));
    }
    
    const double perimeter () const {
        return 4.0 * length(vertices[0], vertices[1]);
    }
    
    const Line getBasicDiagonal () {
        return Line(vertices[0], vertices[2]);
    }
    
    const Line getSecondaryDiagonal () {
        return Line(vertices[1], vertices[3]);
    }
    
    const Circle circumscribedCircle() const {
        Point centerPoint = center();
        return Circle(centerPoint, 0.5 * length(vertices[0], vertices[2]));
    }
    
    const Circle inscribedCircle() const {
        Point centerPoint = center();
        return Circle(centerPoint, 0.5 * length(vertices[0], vertices[1]));
    }
};

class Triangle : public Polygon {
    
public:
    
    ~Triangle () {}
    
    Triangle (std::vector<Point> points) : Polygon (points) {}
    
    Triangle (const Point& a, const Point& b, const Point& c) : Polygon (a, b, c) {}
    
    const double lengthA () const {
        return length(vertices[0], vertices[1]);
    }
    
    const double lengthB () const {
        return length(vertices[1], vertices[2]);
    }
    
    const double lengthC () const {
        return length(vertices[2], vertices[0]);
    }
    
    const Point getPointA () const {
        return vertices[2];
    }
    
    const Point getPointB () const {
        return vertices[0];
    }
    
    const Point getPointC () const {
        return vertices[1];
    }
    
    const double area () const {
        double p = perimeter() / 2.0;
        return sqrt(p * (p - lengthA()) * (p - lengthB()) * (p - lengthC()));
    }
    
    bool isSimilarTo (const Shape& another) const {
        try {
            Polygon anotherPolygon = dynamic_cast<const Polygon&>(another);
            if (anotherPolygon.verticesCount() != verticesCount()) {
                return false;
            }
            for (int i = 0; i < verticesCount(); ++i) {
                for (int j = 1; j < verticesCount() - 1; ++i) {
                    if (std::abs(getAngleCos(j + i) - anotherPolygon.getAngleCos(j)) > accuracy) {
                        break;
                    }
                    if (j == verticesCount() - 2) {
                        return true;
                    }
                }
            }
            return false;
        } catch (std::bad_cast) {
            return false;
        }
    }
    
    const Circle inscribedCircle () const {
        Point center = (getPointC() * (getPointA() - getPointB()).modulus() +
                        getPointA() * (getPointB() - getPointC()).modulus() +
                        getPointB() * (getPointC() - getPointA()).modulus()) * (1.0 / ((getPointA() - getPointB()).modulus() +(getPointB() - getPointC()).modulus() + (getPointC() - getPointA()).modulus()));
        Line segmentC = Line (getPointA(), getPointB());
        double radius = std::abs((segmentC.getCoefficientD() - center * segmentC.getNormalVector()) / segmentC.getNormalVector().modulus());
        return Circle (center, radius);
    }
    
    
    
    const Circle circumscribedCircle () const {
        double d = 2 * (getPointA().x * (getPointB().y - getPointC().y) + getPointB().x * (getPointC().y - getPointA().y) + getPointC().x * (getPointA().y - getPointB().y));
        double bcy = square(getPointA().modulus()) * (getPointB().y - getPointC().y);
        double cay = square(getPointB().modulus()) * (getPointC().y - getPointA().y);
        double aby = square(getPointC().modulus()) * (getPointA().y - getPointB().y);
        double cbx = square(getPointA().modulus()) * (getPointC().x - getPointB().x);
        double acx = square(getPointB().modulus()) * (getPointA().x - getPointC().x);
        double bax = square(getPointC().modulus()) * (getPointB().x - getPointA().x);
        Point center = Point((bcy + cay + aby) / d, (cbx + acx + bax) / d);
        return Circle(center, length(center, getPointA()));
    }
    
    const Point centroid () const {
        return Point((getPointA().x + getPointB().x + getPointC().x) / 3.0, (getPointA().y + getPointB().y + getPointC().y) / 3.0);
    }
    
    const Point orthocenter () const {
        Point o = circumscribedCircle().center();
        return o + (getPointA() - o) + (getPointB() - o) + (getPointC() - o);
    }
    
    const Line EulerLine () const {
        return Line(centroid(), orthocenter());
    }
    
    const Circle ninePointsCircle () const {
        return Circle((orthocenter() + circumscribedCircle().center()) * 0.5, circumscribedCircle().radius());
    }
};

bool Polygon::isSimilarTo (const Shape& another) const {
    try {
        Polygon anotherPolygon = dynamic_cast<const Polygon&>(another);
        if (verticesCount() == 3) {
            return Triangle(vertices).isSimilarTo(Triangle(anotherPolygon.vertices));
        }
        int fix = 0;
        for (int sign = -1; sign <= 1; sign += 2) {
            for (int i = 0; i < verticesCount(); ++i) {
                for (int j = 1; j < verticesCount() - 1; ++j) {
                    if (!Triangle(vertices[fix], vertices[(fix + j) % verticesCount()],
                                  vertices[(fix + j + 1) % verticesCount()]).isSimilarTo(Triangle(anotherPolygon[i], anotherPolygon[verticesNumber(i + sign * j)], anotherPolygon[verticesNumber(i + sign * (j + 1))]))) {
                        break;
                    }
                    if (j == verticesCount() - 2) {
                        return true;
                    }
                }
            }
        }
        return false;
    } catch (std::bad_cast) {
        return false;
    }
}
#endif
