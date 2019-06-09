// Name     : dag.h
// Author   : Geonmo Gu
// Version  : 1.000
// Copyright: Apache License 2.0

#ifndef DAG_H
#define DAG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>

void readDataGraph(char *filename);
void readQueryGraph(FILE *fp, int aSumDegree);
int selectRoot();
void buildDAG();
// void printDAG();
void clearMemory();
int sortByDegreeData(int aNode1, int aNode2);
int sortByLabel(int aNode1, int aNode2);
int sortByDegreeQuery(int aNode1, int aNode2);
int sortByLabelFreqQuery(int aNode1, int aNode2);
int binaryLowerBound(int aLeft, int aRight, int aDegree);


//FOR STABLE SORT : implemented MERGE SORT
void merge_sort(int *a, int begin, int end, int (*f)(int, int));
void merge_array(int *a, int begin, int mid, int end, int* temp, int (*f)(int, int));

extern int numQueryNode;

#endif
