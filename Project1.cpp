/*
   Author: Daniel T. Sweeney
   Class: CSE310 - 83657
   Date: 10/3/2019
   Description: The program includes a method to encode a file into a more compressed format. It also includes the method to reverse the
				compression and rebuild the original file. It has the ability to use insertion-sort or merge-sort when applicable.
*/
#include "Project1.h"



int main(int argc, const char* argv[]) {
	string index, line, str;
	nanoseconds runtime;

	
	if (std::string(argv[0]) == "./encode") {
		if (std::string(argv[1]) == "insertion") {
			while (getline(cin, line)) {
				int strLen = line.length();
				string** arrPtr = new string * [strLen]; //Creates a string array of size 'line.length()'.


				CyclicArray(arrPtr, line);
				auto start = high_resolution_clock::now();
				InsertionSort(arrPtr, strLen);
				auto stop = high_resolution_clock::now();
				OutputEncode(arrPtr, line);
				auto duration = duration_cast<nanoseconds>(stop - start);
				runtime += duration;
			}
			//cout << endl << "Encoding insertion-sort took " << runtime.count() << " nanoseconds." << endl;
		}
		else if (std::string(argv[1]) == "merge") {
			while (getline(cin, line)) {
				int strLen = line.length();
				string** arrPtr = new string * [strLen]; //Creates a string array of size 'line.length()'.


				CyclicArray(arrPtr, line);
				auto start = high_resolution_clock::now();
				MergeSort(arrPtr, 0, strLen - 1);
				auto stop = high_resolution_clock::now();
				OutputEncode(arrPtr, line);
				auto duration = duration_cast<nanoseconds>(stop - start);
				runtime += duration;
			}
			//cout << endl << "Encoding merge-sort took " << runtime.count() << " nanoseconds." << endl;
		}
	}
	else if (std::string(argv[0]) == "./decode") {
		if (std::string(argv[1]) == "insertion") {
			while (getline(cin, index)) {
				getline(cin, line);
				str = DecodeLine(line);
				runtime += RebuildLine(str, "insertion", stoi(index));
			}
			//cout << endl << "Decoding insertion-sort took " << runtime.count() << " nanoseconds." << endl;
		}
		else if (std::string(argv[1]) == "merge") {
			while (getline(cin, index)) {
				getline(cin, line);
				str = DecodeLine(line);
				runtime += RebuildLine(str, "merge", stoi(index));
			}
			//cout << endl << "Decoding merge-sort took " << runtime.count() << " nanoseconds." << endl;
		}
	}
	else if (std::string(argv[0]) == "./analyze") {
		int iSize = 0, iCounter = 0;


		while (getline(cin, line)) { //Gets the size of the input file.
			if (line == "") { iSize--; } 
			else { iSize++; }
		}
		cin.clear(); //Resets the eof status of the input file.
		cin.seekg(0, ios::beg); //Starts the file index @ the beginning of the file.

		int* arrClusters = new int[iSize / 2]; //Creates an array of integers to store the # of clusters per line.
		int* arrChars = new int[iSize / 2]; //Creates an array of integers to store the # of chars per line.

		
		while (getline(cin, line)) { //Puts 'index' into 'line'.
			int numOfClusters = 0;


			getline(cin, line); //Overwrites 'index' as it is not needed within this method.
			for (int i = 0; i < line.length(); i++) { //Records the # of clusters in an input line.
				if (line[i] == ' ') {
					numOfClusters++;
					i += 2;
				}
			}
			str = DecodeLine(line);
			if (str.length() != 0) {
				arrChars[iCounter] = str.length();
				arrClusters[iCounter++] = numOfClusters;
			}
		}
		CompressionData(arrChars, arrClusters, iSize / 2);
	}

	return 0;
}



