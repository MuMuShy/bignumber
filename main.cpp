#include<iostream>
//include ... if needed to your test program

#include "bigNum.h" 
//Before your test this driver program, please replace it by your own file name.

//Example of Big Number Calculator:
//    https://www.calculator.net/big-number-calculator.html
//    https://defuse.ca/big-number-calculator.htm

using namespace std;

int main(int argc, char* argv[]) {
    BigNUM n1("C:\\A.txt"); // all bignum are Positive integer
    //BigNUM n1("A.txt", 5); //Bonus
    BigNUM n2; cin >> n2;
    BigNUM n3(n1 + n2);

    cout << "Hello, I'm a simple driver program: " << endl;

    //If n2 greater than n1, we swap them
    if (n2 > n1)
        BigNUM::Swap(n1, n2);

    //Now, n1 > n2
    cout << "n1 = " << n1 << endl;
    cout << "n2 = " << n2 << endl;
    cout << "n3 = " << n3 << endl;
    cout << "n1 - n2" << (n1 - n2) << endl;
    cout << "n1 * n2" << (n1 * n2) << endl;

    //Hw7------------------------------------
    cout << "n1 / n2" << (n1 / n2) << endl;    //the divide operation
    cout << "n1 mod n2" << (n1 % n2) << endl;    //the modulus operation
    cout << "GCD(n1,n2)" << BigNUM::GCD(n1, n2) << endl; //Greatest common divisor
    cout << "LCM(n1,n2)" << BigNUM::LCM(n1, n2) << endl;

    BigNUM n4, n5(n1);
    cout << "n4 = " << n4 << endl;
    cin >> n4; //format: 1 234 567 890
    cout << "n4 = " << n4 << endl;
    cout << "GCD(n5,n4)" << BigNUM::GCD(n5, n4) << endl; //Greatest common divisor
    cout << "LCM(n5,n4)" << BigNUM::LCM(n5, n4) << endl;

    return 0;
}
