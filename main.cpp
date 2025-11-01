#include <iostream>
#include "include/Publisher.hpp"
#include "include/Observer.hpp"

using std::cout;
using std::endl;

// Concrete observers
class ObA : public Observer {
public:
    void Execute(const std::string& event_name) const override {
        cout << "ObA received event: " << event_name << endl;
    }
};

class ObB : public Observer {
public:
    void Execute(const std::string& event_name) const override {
        cout << "ObB received event: " << event_name << endl;
    }
};

int main() {
    Publisher pub;

    ObA a;
    ObB b;

    cout << "Subscribe ObA and ObB to 'start'" << endl;
    pub.Subscribe("start", &a);
    pub.Subscribe("start", &b);

    cout << "Notify 'start' (both should receive)" << endl;
    pub.Notify("start");

    cout << "Unsubscribe ObB from 'start'" << endl;
    pub.Unsubscribe("start", &b);

    cout << "Notify 'start' (only ObA should receive)" << endl;
    pub.Notify("start");

    cout << "Notify 'unknown' (no observers)" << endl;
    pub.Notify("unknown");

    return 0;
}