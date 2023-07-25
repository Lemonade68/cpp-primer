// #include <form.h>
#include <iostream>

static const float PI = 3.14159f;

class Shape {
public:
    virtual const char *shape_name() = 0;
    virtual void resize_by_percentage(float pct) = 0;
    virtual ~Shape() { };
};

class Shape_2D : public Shape {
public:
    Shape_2D() = default;
    Shape_2D(float x, float y) : x_(x), y_(y) { }

    virtual float area() const = 0;             // 面积
    virtual float diameter() const = 0;         // 直径
    virtual float circumference() const = 0;    // 周长

    //虚析构函数可以不同名，因为析构函数的位置是固定的，且一个类只能有一个
    ~Shape_2D() override { }

private:
    float x_ = 0.f;
    float y_ = 0.f;
};

class Shape_3D : public Shape {
public:
    Shape_3D() = default;
    Shape_3D(float x, float y, float z) : x_(x), y_(y), z_(z) { }

    virtual float volume() const = 0;           // 体积

    ~Shape_3D() override { }

protected:
    float x_ = 0.f;
    float y_ = 0.f;
    float z_ = 0.f;
};

class Box : public Shape_3D {
public:
    Box() = default;
    explicit Box(float width) : half_len_x_(width * 0.5f),
        half_len_y_(width * 0.5f), half_len_z_(width * 0.5f) { }
    Box(float center_x, float center_y, float center_z,
            float len_x, float len_y, float len_z) :
            Shape_3D(center_x, center_y, center_z),
            half_len_x_(len_x * 0.5f), half_len_y_(len_y * 0.5f),
            half_len_z_(len_z * 0.5f) { }

    const char *shape_name() override { return "Box"; }
    void resize_by_percentage(float pct) override {
        half_len_x_ *= pct;
        half_len_y_ *= pct;
        half_len_z_ *= pct;
    }

    float volume() const override {
        return half_len_x_ * half_len_y_ * half_len_z_ * 8;
    }

    ~Box() override { }

private:
    float half_len_x_ = 0.5f;
    float half_len_y_ = 0.5f;
    float half_len_z_ = 0.5f;
};

class Circle : public Shape_2D {
public:
    Circle() = default;
    explicit Circle(float radius) : radius_(radius) { }
    Circle(float center_x, float center_y, float radius) :
        Shape_2D(center_x, center_y), radius_(radius) { }

    float area() const override {
        return PI * radius_ * radius_;
    }
    float diameter() const override {
        return 2 * radius_;
    }
    float circumference() const override {
        return 2 * PI * radius_;
    }

    const char *shape_name() override { return "Circle" ; }
    void resize_by_percentage(float pct) override { radius_ *= pct; }

    ~Circle() override { };

protected:
    float radius_ = 1.f;            // 半径
};

class Sphere : public Shape_3D {
public:
    Sphere() = default;
    explicit Sphere(float radius) : radius_(radius) { }
    Sphere(float center_x, float center_y, float center_z, float radius)
        : Shape_3D(center_x, center_y, center_z), radius_(radius) { }

    const char *shape_name() override { return "Sphere"; }
    void resize_by_percentage(float pct) override { radius_ *= pct; }

    float volume() const override {
        return 4 * PI * radius_ * radius_ * radius_ / 3;
    }

    ~Sphere() override { }

protected:
    float radius_ = 1.f;            // 球体半径
};

class Cone : public Shape_3D {  
public:
    Cone() = default;
    Cone(float radius, float height) : radius_(radius), height_(height) { }
    Cone(float center_x, float center_y, float center_z, float radius,
            float height) : Shape_3D(center_x, center_y, center_z),
            radius_(radius), height_(height) { }

    const char *shape_name() override { return "Cone"; }
    void resize_by_percentage(float pct) override {
        radius_ *= pct;
        height_ *= pct;
    }

    float volume() const override {
        return PI * radius_ * radius_ * height_ / 3;
    }

    ~Cone() override { }

protected:
    float radius_ = 1.f;            // 圆锥体底面圆半径
    float height_ = 1.f;            // 圆锥体高
};


int main() {
    Box boxObj(10);         // 正方体的边长为 10
    std::cout << boxObj.shape_name() << std::endl;
    std::cout << boxObj.volume() << std::endl;
    boxObj.resize_by_percentage(0.3);           // 边长改为 10 * 0.3
    std::cout << boxObj.volume() << std::endl;

    std::cout << std::endl;

    Circle cirObj(10);      // 圆的半径为 10
    std::cout << cirObj.shape_name() << std::endl;
    std::cout << cirObj.area() << std::endl;            // 面积
    std::cout << cirObj.diameter() << std::endl;        // 直径
    std::cout << cirObj.circumference() << std::endl;   // 周长
    cirObj.resize_by_percentage(0.3);           // 半径改为 10 * 0.3
    std::cout << cirObj.area() << std::endl;            // 面积
    std::cout << cirObj.diameter() << std::endl;        // 直径
    std::cout << cirObj.circumference() << std::endl;   // 周长

    return 0;
}