#pragma once

#include <array>
#include <vector>
#include <queue>
#include <map>

#include <iostream>
#include <memory>
#include <algorithm>

/*Struktura z informacją o tym, czy dana strona znajduje się w RAM i jaką ramkę zajmuje. Wykorzystywana do tablicy stronic w PCB.*/
class PageInfo {
public:
	int frame; //Numer ramki, którą zajmuje dana stronica
	bool bit;  //Wartość mówiąca, czy stronica znajduje się w RAM
	PageInfo();
	PageInfo(int frame, bool bit);
};

class Page {
public:
	std::array<char, 16> data;
	Page();
	Page(std::string s);
	void Print();
};

class Memory{
public:
	Memory();
	
	/*---------RAM---------*/

	//Zmienne
	std::array<char, 256> RAM = { ' ' };

	//Funkcje - nie jestem pewny, które się przydadzą

	/*Zapisuje stronę do ramki o podanym numerze (przy założeniu, że data to tablica wielkości 16)*/
	void write_to_ram(int nr, std::array<char, 16> data);

	/*Zapisuje liczbę we wskazane miejsce w RAMie.
	nr - numer komórki RAM
	data - zmienna do zapisania*/
	void insert_to_ram(int nr, int data);

	/*Pobranie danej ramki z RAMu*/
	std::array<char, 16> get_frame(int nr);

	/*Pobranie liczby o podanej ilości cyfr z RAMu.
	nr - numer komórki RAM
	size - ilość cyfr*/
	int get_data(int nr, int size);

	/*Wyświetla daną ramkę.*/
	void show_frame(int nr);

	/*Wyświetla cały RAM.*/
	void show_ram();

	/*---------Virtual---------*/

	std::map<int, std::vector<Page>> PageFile;

	// Ładowanie programu do pliku stronnicowania
	// Na razie do testowania std::string ale to zależy od syetmu plików
	void LoadProgram(std::string kod, int PID);

	/*Tworzy wskaźnik do tablicy stronic procesu znajdującej się w PCB.
	Używana przy tworzeniu nowego procesu.
	pid  - ID procesu*/
	void CreatePageTable(int PID);

	/*Wyświetla strony danego procesu.*/
	void ShowPages(int PID);
};