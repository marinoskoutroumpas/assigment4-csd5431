#ifndef SIMPLE_WINDOW_H
#define SIMPLE_WINDOW_H

#include "Graph.h"

// Ορισμός της κλάσης Simple_window 
struct Simple_window : Graph_lib::Window {
    // Κατασκευαστής της κλάσης Simple_window
    Simple_window(Point xy, int w, int h, const char* title)
        : Window(xy, w, h, title),             // Κλήση του κατασκευαστή της  κλάσης Window
        button_pushed(false),                  // Αρχικοποίηση της μεταβλητής button_pushed σε false
        next_button(Point(x_max() - 70, 0), 70, 20, "Next", cb_next) {  // Δημιουργία κουμπιού "Next" και ενσομάτωση του στο παράθυρο
        attach(next_button);                   // Εφαρμογή του κουμπιού "Next" στο παράθυρο
    }

    // Μέθοδος που περιμένει μέχρι να πατηθεί το κουμπί "Next"
    void wait_for_button()
    {
        while (!button_pushed) Fl::wait();    // Επανάληψη μέχρι να πατηθεί το κουμπί "Next"
        button_pushed = false;                 // Επαναφορά της μεταβλητής button_pushed σε false
        Fl::redraw();                          // Αίτηση για ανανέωση του παραθύρου
    }

private:
    Graph_lib::Button next_button;             // Κουμπί "Next"
    bool button_pushed;                        // Μεταβλητή που δείχνει αν το κουμπί "Next" έχει πατηθεί ή όχι

    // Στατική μέθοδος για το callback του κουμπιού "Next"
    static void cb_next(Fl_Widget*, void* userdata) { static_cast<Simple_window*>(userdata)->next(); }
    
    // Μέθοδος που καλείται όταν πατηθεί το κουμπί "Next"
    void next() { button_pushed = true; }
};

#endif // SIMPLE_WINDOW_H
