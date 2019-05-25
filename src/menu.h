#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Menu {
	int trash;
	string name;
	char opt_char;
	vector<Menu*> children;
	const void (*funcPtr)(Instance*);
	Menu* parent;
	Instance* funcArg;
	bool printChildren = 0; // if children should be printed along.
				/**
							Parent = NULL means the parent of this option is the Menu itself.
							*/

	bool isRoot() {
		//cout << "Testing isRoot():\nopt_char = " << opt_char << ".\nparent = " << parent << ".\n";
		return (opt_char == NULL && parent == NULL);
	}

	// constructor
	Menu(const string n, const char oc, const void (*funcP)(Instance*), Instance* option) {
		name = n;
		funcArg = option;
		opt_char = oc;
		funcPtr = funcP;
		parent = NULL;
	}

	void print() {
		trash = system("clear");
		cout << "========= " << this->name << " =========" << endl;
		for (unsigned int i = 0; i < children.size(); i++) {
			cout << children[i]->opt_char << ". " << children[i]->name << endl;
			if (children[i]->children.size() != 0 && children[i]->printChildren) {
				children[i]->print();
			}
		}
		if (isRoot()) {
			cout << "x"
			     << ". "
			     << "Exit" << endl;
		} else {
			cout << "x"
			     << ". "
			     << "Go back" << endl;
		}
	}

	Menu* addOption(const string n, const char oc, const void (*funcPtr)(Instance*), Instance* option = 0) {
		//cout << "Adding option to " << this->name << "\nOption name: " << n << ".\n";
		if (oc == 'x') {
			cerr << "[MENU] The option you want to add: \"" << n << "\" uses a reserved character." << endl;
			return NULL;
		}
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->opt_char == oc) {
				cerr << "[MENU] The option you want to add: \"" << n << "\" makes use of a character which is already in use." << endl;
				return NULL;
			}
		}
		Menu* nOption = new Menu(n, oc, funcPtr, option);
		this->children.push_back(nOption);
		nOption->parent = this;
		//cout << "Option " << nOption->name << " added. Its parent is "<< this->name <<" == " <<nOption->parent->name << ".\nIts parent now has " << this->children.size() << " children.\n\n";

		return nOption;
	}
	int waitAndRun() {
		char a = getc(stdin);
		getc(stdin); // get \n out of buffer
		bool caught = false;
		if (a == 'x') { //back
			if (isRoot()) {
				/*
				=> MEMORY LEAK HERE
				free ao menu aqui (atexit handler?)
				*/

				exit(0);
			} else {
				//cout << "not root. falling back into parent\n";
				this->parent->print();
				this->parent->waitAndRun();
				return 0;
			}
		}
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->opt_char == a) {
				//cout << "char: " << a << ". Option name: " << children[i]->name << ".\n" << "nChildren: "<< children[i]->children.size()<<".\n\n";

				if (children[i]->funcPtr != NULL) {
					children[i]->funcPtr(children[i]->funcArg);
					//cin.ignore(100, '\n');
					cout << endl
					     << endl
					     << "Press [ENTER] to return to the menu...\n";
					getchar();
					this->print();
					this->waitAndRun();
				} else if (children[i]->children.size() == 0) { // This function's funcPtr is NULL
					trash = system("clear");
					cout << "Esta opcao ainda nao foi associada a nenhuma funcao e nao tem sub-opcoes!\n\n";
					cout << "Prima [ENTER] para continuar...\n";
					getchar();
					this->print();
					this->waitAndRun();
				} else {
					children[i]->print();
					children[i]->waitAndRun();
				}

				caught = 1;
			}
		}
		if (!caught) { // The char we got didn't match any of the options
			trash = system("clear");
			cout << "This option does not exist!" << endl
			     << endl;
			cout << "Press [ENTER] to continue...\n";
			getchar();
			this->print();
			this->waitAndRun();
		}
		return 0;
	}
};