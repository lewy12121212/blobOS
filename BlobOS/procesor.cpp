#pragma once
#include "procesor.h"
#include "process.h"
#include <memory>
#include <list>
#include <iostream>

using namespace std;

Planist planist;
extern ProcTree PTree;


void Planist::init() {
    this->add_process(PTree.init_proc);
}

void Planist::check(){
    // ReadyPCB
	// init check
	cout << "CHECK" << endl;
	process_state init_state;

	if (PTree.init_proc->state == ready || PTree.init_proc->state == run) {  // czyli jest w kolejce ready 
		if (this->ReadyPCB.size() > 1) {
			init_state = wait;
			PTree.init_proc->change_state(init_state);
		}
	}
	else if (PTree.init_proc->state == wait) {
		if (this->ReadyPCB.size() == 0 || this->ReadyPCB.size() == 1 && this->ReadyPCB[0]->state == terminated) {
			init_state = ready;
			PTree.init_proc->change_state(init_state);
		}
	}
	//PTree.init_proc->show_info();


	//cout << "ready" << endl;

	for (int i = 0; i < this->ReadyPCB.size(); i++) {
		if (this->ReadyPCB[i]->state != ready || this->ReadyPCB[i]->state != run) {
			if (this->ReadyPCB[i]->state == terminated) {
				this->ReadyPCB.erase(this->ReadyPCB.begin() + i);
			}
			else if (this->ReadyPCB[i]->state == wait) {
				this->WaitPCB.push_back(this->ReadyPCB[i]);
				this->ReadyPCB.erase(this->ReadyPCB.begin() + i);
				
			}
		}
	}

	//cout << "wait" << endl;

	for (int i = 0; i < this->WaitPCB.size(); i++) {
		if (this->WaitPCB[i]->state != wait) {
			if (this->WaitPCB[i]->state == terminated) {
				this->WaitPCB.erase(this->WaitPCB.begin() + i);
			}
			else if (this->WaitPCB[i]->state == ready || this->WaitPCB[i]->state == run) {
				this->ReadyPCB.push_back(this->WaitPCB[i]);
				this->WaitPCB.erase(this->WaitPCB.begin() + i);
				
			}
		}
	}


	this->sort_list(); // ustawienie pierwszego procesu na run 

	/*
    for(auto rPCB = ReadyPCB.begin(); rPCB != ReadyPCB.end(); ++rPCB){
        if ((*rPCB)->state == terminated){
            rPCB = ReadyPCB.erase(rPCB);
        }
        else if ((*rPCB)->state != ready && (*rPCB)->state != run){
            WaitPCB.push_back(*rPCB);
            rPCB = ReadyPCB.erase(rPCB);
        }
    }

    sort_list();

    // WaitPCB
    for (auto wPCB = WaitPCB.begin(); wPCB != WaitPCB.end(); ++wPCB){
        if ((*wPCB)->state == terminated){
            wPCB = WaitPCB.erase(wPCB);
        }
        else if ((*wPCB)->state == ready || (*wPCB)->state == run){
            add_process(*wPCB);
            wPCB = WaitPCB.erase(wPCB);
        }

        if (wPCB == WaitPCB.end()){
            break;
        }
    }

	*/
}

/*bool Planist::compare_PCB(const shared_ptr<PCB>& first, const shared_ptr<PCB>& second){
    if (first->executionTimeLeft < second->executionTimeLeft){
        return true;    // brak zmiany
    }
    else if (first->executionTimeLeft == second.executionTimeLeft && first->pid < second->pid){
        return true;    // brak zmiany
    }
    return false;   // zamiana pierwszego z drugim
}*/

void Planist::sort_list(){
    // sortowanie listy
    //ReadyPCB.sort(compare_PCB);

    // zmiana first process state na RUN reszta na READY
    for (auto it = ReadyPCB.begin(); it != ReadyPCB.end(); ++it){
        if (it == ReadyPCB.begin()){
            (*it)->state = run;
        }
        else if ((*it)->state != ready){
            (*it)->state = ready;
        }
    }
}

