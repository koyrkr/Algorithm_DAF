#include "dag.h"
#include "intset.h"

//Variables for data graph
int numDataNode = 0;
int numLabel = 0;
int* labelData = NULL; //labelData[i] contains label of vertex i
int* degreeData = NULL; //degreeData[i] contains degree of vertex i
int* sortedData = NULL; //sortedData contains V in sorted order first by label frequency and second by degree
int* idxSortedData = NULL; //idxSortedData[l] contains the last index in sortedData such that labelData[sortedData[index]] is l
int* labelFrequency = NULL; //labelFrequency[l] contains the number of vertices having label l
int* renamedLabel = NULL; //labels are renamed as consecutive numbers

//Variables for query graph
int root = -1;
int numQueryNode = 0;
int sumQueryDegree = 0;
int* labelQuery = NULL;
int* degreeQuery = NULL;
int* adjListQuery = NULL;
int* adjIndexQuery = NULL;

//Variables for query DAG
int** dagChildQuery = NULL; //dagChildQuery[i]: children of node i
int** dagParentQuery = NULL; //dagParentQuery[i]: parent of node i
int* dagChildQuerySize = NULL; //dagChildQuerySize[i]: the number of children of node i
int* dagParentQuerySize = NULL; //dagParentQuerySize[i]: the number of parent on node i

#define LINE_SIZE 1000

void buildDAG(){
  //////////////////
  //allocate memory for dag data structure
  // printf("Build DAG\n");
	if (dagChildQuery == NULL){
		//dagChildQuery = new int*[numQueryNode];
		dagChildQuery = (int **)malloc(numQueryNode * sizeof(int *));
    // free(dagChildQuery);
		for (int i = 0; i < numQueryNode; ++i){
      // printf("%d\n", i);
			dagChildQuery[i] = NULL;
		}
	}

	if (dagParentQuery == NULL){
		//dagParentQuery = new int*[numQueryNode];
		dagParentQuery = (int **) malloc(numQueryNode * sizeof(int *));
    for (int i = 0; i < numQueryNode; ++i){
      dagParentQuery[i] = NULL;
    }
	}

	if (dagChildQuerySize == NULL){
		dagChildQuerySize = (int *) malloc(numQueryNode * sizeof(int));
	}

  if (dagParentQuerySize == NULL){
    dagParentQuerySize = (int *) malloc(numQueryNode * sizeof(int));
  }

	memset(dagChildQuerySize, 0, sizeof(int) * numQueryNode);
	memset(dagParentQuerySize, 0, sizeof(int) * numQueryNode);

	for (int i = 0; i < numQueryNode; ++i){
    // printf("%d\n", i);
		if (dagChildQuery[i] != NULL){
			//delete[] dagChildQuery[i];
			free(dagChildQuery[i]);
			dagChildQuery[i] = NULL;
		}

		// dagChildQuery[i] = new int[degreeQuery[i]];
		dagChildQuery[i] = (int *) malloc(degreeQuery[i] * sizeof(int));

    if (dagParentQuery[i] != NULL){
      //delete[] dagParentQuery[i];
      free(dagParentQuery[i]);
      dagParentQuery[i] = NULL;
    }

    // dagParentQuery[i] = new int[degreeQuery[i]];
		dagParentQuery[i] = (int *) malloc(degreeQuery[i] * sizeof(int));

	}

  // free(dagChildQuery);
  // free(dagParentQuery);
//////////////////
//construct dag data structure
    //char* popped = new char[numQueryNode];
    char* popped = (char *)malloc(numQueryNode * sizeof(char));
    memset(popped, 0, sizeof(char) * numQueryNode);
    //char* visited = new char[numQueryNode];
    char* visited = (char *)malloc(numQueryNode * sizeof(char));
    memset(visited, 0, sizeof(char) * numQueryNode);
    //int* queue = new int[numQueryNode];
    int* queue = (int *) malloc(numQueryNode * sizeof(int));
    int currQueueStart = 0;
    int currQueueEnd = 1;
    int nextQueueStart = 1;
    int nextQueueEnd = 1;

    //visit root
    root = selectRoot();
    // printf("root: %d\n", root);
    visited[ root ] = 1;
    queue[0] = root;

    //BFS traversal using queue
    while (1) {
        int * temp = (int *) malloc((currQueueEnd - currQueueStart) * sizeof(int));
        //stable_sort(queue + currQueueStart, queue + currQueueEnd, sortByDegreeQuery);
        //stable_sort(queue + currQueueStart, queue + currQueueEnd, sortByLabelFreqQuery);

        merge_sort(queue, currQueueStart, currQueueEnd - 1, sortByDegreeQuery);
        free(temp);
        temp = (int *) malloc((currQueueEnd - currQueueStart) * sizeof(int));
        merge_sort(queue, currQueueStart, currQueueEnd - 1, sortByLabelFreqQuery);
        // printf("sort well\n");

        free(temp);

        while( currQueueStart != currQueueEnd ) {
            int currNode = queue[ currQueueStart ];
            ++currQueueStart;
            popped[currNode] = 1;
	          printf("%d ", currNode);
            //cout << currNode << " ";

            for (int i = adjIndexQuery[currNode]; i < adjIndexQuery[currNode + 1]; ++i) {
                int childNode = adjListQuery[i];
                if(popped[childNode] == 0) {
                    dagChildQuery[currNode][ dagChildQuerySize[currNode] ] = childNode;
                    dagParentQuery[childNode][ dagParentQuerySize[childNode] ] = currNode;

                    ++dagChildQuerySize[currNode];
                    ++dagParentQuerySize[childNode];
                }
                if(visited[childNode] == 0) {
                    visited[childNode] = 1;
                    queue[nextQueueEnd] = childNode;
                    ++nextQueueEnd;
                }
            }
        }

        if(currQueueEnd == nextQueueEnd) //no nodes have been pushed in
            break;

        currQueueStart = currQueueEnd;
        currQueueEnd = nextQueueEnd;
    }
    // printf("until here?\n");
    //cout << endl;
    printf("\n");
    //delete[] popped;
    //delete[] visited;
    //delete[] queue;
    free(popped);
    free(visited);
    free(queue);

    // printf("End Build DAG\n");
}

