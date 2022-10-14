#ifndef _COMPITO1
#define _COMPITO1
#include<TVector2.h>
#include<iostream>
#include<cmath>

class Complex;
std::ostream& operator<<(std::ostream& stream,const Complex&);

class Complex:public TVector2{

public:
using TVector2::TVector2;

Complex(TVector2 C):TVector2(C){}


Complex operator+(Complex b){
    double x1=this->X();
    double y1=this->Y();
    double x2=b.X();
    double y2=b.Y();
    Complex a(x1+x2,y1+y2);
    return a;
}

Complex operator-(Complex b){
    double x1=this->X();
    double y1=this->Y();
    double x2=b.X();
    double y2=b.Y();
    Complex a(x1-x2,y1-y2);
    return a;
}

Complex Coniugate(){
    double x=this->X();
    double y=this->Y()*(-1);
    Complex a(x,y);
    return a;
}

Complex operator*(Complex b){
    double x1=this->X();
    double y1=this->Y();
    double x2=b.X();
    double y2=b.Y();
    Complex c(x1*x2-y1*y2,x1*y2+x2*y1);
    return c;
}


inline friend std::ostream& operator<<(std::ostream& stream,const Complex& C){
  if(C.Y()>=0){
  stream<<"("<<C.X()<<"+"<<C.Y()<<"i)";
  return stream;
  }
  else{
  stream<<"("<<C.X()<<C.Y()<<"i)";
  return stream;
  }
}


};

#endif