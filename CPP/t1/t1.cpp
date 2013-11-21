#include <iostream>
using namespace std;
class X;
class F;
class Y;
void print1(X x1);

class X {
  private:
  int a;
  protected:
  int c;
  friend void Y::print(X& x);
  friend void print1(X x);
public:
  X() : a(1), c(2) { }
};

class Y {
public:
  void print(X& x);
};


class F
{
public:
	void print(X &x)
	{

	}

}


void print1(X x1)
{
  cout << "a is " << x1.a << endl;
  cout << "c is " << x1.c << endl;

}

void Y::print(X& x) {
  cout << "called by memeber function" << __FUNC__ << "of the class" << __CLASS__ << endl;
  cout << "a is " << x.a << endl;
  cout << "c is " << x.c << endl;
}

int main() {
  X xobj;
  Y yobj;
  yobj.print(xobj);
print1(xobj);
} 

