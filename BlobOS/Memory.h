#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <array>
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
	void print();
};

class Memory{
public:
	Memory();
	
	/*---------RAM---------*/

	//Zmienne
	char RAM[256] = {' '};

	/*Zapisuje dane do RAMu*/
	void write_to_ram(int address, char *data);

	/*Wyświetla daną ramkę.*/
	void show_frame(int nr);

	/*Wyświetla cały RAM.*/
	void show_ram();

	/*---------Virtual---------*/

	std::map<int, std::vector<Page>> PageFile;

	// Ładowanie programu do pliku stronnicowania
	// Na razie do testowania std::string ale to zależy od syetmu plików
	void load_program(std::string kod, int PID);

	/*Tworzy wskaźnik do tablicy stronic procesu znajdującej się w PCB.
	Używana przy tworzeniu nowego procesu.
	size - potrzebna ilość bajtów
	pid  - ID procesu*/
	void create_page_table(int PID);

	/*Wyświetla strony danego procesu.*/
	void show_pages(int PID);
};

extern Memory memory;