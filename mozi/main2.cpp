/************************************\
 *Készítette: Darók Kíra            *
 *Dátum: 2020.06.06.                *
 *Feladat leírása feladat.txt-ben   *
 *                                  *
\***********************************/

#include <iostream>
#include <sstream>
#include "../library/summation.hpp"
#include "../library/stringstreamenumerator.hpp"
#include "../library/linsearch.hpp"
#include "../library/arrayenumerator.hpp"
#include "../library/counting.hpp"
#include "../library/seqinfileenumerator.hpp"

struct Jegyek
{
    std::string szek;
    char tipus;

    friend std::istream& operator>>(std::istream& is, Jegyek& jj)
    {
        is >> jj.szek >> jj.tipus;
        return is;
    }
};

class SsToVector : public Summation<Jegyek, std::vector<Jegyek> >
{
    Jegyek func(const Jegyek& e) const override { return e; }
};

class HanyDiak : public Summation<Jegyek, int>
{
    int func(const Jegyek& e) const override { return 1; }
    int neutral() const override { return 0; }
    int add(const int& a, const int& b) const override { return a + b; }
    bool cond(const Jegyek& e) const override { return e.tipus == 'd'; }
};

struct Mozi
{
    std::string filmazon;
    std::string terem;
    std::string kezdes;
    std::vector<Jegyek> jegyek;
    //harmasert
    bool csakdiak() const {
        HanyDiak d;
        ArrayEnumerator<Jegyek> Q(&jegyek);
        d.addEnumerator(&Q);
        d.run();
        return (d.result() == jegyek.size() && d.result() > 3);
    }
   
    friend std::istream& operator>>(std::istream& is, Mozi& mm)
    {
        std::string line;
        std::getline(is, line, '\n');
        std::stringstream ss;
        ss << line;
        ss >> mm.filmazon >> mm.terem >> mm.kezdes;
        mm.jegyek.clear();  //!!!!
        StringStreamEnumerator<Jegyek> asd(ss);
        SsToVector stv;
        stv.addEnumerator(&asd);
        stv.run();
        mm.jegyek = stv.result();
        ss.clear();
        return is;
    }
};

class CsakDiak : public LinSearch<Mozi>
{
    bool cond(const Mozi& e) const override { return e.csakdiak(); }
};

int main()
{

    SeqInFileEnumerator<Mozi> Q("test.txt");  
    CsakDiak A;
    A.addEnumerator(&Q);                
    A.run();

    if (A.found()) {
        std::cout << "Volt" << std::endl;
    }
    else {
        std::cout << "Nem volt" << std::endl;
    }


    return 0;
}


