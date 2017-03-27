#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;

int comparitor(string, string, int, int);
int comparitor2(string, string, int);
void subComp2(string, string, int, int, int&);
int comparitor3(string, string, int, int, vector<vector<string>>&, vector<int>&);
int comparitor4(char *, char*, int);
void subComp4(char*, char*, int, int *, int);
int len(char*);


void main()
{
	string test1a, test1b;
	char* test2a;
	char* test2b;

	cout << "Input first character string: ";
	getline(cin, test1a);
	cout << endl << "Input second character string: ";
	getline(cin, test1b);

	test2a = (char*)test1a.c_str();
	test2b = (char*)test1b.c_str();
	//Testing values
	/* 
	string test1a = "821808834226131836647366439330839711511726";
	string test1b = "821808322613184666473664369330683971161766";
	char * test2a = "821808834226131836647366439330839711511726";
	char * test2b = "821808322613184666473664369330683971161766";
	*/
	clock_t startTime, endTime;
	int DEPTH;
	cout << "Enter maximum number of differences to be tallied.\nWARNING: Values above 10 may cause some versions to take a very long time." << endl;
	cin >> DEPTH;

	startTime = clock();
	int diff = comparitor4(test2a, test2b, DEPTH);
	endTime = clock();
	cout << endl << endl << "Method 4 found " << diff << " differences in " << endTime - startTime << " ms." << endl << endl;

	startTime = clock();
	vector<vector<string>> testList;
	vector<int> listDiff;
	diff = comparitor3(test1a, test1b, DEPTH, 0, testList, listDiff);
	endTime = clock();
	cout << "Method 3 found " << diff << " differences in " << endTime - startTime << " ms." << endl << endl;

	startTime = clock();
	diff = comparitor2(test1a, test1b, DEPTH);
	endTime = clock();
	cout << "Method 2 found " << diff << " differences in " << endTime - startTime << " ms." << endl << endl;

	startTime = clock();
	diff = comparitor(test1a, test1b, DEPTH, 0);
	endTime = clock();
	cout << "Method 1 found " << diff << " differences in " << endTime - startTime << " ms." << endl << endl;
}

/*-------------- Comparitor Version 1 --------------*/

int comparitor(string code1, string code2, int lim, int diff)
{
	//Testing early termination conditions
	if (code1 == code2) return 0;
	int temp = code1.length() - code2.length();
	if (abs(temp + diff) >= lim) return abs(temp);
	if (code1.length() == 0) return code2.length();
	if (code2.length() == 0) return code1.length();

	int temp_diff;

	//If the last characters are the same, create recur once, removing the last character from both strings.
	if (code1[code1.length() - 1] == code2[code2.length() - 1])
		temp_diff = comparitor(code1.substr(0, code1.length() - 1), code2.substr(0, code2.length() - 1), lim, diff);
	else
	{
		int ndiff = diff + 1;
		if (ndiff >= lim) return 1;
		//If the last characters differ, recur three times, once for each: removing the last character from the first string, from the second string,
		//and from both strings.
		else
		{
			temp_diff = min(comparitor(code1.substr(0, code1.length() - 1), code2, lim, ndiff) + 1,
				comparitor(code1, code2.substr(0, code2.length() - 1), lim, ndiff) + 1);
			temp_diff = min(comparitor(code1.substr(0, code1.length() - 1), code2.substr(0, code2.length() - 1), lim, ndiff) + 1, temp_diff);
		}
	}

	return temp_diff;
}

/*-------------- Comparitor Version 2 --------------*/

int comparitor2(string code1, string code2, int lim)
{
	int best = lim;
	int& test = best;

	subComp2(code1, code2, lim, 0, test);

	return best;
}

