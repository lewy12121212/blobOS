#pragma once

#include <vector>
#include <memory>
#include <iostream>

/*Struktura z informacją o tym, czy dana strona znajduje się w RAM i jaką ramkę zajmuje. Wykorzystywana do tablicy stronic w PCB.*/
struct PageInfo {
	int frame; //Numer ramki, którą zajmuje dana stronica
	bool bit;  //Wartość mówiąca, czy stronica znajduje się w RAM

	PageInfo();
	PageInfo(int frame, bool bit);
};

class Memory
{
	//Zmienne
public:
	char RAM[256] = {' '};

	//Metody użytkowe

	Memory();

	/*Zapisuje dane do RAMu*/
	void write_to_ram(int address, char *data);

	/*Zapisuje rozkaz do RAMu*/
	void write_instruction(std::shared_ptr<std::vector<PageInfo>> pt, char *data);

	/*Wyświetla daną ramkę.*/
	void show_frame(int nr);

	/*Tworzy wskaźnik do tablicy stronic procesu znajdującej się w PCB.
	Używana przy tworzeniu nowego procesu.
	size - potrzebna ilość bajtów
	pid  - ID procesu*/
	std::shared_ptr<std::vector<PageInfo>> create_page_table(int size, int pid);

	//Metody pracy krokowej i wyświetlania pamięci
	
	/*Wyświetla cały RAM.*/
	void show_ram();

	/*Wyświetla strony danego procesu.*/
	void show_pages(int pid);
};