/*****************************************************************************************\
 * Készítette: Barkó Balázs                                                               * 
 * Dátum: 2020.06.09.                                                                     *
 *                                                                                        *
 * Megjegyzés: Azt a feltételt, hogy "nem használhat a szöveges állomány sorainak         *
 *     számától függ? méret? változót" nem tartja be, hiszen használok ArrayEnumerator-t. *
 *                                                                                        *
\*****************************************************************************************/

#include <string>
#include <iostream>
#include <sstream>
#include "../library/summation.hpp"
#include "../library/stringstreamenumerator.hpp"
#include "../library/seqinfileenumerator.hpp"
#include "../library/arrayenumerator.hpp"
#include "../library/maxsearch.hpp"

class Catch {
public:
    std::string type;
    int weight;

    friend std::istream& operator>>(std::istream& is, Catch& obj)
    {
        is >> obj.type >> obj.weight;
        return is;
    }

};

class LineWeightSum : public Summation <Catch, int> {
protected:
    int   func(const Catch& e) const override             { return e.weight; }
    int   neutral() const override                        { return 0; }
    int   add(const int& a, const int& b) const override  { return a + b; }
    bool  cond(const Catch& e) const override             { return true; }
public:
    LineWeightSum() : Summation<Catch, int>::Summation() {}
};

class LineData {
public:
    std::string name;
    int sum = 0;

    friend std::istream& operator>>(std::istream& is, LineData& obj)
    {
        std::string dx, tmp;
		std::getline(is, dx);
        
		std::stringstream ss(dx);
        std::string competetionID;
        std::string name;
        ss >> name >> competetionID;
        
        StringStreamEnumerator<Catch> ssenum(ss);
        LineWeightSum sum;
        sum.addEnumerator(&ssenum);
        sum.run();

        obj.sum = sum.result();
        obj.name = name;

        
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const LineData& obj)
    {
        os << "[name=" << obj.name << ", sum=" << obj.sum << "]";
        return os;
    }

    friend bool operator<(const LineData& l, const LineData& r)
    {
        return l.sum < l.sum;
    }

    friend bool operator>(const LineData& l, const LineData& r)
    {
        return l.sum > l.sum;
    }
};

class LineToVector : public Summation <LineData, std::vector<LineData> > {
protected:
    LineData func(const LineData& e) const override { return e; }
    bool cond(const LineData& e) const override     { return true; }
public:
    LineToVector() : Summation <LineData, std::vector<LineData> >::Summation() {}
};


class Person {
public:
    std::string name;
    int sum = 0;

    friend std::ostream& operator<<(std::ostream& os, const Person& obj)
    {
        os << "[name=" << obj.name << ", sum=" << obj.sum << "]";
        return os;
    }
};

class SumData : public Summation <LineData, int > {
private:
    std::string _name;
protected:
    int   func(const LineData& e) const override         { return e.sum; }
    int   neutral() const override                       { return 0; }
    int   add(const int& a, const int& b) const override { return a + b; }
    bool  cond(const LineData& e) const override         { return e.name == _name; }
public:
    SumData(std::string name) : _name(name), Summation<LineData, int>::Summation() {}
};

class LineToPerson : public Summation <LineData, std::vector<Person> > {
private:
    std::vector<LineData> _arr;
protected:
    Person func(const LineData& e) const override {
        Person result;
        result.name = e.name;

        ArrayEnumerator<LineData> linearr(&_arr);
        SumData asd(e.name);
        asd.addEnumerator(&linearr);
        asd.run();
        result.sum = asd.result();

        return result;
    }
    bool cond(const LineData& e) const override { return true; }
public:
    LineToPerson(std::vector<LineData> arr) : _arr(arr), Summation <LineData, std::vector<Person> >::Summation() {}
};


class Mins : public MaxSearch <Person, int, Less <int>> {
protected:
    int   func(const Person& e) const override { return e.sum; }
    bool  cond(const Person& e) const override { return e.sum > 40; }
};

int main() {

    SeqInFileEnumerator<LineData> fileenum("input.txt");
    LineToVector ltv;
    ltv.addEnumerator(&fileenum);
    ltv.run();

    ArrayEnumerator<LineData> linearr(&ltv.result());
    LineToPerson ltp(ltv.result());
    ltp.addEnumerator(&linearr);
    ltp.run();

    ArrayEnumerator<Person> arr(&ltp.result());
    Mins a;
    a.addEnumerator(&arr);
    a.run();

    std::cout << a.optElem() << std::endl;

    return 0;
}