void readDataGraph(char *aFileName){
/////////////////
//1st read: set degreeData, and calculate the largest label and the number of labels
    // printf("Read Data Graph\n");
    FILE *fp = fopen(aFileName, "r");

    int largestLabel = -1;
    //set<int> labelSet;
    intset labelSet;
    intset_init(&labelSet);

    char line[LINE_SIZE];

    while( fgets(line, LINE_SIZE, (FILE *) fp) ) {
        // printf("%s", line);
        if( line[0] == 't' ) {
            char *tag;
            char *id_char;
            char *numDataNode_char;
            int id;

            tag = strtok(line, " ");
            id_char = strtok(NULL, " ");
            numDataNode_char = strtok(NULL, " ");

            id = atoi(id_char);
            numDataNode = atoi(numDataNode_char);

            // printf("%s %d %d\n", tag, id, numDataNode);

            if( labelData != NULL) {
                //delete[] labelData;
                free(labelData);
                labelData = NULL;
            }

            if( degreeData != NULL ) {
                //delete[] degreeData;
                free(degreeData);
                degreeData = NULL;
            }
            //labelData = new int[numDataNode];
            labelData = (int *) malloc(numDataNode * sizeof(int));
            //degreeData = new int[numDataNode];
            degreeData = (int *) malloc(numDataNode * sizeof(int));
            memset(degreeData, 0, sizeof(int) * (numDataNode));

        }

        else if( line[0] == 'v' ) {
            char* tag;
            char* id_char;
            char* label_char;
            int id;
            int label;

            tag = strtok(line, " ");
            id_char = strtok(NULL, " ");
            label_char = strtok(NULL, " ");

            id = atoi(id_char);
            label = atoi(label_char);

            // printf("%s %d %d\n", tag, id, label);

            if( labelData == NULL || numDataNode < id ) {
                //cout << "ERROR: in readDataGraph, vertex id out of range" << endl;
                printf("ERROR: in readDataGraph, vertex id out of range\n");
                exit(-1);
            }
            //labelData[id] = label;
            if( largestLabel < label )
                largestLabel = label;
            //labelSet.insert(label);//
	          intset_insert(&labelSet, label);
        }

        else if( line[0] == 'e' ) {
            //istringstream iss(line);//
            char *tag;
            char *left_char;
            char *right_char;
            char *label_char;
            int left;
            int right;
            int label;
            //iss >> tag >> left >> right >> label;//

            tag = strtok(line, " ");
            left_char = strtok(NULL, " ");
            right_char = strtok(NULL, " ");
            label_char = strtok(NULL, " ");

            left = atoi(left_char);
            right = atoi(right_char);
            label = atoi(label_char);

            // printf("%s %d %d %d\n", tag, left, right, label);

            if( degreeData == NULL || numDataNode < left || numDataNode < right ) {
                //cout << "ERROR: in readDataGraph, vertex id out of range" << endl;
                printf("ERROR: in readDataGraph, vertex id out of range\n");
		            exit(-1);
            }

            //Make sure not to increase two times
            ++degreeData[left];
            ++degreeData[right];
        }

    }

    //numLabel = labelSet.size();//
    numLabel = intset_size(&labelSet);
    //printf("numLabel: %d\n", numLabel);

    intset_destroy(&labelSet);

//////////////////
//2nd read: rename label, set labelData, and set labelFrequency
    // printf("2nd reading\n");
    int labelId = 0;

    if( renamedLabel != NULL ) {
        //delete[] renamedLabel;
        free(renamedLabel);
        renamedLabel = NULL;
    }
    //renamedLabel = new int[largestLabel + 1];
    renamedLabel = (int *) malloc((largestLabel + 1) * sizeof(int));
    memset(renamedLabel, -1, sizeof(int) * (largestLabel + 1) );

    if( labelFrequency != NULL ) {
        //delete[] labelFrequency;
        free(labelFrequency);
        labelFrequency = NULL;
    }
    //labelFrequency = new int[numLabel];
    labelFrequency = (int *) malloc(numLabel * sizeof(int));
    memset(labelFrequency, 0, sizeof(int) * (numLabel));


    //inFile.clear();//
    //inFile.seekg(0, ios::beg);//
    fseek(fp, 0L, SEEK_SET);

    while( fgets(line, LINE_SIZE, fp) ) {
        // printf("%s", line);
        if( line[0] == 't' ) {}

        else if( line[0] == 'v' ) {
            //istringstream iss(line);//
            char *tag;
            char *id_char;
            char *label_char;
            int id;
            int label;
            //iss >> tag >> id >> label;//

            tag = strtok(line, " ");
            id_char = strtok(NULL, " ");
            label_char = strtok(NULL, " ");

            id = atoi(id_char);
            label = atoi(label_char);

            // printf("2: %s %d %d\n", tag, id, label);

            if( renamedLabel[label] == -1 ) {
                renamedLabel[label] = labelId;
                ++labelId;
            }

            labelData[id] = renamedLabel[label];

            ++labelFrequency[ renamedLabel[label] ];
        }

        else if( line[0] == 'e' ) {}

    }

    //inFile.close();
    fclose(fp);

//////////////////
//sort data vertices by label name.
//Then, sort by degree for each label group
    // printf("sorting\n");


    if (sortedData != NULL ) {
        //delete[] sortedData;
        free(sortedData);
        sortedData = NULL;
    }

    //sortedData = new int[numDataNode];
    sortedData = (int *) malloc(numDataNode * sizeof(int));

    for(int i = 0; i < numDataNode; ++i)
        sortedData[i] = i;

    // int *temp = (int *) malloc((numDataNode) * sizeof(int));
    //stable_sort(sortedData, sortedData + numDataNode, sortByDegreeData);
    //stable_sort(sortedData, sortedData + numDataNode, sortByLabel);
		// printf("bf: %d\n", sortedData[0]);
    merge_sort(sortedData, 0, numDataNode - 1, sortByDegreeData);
		// mergesort(sortedData, sizeof(sortedData)/sizeof(*sortedData), sizeof(*sortedData), sortByDegreeData);
    // free(temp);
    // temp = (int *) malloc((numDataNode) * sizeof(int));
		// printf("af: %d\n", sortedData[0]);
    merge_sort(sortedData, 0, numDataNode - 1, sortByLabel);

    // free(temp);



    if( idxSortedData != NULL ) {
        //delete[] idxSortedData;
        free(idxSortedData);
        idxSortedData = NULL;
    }

    //idxSortedData = new int[numLabel + 1];
    idxSortedData = (int *) malloc((numLabel + 1) * sizeof(int));

    if( numDataNode < 1 ) {
        //cout << "ERROR: in readDataGraph, 0 vertices" << endl;
        printf("ERROR: in readDataGraph, 0 vertices\n");
        exit(-1);
    }

    idxSortedData[ labelData[sortedData[0]] ] = 0;

    for(int i = 1; i < numDataNode; ++i) {
        if( labelData[sortedData[i - 1]] != labelData[sortedData[i]] )
            idxSortedData[ labelData[sortedData[i]] ] = i;
    }

    idxSortedData[ numLabel ] = numDataNode;

    // printf("Finish Reading Data Graph\n");

}

