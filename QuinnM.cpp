/*
Author: Aman Saini
ROLL NO: 2020EEB1155
INDIAN INSTITUTE OF TECHNOLOGY ROPAR
*/

// QUINE MC-CLUSKEY C++ PROGRAM

/*
THIS PROGRAM HAS FOLLOWING FEATURES:

=> PRINTS THE TABLE CONSTRUCTED DURING THE SIMULATION OF QM ALGORITHM (FOR TROUBLE-SHOOTING AND UNDERSTANDING THE PROCEDURE)
=> PRINTS THE ESSENTIAL PRIME IMPLICANTS
=> PRINTS THE NON-ESSENTIAL PRIME IMPLICANTS
=> PRINTS THE FINAL EXPRESSION

THE PROGRAM ACCECPTS INPUTS IN TERMS OF 2 VECTOR AND A NUMBER.
NUMBER DENOTES THE N0. OF TERMS

MINTERM VECTOR STORES THE MINTERMS
DONTCARE VECTOR STORES THE DONT CARE CONDITION


*/
#include<bits/stdc++.h>
#include<iostream>
#include<vector>
#include<cmath>
#include<string.h>

//=============================================================================================
using namespace std;
#define pb push_back
#define int long long int


int pow(int a, int b) { //POWER
	if (!b) return 1; int temp = pow(a, b / 2); temp = (temp * temp);
	if (b % 2) return (a * temp); return temp;
}

//=============================================================================================
vector<int> decompose(int n, int num) {
	// CONVERTS THE NUMBER INTO BINARY.
	vector<int> a(num, 0);
	int idx = num - 1;
	while (n != 0) {
		a[idx] = n % 2;
		n /= 2; idx--;
	} return a;
}

//=============================================================================================
// THE FOLLOWING FUNCTION PRINTS THE TABLE CONSTRUCTER DURING VARIOUS STEPS OF THE PROCESS
void printit(vector<vector<pair<vector<int>, vector<int> > > > &groups) {
	cout << endl;
	for (int i = 0; i < groups.size(); i++) { //PRINTING THE TABLE CONSTRUCTED
		for (int j = 0; j < groups[i].size(); j++) {
			cout.width(9);
			cout << i;
			cout.width(6);
			cout << " ";
			for (int k = 0; k < groups[i][j].first.size(); k++) {
				if (k != (groups[i][j].first.size() - 1)) {
					cout.width(3);
					cout << groups[i][j].first[k] << ",";
				}
				else {
					cout.width(3);
					cout << groups[i][j].first[k];
				}
			} cout.width(15);
			cout << "";
			for (int k = 0; k < groups[i][j].second.size(); k++) {
				if (groups[i][j].second[k] != -1){
			
					cout.width(3);
					cout << groups[i][j].second[k] ;
				}
				else {
					cout.width(3);
					cout << "_";
				}
			} cout << endl;
		}
		if(i != groups.size() -1 )cout << "----------------------------------------------------------------------------------" << endl;
		else cout << endl;
	}
	cout << "==================================================================================" <<  endl;
}

