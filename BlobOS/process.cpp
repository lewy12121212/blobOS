#include <string>
#include <vector>
#include <array>
#include <iostream>
#include "process.h"
using namespace std;


void PCB::copy_register(array<int, 5> &cpu_register){ //kopiowanie zawartości rejestru do PCB
        for(int i=0; i<5; i++){
            this->copy_register[i] = cpu_register[i];
        }
    }

void PCB::restore_register(array<int, 5> &cpu_register){ // przywracanie zawartości rejestru
        for(int i=0; i<5; i++){
            cpu_register[i] = this->copy_register[i];
        }
    }

void PCB::kill(){

    this->change_state(terminated);
}

void PCB::change_state(process_state &new_state){
    this->state = new_state;
}

void create_process_size(string &name, int &size, int &ppid){
    
}

void create_process_file(string &name, string &file_name){ // zakładająć że nie mamy podfolderów i ścieżka będzię jedynie nazwą pliku

}