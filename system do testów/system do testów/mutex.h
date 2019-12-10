#pragma once
#include <queue>
#include <memory>
#include "process.h"
class mutex
{
	bool LOCKED;
	int OWNER_ID;

	void wait(std::shared_ptr<PCB>process);
	void notify();
public:
	std::queue<std::shared_ptr<PCB>> WAITING_PCB;//kolejka ID procesow czekajacych na plik

	mutex();
	void lock(std::shared_ptr<PCB>process);//przyjmuje wskaznik na proces zamykajacy zamek
	void unlock();
	const int get_owner_id() const;
	const string get_owner_name() const;
};

//extern Planist planist;//potrzebny obiekt planisty

