#pragma once
#include <queue>
#include <memory>
#include "process.h"
#include "procesor.h"
class mutex
{
	bool LOCKED;
	int OWNER_ID;
	std::queue<std::shared_ptr<PCB>> WAITING_PCB;//kolejka ID procesow czekajacych na plik

	void wait(std::shared_ptr<PCB>process);		//zmienia stan procesu i wywoluje planiste
	void notify();								//przydziela dostep, zmienia stan procesu i wywoluje planiste lub zwalnia zasob
public:
	mutex();									//ustawia LOCKED i OWNER_ID na 0
	void lock(std::shared_ptr<PCB>process);		//przyjmuje wskaznik na proces zamykajacy zamek
	void unlock(std::shared_ptr<PCB>process);	//przyjmuje wskaznik na proces otwierajacy zamek (wlasciciela)
	bool lock_for_editor();						//lock() wylacznie dla edytora, sprawdza zamek ale go nie zajmuje, unlock() niepotrzebne
};

