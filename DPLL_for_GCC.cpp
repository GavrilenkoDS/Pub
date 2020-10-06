


#include <iostream>
#include <vector>
#include <string>

using namespace std;

class var {
public:
	int name;
	int neg;
	int value;
};




void replace_all2(vector <vector<var> > & cnf, var lit, int val) {
	for (int i = 0; i < cnf.size(); i++) {
		for (int j = 0; j < cnf[i].size(); j++) {
			if (cnf[i][j].name == lit.name) {
				cnf[i][j].value = val;
			}
		}
	}
}

int check_clos(vector <var> clos) {
	for (int i = 0; i < clos.size(); i++) {
		if (clos[i].value == clos[i].neg)
			return 1;
		if (clos[i].value == -1)
			return -1;
	}

	return 0;
}

int check_cnf(vector <vector<var> > cnf) {
	for (int i = 0; i < cnf.size(); i++) {
		if (check_clos(cnf[i]) == 0)
			return 0;
		if (check_clos(cnf[i]) == -1)
			return -1;
	}
	return 1;
}

var choose_lit(vector <vector<var> > cnf) {

	for (int i = 0; i < cnf.size(); i++) {
		for (int j = 0; j < cnf[i].size(); j++) {
			if (cnf[i][j].value == -1)
				return cnf[i][j];
		}
	}
}

int pure_literal(vector <vector<var> > cnf, var lit) {
	for (int i = 0; i < cnf.size(); i++) {
		for (int j = 0; j < cnf[i].size(); j++) {
			if (cnf[i][j].name == lit.name && cnf[i][j].neg != lit.neg)
				return 0;
		}
	}
	return 1;
}

void pure_literal_check(vector <vector<var> >& cnf) {
	for (int i = 0; i < cnf.size(); i++) {
		for (int j = 0; j < cnf[i].size(); j++) {
			if (pure_literal(cnf, cnf[i][j]) == 1) {
				replace_all2(cnf, cnf[i][j], cnf[i][j].neg);
			}

		}
	}
}

void unit_clos_check(vector <vector<var> >& cnf) {
	for (int i = 0; i < cnf.size(); i++) {
		int uno = 0;
		var lit;
		for (int j = 0; j < cnf[i].size(); j++) {
			if (cnf[i][j].value == -1) {
				uno++;
				lit = cnf[i][j];
			}
			if (cnf[i][j].value == 1) {
				uno = -1;
				break;
			}
		}
		if (uno == 1)
			replace_all2(cnf, lit, lit.neg);
	}
}

bool dpll(vector <vector<var> >& cnf) {
	if (check_cnf(cnf) == 1)
		return 1;
	if (check_cnf(cnf) == 0)
		return 0;


	pure_literal_check(cnf);

	unit_clos_check(cnf);

	vector <vector<var> > cnf2;
	cnf2 = cnf;
	replace_all2(cnf2, choose_lit(cnf), 1);
	if (dpll(cnf2) == 1) {
		cnf = cnf2;
		return 1;
	}

	vector <vector<var> > cnf3;
	cnf3 = cnf;
	replace_all2(cnf3, choose_lit(cnf), 0);
	if (dpll(cnf3) == 1) {
		cnf = cnf3;
		return 1;
	}
	else return 0;
}

int main() {

	int k;
	int clos_num;
	cout << "write num of clause ";
	cin >> clos_num;
	vector <vector <var> > cnf(clos_num);

	for (int i = 0; i < clos_num; i++) {
		cout << "write var num in clause ";
		cin >> k;
		for (int j = 0; j < k; j++) {
			var lit;
			cin >> lit.name;
			lit.value = -1;
			lit.neg = lit.name < 0 ? 0 : 1;
			lit.name = lit.name < 0 ? lit.name*(-1) : lit.name;
			cnf[i].push_back(lit);
		}
	}

	if (dpll(cnf) == 1) {
		dpll(cnf);
		for (int i = 0; i < clos_num; i++) {
			for (int j = 0; j < cnf[i].size(); j++) {
				if (cnf[i][j].neg == 1)
					cout << cnf[i][j].value;
				if (cnf[i][j].neg == 0)
					cout << 1 - cnf[i][j].value;
				if (j < cnf[i].size() - 1)
					cout << "&";
			}
			if (i < clos_num - 1)
				cout << "  ||  ";
		}
	}
	else
		cout << "cant do it";

	return 0;
}