void CompressionData(int* pArrChars, int* pArrClusters, int pSizeOf) {
	double dMin = 1, dMax = 0, dAvg = 0, dStdDev = 0;
	

	for (int i = 0; i < pSizeOf; i++) {
		double dRatio = ((pArrChars[i] - (double)pArrClusters[i]) / pArrChars[i]);
			
		dAvg += dRatio;
		if (dRatio < dMin) { dMin = dRatio; }
		if (dRatio > dMax) { dMax = dRatio; }
	}
	dAvg = dAvg / pSizeOf;

	for (int i = 0; i < pSizeOf; i++) {
		double dRatio = ((pArrChars[i] - (double)pArrClusters[i]) / pArrChars[i]);
			
		
		dStdDev += pow((dRatio - dAvg), 2);
	}
	dStdDev = sqrt(dStdDev / pSizeOf);

	cout << "Average: " << (dAvg) * 100 << "%" << endl << "Standard Deviation: " << dStdDev * 100 << "%" << endl;
	cout << "Min: " << dMin * 100 << "%" << endl << "Max: " << dMax * 100 << "%" << endl;
}



//Fills the parameter array with pointer values that point to the generated cyclic values.
void CyclicArray(string** pArr, string pStr) {
	int strLen = pStr.length();
	string* arrStr = new string[strLen];	//Creates a string array of size 'strLen'.

	
	for (int i = 0; i < strLen; i++) {
		arrStr[i] = pStr;
		pArr[i] = &arrStr[i];
		pStr = pStr.substr(1, strLen - 1) + pStr.substr(0, 1);
	}
}



//Decodes pLine and creates a string of letters.
string DecodeLine(string pLine) {
	string str, letter, numOf;
	bool restart = false;


	for (int i = 0; i < pLine.length(); i++) {
		if (pLine[i] == ' ') {
			letter = pLine[i + 1];
			i += 2;
			restart = true;
		}
		else { numOf += pLine[i]; }

		if (restart == true) {
			for (int i = 0; i < stoi(numOf); i++) { str.append(letter); }
			numOf = "";
			letter = "";
			restart = false;
		}
	}

	return str;
}



//Insertion sort method. 
void InsertionSort(string* pArr, int pLen) {
	for (int i = 1; i < pLen; i++) {
		string key = pArr[i];
		int j = i - 1;


		while (j >= 0 && pArr[j] > key) {
			pArr[j + 1] = pArr[j];
			j--;
		}

		pArr[j + 1] = key;
	}
}



//Insertion sort method. 
void InsertionSort(string** pArr, int pLen) {
	for (int i = 1; i < pLen; i++) {
		string* key = pArr[i];
		int j = i - 1;

		
		while (j >= 0 && *pArr[j] > *key) {
			pArr[j + 1] = pArr[j];
			j--;
		}

		pArr[j + 1] = key;
	}
}



//The merge portion of merge sort method.
void Merge(string* pArr, int l, int m, int r) {
	int i = l, j = m + 1, k = 0;
	string* arrTemp = new string [r - l + 1];
	

	while (i <= m && j <= r) {
		if (pArr[i] < pArr[j]) { arrTemp[k++] = pArr[i++]; }
		else { arrTemp[k++] = pArr[j++]; }
	}

	while (i <= m) { arrTemp[k++] = pArr[i++]; }
	while (j <= r) { arrTemp[k++] = pArr[j++]; }
	for (int i = r; i >= l; i--) { pArr[i] = arrTemp[--k]; }
}



//The merge portion of merge sort method.
void Merge(string** pArr, int l, int m, int r) {
	int i = l, j = m + 1, k = 0;
	string** arrTemp = new string * [r - l + 1];


	while (i <= m && j <= r) {
		if (*pArr[i] < *pArr[j]) { arrTemp[k++] = pArr[i++]; }
		else { arrTemp[k++] = pArr[j++]; }
	}

	while (i <= m) { arrTemp[k++] = pArr[i++]; }
	while (j <= r) { arrTemp[k++] = pArr[j++]; }
	for (int i = r; i >= l; i--) { pArr[i] = arrTemp[--k]; }
}



//Merge sort method.
void MergeSort(string* pArr, int l, int r) {
	if (l < r) {
		int m = floor((l + r) / 2);


		MergeSort(pArr, l, m);
		MergeSort(pArr, m + 1, r);
		Merge(pArr, l, m, r);
	}
}



//Merge sort method.
void MergeSort(string** pArr, int l, int r) {
	if (l < r) {
		int m = floor((l + r) / 2);


		MergeSort(pArr, l, m);
		MergeSort(pArr, m + 1, r);
		Merge(pArr, l, m, r);
	}
}



