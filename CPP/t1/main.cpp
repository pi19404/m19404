#include <iostream>
#include <typeinfo>
#include <stdlib.h>
using namespace std;
class F;
class X;
class X1;
void print2();
void print1(X x1);
class Y {
public:
  void print(X& x);
};

class X {
  private:
  int a;
  protected:
  int c;
  friend void Y::print(X& x);
  friend void print1(X x);
  public:
  friend class F;

  friend void print2()
  {
      cout << "defining a friend function in the class " <<endl;
  }

public:
  X() : a(1), c(2) { }
};

class X1 : public X
{

};


class H
{
public:
    void print(X& x) {
      cout << "In member function  " << __PRETTY_FUNCTION__ <<endl;
      cout << "a is " << x.a << endl;
      cout << "c is " << x.c << endl;
    }
};

class F
{
public:
    void print(X& x) {
      cout << "In member function  " << __PRETTY_FUNCTION__ <<endl;
      cout << "a is " << x.a << endl;
      cout << "c is " << x.c << endl;
    }

    void print(X1 & x1) {
      cout << "In member function  " << __PRETTY_FUNCTION__ <<endl;
      cout << "a is " << x1.a << endl;
      cout << "c is " << x1.c << endl;
    }
friend class H;
};


void print1(X x1)
{
 cout << "In member function  " << __PRETTY_FUNCTION__ <<endl;
  cout << "a is " << x1.a << endl;
  cout << "c is " << x1.c << endl;

}

void Y::print(X& x) {

  cout << "In member function  " << __PRETTY_FUNCTION__ <<endl;
  cout << "a is " << x.a << endl;
  cout << "c is " << x.c << endl;
}
class Z;
void print3(Z &z);
class Z
{
public:

    class Z1
    {
    private :
        int z1;
    public:
        Z1() : z1(4){}

        friend void print3(Z &z)
        {
            cout << "In member function  " << __PRETTY_FUNCTION__ <<endl;
            //cout << "a is " << z.z2 << endl;
            cout << "b is " << z.z11.z1 << endl;
        }
    };
Z1 z11;
private :
int z2;
public:
Z() : z2(3){}
};


class G : public F
{

};



int main() {
  Z zobj;
  X xobj;
  Y yobj;
  F fobj;
  yobj.print(xobj);
  print1(xobj);
  fobj.print(xobj);
  print2();
  print3(zobj);
  X1 x1obj;
  G gobj;
  //cerr << x1obj.a << endl;
  fobj.print(x1obj);
  gobj.print(xobj);
  H hobj;
  hobj.print(xobj); // this line will give compilation error
}

