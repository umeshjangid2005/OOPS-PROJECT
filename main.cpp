#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
using namespace std;

struct Customer {
    string name;
    string phone;
};

class Pizza {
protected:
    string name;
    float price;
    static int totalPizzas;
public:
    const string brand = "Umesh Pizza Point";

    Pizza(string n = "", float p = 0.0) : name(n), price(p) {
        totalPizzas++;
    }

    virtual void showPizza() {
        cout << name << " - Rs." << price << endl;
    }

    float getPrice() const { return price; }
    string getName() const { return name; }

    static int getTotalPizzas() { return totalPizzas; }

    virtual ~Pizza() {
        totalPizzas--;
    }
};

int Pizza::totalPizzas = 0;

class VegPizza : public Pizza {
public:
    VegPizza(string n, float p) : Pizza(n, p) {}
    void showPizza() override {
        cout << name << " (Veg) - Rs." << price << endl;
    }
};

class NonVegPizza : public Pizza {
public:
    NonVegPizza(string n, float p) : Pizza(n, p) {}
    void showPizza() override {
        cout << name << " (Non-Veg) - Rs." << price << endl;
    }
};

template <typename T>
T applyDiscount(T total, T percent) {
    return total - (total * percent / 100);
}

class Order; // forward declaration

class Bill {
public:
    friend void showBill(const Order& o);
};

class Order {
private:
    Customer c;
    Pizza* items[10];
    int count;
    float total;
public:
    Order(string cname = "Guest", string cphone = "0000000000") {
        c.name = cname;
        c.phone = cphone;
        count = 0;
        total = 0;
    }

    void addPizza(Pizza* p) {
        if (count < 10) {
            items[count++] = p;
            total += p->getPrice();
            cout << p->getName() << " added to order.\n";
        } else {
            cout << "Order limit reached!\n";
        }
    }

    void addPizza(string name, float price) {  // function overloading
        Pizza temp(name, price);
        total += price;
        cout << "Custom pizza '" << name << "' added for Rs." << price << endl;
    }

    friend void showBill(const Order& o); // friend function

    void showCustomer() {
        cout << "\nCustomer Name: " << this->c.name
             << "\nPhone: " << this->c.phone << endl;
    }

    void giveDiscount(float percent) {
        total = applyDiscount(total, percent);
        cout << "Discount applied! New total = Rs." << total << endl;
    }

    void makePayment(float amount) {
        try {
            if (amount < total)
                throw runtime_error("Insufficient amount!");
            cout << "Payment successful! Change: Rs." << amount - total << endl;
        } catch (exception& e) {
            cout << "Payment error: " << e.what() << endl;
        }
    }

    void saveToFile() {
        ofstream out("Bill.txt", ios::app);
        if (!out) {
            cout << "File error!" << endl;
            return;
        }
        out << "----- " << items[0]->brand << " -----\n";
        out << "Customer: " << c.name << "\nPhone: " << c.phone << endl;
        for (int i = 0; i < count; i++) {
            out << i + 1 << ". " << items[i]->getName()
                << " - Rs." << items[i]->getPrice() << endl;
        }
        out << "Total = Rs." << total << "\n------------------------\n\n";
        out.close();
        cout << "Bill saved successfully!\n";
    }

    Order operator+(const Order& other) {  // operator overloading
        Order combined("Combo Order", "0000000000");
        combined.total = this->total + other.total;
        cout << "\n[Two orders combined. Total = Rs." << combined.total << "]\n";
        return combined;
    }

    ~Order() {
        cout << "\nOrder for " << c.name << " completed.\n";
    }
};

void showBill(const Order& o) {
    cout << "\n----- BILL -----\n";
    for (int i = 0; i < o.count; i++) {
        cout << i + 1 << ". " << o.items[i]->getName()
             << " - Rs." << o.items[i]->getPrice() << endl;
    }
    cout << "Total = Rs." << o.total << endl;
    cout << "----------------\n";
}

int main() {
    cout << "*** Welcome to Umesh Pizza Point ***\n";

    VegPizza v1("Margherita", 199);
    VegPizza v2("Farmhouse", 249);
    NonVegPizza n1("Chicken Tikka", 299);
    NonVegPizza n2("Pepper Barbecue", 349);

    cout << "Total Pizzas available: " << Pizza::getTotalPizzas() << endl;

    string name, phone;
    cout << "\nEnter your name: ";
    getline(cin, name);
    cout << "Enter your phone: ";
    getline(cin, phone);

    Order order(name, phone);
    order.showCustomer();

    int ch;
    do {
        cout << "\n===== MENU =====\n";
        cout << "1. "; v1.showPizza();
        cout << "2. "; v2.showPizza();
        cout << "3. "; n1.showPizza();
        cout << "4. "; n2.showPizza();
        cout << "5. Add Custom Pizza\n";
        cout << "6. Show Bill\n";
        cout << "7. Apply Discount\n";
        cout << "8. Make Payment\n";
        cout << "9. Save Bill to File\n";
        cout << "10. Combine with Another Order\n";
        cout << "11. Exit\n";
        cout << "Enter choice: ";
        cin >> ch;

        switch (ch) {
            case 1: order.addPizza(&v1); break;
            case 2: order.addPizza(&v2); break;
            case 3: order.addPizza(&n1); break;
            case 4: order.addPizza(&n2); break;
            case 5: {
                string pname; float pprice;
                cout << "Enter pizza name: ";
                cin.ignore();
                getline(cin, pname);
                cout << "Enter price: Rs.";
                cin >> pprice;
                order.addPizza(pname, pprice);
                break;
            }
            case 6: showBill(order); break;
            case 7: {
                float disc;
                cout << "Enter discount (%): ";
                cin >> disc;
                order.giveDiscount(disc);
                break;
            }
            case 8: {
                float pay;
                cout << "Enter payment amount: ";
                cin >> pay;
                order.makePayment(pay);
                break;
            }
            case 9: order.saveToFile(); break;
            case 10: {
                Order other("Combo", "1111111111");
                other.addPizza(&v1);
                other.addPizza(&n2);
                Order combined = order + other;
                break;
            }
            case 11: cout << "Thank you for visiting!\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (ch != 11);

    return 0;
}
