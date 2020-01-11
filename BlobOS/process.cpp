#pragma once

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include "Memory.h"
#include "process.h"
//#include "procesor.h"

extern Memory memory;

ProcTree PTree(make_shared<PCB>());

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

/*
void PCB::kill() // zmiana stanu procesu za zakończony
{
    process_state state = terminated;
    this->change_state(state);
}*/

void PCB::change_state(process_state &new_state) // zmiana stanu procesu (planista)
{
    this->state = new_state;
	// planista - oczekiwanie
	// Planist::manage();
}

bool PCB::null_vector_child() { // zwraca 1 gdy pusty i rodzic może się zakończyć 

	if (this->children_vector.size() == 0) {
		return 1;
	}
	else {
		return 0;
	}
};

shared_ptr<PCB> PCB::get_kid_pid(int &find_pid)
{
    if(this->children_vector.size() == 0){
        return nullptr;
    }

    for(int i=0; i<this->children_vector.size(); i++){ // przeszukujemy wektor dzieci w celu znalezienia szukanego PID
        if(this->children_vector[i]->pid == find_pid){
            return children_vector[i];  // zwracamy wskaźnik na PCB o znalezionym PID
        }
    }

    for(int i=0; i<this->children_vector.size(); i++){ // jeśli nie znajdziemy szukanego PID dla każdego z dzieci wywołujemy funkcję get_kid 
        children_vector[i]->get_kid_pid(find_pid); // rekurencja
    }

    return nullptr; // jeśli nie znajdziemy PID , zwracamy nullptr
}

shared_ptr<PCB> PCB::get_kid_name(string &find_name)
{
	if (this->children_vector.size() == 0) {
		return nullptr;
	}

	for (int i = 0; i < this->children_vector.size(); i++) { // przeszukujemy wektor dzieci w celu znalezienia szukanego name
		if (this->children_vector[i]->name == find_name) {
			return children_vector[i];  // zwracamy wskaźnik na PCB o znalezionym name
		}
	}

	for (int i = 0; i < this->children_vector.size(); i++) { // jeśli nie znajdziemy szukanego PID dla każdego z dzieci wywołujemy funkcję get_kid 
		children_vector[i]->get_kid_name(find_name); // rekurencja
	}

	return nullptr; // jeśli nie znajdziemy name , zwracamy nullptr
}

void PCB::kill_kid(int &kid_pid) {
	
	for (int i = 0; i < this->children_vector.size(); i++) {
		if (children_vector[i]->pid == kid_pid) {
			this->children_vector.erase(this->children_vector.begin() + i); // usunięcie procesu 
			
		}
	}

	// przesunięcie pozostałych
}

void ProcTree::create_process_file(string &name, string &file_name, int parent_pid)  // zakładająć że nie mamy podfolderów i ścieżka będzię jedynie nazwą pliku
{
        cout << parent_pid << " " << this->init_proc->pid << "\n";
    if(parent_pid == this->init_proc->pid){
        shared_ptr<PCB> proc = make_shared<PCB>(name, parent_pid);  // utworzenie wskaźnika na proces - wywołanie konstruktora procesu
        this->init_proc->children_vector.push_back(proc); // dodanie procesu jako dziecka procesu INIT
        memory.LoadProgram(file_name, proc->pid); // Załaduj kod programu do pamięci wirtualnej
    } else {
        shared_ptr<PCB> parent = this->init_proc->get_kid_pid(parent_pid);

        if(parent == nullptr){
            cout<<"Brak takiego rodzica "<<endl; 
        } else {
            shared_ptr<PCB> proc = make_shared<PCB>(name, parent_pid);  // utworzenie wskaźnika na proces - wywołanie konstruktora procesu
            parent->children_vector.push_back(proc); // dodanie procesu jako dziecka odnalezionego procesu rodzica
            memory.LoadProgram(file_name, proc->pid); // Załaduj kod programu do pamięci wirtualnej
        }
    }


}

