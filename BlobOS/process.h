#pragma once

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <memory>
#include "Memory.h"
//#include "procesor.h"

using namespace std;
static int pid_pcb = 1;

enum process_state
{
    ready,
    run,
    wait,
    terminated
}; // terminated?

class PCB
{

public:
    string name;
    int pid;
    int parent_pid;                             // pid rodzica
    int time_run;                               //przydzielony kwant czasu do wykonania
    process_state state;                        // stan procesu
    array<int, 5> register_copy{0, 0, 0, 0, 0}; // kopia rejestru
    shared_ptr<PCB> parent_pcb = nullptr;       // wskaźnik na rodzica
    vector<shared_ptr<PCB>> children_vector;    // wektor dzieci
    vector<PageInfo> page_table;                // coś tam co Bartek chce i mi truł że mo być - to jest. :D
    int executionTimeLeft = 0;                  // ilość cykli do końca 

    PCB()
    { // konstruktor INIT
        this->name = "init";
        this->pid = 0;							// poprawione na 0 :) 
        this->parent_pid = 0;
        this->state = ready;
    }

    PCB(string &name, int &parent_pid)
    {
        this->name = name;
        pid_pcb++;
        this->pid = pid_pcb;
        this->parent_pid = parent_pid;
        this->state = ready;
    }

    shared_ptr<PCB> get_kid_pid(int &find_pid);                             // przeszukiwanie dzieci po pid
	shared_ptr<PCB> get_kid_name(string &find_name);                             // przeszukiwanie dzieci po pid
    void copy_register(array<int, 5> &cpu_register);					//kopiowanie zawartości rejestru do PCB
    void restore_register(array<int, 5> &cpu_register);					// przywracanie zawartości rejestru
    void change_state(process_state &new_state);						// zmiana stanu procesu
	bool null_vector_child();											// sparwdza czy są dzieci 
    //void kill();														//zabicie procesu
	void kill_kid(int &kill_pid);										// usunięcie dziecka z vektora dzieci
	void show_vector_child();  // wypisanie potomków
};

//void create_process_size(string &name, int &size, int &parent_pid);


class ProcTree {

public:

    shared_ptr<PCB> init_proc;


    ProcTree(shared_ptr<PCB> init_proc){
        this->init_proc = init_proc;
    }

    void create_process_file(string &name, string &file_name, int parent_pid); // zakładająć że nie mamy podfolderów i ścieżka będzię jedynie nazwą pliku
    shared_ptr<PCB> find_pid(int &pid_proc); // przeszukiwanie drzewa po pid
	shared_ptr<PCB> find_name(string &name); // przeszukiwanie drzewa po name 

    void kill_pid(int &pid); // zabicie procesu po PID
    void kill_name(string &name); // zabicie procesu po nazwie

    void display_tree(); // wyświetlenie całego drzewa procesów

};