void Planist::add_process(const shared_ptr<PCB>& process){
    // dodawanie procesow w stanie READY i RUN
	cout << "process" << process->name << " is added to ready queue" << endl;
    if (process->state == ready || process->state == run){
        ReadyPCB.push_back(process);
        sort_list();
    }
    else{
        WaitPCB.push_back(process);
    }
}

void Planist::remove_process(const shared_ptr<PCB>& process){


	cout << "ready" << endl;

	for (int i = 0; i < this->ReadyPCB.size(); i++) {
		if (this->ReadyPCB[i]->pid == process->pid) {
			this->ReadyPCB.erase(this->ReadyPCB.begin() + i);
		}
	}

	cout << "wait" << endl;

	for (int i = 0; i < this->WaitPCB.size(); i++) {
		if (this->WaitPCB[i]->pid == process->pid) {
			this->WaitPCB.erase(this->WaitPCB.begin() + i);
		}
	}
	/*
    for (auto rPCB = ReadyPCB.begin(); rPCB != ReadyPCB.end(); ){
        if ((*rPCB)->pid == process->pid){
            rPCB == ReadyPCB.erase(rPCB);
        }
        else{
            ++rPCB;
        }
    }

    for (auto wPCB = WaitPCB.begin(); wPCB != WaitPCB.end(); ){
        if ((*wPCB)->pid == process->pid){
            wPCB = WaitPCB.erase(wPCB);
        }
        else{
            ++wPCB;
        }
    } */
}

void Planist::display_PCB_list(){

	if (this->ReadyPCB.size() > 0) {
		if (this->ReadyPCB[0]->time_run == 0) { // dla pierszego ( mo¿e bezpieczniej bêdzie sprawdziæ wszystkie
			this->manager();
		}
	}


    // ReadyPCB
    cout << "PROCESY READY: " << (ReadyPCB.empty() ? "pusta" : "") << endl; // sprawdza czy lista jest pusta
    for (const auto& rPCB : ReadyPCB){
        cout << " - " << rPCB->name << ", pid: " << rPCB->pid;
        
        if (rPCB->executionTimeLeft != 9999){
            cout << "   (cykle: " << rPCB->time_run;
        }
        else{
            cout << "   (cykle: nie dotyczy";
        }

        if (rPCB->state == run){
            cout << ", run";
        }
        else if (rPCB->state == ready){
            cout << ", ready";
        }
        else if (rPCB->state == wait){
            cout << ", wait";
        }

        cout << ")" << endl;
    }

    // WaitPCB
    cout << "PROCESY WAIT: " << (WaitPCB.empty() ? "pusta" : "") << endl;
    for (const auto& wPCB : WaitPCB){
        cout << " - " << wPCB->name << ", pid: " << wPCB->pid;
       // cout << "   (cykle: " << wPCB->executionTimeLeft;

        if (wPCB->state == run){
            cout << ", run";
        }
        else if (wPCB->state == ready){
            cout << ", ready";
        }
        else if (wPCB->state == wait){
            cout << ", wait";
        }

        cout << ")" << endl;
    }

    cout << endl;
}

void Planist::first_to_end() {

	shared_ptr<PCB> process = this->ReadyPCB[0];
	this->ReadyPCB.erase(this->ReadyPCB.begin());
	this->ReadyPCB.push_back(process);

}

void Planist::manager() { // przydzielenie kwantu czasu 

	this->check();


	this->amount_process = this->ReadyPCB.size();

	if (this->amount_process >= this->time) { // je¿eli procesów bêdzie 10 lub wiêcej to ka¿dy dostanie 1 rozkaz przydzia³u

		this->time_per_one = 1;
		this->time_sum = this->amount_process;

	}
	else {
		this->time_per_one = this->time / this->amount_process;
		this->time_sum = this->time_per_one * this->amount_process;

	}
	/*for (auto rPCB = ReadyPCB.begin(); rPCB != ReadyPCB.end(); ++rPCB) {
		amount_process++;
	}*/

	//cout << "ap: " << amount_process << endl;
	//cout << "sizeR: " << ReadyPCB.size() << endl;
	//cout << "time p o " << time_per_one << endl;
	//cout << "time sum: " << time_sum << endl;


	for (int i = 0; i < this->ReadyPCB.size(); i++) {
		this->ReadyPCB[i]->time_run = this->time_per_one;
	}



}