shared_ptr<PCB> ProcTree::find_pid(int &pid_proc){

    shared_ptr<PCB> PCB_return = nullptr;

    for(int i=0; i<init_proc->children_vector.size(); i++){ // przeszukujemy wektor dzieci w celu znalezienia szukanego PID
        if(init_proc->children_vector[i]->pid == pid_proc){
            return init_proc->children_vector[i];  // zwracamy wskaźnik na PCB o znalezionym PID
        }
    }

    for(int i=0; i<init_proc->children_vector.size(); i++){ // jeśli nie znajdziemy szukanego PID dla każdego z dzieci wywołujemy funkcję get_kid 
        PCB_return = init_proc->children_vector[i]->get_kid_pid(pid_proc); // rekurencja
    }

    return PCB_return;
}

shared_ptr<PCB> ProcTree::find_name(string &name) {

	shared_ptr<PCB> PCB_return = nullptr;

	for (int i = 0; i < init_proc->children_vector.size(); i++) { // przeszukujemy wektor dzieci w celu znalezienia szukanego PID
		if (init_proc->children_vector[i]->name == name) {
			return init_proc->children_vector[i];  // zwracamy wskaźnik na PCB o znalezionym name
		}
	}

	for (int i = 0; i < init_proc->children_vector.size(); i++) { // jeśli nie znajdziemy szukanego name dla każdego z dzieci wywołujemy funkcję get_kid 
		PCB_return = init_proc->children_vector[i]->get_kid_name(name); // rekurencja
	}

	return PCB_return;

}

void ProcTree::kill_pid(int &pid) { // zabicie procesu po PID

	shared_ptr<PCB> PCB_proc_kill = ProcTree::find_pid(pid); 
	shared_ptr<PCB> PCB_parent = ProcTree::find_pid(PCB_proc_kill->parent_pid);
	bool vector_child = 0;
	process_state PCB_new_state;

	if (PCB_proc_kill == nullptr) {
		// niedobrze
		cout << "Brak procesu o podanym PID" << endl;
	}
	else {
		vector_child = PCB_proc_kill->null_vector_child();
		if (vector_child == 1) {
			// kill proces
			PCB_new_state = terminated;
			PCB_proc_kill->change_state(PCB_new_state);  // zmiana stanu na terminated 
			// usuwanie z drzewa
			PCB_parent->kill_kid(pid);

			//usuwanie z kolejek - czeka na zmiany w planiście
//Planist::remove_pcb_from_ready(pid); 
//Planist::remove_pcb_from_wait(pid); 

		}
		else {
			cout << "Podany proces oczekuje na zakończenie procesów potomnych" << endl;
			PCB_new_state = wait;
			// planista - czeka na funkcję 
			// Planist::manage(); 

			PCB_proc_kill->change_state(PCB_new_state);

		}

	}

}
	
void ProcTree::kill_name(string &name) {

	shared_ptr<PCB> PCB_proc_kill = ProcTree::find_name(name);
	int pid = PCB_proc_kill->pid;
	shared_ptr<PCB> PCB_parent = ProcTree::find_pid(PCB_proc_kill->parent_pid);
	bool vector_child = 0;
	process_state PCB_new_state;

	if (PCB_proc_kill == nullptr) {
		// niedobrze
		cout << "Brak procesu o podanym PID" << endl;
	}
	else {
		vector_child = PCB_proc_kill->null_vector_child();
		if (vector_child == 1) {
			// kill proces
			PCB_new_state = terminated;
			PCB_proc_kill->change_state(PCB_new_state);  // zmiana stanu na terminated 
			// usuwanie z drzewa
			PCB_parent->kill_kid(pid);

			//usuwanie z kolejek - czeka na zmiany w planiście
//Planist::remove_pcb_from_ready(pid); 
//Planist::remove_pcb_from_wait(pid); 

		}
		else {
			cout << "Podany proces oczekuje na zakończenie procesów potomnych" << endl;
			PCB_new_state = wait;
			// planista - czeka na funkcję 
			// Planist::manage(); 

			PCB_proc_kill->change_state(PCB_new_state);

		}

	}

}

void PCB::show_vector_child() {

	/*

	for (int i = 0; i < this->children_vector.size(); i++) {
		cout << this->children_vector[i]->name << " - " << this->children_vector[i]->pid << endl;
	}

	*/
}

void ProcTree::display_tree()
{
		
//	cout << this->init_proc->name << " - "<< this->init_proc->pid << endl;

	


    // sposób wyświetlania drzewa do ugadania z Anią :) 
}
