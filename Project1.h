#include <string>
#include <iostream>
#include <math.h>
#include <chrono>
using namespace std;
using namespace std::chrono;



void CompressionData(int* pArrChars, int* pArrClusters, int pSizeOf);
void CyclicArray(string** pArr, string pStr);
string DecodeLine(string pLine);
void InsertionSort(string* pArr, int pLen);
void InsertionSort(string** pArr, int pLen);
void Merge(string* pArr, int l, int m, int r);
void Merge(string** pArr, int l, int m, int r);
void MergeSort(string* pArr, int l, int r);
void MergeSort(string** pArr, int l, int r);
void OutputEncode(string** pArr, string pStr);
nanoseconds RebuildLine(string pStr, string pMethod, int pIndex);