int sortByDegreeData(int aNode1, int aNode2){

	return (degreeData[aNode1] <= degreeData[aNode2]);
}

int sortByLabel(int aNode1, int aNode2){
	return (labelData[aNode1] <= labelData[aNode2]);
}

int sortByDegreeQuery(int aNode1, int aNode2){
	return (degreeQuery[aNode1] < degreeQuery[aNode2]);
}

int sortByLabelFreqQuery(int aNode1, int aNode2){

	int label1 = labelQuery[aNode1];
	int label2 = labelQuery[aNode2];

	return (labelFrequency[label1] < labelFrequency[label2]);
}

void readQueryGraph(FILE* file, int aSumDegree){
  // printf("Read Query Graph\n");
    //allocate memory
	if( labelQuery == NULL )
		//labelQuery = new int[numQueryNode];
    labelQuery = (int *) malloc(numQueryNode * sizeof(int));

	if( degreeQuery == NULL )
        	//degreeQuery = new int[numQueryNode];
          degreeQuery = (int *) malloc(numQueryNode * sizeof(int));
          if( adjIndexQuery == NULL )
        	//adjIndexQuery = new int[numQueryNode + 1];
          adjIndexQuery = (int *) malloc((numQueryNode+1) * sizeof(int));

          if( adjListQuery == NULL ) {
        	//adjListQuery = new int[aSumDegree];
          adjListQuery = (int *) malloc(aSumDegree * sizeof(int));
        	sumQueryDegree = aSumDegree;
    	}

    //(re)allocate memory for adjacency list of query graph
    if( sumQueryDegree < aSumDegree ) {
      if( adjListQuery != NULL ) {
        //delete[] adjListQuery;
        free(adjListQuery);
        adjListQuery = NULL;
      }

      //adjListQuery = new int[aSumDegree];
      adjListQuery = (int *) malloc(aSumDegree* sizeof(int));
      sumQueryDegree = aSumDegree;
    }
//////////////////
    //read query graph
    	int index = 0;
    	adjIndexQuery[0] = index;

      // printf("nqn: %d\n", numQueryNode);
    	for (int i = 0; i < numQueryNode; ++i) {
        	int id;
        	int label;
        	int degree;
        	//aInFile >> id >> label >> degree;
          fscanf(file, "%d %d %d\n", &id, &label, &degree);
          // printf("%d %d %d\n", id, label, degree);
        	labelQuery[i] = renamedLabel[label];
        	degreeQuery[i] = degree;
          // printf("degree: %d\n", degree);
        	for (int j = 0; j < degree; ++j) {
            		//aInFile >> adjListQuery[index];
                fscanf(file, "%d\n", &adjListQuery[index]);
                // printf("adjl: %d\n", adjListQuery[index]);
            		++index;
        	}
        	adjIndexQuery[i + 1] = index;
    	}
      // printf("Finish Query Graph\n");
}

