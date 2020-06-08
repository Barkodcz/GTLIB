/************************************\
 *Készítette: Kocsis Máté	    *
 *Dátum: 2020.06.06.	    	    *
 *Feladat leírása feladat.txt-ben   *
\***********************************/

#include <iostream>
#include <sstream>
#include <vector>
#include "../library/summation.hpp"
#include "../library/stringstreamenumerator.hpp"
#include "../library/arrayenumerator.hpp"
#include "../library/linsearch.hpp"
#include "../library/seqinfileenumerator.hpp"

struct dh {
	std::string time;
	int height;
	friend std::istream& operator>>(std::istream& is, dh& obj) {
		is >> obj.time >> obj.height;
		return is;
	}
};

class getdh:public Summation <dh, std::vector<dh> > {
protected:
	dh func(const dh& e) const override { return e; }
	bool cond(const dh& e) const override{ return true; }
public:
	getdh():Summation <dh, std::vector<dh> >::Summation(){}

};

class heightcondition:public Summation <dh, int > {
protected:
	int func(const dh& e) const override { return 1; }
	int neutral() const override { return 0; };
	int add(const int& a, const int& b) const override { return a + b; };
	bool cond(const dh& e) const override { return (e.height>=600); }
public:
	heightcondition() :Summation <dh, int>::Summation() {}
};

struct record {
	std::string place;
	std::string date;
	std::vector<dh> dhs;
	friend std::istream& operator>>(std::istream& is, record& obj) {
		std::string dx;
		std::getline(is, dx);
		std::stringstream ss(dx);
		ss>> obj.place >> obj.date;
		obj.dhs.clear();
		getdh A;
		StringStreamEnumerator<dh> sstreamEnum(ss);
		A.addEnumerator(&sstreamEnum);
		A.run();
		obj.dhs = A.result();
		return is;
	}
	
	bool korrekt() const{
		heightcondition A;
		ArrayEnumerator<dh> B(&dhs);
		A.addEnumerator(&B);
		A.run();
		return(A.result() != 0);
	}
};

class Result :public LinSearch<record, false> {
protected:
	bool cond(const record& e) const override { return e.korrekt() == false; }
public:
	Result() : LinSearch<record, false>() {};
};

int main() {
	Result A;
	SeqInFileEnumerator<record> Q("test.txt");
	A.addEnumerator(&Q);
	A.run();
	std::cout << ((!A.found()) ? "Minden nap volt legalabb egy 600 cm folotti meres" : "Nem volt minden nap 600 cm folotti meres") << std::endl;

	return 0;
}
