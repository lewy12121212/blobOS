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
	if (LOCKED)
	{ 
		std::cout << "lock: " <<  process->name << ": Zamek jest zamkniety przez proces " << WAITING_PCB.front()->name << ", wywolanie planisty\n";
		wait(process);
	}
	else 
	{ 
		//ustawia ID procesu-wlasciciela
		OWNER_ID = process->pid;
		//zamyka zamek
		LOCKED = true; 
		std::cout << "lock: " << process->name << ": Zamknieto zamek\n";
	}
}

void mutex::unlock(std::shared_ptr<PCB>process)
{
	//jesli proces nie jest wlascicielem
	if (process->pid != OWNER_ID)
		std::cout << "unlock(): " << process->name << ": proces nie jest wlascicielem zamka, nie mozna otworzyc\n";
	//jesli proces jest wlascicielem
	if (LOCKED && process->pid == OWNER_ID)
	{
		std::cout << "unlock(): " << process->name << ": Wywolanie notify()\n";
		notify();
	}
	
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
			WAITING_PCB.front()->change_state(state);
			//zmiana wlasciciela, zamek pozostaje zamkniety
			OWNER_ID = WAITING_PCB.front()->pid;
			std::cout << "notify(): Odblokowano zamek, przydzielono dostep kolejnemu procesowi w kolejce zamka(" << WAITING_PCB.front()->name << ") i zmieniono jego stan na ready\n";
			//wywoluje planiste - przekazanie sterowania
			//planist.check();//sexyOS
		}
		//albo otwiera zamek
		else 
		{
			LOCKED = false;
			OWNER_ID = 0; 
			std::cout << "notify(): Odblokowano zamek\n";
		}
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
		return WAITING_PCB.front()->name;
	else return "NO OWNER";
}
