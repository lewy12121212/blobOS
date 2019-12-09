#pragma once
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <memory>
using namespace std;

int pid_pcb = 0;
enum process_state{ ready, run, wait, terminated }; // terminated?

class PCB{

protected:
    string name;
    int pid;
    int ppid;    // pid rodzica 
    int time_run; //przydzielony kwant czasu do wykonania
    process_state state; // stan procesu
    array<int, 5> register_copy{0,0,0,0,0}; // kopia rejestru
    shared_ptr<PCB> parent_pcb = nullptr;

    PCB(){ // konstruktor INIT
        this->name = "init";
        this->pid = 1;
        this->ppid = 0;
        this->state = ready;
    }

    PCB(string &name, int &ppid){
        this->name = name;
        pid_pcb++;
        this->pid = pid_pcb;
        this->ppid = ppid;
        this->state = ready;
    }

    void copy_register(array<int, 5> &cpu_register); //kopiowanie zawartości rejestru do PCB
    void restore_register(array<int, 5> &cpu_register); // przywracanie zawartości rejestru
    void change_state(process_state &new_state); // zmiana stanu procesu
    void kill(); //zabicie procesu

};

//void create_process_size(string &name, int &size, int &ppid); 
void create_process_file(string &name, string &file_name, int &ppid); // zakładająć że nie mamy podfolderów i ścieżka będzię jedynie nazwą pliku