//=============================================================================================
// MAIN PROGRAM (QUINN M SOLVER):
void solve() {
	int num = 4; // number of variables
	vector<int> minterms = {0,2,3,4,9,10,12,13}; // the minterms
	vector<int> dontcare = {6, 8, 14}; // dontcare condition
	int mx = pow(2, num) - 1;
//=============================================================================================
	// FIRST CHECKING FOR OUT OF RANGE MINTERMS/ DONTCARE CONDITIONS
	for (int i = 0; i < minterms.size(); i++) {
		if (minterms[i] > mx or minterms[i] < 0) {
			cout << "Minterm: " << minterms[i] << " is out of range" << endl;
			return;
		}
	}
	for (int i = 0; i < dontcare.size(); i++) {
		if (dontcare[i] > mx or dontcare[i] < 0) {
			cout << "dontcare term: " << dontcare[i] << " is out of range" << endl;
			return;
		}
	}
//=============================================================================================
	// INITIAL CHECKING FOR TRIVIAL SOLUTIONS
	if (minterms.size() == 0) {
		cout << "ESSENTIAL PRIME IMPLICANTS: " << 0 << endl;
		cout << "FINAL EXPRESSION: " << "F(";
		for (int i = 0; i < num; i++) {
			char c = 'A' + i;
			cout << c;
			if (i < num - 1) cout << ',' ;
		} cout << ") = ";
		cout << 0 << endl; return;
	}
	if (minterms.size() + dontcare.size() == pow(2, num)) {
		cout << "ESSENTIAL PRIME IMPLICANTS: " << 1 << endl;
		cout << "FINAL EXPRESSION: " << "F(";
		for (int i = 0; i < num; i++) {
			char c = 'A' + i;
			cout << c;
			if (i < num - 1) cout << ',' ;
		} cout << ") = ";
		cout << 1 << endl; return;
	}
//=============================================================================================
// CONSTRUCTION OF INITIAL TABLES
	sort(minterms.begin(), minterms.end());
	vector<vector<pair<vector<int>, vector<int> > > > groups(num + 1);

	// GROUP VECTOR => INDEX DENOTES THE NUMBER OF 1'S IN BINARY REPRESENTATION
	// EACH TERM IS IN THE FORM OF PAIR OF 2 VECTORS, FIRST VECTOR CONTAINS THE MINTERMS, AND SECOND ONTAINS EQUIVALENT BINARY
	// IN CASE OF MORE THAN ONE MINTERM(2, 4, 8...) WE PLACE A -1 FOR THE BINARY TERM THAT IS DIFFERENT FOR THE 2 BINARY NUMBERS
	for (int i = 0; i < minterms.size(); i++) {
		int grp = 0;
		vector<int> bin = decompose(minterms[i], num);
		for (int i =  0; i < num; i++) {
			if (bin[i] == 1) grp++;
		}
		vector<int> terms;
		terms.pb(minterms[i]);
		groups[grp].pb(make_pair(terms, bin));
	}
	for (int i = 0; i < dontcare.size(); i++) {
		int grp = 0;
		vector<int> bin = decompose(dontcare[i], num);
		for (int i =  0; i < num; i++) {
			if (bin[i] == 1) grp++;
		}
		vector<int> terms;
		terms.pb(dontcare[i]);
		groups[grp].pb(make_pair(terms, bin));
	}

//=============================================================================================

	bool flag = true;
	cout << "TABLE NO. 1" << endl;
	cout.width(9);
	cout << "No. of 1's ";
	cout.width(10);
	cout << "Minterm";
	cout.width(30);
	cout << "Binary Representation";

	printit(groups);
	int tn = 2;
	while (flag) { // CONSTRUCT NEXT TABLE FROM THE CURRENT ONE
		// THIS LOOP WILL RUN AS LONG AS WE DON'T GET AN IRREDUCABLE TABLE

		// EACH LOOP COMBINES 2 MINTERMS THAT DIFFERS IN ONE BIT IN BINARY REPRESENTATION
		flag = false;
		map<vector<int> , bool> mp;
		vector<vector<pair<vector<int>, vector<int> > > > newgrps(num + 1);
		for (int i = 0; i <= num; i++) {
			if (i != num) {
				for (int j = 0; j < groups[i].size(); j++) {
					bool isdone = false;
					for (int k = 0; k < groups[i + 1].size(); k++) {
						int bitdiff = 0;
						vector<int> qq(num, 0), ww;
						for (int l = 0; l < groups[i][j].first.size(); l++) {
							ww.pb(groups[i][j].first[l]);
						}
						for (int l = 0; l < groups[i + 1][k].first.size(); l++) {
							ww.pb(groups[i + 1][k].first[l]);
						}
						for (int l = 0; l < groups[i][j].second.size(); l++) {
							int x = groups[i][j].second[l];
							int y = groups[i + 1][k].second[l];
							if (x != y and (x == -1 or y == -1)) {
								bitdiff += 2; break;
							}
							if (x != y) {
								qq[l] = -1; bitdiff++;
							}
							else qq[l] = x;
						}
						if (bitdiff == 1) {
							mp[groups[i + 1][k].first] = true;
							isdone = true; newgrps[i].pb(make_pair(ww, qq));
							flag = true;
						}
					}
					if (!isdone) {
						if (mp[groups[i][j].first]) {
							continue;
						}
						else {
							newgrps[i].pb(groups[i][j]);
						}
					}
				}
			}
			else {
				for (int j = 0; j < groups[i].size(); j++) {
					if (mp[groups[i][j].first]) {
						continue;
					} else {
						newgrps[i].pb(groups[i][j]);
					}
				}
			}
		}
		
		groups = newgrps;
		if (flag) {
			cout << "TABLE NO. " << tn << endl;
			printit(groups);
			tn++;
		}
	}
//=============================================================================================

	// FINAL GROUPS MADE NOW CONVERT THE GROUPS INTO THE FINAL EXPRESSION
	// FINDING THE ESSENTIAL PRIME IMPLICANTS AND PRIME IMPLICANTS.
	set<int> s;
	set<string> final;
	vector<set<string> > v2(pow(2, num));
	map<string, vector<int> > mp;
	for (int i = 0; i < minterms.size(); i++) {
		s.insert(minterms[i]);
	} for (int i = 0; i <= num; i++) {
		for (int j = 0; j < groups[i].size(); j++) {
			string s;
			int g;
			for (int k = 0; k < num; k++) {
				char c = 'A' + k;
				if (groups[i][j].second[k] == -1) {
					continue;
				} else if (groups[i][j].second[k] == 0) {
					s += c; s += "'";
				} else {
					s += c;
				} g++;
			}
			for (int k = 0; k < groups[i][j].first.size(); k++) {
				v2[groups[i][j].first[k]].insert(s);
			}
			mp[s] = groups[i][j].first;
		}
	} set<string> ess;
	for (int i = 0; i < minterms.size(); i++) {
		if (v2[minterms[i]].size() == 1) {
			string h = *v2[minterms[i]].begin();
			ess.insert(h);
			final.insert(h);
			for (int j = 0; j < mp[h].size(); j++) {
				if (s.find(mp[h][j]) != s.end()) {
					s.erase(s.find(mp[*v2[minterms[i]].begin()][j]));
				}
			} mp.erase(h);
		}
	} cout << endl;
	cout << "ESSENTIAL PRIME IMPLICANTS: " << endl;
	for (set<string>::iterator it = ess.begin(); it != ess.end(); it++) {
		cout << *it << endl;
	} cout << endl;
	cout << "NON ESSENTIAL PRIME IMPLICANTS: " << endl;
	for (map<string, vector<int> >::iterator it = mp.begin(); it != mp.end(); it++) {
		cout << it->first << endl;
	} cout << endl;
	while (s.size() > 0) {
		for (map<string, vector<int> >::iterator it = mp.begin(); it != mp.end(); it++) {
			for (int i = 0; i < it->second.size(); i++) {
				set<int>::iterator itr = s.find(it->second[i]);
				if (itr != s.end()) {
					final.insert(it->first);
					s.erase(itr);
				}
			}
		}
	}	cout << "FINAL EXPRESSION: " << endl;
	cout << "F(";
	for (int i = 0; i < num; i++) {
		char c = 'A' + i;
		cout << c;
		if (i < num - 1) cout << ',' ;
	} cout << ") = ";
	int idx = 0;
	for (set<string>::iterator it = final.begin(); it != final.end(); it++) {
		idx++;
		if (idx < final.size()) cout << *it << " + ";
		else cout << *it << endl;
	}

}


//=============================================================================================

// MAIN FUNCTION THE PROGRAM REQUIRES A OUTPUT.TXT FILE IN THE SAME DIRECTORY AS THE CODE TO PRINT THE RESULTS.
signed main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	freopen("output.txt", "w" , stdout);
	solve();
	return 0;
}

//=============================================================================================