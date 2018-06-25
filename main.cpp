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
        cout << "n / m is " << n / m << endl;
        cout << "n % m is " << n % m << endl;
    }
    else if (s == "2") {
        cout << "Enter a base:" << endl;
        BigInt b;
        cin >> b;
        cout << "Enter two numbers n and m:" << endl;
        BigInt n, m;
        cin >> n >> m;
        NumRing na (b, n);
        NumRing ma (b, m);
        cout << "n + m mod b is " << (na + ma).getNum() << endl;
        cout << "n - m mod b is " << (na - ma).getNum() << endl;
        cout << "n * m mod b is " << (na * ma).getNum() << endl;
    }
}
