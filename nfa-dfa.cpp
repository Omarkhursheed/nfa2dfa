#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>
using namespace std;


#define MAXINT 1000

//Input data variables

int numberOfStates;
int numberOfAlphabets;
vector<string> inputDictionary;
vector<string> inputStates;
vector<string> inputFinalStates;
vector<string> inputInitialState;
string line;
int counter = 0;
int numberOfTransitions = 0;
int i;
int size;
int tempCount=0;


vector<string> outputDictionary;
vector<string> outputStates;
vector<string> outputFinalStates;
vector<string> outputInitialState;

//File variables
ifstream inputfile;
ofstream outputfile;


string transitionTable[100][3];
void calculateStates(string);

//Function to find epsilon transition
string epsilon(string state){
	set<int> bla;
	if(state.find('-') == string::npos){
		int count = 0;
		string temp = "";
		for(int i = 0; i < numberOfTransitions; i++){
			if(transitionTable[i][0] == state && transitionTable[i][1] == "E" && transitionTable[i][2] != "_"){
				bla.insert(stoi(transitionTable[i][2].substr(1,transitionTable[i][2].length()-1)));	
				//cout<<stoi(transitionTable[i][2].substr(1,transitionTable[i][2].length()-1))<<" ";

			}
		}
		cout<<endl;
		set<int>::iterator it;
		for(it = bla.begin(); it != bla.end(); it++){
			int f = *it;
			if(it == bla.begin()){
				temp += "q" + to_string(f);
			} else {
				temp += "-q" + to_string(f);
				}
				//cout<<*it<<" "; 
		}
		cout<<endl;
		cout<<temp<<endl;
		return temp;
	} else {
		string temp = "";
		int count = 0;
		stringstream ss(state);
		string token;
		while(getline(ss, token, '-')){
			for(int i = 0; i < numberOfTransitions; i++){
				if (transitionTable[i][0] == token && transitionTable[i][1] == "E" && transitionTable[i][2] != "_"){
					bla.insert(stoi(transitionTable[i][2].substr(1,transitionTable[i][2].length()-1)));
				}
			}
		}
		set<int>::iterator it;
		for(it = bla.begin(); it != bla.end(); it++){
			int f = *it;
			if(it == bla.begin()){
				temp += "q" + to_string(f);
			} else {
				temp += "-q" + to_string(f);
				} 
		}
		return temp;
	}	
}

string calculate(string state, string alphabet){
	set<int> bla;
	if(state.find('-') == string::npos){
		int count = 0;
		string temp = "";
		for(int i = 0; i < numberOfTransitions; i++){
			if(transitionTable[i][0] == state && transitionTable[i][1] == alphabet && transitionTable[i][2] != "_"){
				bla.insert(stoi(transitionTable[i][2].substr(1,transitionTable[i][2].length()-1)));				
			}
		}
		set<int>::iterator it;
		for(it = bla.begin(); it != bla.end(); it++){
			int f = *it;
			if(it == bla.begin()){
				temp += "q" + to_string(f);
			} else {
				temp += "-q" + to_string(f);
				} 
		}

		return temp;
	} else {
		//cout<<"SDASDASD"<<endl;
		string temp = "";
		int count = 0;
		stringstream ss(state);
		string token;
		while(getline(ss, token, '-')){
			cout<<token<<endl;
			for(int i = 0; i < numberOfTransitions; i++){
				if (transitionTable[i][0] == token && transitionTable[i][1] == alphabet && transitionTable[i][2] != "_"){
					bla.insert(stoi(transitionTable[i][2].substr(1,transitionTable[i][2].length()-1)));
				}
			}
		}
		set<int>::iterator it;
		for(it = bla.begin(); it != bla.end(); it++){
			int f = *it;
			if(it == bla.begin()){
				temp += "q" + to_string(f);
			} else {
				temp += "-q" + to_string(f);
				} 
		}
		return temp;
	}	
}

