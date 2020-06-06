/************************************\
 *Készítette: Kocsis Máté	    *
 *Dátum: 2020.06.06.	    	    *
 *Feladat leírása feladat.txt-ben   *
 *                                  *
 *Kommentelte: Barkó Balázs         *
\***********************************/

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "..\library\summation.hpp"
#include "..\library\stringstreamenumerator.hpp"
#include "..\library\arrayenumerator.hpp"
#include "..\library\seqinfileenumerator.hpp"

//Eltárolja, hogy kik vettek jegyett
struct ticket {
	std::string Qid;
	char g;

	//Beolvasáshoz szükséges felül definálni a >> operatort
	friend std::istream& operator>>(std::istream& is, ticket& obj) {
		is >>obj.Qid >>obj.g;
		return is;
	}
};

//Eltárol egy-egy sor adatát
struct record {
public:
	std::string mid;
	std::string rid;
	std::string time;
	std::vector<ticket> tickets;

	//Azt adja vissza, hogy a feladat leírása alapján helyese-e ez a "sor"
	//Minden jegyet diák vett meg és legalább 3000 értékben (egy diák jegy 800 --> ha azt akarjuk legyen legalább 3000 akkor legalább 4 diáknak kell venni (vagyis diákok száma > 3))
	bool korrekt() const {
		StudentCount S;
		ArrayEnumerator<ticket> Q(&tickets);
		S.addEnumerator(&Q);
		S.run();
		return (S.result() == tickets.size() && S.result() > 3);
	}

	//Beolvasáshoz szükséges felül definálni a >> operatort
	friend std::istream& operator>>(std::istream& is, record& obj) {
		std::string dx;
		std::getline(is, dx);
		std::stringstream ss(dx);
		ss >> obj.mid >> obj.rid >> obj.time;
		obj.tickets.clear();	//FONTOS!!! 
					//Szükséges clearelni a vectort, mert különben benne marad az előző "sor" adata

		StringStreamEnumerator<ticket> sstreamEnum(ss);
		TicketGenerator ticketgen;
		ticketgen.addEnumerator(&sstreamEnum);
		ticketgen.run();

		obj.tickets = ticketgen.result();

		return is;
	}
	
};

//Segédosztály, segítségével egy StringStreamEnumerator-ból megkaphatjuk vectorban ami a sstream-ben van
class TicketGenerator:public Summation<ticket,std::vector<ticket> >{
protected:
	ticket func(const ticket& e) const override{ return e; }
	bool cond(const ticket& e) const override{ return true; }
public:
	TicketGenerator() : Summation<ticket, std::vector<ticket> >::Summation() {}
};

//Segédosztály, segítségével megszámoljuk, hány darab diák vett jegyett
class StudentCount :public Summation< ticket, int > {
protected:
	int func(const ticket& e) const override{ return 1; }
	int neutral() const override{return 0;}
	int add(const int& a, const int& b) const override{ return a + b; }
	virtual bool  cond(const ticket& e) const override{ return e.g == 'd'; }

public:
	StudentCount() :Summation<ticket, int> ::Summation() {}
};

//Az osztály ami megoldja az egész feladatott.
//Megszámolja hány olyan sor volt ahol minden jegyet diákok vettek és legalább 3000 értékben
class Result : public Summation< record, int > {
protected:
	int func(const record& e) const override { return 1; }
	int neutral() const override {return 0;}
	int add(const int& a, const int& b) const override { return a + b; }
	bool  cond(const record& e) const override{ return e.korrekt(); }
public:
	Result() : Summation<record, int> ::Summation() {}

};

int main() {
	SeqInFileEnumerator<record> Q("test.txt");  //enumerator ami beolvas egy fájlt amiben record típusok vannak tárolva
	Result B; 				    //Az summiton ami megoldja a feladatott (lásd fentebb)
	B.addEnumerator(&Q); 			    //Ha leszármaztatunk egy osztályból (gtlibben) és nem enumarator (abból SOHA nem származtatunk le [nagy pain in the ass])
						    //akkor mindig hozzá kell adni egy enumerator-t amin végig menjen a summation/stb...
	B.run();				    //FUTATNI IS MINDIG KELL mert akkor fog megtörténi a "csoda"
	//miután futtatuk utána (általában) a resultban van a végeredmény

	//itt megszámolta, hogy hány darab olyan sor volt ahol minden jegyet diák vett és legalább 3000 értékben
	//ha nem nulla (vagyis volt olyan sor) akkor kiírja, hogy "Volt" egyébként pedig azt, hogy "Nem volt"
	if (B.result() != 0) {
		std::cout << "Volt" << std::endl;
	}
	else {
		std::cout << "Nem volt" << std::endl;
	}

	//Ha egysorba akarnátok ilyen "egyszerű" dolgot kiírni akkor így is lehet:
	//std::cout << ((B.result() != 0) ? "Volt" : "Nem volt") << std::endl;
	return 0;
}
