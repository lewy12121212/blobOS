#pragma once

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include "process.h"

using namespace std;

void PCB::copy_register(array<int, 5> &cpu_register) //kopiowanie zawartości rejestru do PCB
{ 
    for (int i = 0; i < 5; i++)
    {
        this->register_copy[i] = cpu_register[i];
    }
}

void PCB::restore_register(array<int, 5> &cpu_register) // przywracanie zawartości rejestru
{ 
    for (int i = 0; i < 5; i++)
    {
        cpu_register[i] = this->register_copy[i];
    }
}

void PCB::kill() // zmiana stanu procesu za zakończony
{
    process_state state = terminated;
    this->change_state(state);
}

void PCB::change_state(process_state &new_state) // zmiana stanu procesu (planista)
{
    this->state = new_state;
}

shared_ptr<PCB> PCB::get_kid(int &find_pid)
{
    for(int i=0; i<this->children_vector.size(); i++){ // przeszukujemy wektor dzieci w celu znalezienia szukanego PID
        if(this->children_vector[i]->pid == find_pid){
            return children_vector[i];  // zwracamy wskaźnik na PCB o znalezionym PID
        }
    }

    for(int i=0; i<this->children_vector.size(); i++){ // jeśli nie znajdziemy szukanego PID dla każdego z dzieci wywołujemy funkcję get_kid 
        children_vector[i]->get_kid(find_pid); // rekurencja
    }

    return nullptr; // jeśli nie znajdziemy PID , zwracamy nullptr
}

void ProcTree::create_process_file(string &name, string &file_name, int &parent_pid)  // zakładająć że nie mamy podfolderów i ścieżka będzię jedynie nazwą pliku
{
    if(parent_pid == this->init_proc->pid){
        shared_ptr<PCB> proc = make_shared<PCB>(name, parent_pid);  // utworzenie wskaźnika na proces - wywołanie konstruktora procesu
        this->init_proc->children_vector.push_back(proc); // dodanie procesu jako dziecka procesu INIT
    } else {
        shared_ptr<PCB> parent = this->init_proc->get_kid(parent_pid);

        if(parent == nullptr){
            cout<<"Brak takiego rodzica "<<endl; 
        } else {
            shared_ptr<PCB> proc = make_shared<PCB>(name, parent_pid);  // utworzenie wskaźnika na proces - wywołanie konstruktora procesu
            parent->children_vector.push_back(proc); // dodanie procesu jako dziecka odnalezionego procesu rodzica
        }
    }


}

void ProcTree::display_tree()
{
    // sposób wyświetlania drzewa do ugadania z Anią :) 
}