void subComp2(string code1, string code2, int lim, int diff, int& best)
{
	//Testing early termination conditions
	if (code1 == code2)
	{
		best = min(best, diff);
		return;
	}
	int temp = code1.length() - code2.length();
	if (abs(temp + diff) >= lim)
	{
		best = min(best, diff + abs(temp));
		return;
	}
	if (code1.length() == 0)
	{
		temp = code2.length();
		best = min(best, diff + temp);
		return;
	}
	if (code2.length() == 0)
	{
		temp = code1.length();
		best = min(best, diff + temp);
		return;
	}

	//If the last characters are the same, create recur once, removing the last character from both strings.
	if (code1[code1.length() - 1] == code2[code2.length() - 1])
		subComp2(code1.substr(0, code1.length() - 1), code2.substr(0, code2.length() - 1), lim, diff, best);
	else
	{
		int ndiff = diff + 1;
		if (ndiff >= lim || ndiff >= best) return;
		//If the last characters differ, recur three times, once for each: removing the last character from the first string, from the second string,
		//and from both strings.
		else
		{
			subComp2(code1.substr(0, code1.length() - 1), code2, lim, ndiff, best);
			subComp2(code1, code2.substr(0, code2.length() - 1), lim, ndiff, best);
			subComp2(code1.substr(0, code1.length() - 1), code2.substr(0, code2.length() - 1), lim, ndiff, best);
		}
	}

	return;
}

/*-------------- Comparitor Version 3 --------------*/

int comparitor3(string code1, string code2, int lim, int diff, vector<vector<string>>& testList, vector<int>& listDiff)
{
	//Testing early termination conditions
	if (code1 == code2) return 0;
	int temp = code1.length() - code2.length();
	if (abs(temp + diff) >= lim) return abs(temp);
	if (code1.length() == 0) return code2.length();
	if (code2.length() == 0) return code1.length();

	vector<string> runTry;
	runTry.push_back(code1);
	runTry.push_back(code2);
	vector<vector<string>>::iterator it = find(testList.begin(), testList.end(), runTry);
	if (it != testList.end())
	{
		int itd = distance(testList.begin(), it);
		return listDiff.at(itd);
	}
	else
	{
		int temp_diff;
		//If the last characters are the same, create recur once, removing the last character from both strings.
		if (code1[code1.length() - 1] == code2[code2.length() - 1])
			temp_diff = comparitor(code1.substr(0, code1.length() - 1), code2.substr(0, code2.length() - 1), lim, diff);
		else
		{
			int ndiff = diff + 1;
			if (ndiff >= lim) return 1;
			//If the last characters differ, recur three times, once for each: removing the last character from the first string, from the second string,
			//and from both strings.
			else
			{
				temp_diff = min(comparitor(code1.substr(0, code1.length() - 1), code2, lim, ndiff) + 1,
					comparitor(code1, code2.substr(0, code2.length() - 1), lim, ndiff) + 1);
				temp_diff = min(comparitor(code1.substr(0, code1.length() - 1), code2.substr(0, code2.length() - 1), lim, ndiff) + 1, temp_diff);
			}
		}
		testList.push_back(runTry);
		listDiff.push_back(temp_diff);
		return temp_diff;
	}
}

/*-------------- Comparitor Version 4 --------------*/

int comparitor4(char * code1, char * code2, int lim)
{
	//Test immediate termination conditions
	if (code1 == code2) return 0;
	if (abs(len(code1) - len(code2)) >= lim) return lim;


	int * minDif = new int;
	*minDif = lim;

	subComp4(code1, code2, lim, minDif, 0);
	int tmp = *minDif;
	delete minDif;
	return tmp;
}

void subComp4(char * code1, char * code2, int lim, int * minDif, int runDif)
{

	if (code1 == code2 && runDif <= *minDif)
	{
		*minDif = runDif;
		return;
	}
	int lenDif = abs(len(code1) - len(code2));
	if (lenDif + runDif >= *minDif) return;
	if (len(code1) == 0 || len(code2) == 0)
	{
		if (lenDif + runDif < *minDif)
		{
			*minDif = lenDif + runDif;
			return;
		}
		else return;
	}
	if (code1[0] == code2[0])
	{
		subComp4(++code1, ++code2, lim, minDif, runDif);
	}
	else
	{
		subComp4(code1 + 1, code2 + 1, lim, minDif, runDif + 1);
		subComp4(code1, code2 + 1, lim, minDif, runDif + 1);
		subComp4(code1 + 1, code2, lim, minDif, runDif + 1);
	}
}

int len(char * code)
{
	int ans = 0;
	while (code[ans] != '\0') { ans++; }
	return ans;
}