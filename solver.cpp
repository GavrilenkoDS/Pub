#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

enum Cat {
	satisfied,  
	unsatisfied, 	 
	normal,
	completed 
};

class Formula {
public:

	vector<int> literals; //-1 - unassigned 1 - true 0 - false
	vector<int> literal_polarity; //if + 2n else if - 2n-1
	vector<vector<int>> clauses;
	Formula() {}
	Formula(const Formula &f) {
		literals = f.literals;
		clauses = f.clauses;
		literal_polarity = f.literal_polarity;
	}
};

class SATSolverDPLL {
private:
	Formula formula;               
	int literal_count;            
	int clause_count;              
	int unit_propagate(Formula &); 
	int DPLL(Formula);            
	int apply_transform(Formula &,int);
	void clear_literal(Formula &);
	void show_result(Formula &, int); 
public:
	SATSolverDPLL() {}
	void initialize(); 
	void solve();     
};

void SATSolverDPLL::initialize() {

	//skip part
	char c;
	string s;
	cin >> c;
	cin >> c;
	cin >> s;

	//important things
	cin >> literal_count;
	cin >> clause_count;

	//resize
	formula.literals.clear();
	formula.literals.resize(literal_count + 1, -1);
	formula.clauses.clear();
	formula.clauses.resize(clause_count);
	formula.literal_polarity.clear();
	formula.literal_polarity.resize(2 * literal_count + 1, 0);

	int literal;
	for (int i = 0; i < clause_count; i++) {
		while (true) {
			cin >> literal;
			if (literal != 0) 
				formula.clauses[i].push_back(literal);
			if (literal > 0)
				formula.literal_polarity[2*abs(literal)]++;
			else if (literal < 0 )
				formula.literal_polarity[2 * abs(literal)-1]++;
			if (literal==0)
				break; //next clause
		}
	}
}

void SATSolverDPLL::clear_literal(Formula &f) {
	for (int i = 1; i < f.literal_polarity.size(); i++) {
		if (f.literal_polarity[i] == 0) {
			f.literals[(i + 1) / 2] = i % 2 ? 1 : 0;
			apply_transform(f, (i + 1) / 2);
		}
	}
}

int SATSolverDPLL::unit_propagate(Formula &f) {
	bool unit_clause_found = false; 
	if (f.clauses.size() == 0) 
	{
		return Cat::satisfied; 
	}

	do {
		unit_clause_found = false;
		for (int i = 0; i < f.clauses.size(); i++) {
			if (f.clauses[i].size() == 1) 
			{
				unit_clause_found = true;
				int lit_num = f.clauses[i][0] > 0 ? f.clauses[i][0] : f.clauses[i][0] * (-1);
				f.literals[lit_num] = f.clauses[i][0]>0?1:0 ; 

				int result = apply_transform(f, lit_num); 

				if (result == Cat::satisfied || result == Cat::unsatisfied) {
					return result;
				}
				break; // exit the loop 
			}
			else if (f.clauses[i].size() == 0) 
			{
				return Cat::unsatisfied; 
			}
		}
	} while (unit_clause_found = false);

	return Cat::normal; 
}

int SATSolverDPLL::apply_transform(Formula &f, int literal_to_apply) {
	int value_to_apply = f.literals[literal_to_apply]; 
													 
	for (int i = 0; i < f.clauses.size(); i++) {
		for (int j = 0; j < f.clauses[i].size(); j++) {
			
			if (literal_to_apply == f.clauses[i][j] || literal_to_apply == (-1)*f.clauses[i][j]) {
				if (f.literals[literal_to_apply] > 0 && f.clauses[i][j] > 0 || f.literals[literal_to_apply] == 0 && f.clauses[i][j] < 0) {
					f.clauses.erase(f.clauses.begin() + i); // remove the clause from the list
					i--;// reset iterator
					if (f.clauses.size() == 0) // if all clauses have been removed, the formula is satisfied
					{
						return Cat::satisfied;
					}
					break;
				}
				else {
					f.clauses[i].erase(f.clauses[i].begin() + j); // remove the literal from the clause, as it is false in it
					j--;   
					if (f.clauses[i].size() == 0) // if the clause is empty, the formula is unsatisfiable currently
					{
						return Cat::unsatisfied;
					}
					break;
				}
			}
		}
	}
	return Cat::normal;
}


int SATSolverDPLL::DPLL(Formula f) {
	int result = unit_propagate(f); 
	if (result == Cat::satisfied) 
	{
		show_result(f, result);
		return Cat::completed;
	}
	else if (result == Cat::unsatisfied) 				
	{
		return Cat::normal;
	}
	
	for (int j = 0; j < 2; j++) {
		Formula new_f = f; // copy 
		
		int literal_to_change = 1;
		for (int k = 1; k < f.literals.size(); k++)
			if (f.literals[k] == -1) {
				literal_to_change = k;
				break;
			}
		
		new_f.literals[literal_to_change] = j;

		int transform_result = apply_transform(new_f, literal_to_change); 
		if (transform_result == Cat::satisfied) 
		{
			show_result(new_f, transform_result);
			return Cat::completed;
		} else if (transform_result == Cat::unsatisfied) continue;

		int dpll_result = DPLL(new_f); 
		if (dpll_result == Cat::completed) 
		{
			return dpll_result;
		}
	}
	return Cat::normal;
}

void SATSolverDPLL::show_result(Formula &f, int result) {
	if (result == Cat::satisfied) 
	{
		cout << "SAT" << endl;
		for (int i = 1; i < f.literals.size(); i++) {
			cout << i*(f.literals[i]>0?1:(-1));
			cout << " ";
		}
		cout << "0";
	}
	else 
	{
		cout << "UNSAT";
	}
}

void SATSolverDPLL::solve() {
	clear_literal(formula);
	int result = DPLL(formula); 
	if (result == Cat::normal) {
		show_result(formula, Cat::unsatisfied); 
	}
}

int main() {
	SATSolverDPLL solver;
	solver.initialize(); 
	solver.solve();       
	return 0;
}

