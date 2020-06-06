/************************************\
 *Készítette: Barkó Balázs          *
 *Dátum: 2020.06.06.	    	    *
 *Feladat leírása feladat.txt-ben   *
 *                                  *
 *Kommentelte: Barkó Balázs         *
\***********************************/

#include "../library/summation.hpp"
#include "../library/seqinfileenumerator.hpp"

#include <string>
#include <iostream>



//Az osztály ami egy fájlba írja ki azokat az elemek amire igaz a cond függvény (páros számokat)
class Assortment : public Summation<int, std::ostream>
{
public:
	Assortment(std::ostream* f) : Summation<int, std::ostream>::Summation(f) { }
protected:
	std::string func(const int& e) const override {
		std::ostringstream os;
		os << e << " ";
		return os.str();
	}
	bool cond(const int& e) const override { return e % 2 == 0; }
};

int main(int argc, char* arg[])
{
	//paramétrek ellenőrzése és változóba való elmentése
	if (argc != 3) { std::cout << "Error" << std::endl; return 0; }

	std::string input = arg[1];
	std::string output = arg[2];

	std::ofstream out(output);

	//A feladatott megoldó osztály létrehozzása és futtatása
	Assortment sum(&out);
	SeqInFileEnumerator<int> in(input);
	sum.addEnumerator(&in);
	sum.run();

	return 0;
}