int main(int argc, char const *argv[])
{
	inputfile.open("input.txt");
	outputfile.open("output.txt");
	
	//Input file stuff
	// _ in the input file represents an unknown state which will become a trap state in the DFA
	// An epsilon (lambda) transition is represented in the dictionary as E
	//Input file format
	/*
	Input file format: 

	Number Of States, number of alphabets (if only implicit epsilon transitions, then E need not be included)
	Comma separated string of states
	Initial State
	Comma separated string of final states
	Dictionary elements (i.e. possible input alphabets separated by commas) If an explicit lambda transition is required, include the element E in the dictionary string
	Transition states:
	Formatted as 
	Initial state, dictionary element, destination states separated by commas
	
	*/
 	for(int i = 0; i < 100; i++){
 		transitionTable[i][2] = "_";
 	}
	while(getline(inputfile, line)){
		if(counter == 0){
			stringstream ss(line);									//Get number of states and number of alphabets
			string token;
			int count2 = 0;
			while(getline(ss, token, ',')){
				if(count2 == 0){
					numberOfStates = stoi(token);
					cout << numberOfStates;
				} else if (count2 == 1){
					numberOfAlphabets = stoi(token);
					cout << numberOfAlphabets;
				}
				count2++;
			}
		} else if(counter == 1){									// Get a list of states
			stringstream ss(line);
			string token;
			while(getline(ss, token, ',')){

				cout << token;
				inputStates.push_back(token);
			}
		} else if(counter == 2){									//get initial state

			cout << line;
			inputInitialState.push_back(line);
		} else if(counter == 3){									//get the list of final states

			stringstream ss(line);
			string token;
			while(getline(ss, token, ',')){			

				inputFinalStates.push_back(token);
			}
		} else if(counter == 4){									//get the list of alphabets
			stringstream ss(line);
			string token;
			int x = 0;
			while(getline(ss, token, ',')){			

				
					inputDictionary.push_back(token);
				
			}
		} else if(counter >= 5){									//Split by delimiter and create the transition table for NFA
			stringstream ss(line);
			string token;
			int k = 0;
				if(numberOfTransitions == 0){
					for(i = 0; i < numberOfStates; i++){
					transitionTable[i][0] = inputStates[i];
					transitionTable[i][1] = "E";
					transitionTable[i][2] = inputStates[i];
					numberOfTransitions++;
				}
			}
			while(getline(ss, token, ',')){			
				{transitionTable[numberOfTransitions][k] = token;
				k++;
				}

			}
			numberOfTransitions++;
		}
		counter++;
	}

	cout<<numberOfTransitions<<endl;
	//NFA Transition table
	cout << "NFA Transition table" << endl;
	for(int i = 0; i < numberOfTransitions-1; i++){
		cout << transitionTable[i][0] << "\t" << transitionTable[i][1] << "\t" << transitionTable[i][2] << endl;
	}

	//Computation of the DFA

	//Compute epsilon transition of the initial state
	string initialState = inputInitialState[0];
	string DFAInitialState = epsilon(initialState);
	outputStates.push_back(DFAInitialState);
	outputInitialState.push_back(DFAInitialState);
	//Using the DFA initial state obtained, find the rest of the states obtained
	cout << DFAInitialState << endl;
	cout<<calculate("q0-q2","b")<<endl;
	size = 1;
	//Write to the output file (temporary printing)
	while(tempCount < size){
		calculateStates(outputStates[tempCount++]);
		cout<<tempCount<<" iteration\n";
		size = outputStates.size();
	}
	string outputStateTable[outputStates.size()][numberOfAlphabets+1];
	/*for(int i = 0; i < size; size++){
		for(int j = 0; j < numberOfAlphabets+1; j++){
			if(j==0){
				outputStateTable[i][j] = outputStates[i];
			} else {
				outputStateTable[i][j] = calculate(outputStates[i], inputDictionary[j-1]);
			}
		}
	}*/

	cout << "State" << "\t";
	for(int j = 0; j < numberOfAlphabets; j++){
		cout << inputDictionary[j] << "\t"; 
	}
	for(int i = 0; i < size; size++){
		for(int j = 0; j < numberOfAlphabets+1; j++){
			cout << outputStateTable[i][j] << "\t";
		}
		cout << endl;
	}

	vector<string>::iterator i;
	/*for(i = outputStates.begin(); i != outputStates.end(); i++){
		cout << *i << endl;
	}*/
	//cout<<calculate("q0","b")<<endl;
	
	inputfile.close();
	outputfile.close();
	return 0;
}


void calculateStates(string state){
	vector<string>::iterator j;

	//int size = outputStates.size();
	for(int i = 0; i < numberOfAlphabets; i++){
		string temp = epsilon(calculate(state, inputDictionary[i]));
		if(find(outputStates.begin(), outputStates.end(), temp) != outputStates.end()){}
		else {
			outputStates.push_back(temp);
			

		}
	}
	/*if(outputStates.size() > size){
		for(j = outputStates.begin(); j != outputStates.end(); j++){
			calculateStates(*j);
		}*/

	
	
}