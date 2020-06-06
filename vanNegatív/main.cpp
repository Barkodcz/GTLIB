/************************************\
 *Készítette: Barkó Balázs	    *
 *Dátum: 2020.06.06.	    	    *
 *Feladat leírása feladat.txt-ben   *
 *                                  *
 *Kommentelte: Barkó Balázs         *
\***********************************/

#include "../library/seqinfileenumerator.hpp"
#include "../library/intervalenumerator.hpp"
#include "../library/summation.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <sstream> 


//Az osztály ami az enumeratorból (itt SeqInFileEnumerator) csinál egy vektort
class ReadSum : public Summation<int, std::vector<int> > {
public:
    //Ez a sor nélkül is működdik, de inkább legyen ott
    ReadSum() : Summation<int, std::vector<int>>::Summation() {}
protected:
    int func(const int& e) const override { return e; }
    bool cond(const int& e) const override { return true; }
};

//Az osztály ami a feladatott megoldja summation
//Megnézi, hogy az adott indexen lévő elem negatív-e
class NegativSum : public Summation<int>
{
public:
	NegativSum(std::vector<int> x) : _x(x), Summation<int>::Summation() { }
protected:
    int func(const int& e) const override { return 1;}
    int neutral() const override { return 0;}
    int add( const int& a, const int& b) const override { return a + b;}

    bool cond(const int& e) const override { return _x[e] < 0; }
private:
    std::vector<int> _x;
};
/*
  Azért volt itt szükség, hogy átadjuk a vektort is 
  mivel IntervalEnumerator használunk és az indexeket add vissza és így tudjuk megnézni, 
  hogy adott elem pozitív vagy negatív
*/


int main(int argc, char* arg[]) {
    //A bemeneti adatokat ellenőrzi és elmenti változóban
    if(argc != 4) { std::cout << "ERROR" << std::endl; return 0; }
        
    int a = atoi(arg[1]);
    int b = atoi(arg[2]);
    std::string inputf = arg[3];

    //Ami megcsinálja az input fájból a vektort
    SeqInFileEnumerator<int> seq(inputf);
    ReadSum read;
    read.addEnumerator(&seq);
    read.run();

    //Megadja, hogy volt-e negatív szám adott index között
    IntervalEnumerator inter(a, b);
    NegativSum sum(read.result());
    sum.addEnumerator(&inter);
    sum.run();

    std::cout << (sum.result() > 0 ? "" : "not ") << "exist negative number." << std::endl;


    return 0;
}
