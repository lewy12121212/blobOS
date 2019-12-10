// system do testów.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <iostream>
#include "process.h"
#include "mutex.h"	
#include <string>

//przyklad - 3 procesy chca uzyskac dostep do zasobu czytaj pliku
int main()
{
	std::string nazwa1 = "proces1", nazwa2="proces2", nazwa3 = "proces3";
	int numer1 = 1, numer2 = 2, numer3 = 3;
	mutex zamek;
	PCB p1(nazwa1, numer1), p2(nazwa2, numer2), p3(nazwa3, numer3);
	std::shared_ptr<PCB>wsk1 = make_shared<PCB>(p1), wsk2 = make_shared<PCB>(p2), wsk3 = make_shared<PCB>(p3);
	zamek.lock(wsk1);												//owner queue: p1
	zamek.lock(wsk3);												//owner queue: p1, p3
	std::cout << "Sekcja krytyczna 1 (bez sprawdzania)\n";			//proba wykonania kodu bez sprawdzania zamka - wykona sie (pomimo że p1 jest wlascicielem, nie mozna pomijac sprawdzania)
	std::cout << "Sekcja krytyczna 3 (bez sprawdzenia)\n";			//proba wykonania kodu bez sprawdzania zamka - wykona sie (zawsze sprawdzamy zamek przed otwarciem pliku do zapisu(!!!)/odczytu, po to w ogóle jest)
	zamek.lock(wsk2);												//owner queue: p1 p3 p2
	if(zamek.WAITING_PCB.front().get()->name == wsk3.get()->name)	//sprawdzenie zamka za pomoca jego kolejki 
		std::cout << "Sekcja krytyczna 3 (ze sprawdzaniem)\n";		//nie ok - p3 nie jest wlascicielem zamka
	zamek.unlock();													//owner queue: p3 p2, zmiana wlasciciela - zamek NIE OTWIERA SIE w tym wywolaniu funkcji
	if (zamek.get_owner_id() == wsk2.get()->pid)					//ładne sprawdzenie zamka (PID)
		std::cout << "Sekcja krytyczna 2 (ze sprawdzaniem)\n";		//nie ok - p2 nie jest wlascicielem zamka
	if (zamek.get_owner_name() == wsk3.get()->name)					//bardzo ładne sprawdzenie zamka (nazwa)
		std::cout << "Sekcja krytyczna 3\n";						//ok - p3 jest wlascicielem zamka
	zamek.unlock();													//owner queue: p2
	if (zamek.get_owner_name() == wsk2.get()->name)					//bardzo ładne sprawdzenie zamka (nazwa)
		std::cout << "Sekcja krytyczna 2\n";						//ok - p2 jest wlascicielem zamka
	zamek.unlock();													//owner queue: nic - zamek jest otwierany
	std::cout << zamek.get_owner_name() << std::endl;				//zamek jest otwarty - nie ma wlasciciela
	return 0;
}
