#include "mutex.h"

mutex::mutex()
{
	LOCKED = false;
	OWNER_ID = 0;
}

void mutex::lock(std::shared_ptr<PCB>process)
{
	set_color(white);
	//dodaje wykonywany proces do kolejki zamka
	WAITING_PCB.push(process);
	if (LOCKED)
	{ 
		std::cout << "lock: " <<  process->name << ": File locked by: " << WAITING_PCB.front()->name << ", calling planist.\n";
		wait(process);
	}
	else 
	{ 
		//ustawia ID procesu-wlasciciela
		OWNER_ID = process->pid;
		//zamyka zamek
		LOCKED = true; 
		std::cout << "lock: " << process->name << ": File has been locked.\n";
	}
}

void mutex::unlock(std::shared_ptr<PCB>process)
{
	set_color(white);
	//jesli proces nie jest wlascicielem
	if (process->pid != OWNER_ID)
		std::cout << "unlock(): " << process->name << ": the process is not the lock's owner, cannot unlock.\n";
	//jesli proces jest wlascicielem
	if (LOCKED && process->pid == OWNER_ID)
	{
		//sprawdza stan kolejki i usuwa z niej wykonany proces 
		if (!WAITING_PCB.empty())
			WAITING_PCB.pop();
		notify();
	}
}

void mutex::wait(std::shared_ptr<PCB>process)
{
	//zmienia stan procesu: running->waiting
	process_state state = process_state::wait;//ing
	process->change_state(state);
	//wywoluje planiste - przekazanie sterowania
	planist.check();
}

void mutex::notify()

{		set_color(white);
		if (!WAITING_PCB.empty())
		{
			//zmienia stan procesu kolejnego w kolejce: waiting->ready
			process_state state = ready;
			WAITING_PCB.front()->change_state(state);
			//zmiana wlasciciela, zamek pozostaje zamkniety
			OWNER_ID = WAITING_PCB.front()->pid;
			std::cout << "notify(): The file's lock ownership moved to (" << WAITING_PCB.front()->name << "), the process is now ready\n";
			//wywoluje planiste - przekazanie sterowania
			planist.check();
		}
		//albo otwiera zamek
		else 
		{
			LOCKED = false;
			OWNER_ID = 0; 
			std::cout << "notify(): unlocked\n";
		}
		//badz wolny pliku		
}

bool mutex::lock_for_editor()
{
	if (LOCKED)
	{
		set_color(white);
		cout << "The file is being edited by another process, cannot save. Use '$ go' and try again.\n";
		return false;
	}
	else return true;
}