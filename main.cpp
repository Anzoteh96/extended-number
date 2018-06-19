#include <iostream>
#include "big_int.h"

using namespace std;

int main () {
    BigInt n, m;
    cin >> n >> m;
    cout << n << " " << m << endl;
    cout << n + m << endl;
    cout << n - m << endl;
    cout << n * m << endl;
    cout << n / m << endl;
    cout << n % m << endl;
    /*
    vector<unsigned int> a1 = (n + m).getVec();
    vector<unsigned int> a2 = (n - m).getVec();
    vector<unsigned int> a3 = (n * m).getVec();
    vector<unsigned int> a4 = (n / m).getVec();
    vector<unsigned int> a5 = (n % m).getVec();
    for (int i = 0; i < a1.size(); ++i) {
        cout << a1[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < a2.size(); ++i) {
        cout << a2[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < a3.size(); ++i) {
        cout << a3[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < a4.size(); ++i) {
        cout << a4[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < a5.size(); ++i) {
        cout << a5[i] << " ";
    }
    cout << endl;
     */
}
