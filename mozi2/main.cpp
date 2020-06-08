/************************************\
 *Készítette: Kocsis Máté	    *
 *Dátum: 2020.06.08.	    	    *
 *Feladat leírása feladat.txt-ben   *
\***********************************/

#include <iostream>
#include <sstream>
#include <vector>
#include "../library/summation.hpp"
#include "../library/stringstreamenumerator.hpp"
#include "../library/arrayenumerator.hpp"
#include "../library/seqinfileenumerator.hpp"

struct ticket {
	std::string Qid;
	char g;
	friend std::istream& operator>>(std::istream& is, ticket& obj) {
		is >> obj.Qid; is >> obj.g;
		return is;
	}
};


class getTicket :public Summation<ticket, std::vector<ticket> > {
protected:
	ticket func(const ticket& e) const override{ return e; }
	bool cond(const ticket& e) const override { return true; }
public:
	getTicket():Summation<ticket, std::vector<ticket> >::Summation() {}
};

class StudentT :public Summation < ticket, int> {
protected:
	int func(const ticket& e) const override { return 1; }
	int neutral() const override {return 0;}
	int add(const int& a, const int& b) const override {return a + b; }
	bool  cond(const ticket& e) const { return (e.g=='d'); }
public:
	StudentT():Summation< ticket, int>::Summation(){}

};
struct record {
	std::string mid;
	std::string rid;
	std::string time;
	std::vector<ticket> tickets;
	friend std::istream& operator>>(std::istream& is, record& obj) {
		std::string dx;
		std::getline(is, dx);
		std::stringstream ss(dx);
		ss >> obj.mid >> obj.rid >> obj.time;
		obj.tickets.clear();
		getTicket A;
		StringStreamEnumerator<ticket> B(ss);
		A.addEnumerator(&B);
		A.run();
		obj.tickets = A.result();
		return is;
	}
	bool korrekt() const {
		StudentT A;
		ArrayEnumerator<ticket> B(&tickets);
		A.addEnumerator(&B);
		A.run();
		return (A.result() == tickets.size() && A.result() < 3);
	}
};

class Result:public Summation< record, int > {
protected:
	int func(const record& e) const override { return 1; }
	int neutral() const override { return 0; }
	int add(const int& a, const int& b) const override { return a + b; }
	bool  cond(const record& e) const override { return e.korrekt(); }
public:
	Result():Summation<record,int>::Summation(){}
};

class Length :public Summation< record, int > {
protected:
	int func(const record& e) const override { return 1; }
	int neutral() const override { return 0; }
	int add(const int& a, const int& b) const override { return a + b; }
	bool  cond(const record& e) const override { return true; }
public:
	Length() :Summation<record, int>::Summation() {}
};

int main() {

	Result A;
	Length C;
	SeqInFileEnumerator<record> B("test.txt");
	A.addEnumerator(&B);
	C.addEnumerator(&B);
	A.run();
	C.run();
	std::cout << ((A.result() == C.result()) ? "Igaz" : "Hamis") << std::endl;;
	return 0;
}
