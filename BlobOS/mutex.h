#pragma once
#include <deque>
#include <memory>
#include "process.h"
#include "procesor.h"
#include"Shell.h"
class mutex {
	//stan zamka
	bool LOCKED;
	//pid procesu w�a�ciciela
	int OWNER_ID;
	//kolejka ID proces�w oczekuj�cych na plik
	std::deque<std::shared_ptr<PCB>> WAITING_PCB;	

	void wait(std::shared_ptr<PCB>process);			//zmienia stan procesu i wywo�uje planist�
	void notify();									//przydziela dostep, zmienia stan procesu i wywoluje planiste lub zwalnia zasob
public:
	mutex();										//ustawia LOCKED i OWNER_ID na 0
	void lock(std::shared_ptr<PCB>process);			//przyjmuje wska�nik na proces zamykaj�cy zamek
	void unlock(std::shared_ptr<PCB>process);		//przyjmuje wska�nik na proces otwierajacy zamek (wlasciciela)
	bool lock_for_editor();							//lock() wy�acznie dla edytora, sprawdza zamek ale go nie zajmuje, unlock() niepotrzebne
	const int get_owner_id() const;					//PID w�a�ciciela
	std::deque<std::shared_ptr<PCB>> copy_queue();  //kopiuje kolejk� zamka
};

