#include "Interpreter.h"
#include <iostream>
#include <sstream>
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

	instruction_counter++;

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
	std::istringstream iss(instruction);
	std::vector<std::string> results((std::istream_iterator<std::string>{iss}),
		std::istream_iterator<std::string>());
	array<string, 4> command = array<string, 4>();
	for (int i = 0; i < results.size(); i++) {
		command[i] = results[i];
	}

	//for (const char& x : instruction) {
	//	/*if (x != ' ') {
	//		tmp += x;
	//	}
	//	else if (!tmp.empty())
	//	{
	//		command[i] = tmp;
	//		tmp.clear();
	//		i++;
	//	}*/
	//}

	return command;
}


void Interpreter::display_registers()
{
	
	cout << "Registers: " << endl;
	cout << "A:  " << A << "  " << "B:  " << B << "  " << "C:  " << C << "  " << "D:  " << D << endl;
	cout << "Instruction counter:  " << instruction_counter << endl<<endl;

}


int Interpreter::execute_instruction(std::string& instruction, shared_ptr<PCB>& running_proc)
{


	//tutaj będą pobierane rejestry itd, szukanie procesu i wrzucenie go do instrukcji 
	exec_instruction = instruction_separate(instruction);
	int i = 0;
	int adres = 0;
	int from_memory = 0;


	vector<char>value;
	char val = '0';

	int rej1np = 0;
	int rej2np = 0;
	

	int* rej1 = &rej1np;
	int* rej2 = &rej2np;



	string command;
	string file_name;
	string text;
	string tmp;

	command = exec_instruction[0];


	if (!exec_instruction[1].empty()) {
		if (exec_instruction[1] == "A") { rej1 = &A; }
		else if (exec_instruction[1] == "B") { rej1 = &B; }
		else if (exec_instruction[1] == "C") { rej1 = &C; }
		else if (exec_instruction[1] == "D") { rej1 = &D; }
		else if (exec_instruction[1][0] == '[') {

			exec_instruction[1].pop_back();
			exec_instruction[1].erase(exec_instruction[1].begin());

			adres = stoi(exec_instruction[1]);

		}
		else if (exec_instruction[1][0] == '(') {

			exec_instruction[1].pop_back();
			exec_instruction[1].erase(0, 1);

			file_name = exec_instruction[1];

		}
		else if (exec_instruction[1] == "LP"){}
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
		else if (exec_instruction[2] == "D") { rej2 = &D; }
		else if (exec_instruction[2][0] == '[') {

			exec_instruction[2].pop_back();
			exec_instruction[2].erase(exec_instruction[2].begin());

			adres = stoi(exec_instruction[2]);

			if (command == "AD" || command == "SB" || command == "ML" || command == "DV" || command == "MD" || command == "MV") {
				while (true) {
					val = memory.get(adres, running_proc->pid);
					if (val == ';') break;
					else tmp.push_back(val);
					adres++;
				}

				from_memory = stoi(tmp);
				*rej2 = from_memory;
			}

		}
		else if (exec_instruction[2][0] == '(') {

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

	//tu są ogarniane rozkazy 
	if (command == "AD") { *rej1 += *rej2; }
	else if (command == "SB") { *rej1 -= *rej2; }
	else if (command == "ML") { *rej1 *= *rej2; }
	else if (command == "DV") {
		if (rej2 == 0) {
			cout << "Division by 0!" << endl;
			*rej1 /= *rej2;
		}
	}
	else if (command == "MD") {
		if (rej2 == 0) {
			cout << "Division by 0!" << endl;
			*rej1 = *rej1 % *rej2;
		}
	}
	else if (command == "IN") { (*rej1)++; }
	else if (command == "DE") { (*rej1)--; }
	else if (command == "MV") { *rej1 = *rej2; }
	else if (command == "WR") {
		if (!value.empty()) {
			auto it = value.begin();
			while (it != value.end()) {
				memory.set(adres, *it, running_proc->pid);
				adres++;
				it++;
			}
		}
		else {
			tmp = to_string(*rej2);
			for (auto& a : tmp) {
				value.push_back(a);
			}
			value.push_back(';');
			auto it = value.begin();
			while (it != value.end()) {
				memory.set(adres, *it, running_proc->pid);
				adres++;
				it++;
			}
		}
	}
	else if (command == "GT") {

		tmp.clear();
		while (true) {
			val = memory.get(adres, running_proc->pid);
			if (val == ';') break;
			else tmp.push_back(val);
			//cout << "!!!!" << tmp << endl;
			adres++;
		}

		//cout << "!!!!" << tmp << endl;
		from_memory = stoi(tmp);
		*rej1 = from_memory;

	}
	else if (command == "LP") {
		C = instruction_counter+1;
	}
	else if (command == "JP") {
		instruction_counter = C;
	}
	else if (command == "JZ") {
		if (D == 0)
			instruction_counter = C-1;
	}
	else if (command == "JN") {
		if (D != 0)
			instruction_counter = C-1;
	}
	else if (command == "MF") {

		FM.create_file(file_name);
		
	}
	else if (command == "OF") {

		FM.open_file(file_name);
	}
	else if (command == "WF") {
		if(!text.empty())
		FM.save_data_to_file(file_name, text);
		else FM.save_data_to_file(file_name, to_string(*rej2));
	}
	else if (command == "AF") {
		if (!text.empty())
			FM.add_to_file(file_name, text);
		else FM.add_to_file(file_name, to_string(*rej2));
	}
	else if (command == "RF") {
		string textt = FM.show_file(file_name);
		cout << textt << endl;
	}
	else if (command == "CF") {

		FM.close_file(file_name);
	}
	else if (command == "CP") {
		PTree.create_process_file(file_name, text, running_proc->pid);
	}
	else if (command == "KP") {
		
		PTree.kill_name(file_name);
		
	}
	else if (command == "HT") {
		PTree.kill_pid(running_proc->pid);
		return -1;
	}
	else if (command == "NN") {

	}


	return 1;
}

int Interpreter::execute_line() 
{
	shared_ptr<PCB> running_proc = planist.ReadyPCB.front();
	
	if (running_proc != nullptr) {
		if (running_proc->time_run == 0) {
			planist.manager();
		}

		/*
			- zakomentowane w celu sprawdzenia poprawności odejmowania i przydziału kwantów czasu
			po odkomentowaniu poprawne działanie dla procesu INIT
			błąd krytyczny przy nowo utworzonych procesach ze względu na brak jakochkolwiek rozkazów :)
		*/

		cout << "Running proccess: " << running_proc->name << endl;
		take_from_proc(running_proc);
		instruction = get_instruction(instruction_counter, running_proc);
		cout << instruction << "\n";
		execute_instruction(instruction, running_proc);
		this->display_registers();
		update_proc(running_proc);


		// planista i procesy
		running_proc->time_run--;  // zmiejszenie przydzielonego kwantu czasu o 1
		planist.time_sum--; // zmiejszenie sumy przydzielonych kwantów o 1

		cout << "Time to run: " << running_proc->time_run << endl;

		if (running_proc->time_run == 0) { // jeżeli proces kwant czasu ma na 0 to zostaje przeniesiony na koniec i run otrzymuje następny proces
			planist.first_to_end();
		}

		if (planist.time_sum == 0) {	 // jeżeli suma kwantów jest 0 to na nowo przydzielamy kwanty dla procesów
			planist.manager();
		}
	}
	else
		cout << "The process does not exist!" << endl;

	return 0;
}


