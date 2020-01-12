#pragma once

#include "process.h"
#include <list>
#include <memory>

using namespace std;

class Planist{
    private:
         
    //proces_state state;     //stan procesu
    //array<int, 5> register_copy{0, 0, 0, 0, 0};     // kopia rejestru
    void sort_list();
    //static bool compare_PCB(const shared_ptr<PCB>& first, const shared_ptr<PCB>& second);

    public:
    list<shared_ptr<PCB>> ReadyPCB;     // lista procesow gotowych do wykonania
	list<shared_ptr<PCB>> WaitPCB;		// lista procesow oczekujacych


    Planist() = default;
    ~Planist() = default;

    //BARTEK: ¿eby go dzia³a³o przy starcie systemu
    void init();

    void add_process(const shared_ptr<PCB>& process);
    void remove_process(const shared_ptr<PCB>& process);

    void check();
    void display_PCB_list();
};

extern Planist planist;

