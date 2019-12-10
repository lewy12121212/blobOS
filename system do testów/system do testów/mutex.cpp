#include "pch.h"
#include "mutex.h"

mutex::mutex()
{
	LOCKED = false;
	OWNER_ID = 0;
}

void mutex::lock(std::shared_ptr<PCB>process)
{
	//dodaje wykonywany proces do kolejki zamka
	WAITING_PCB.push(process);
	if (LOCKED) { std::cout << "Zamek jest zamkniety przez proces " << WAITING_PCB.front().get()->name << ", wywolanie planisty\n"; wait(process);  }
	else 
	{ 
		//ustawia ID procesu-wlasciciela
		OWNER_ID = process.get()->pid;
		//zamyka zamek
		LOCKED = true; 
		std::cout << "Zamknieto zamek\n";
	}
}

void mutex::unlock()
{
	if (LOCKED) notify();
}

void mutex::wait(std::shared_ptr<PCB>process)
{
	//zmienia stan procesu: running->waiting
	process_state state = process_state::wait;//ing
	process->change_state(state);
	//wywoluje planiste - przekazanie sterowania
	//planist.check();//sexyOS
}

void mutex::notify()
{
	//sprawdza stan kolejki i:
	if (!WAITING_PCB.empty()) 
	{
		//usuwa wykonany proces z kolejki zamka
		WAITING_PCB.pop();
		if (!WAITING_PCB.empty())
		{
			//zmienia stan procesu kolejnego w kolejce: waiting->ready
			process_state state = ready;
			WAITING_PCB.front().get()->change_state(state);
			//zmiana wlasciciela, zamek pozostaje zamkniety
			OWNER_ID = WAITING_PCB.front().get()->pid;
			std::cout << "Odblokowano zamek, przydzielono dostep kolejnemu procesowi w kolejce (" << WAITING_PCB.front().get()->name << ") i zmieniono jego stan na ready\n";
		}
		//albo otwiera zamek
		else { LOCKED = false; OWNER_ID = 0; std::cout << "Odblokowano zamek\n"; }
		//badz wolny pliku
	}
}

const int mutex::get_owner_id() const
{
	return OWNER_ID;
}

const string mutex::get_owner_name() const
{
	if (LOCKED)
		return WAITING_PCB.front().get()->name;
	else return "NO OWNER";
}
