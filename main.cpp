#include <iostream>
#include "big_int.h"
#include "num_ring.h"

using namespace std;

int main (int argc, char** argv) {
    if (argc > 2) {
        cerr << "Usage: ./extnum [1 2 3]" << endl;
        exit(1);
    }
    string s;
    if (argc == 2) {
        s = argv[1];
    }
    if (argc == 1 || s == "1") {
        cout << "Enter two numbers n and m:" << endl;
        BigInt n, m;
        cin >> n >> m;
        cout << "n is " << n << endl;
        cout << "m is " << m << endl;
        cout << "n + m is " << n + m << endl;
        cout << "n - m is " << n - m << endl;
        cout << "n * m is " << n * m << endl;
        try {
            n / m;
            cout << "n / m is " << n / m << endl;
            cout << "n % m is " << n % m << endl;
        } catch (std::overflow_error) {
            cout << "m is 0, division is undefined" << endl;
        }
        
    }
    else if (s == "2") {
        cout << "Enter a base:" << endl;
        BigInt b;
        cin >> b;
        cout << "Enter two numbers n and m:" << endl;
        BigInt n, m;
        cin >> n >> m;
        try {
            NumRing na (b, n);
            NumRing ma (b, m);
            cout << "n mod b is " << na.getNum() << endl;
            cout << "m mod b is " << ma.getNum() << endl;
            cout << "n + m mod b is " << (na + ma).getNum() << endl;
            cout << "n - m mod b is " << (na - ma).getNum() << endl;
            cout << "n * m mod b is " << (na * ma).getNum() << endl;
            try {
                NumRing quo = na / ma;
                cout << "n / m mod b is " << quo.getNum() << endl;
                cout << "To verify: (n / m) * m mod b is " << (quo * ma).getNum();
                cout << ", which is equal to n" << endl;
            }
            catch (...) {
                cout << "gcd not 1" << endl;
            }
            cout << "n ^ m mod b is " << exp(na, m).getNum() << endl;
        }
        catch (std::overflow_error) {
            cout << "Base is 0" << endl;
        }
        
    }
}
