#ifndef GRAPH_H
#define GRAPH_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <FL/Fl_PNG_Image.H>

#include <vector>
#include <cmath>

namespace Graph_lib {

// Βασική κλάση για γραφικά στο παράθυρο
struct Shape {
    void draw() const;
    virtual void move(int dx, int dy);
    void set_color(Color col) { lcolor = col; }
    Color color() const { return lcolor; }
    void set_style(Line_style sty) { ls = sty; }
    Line_style style() const { return ls; }

    Shape() : lcolor(fl_color()), ls(0) {}
    Shape(std::initializer_list<Point> lst) : points{lst} {}
    ~Shape();

    Shape(const Shape&) = delete;
    Shape& operator=(const Shape&) = delete;

protected:
    std::vector<Point> points; // Οι συντεταγμένες των σημείων του σχήματος
    Color lcolor;              // Χρώμα γραμμής
    Line_style ls;             // Στυλ γραμμής
};

// Αναπαράσταση συνάρτησης
struct Function : Shape {
    Function(Fct f, double r1, double r2, Point xy, int count = 100, double xscale = 25, double yscale = 25);
};

//Αναπαράσταση γραμμής
struct Line : Shape {
    Line(Point p1, Point p2);
};

//Αναπαράσταση  ορθογωνίου
struct Rectangle : Shape {
    Rectangle(Point xy, int ww, int hh) : w{ ww }, h{ hh }
    {
        if (h <= 0 || w <= 0) error("Bad rectangle: non-positive side");
        add(xy);
    }
    Rectangle(Point x, Point y) : w{ y.x - x.x }, h{ y.y - x.y }
    {
        if (h <= 0 || w <= 0) error("Bad rectangle: first point is not top left");
        add(x);
    }
    void draw_lines() const;

    int height() const { return h; }
    int width() const { return w; }

private:
    int h;  // Ύψος του ορθογωνίου
    int w;  // Πλάτος του ορθογωνίου
};

//Αναπαράσταση  ανοικτής πολύγραμμης γραμμής
struct Open_polyline : Shape {
    using Shape::Shape;
    void add(Point p) { Shape::add(p); }
    void draw_lines() const;
};

// Αναπαράσταση  κλειστής πολύγραμμης γραμμής
struct Closed_polyline : Open_polyline {
    using Open_polyline::Open_polyline;
    void draw_lines() const;
};

// Αναπαράσταση πολύγωνου
struct Polygon : Closed_polyline {
    using Closed_polyline::Closed_polyline;
    void add(Point p);
    void draw_lines() const;
};

// Αναπαράσταση άξονα
struct Axis : Shape {
    enum Orientation { x, y, z };
    Axis(Orientation d, Point xy, int length, int nummber_of_notches = 0, const std::string& label = "");

    void draw_lines() const;
    void move(int dx, int dy);

    void set_color(Color c);

    Text label;
    Lines notches;
};

//Αναπαράσταση  κειμένου
struct Text : Shape {
    Text(Point x, const std::string& s) : lab{ s } { add(x); }

    void draw_lines() const;
    void set_label(const std::string& s) { lab = s; }
    std::string label() const { return lab; }

    Font font() const { return fnt; }
    void set_font(Font f) { fnt = f; }
    Font font() { return fnt; }

private:
    std::string lab;   // Κείμενο
    Font fnt{ fl_font() }; // Γραμματοσειρά
};

//Αναπαράσταση  κύκλου
struct Circle : Shape {
    Circle(Point p, int rr) : r{ rr }
    {
        add(Point{ p.x - r, p.y - r });
    }

    void draw_lines() const;

    Point center() const { return { point(0).x + r, point(0).y + r }; }
    Point center() { return { point(0).x + r, point(0).y + r }; }
    int radius() const { return r; }
    int radius() { return r; }
    void set_radius(int rr) { set_point(0, { center().x - rr, center().y - rr }); r = rr; }

private:
    int r; // Ακτίνα του κύκλου
};

// Αναπαράσταση έλλειψης
struct Ellipse : Shape {
    Ellipse(Point p, int ww, int hh) : w{ ww }, h{ hh }
    {
        add(Point{ p.x - ww, p.y - hh });
    }

    void draw_lines() const;

    Point center() const { return{ point(0).x + w, point(0).y + h }; }
    Point center() { return{ point(0).x + w, point(0).y + h }; }
    Point focus1() const;
    Point focus2() const;

    void set_major(int ww) { w = ww; }
    int major() const { return w; }
    int major() { return w; }

    void set_minor(int hh) { h = hh; }
    int minor() const { return h; }
    int minor() { return h; }

private:
    int w; // Μείον της έλλειψης
    int h; // Μείον της έλλειψης
};

//Αναπαράσταση  πολύγωνου με σημεία
struct Marked_polyline : Open_polyline {
    Marked_polyline(const std::string& m) : mark{ m } { }
    void draw_lines() const;

private:
    std::string mark; // Σημαία των σημείων
};

// Αναπαράσταση σημείων
struct Marks : Marked_polyline {
    Marks(const std::string& m) : Marked_polyline{ m } { }
};

// Αναπαράσταση  σημείου
struct Mark : Shape {
    Mark(Point xy, char c) : c{ c } { add(xy); }
    void draw_lines() const;

private:
    char c; // Σύμβολο σημείου
};

//Αναπαράσταση  εικόνας
struct Bad_image : Fl_Image {
    Bad_image(int h, int w) : Fl_Image(h, w, 0) {}
    void draw(int x, int y, int, int, int, int) {}
};

// Αναπαράσταση εικόνας
struct Suffix {
    enum Encoding { none, jpg, gif, bmp };
};

Suffix::Encoding get_encoding(const std::string& s);

// Αναπαράσταση  εικόνας
struct Image : Shape {
    Image(Point xy, std::string s, Suffix::Encoding e = Suffix::none);
    ~Image() { delete p; }
    void draw_lines() const;
    void set_mask(Point xy, int ww, int hh) { w = ww; h = hh; cx = xy.x; cy = xy.y; }
    void move(int dx, int dy) { Shape::move(dx, dy); p->draw(point(0).x, point(0).y); }

private:
    int w, h, cx, cy;
    Fl_Image* p;
    std::string file;
};

// Βοηθητικές συναρτήσεις για τον υπολογισμό του κέντρου σχημάτων
Point n(const Rectangle&);
Point center(const Rectangle&);
Point center(const Circle&);
Point center(const Ellipse&);
Point center(const Text&);

// Κλάση που παρέχει ένα παράθυρο με ένα κουμπί "Next"
struct Shape_window : Graph_lib::Window {
    Shape_window(Point xy, int w, int h, const std::string& title)
        : Window{ xy, w, h, title },
        next_button{ Point{ x_max() - 70, 0 }, 70, 20, "Next", cb_next },
        next_pushed{ false }
    {
        attach(next_button);
    }

    bool wait_for_button()
    {
        next_pushed = false;
        show();
        Fl::run();
        return next_pushed;
    }

    bool next_pushed;

private:
    Button next_button;

    static void cb_next(Address, Address addr) { reference_to<Shape_window>(addr).next(); }
    void next() { next_pushed = true; hide(); }
};

} // namespace Graph_lib

#endif // GRAPH_H