int selectRoot(){

    int root = -1;
    int label;
    int degree;
    double rank;
    double rootRank = DBL_MAX;

    for (int i = 0; i < numQueryNode; ++i) {
        label = labelQuery[i];
        degree = degreeQuery[i];
        // printf("%d%d\n", label, degree);
        int start = idxSortedData[label];
        int end = idxSortedData[label + 1];
        int mid = binaryLowerBound(start, end - 1, degree);
        // printf("%d\n", mid);
        int numInitCand = end - mid;

        rank = numInitCand/(double)degree;
        // printf("%f\n", rank);

        if( rank < rootRank ) {
            root = i;
            rootRank = rank;
        }
    }

    return root;

}

int binaryLowerBound(int aLeft, int aRight, int aDegree){
  // printf("%d%d\n", aLeft, aRight);
	int left = aLeft;
	int right = aRight;

	while (left < right){
    // printf("%d%d\n", left, right);
		int mid = left + (right - left) / 2;
    // printf("%d\n", mid);
    // printf("%d\n", sortedData[mid]);
		if (degreeData[sortedData[mid]] < aDegree)

      left = mid + 1;
		else
      right = mid;

	}

  // printf("%d%d\n", left, right);
	return left;
}

void clearMemory(){

      // printf("clearing Memory\n");
      if(labelData != NULL) free(labelData);
    	if(degreeData != NULL) free(degreeData);
    	if(sortedData != NULL) free(sortedData);
    	if(idxSortedData != NULL) free(idxSortedData);
    	if(labelFrequency != NULL) free(labelFrequency);
    	if(renamedLabel != NULL) free(renamedLabel);
    	if(labelQuery != NULL) free(labelQuery);
    	if(degreeQuery != NULL) free(degreeQuery);
    	if(adjListQuery != NULL) free(adjListQuery);
    	if(adjIndexQuery != NULL) free(adjIndexQuery);
    	if(dagChildQuerySize != NULL) free(dagChildQuerySize);
    	if(dagParentQuerySize != NULL) free(dagParentQuerySize);
    	if(dagChildQuery != NULL) {
        	for(int i = 0; i < numQueryNode; ++i) {
            		if(dagChildQuery[i] != NULL)
                		//delete[] dagChildQuery[i];
                    free(dagChildQuery[i]);
        	}
        	//delete[] dagChildQuery;
		      free(dagChildQuery);
    	}

    	if(dagParentQuery != NULL) {
        	for(int i = 0; i < numQueryNode; ++i) {
            	if(dagParentQuery[i] != NULL)
                	//delete[] dagParentQuery[i];
			            free(dagParentQuery[i]);
        	}
        	free(dagParentQuery);
    	}

      // printf("finished clearing Memory\n");

}


//FOR STABLE SORT : implemented MERGE SORT
// void merge_array(int *a, int begin, int mid, int end, int *temp, int (*f)(int, int))
void merge_array(int *a, int first, int mid, int end, int *temp, int (*f)(int, int))
{
    int i = first;
    int j = mid + 1;
    int k = 0;
    while (i <= mid && j <= end) {

        if (f(a[i], a[j])) {
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = a[i++];
    }

    while (j <= end) {
        temp[k++] = a[j++];
    }


		for (int s = 0; s < k; s++) {
				a[first+s] = temp[s];
	}
}

void merge_sort(int *a, int begin, int end, int (*f) (int, int))
{

		int* temp = (int *) malloc((end - begin + 1) * sizeof(int));
    if (begin < end) {
        int mid = (begin + end) / 2;
        merge_sort(a, begin, mid, f);
        merge_sort(a, mid + 1, end, f);
        merge_array(a, begin, mid, end, temp, f);
    }

		free(temp);
}
