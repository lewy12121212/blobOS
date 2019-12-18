#pragma once
#include "procesor.h"
#include "process.h"
#include <memory>
#include <list>
#include <iostream>

using namespace std;

Planist planist;

void Planist::check(){
    // ReadyPCB
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
    if (process->state == ready || process->state == run){
        ReadyPCB.push_back(process);
        sort_list();
    }
    else{
        WaitPCB.push_back(process);
    }
}

void Planist::remove_process(const shared_ptr<PCB>& process){
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
    }
}

void Planist::display_PCB_list(){
    // ReadyPCB
    cout << "PROCESY READY: " << (ReadyPCB.empty() ? "pusta" : "") << endl; // sprawdza czy lista jest pusta
    for (const auto& rPCB : ReadyPCB){
        cout << " - " << rPCB->name << ", pid: " << rPCB->pid;
        
        if (rPCB->executionTimeLeft != 9999){
            cout << "   (cykle: " << rPCB->executionTimeLeft;
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
        cout << "   (cykle: " << wPCB->executionTimeLeft;

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