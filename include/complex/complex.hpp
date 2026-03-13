#pragma once

#include <iostream>

template <typename T>
class Complex{

private:
  T real;
  T imag;

public: 
  Complex(T r, T i): real(r), imag(i) {}
  Complex operator + (const Complex &other) {
    return Complex(real + other.real, imag + other.imag);
  }

  void print() const{
    std::cout << "(" << real << " + " << imag << "i" << ")" << std::endl;
  }

  friend bool operator==(const Complex& a, const Complex& b) {
    return a.real == b.real && a.imag == b.imag;
  }

  // Optional: pretty print failures
  friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
    return os << "(" << c.real << "," << c.imag << ")";
  }

//   inline bool operator == (const Complex & other) {
//     return ((real == other.real) && (imag == other.imag));
//   }

};


// Non type argument
// one of the argument type is non type
// The compiler must know the value of the argument at compile time

// function

template <typename T, int max>
int arr_min(T arr[], int n) {
    int m = max;
    for(int i=0;i<n;i++){
        if(arr[i] < m)
            m = arr[i];
    }
    return m;
}

//Class Template Arguments Deduction (C++17 Onwards)

// Template meta programming 
template <int N>
struct Factorial{
    static const int val = N * Factorial<N-1>::val;
};

// Specialization for base class Fcatorial<0>
template <>
struct Factorial<0>{
    static const int val=1;
};