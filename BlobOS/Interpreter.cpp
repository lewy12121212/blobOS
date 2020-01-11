#include "Interpreter.h"
#include <iostream>
#include "process.h"
#include "procesor.h"
#include "File.h"


using namespace std;

extern FileManager FM;
extern ProcTree PTree;
extern Memory memory;

Interpreter interpreter;

string Interpreter::get_instruction(unsigned int& instruction_counter, const shared_ptr<PCB>& running_proc)
{

	
	string instruction;
	char sign; 

	while (memory.get(instruction_counter, running_proc->pid) != ';') {
		sign = memory.get(instruction_counter, running_proc->pid);
		instruction.push_back(sign);
		instruction_counter++;
	}


	return instruction;
}

void Interpreter::take_from_proc(const shared_ptr<PCB>& running_proc)
{
	instruction_counter = running_proc->register_copy[4];
	A = running_proc->register_copy[0];
	B = running_proc->register_copy[1];
	C = running_proc->register_copy[2];
	D = running_proc->register_copy[3];

}

void Interpreter::update_proc(const shared_ptr<PCB>& running_proc)
{
	running_proc->register_copy[4] = instruction_counter;
	running_proc->register_copy[0] = A;
	running_proc->register_copy[1] = B;
	running_proc->register_copy[2] = C;
	running_proc->register_copy[3] = D;
}


array<string, 4> Interpreter::instruction_separate(const std::string & instruction)
{
	string tmp;
	array<string, 4> command;
	int i = 0;

	for (const char& x : instruction) {
		if (x != ' ' || x == ';') {
			tmp += x;
		}
		else if (!tmp.empty())
		{
			command[i] = tmp;
			tmp.clear();
			i++;
		}
	}

	return command;
}


void Interpreter::display_registers()
{

	cout << "Registers: " << endl;
	cout << "A:  " << "  " << A << "B:  " << B << "  " << "C:  " << C << "  " << "D:  " << D << endl;
	cout << "Instruction counter:  " << instruction_counter << endl;

}


int Interpreter::execute_instruction(std::string& instruction, shared_ptr<PCB>& running_proc)
{
	

	//tutaj bêd¹ pobierane rejestry itd, szukanie procesu i wrzucenie go do instrukcji 
	exec_instruction = instruction_separate(instruction);
	int i = 0;
	int adres = 0;
	int from_memory = 0;


	vector<char>value; 
	char val = '0';

	int *rej1=0;
	int *rej2=0;
	int counter=0;



	string command;
	string file_name;
	string text;
	string tmp;

	command = exec_instruction[0];


	if (!exec_instruction[1].empty()) {
		if (exec_instruction[1] == "A") { rej1 = &A; }
		else if (exec_instruction[1] == "B") { rej1 = &B; }
		else if (exec_instruction[1] == "C") { rej1 = &C; }
		else if (exec_instruction[1] == "D") { counter = D; }
		else if (exec_instruction[1] == "[") {
			
			exec_instruction[1].pop_back();
			exec_instruction[1].erase(exec_instruction[1].begin());

			adres = stoi(exec_instruction[1]);

		}
		else if (exec_instruction[1] == "(") {

			exec_instruction[1].pop_back();
			exec_instruction[1].erase(exec_instruction[1].begin());

			file_name=exec_instruction[1];

		}
		else {
			tmp = exec_instruction[1];

			auto it = tmp.begin();
			while (it != tmp.end()) {
				value.push_back(*it);
				it++;
			}

			value.push_back(';');

			//val = exec_instruction[1].c_str;
			i = stoi(exec_instruction[1]);
			*rej1 = i;
		}
	}

	if (!exec_instruction[2].empty()) {
		if (exec_instruction[2] == "A") { rej2 = &A; }
		else if (exec_instruction[2] == "B") { rej2 = &B; }
		else if (exec_instruction[2] == "C") { rej2 = &C; }
		else if (exec_instruction[2] == "D") { counter = D; }
		else if (exec_instruction[2] == "[") {
			
			exec_instruction[2].pop_back();
			exec_instruction[2].erase(exec_instruction[2].begin());

			adres = stoi(exec_instruction[2]);

		}
		else if (exec_instruction[2] == "(") {

			exec_instruction[2].pop_back();
			exec_instruction[2].erase(exec_instruction[2].begin());
			//bal
			text = exec_instruction[2];

		}
		else {
			//val = exec_instruction[2].c_str;
			//blaa

			tmp = exec_instruction[2];

			auto it = tmp.begin();
			while (it != tmp.end()) {
				value.push_back(*it);
				it++;
			}
			value.push_back(';');
			i = stoi(exec_instruction[2]);
			*rej2 = i;
		}
	}

	//tu s¹ ogarniane rozkazy 
	if (command == "AD") { *rej1 += *rej2; }
	else if (command == "SB") { *rej1 -= *rej2; }
	else if (command == "ML") { *rej1 *= *rej2; }
	else if (command == "DV") { *rej1 /= *rej2; }
	else if (command == "MD") {
		if (rej2 == 0) {
			cout << "Dzielenie przez 0!" << endl;
			*rej1 = *rej1 % *rej2;
		}
	}
	else if (command == "IN") { *rej1++; }
	else if (command == "DE") { *rej1--; }
	else if (command == "MV") { *rej1 = *rej2; }
	else if (command == "WR") {

		auto it = value.begin();
		while (it != value.end()) {
			memory.set(adres, *it, running_proc->pid);
			it++;
		}
	}
	else if (command == "GT") {
		
		tmp.clear();
		while (true) {
			val = memory.get(adres, running_proc->pid);
			if (val == ';') false;
			else tmp.push_back(val);
		}

		from_memory = stoi(tmp);

	}
	else if (command == "LP") {
		C = instruction_counter++;
	}
	else if (command == "JP") {
		instruction_counter = adres;
	}
	else if (command == "JZ") {
		if (counter == 0)
			instruction_counter = C;
	}
	else if (command == "JN") {
		if (counter != 0)
			instruction_counter = adres;
	}
	else if (command == "MF") {

		FM.create_file(file_name);
		//tu poleci coœ od plików
	}
	else if (command == "OF") {

		//tu te¿
	}
	else if (command == "WF") {

		FM.save_data_to_file(file_name, text);
		//i tu
	}
	else if (command == "AF") {
		FM.edit_file(file_name, text);
	}
	else if (command == "RF") {

		FM.show_file(file_name);
		//kurwa ile jeszcze 
	}
	else if (command == "CF") {
		//dobra koniec xd	
	}
	else if (command == "CP") {
		PTree.create_process_file(file_name, text, running_proc->pid);
		//tu teoretycznie moge stworzyc proces
	}
	else if (command == "KP") {
		//a tu go zabic 
		//running_proc->kill();
		PTree.kill_pid(running_proc->pid);
		//blaa
	}
	else if (command == "HT") {
		//
		//running_proc->kill();
		PTree.kill_pid(running_proc->pid);
		return -1;
	}
	else if (command == "NN") {

	}

	

	return 1;
}

int Interpreter::execute_line() //czy na pewno nazwa?? 
{
	shared_ptr<PCB> running_proc = planist.ReadyPCB.front();
	take_from_proc(running_proc);
	instruction = get_instruction(instruction_counter, running_proc);
	execute_instruction(instruction, running_proc);
	update_proc(running_proc);

	return 0;
}