//Outputs the # of clustered 'char's as well as the 'char' itself as well as the index.
void OutputEncode(string** pArr, string pStr) {
	int strLen = pStr.length(), index = 0, counter;
	string charStr, lastChar;


	for (int i = 0; i < strLen; i++) {
		if (*pArr[i] == pStr) { index = i; } //Records the index where the pArr element matches pStr.

		if (i == 0) { //If its the first element in the array then do the following.
			lastChar = (*pArr[i]).substr(strLen - 1, 1); //Sets 'lastChar' to the last char in the string.
			counter = 1; //Sets counter equal to 1.
		}
		else if ((*pArr[i]).substr(strLen - 1, 1) != lastChar) { //If the last char is different than 'lastChar', do the following. 
			charStr.append(to_string(counter) + " " + lastChar + " "); //Adds 'counter' space 'lastChar' space to 'charStr'.
			lastChar = (*pArr[i]).substr(strLen - 1, 1); //Sets 'lastChar' to the last char in the string.
			counter = 1; //Resets the 'counter' to 1.
		}
		else { counter++; } //Increases the 'counter' if the last char in the string matches 'lastChar'.
	}

	if (strLen > 0) { charStr.append(to_string(counter) + " " + lastChar); } //Adds the final 'counter' space 'lastChar' to 'charStr'.
	cout << index << endl << charStr; //Outputs the index of the 'line' in the array and then a new line with charStr.
	if (!cin.eof()) { cout << endl; } //Outputs an 'endl' if there are more lines in the input file.
}



//Uses a series of arrays to rebuild the original lines that were encoded and then outputs the 'str'.
nanoseconds RebuildLine(string pStr, string pMethod, int pIndex) {
	int strLen = pStr.length(), index = pIndex, counter = 0, k = 0;
	int* arrNext = new int[strLen];
	string str = "";
	string* arrFirstChar = new string[strLen];
	string* arrLastChar = new string[strLen];
	nanoseconds runtime;


	//Fills arrFirstChar, arrLastChar, and arrNext with initial values.
	for (int i = 0; i < strLen; i++) {
		arrFirstChar[i] = pStr[i];
		arrLastChar[i] = pStr[i];
		arrNext[i] = 0;
	}

	if (pMethod == "insertion") { //Uses insertion sort to sort the elements of 'arrFirstChar'.
		auto start = high_resolution_clock::now();
		InsertionSort(arrFirstChar, strLen);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<nanoseconds>(stop - start);
		runtime = duration;
	} 
	else if (pMethod == "merge") { //Uses merge sort to sort the elements of 'arrFirstChar'.
		auto start = high_resolution_clock::now();
		MergeSort(arrFirstChar, 0, strLen - 1);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<nanoseconds>(stop - start);
		runtime = duration;
	}

	for (int i = 0; i < strLen; i++) {
		if (i > 0 && arrFirstChar[i] == arrFirstChar[i - 1]) { k = ++counter; } //If the current element matches the prior, then add 1 to counter.
		else if (i > 0 && arrFirstChar[i] != arrFirstChar[i - 1]) { counter = 0; } //If the current element does not match the prior, then reset counter to 0.

		for (int j = 0; j < strLen; j++) { //Goes through 'arrLastChar' looking for a match to the current element in 'arrFirstChar'.
			if (arrFirstChar[i] == arrLastChar[j] && k == 0) { //If the elements @ i & j match and 'k' is = to 0 then proceed. 
				arrNext[i] = j; //Sets the current element's 'arrNext' value to 'j'.
				j += strLen; //Exits the for loop of 'j'.
			}
			//Decramants 'k' if 'arrFirstChar' matches 'arrLastChar' which ultimatelly skips the current matching case.
			else if (arrFirstChar[i] == arrLastChar[j] && k != 0) { k--; }
		}
	}
	
	if (strLen > 0) {
		do {
			str.append(arrFirstChar[index]); //Adds characters to 'str' to reconstruct the original line.
			index = arrNext[index]; //Sets 'index' to the next index in 'arrNext'.
		} while (index != pIndex); //Continue to cycle through 'arrFirstChar' until the 'arrNext' element matches 'pIndex'. 
	}

	cout << str; //Outputs the original line.
	if (!cin.eof()) { cout << endl; } //Outputs an 'endl' if there are more lines in the input file.
	return runtime;
}