#include "dag.h"
#include "intset.h"

int main(int argc, char **argv){

	//argv[1] : name of data graph file
	//argv[2] : name of query graph file
	//argv[3] : the number of query in query graph
	// printf("%s %s %s\n", argv[1], argv[2], argv[3]);

  FILE *DF, *QF;
  int numQuery;

  numQuery = atoi(argv[3]);

	if (argc != 4){
    //./program human human_40n 100 > human_40n.dag
		printf("Usage: ./program <dataFile> <queryFile> <numQuery>\n");
		return -1;

	}

	// printf("Data File: %s\n", argv[1]);
	// printf("Query File: %s\n", argv[2]);
	// printf("Num Query: %d\n", atoi(argv[3]));



	//DF = fopen(argv[1], "r");

	//read data graph file

	readDataGraph(argv[1]);

  QF = fopen(argv[2], "r");

	//read query graph file & find the dag for each query graph

  int LINE_SIZE = 255;
  char line[LINE_SIZE];

  // while (fgets(line, LINE_SIZE, QF))
  // {
  //   char *tag;
  //   char *id_char;
  //   char *num_char;
  //   char *sumDegree_char;
  //   int id, num, sumDegree;
  //
  //   tag = strtok(line, " ");
  //   id_char = strtok(NULL, " ");
  //   num_char = strtok(NULL, " ");
  //   sumDegree_char = strtok(NULL, " ");
  //
  //   id = atoi(id_char);
  //   num = atoi(num_char);
  //   sumDegree = atoi(num_char);
  //
  //   printf("%s %d %d %d\n", tag, id, num, sumDegree);
  //
  //   numQueryNode = num;
  //   printf("numquerynode: %d\n", numQueryNode);
  //
  //   readQueryGraph(QF, sumDegree);
  //
	// 	buildDAG();
  //
  // }

  // printf("nqn: %d\n", numQuery);
	for (int i = 0; i < numQuery; ++i){
    // printf("i: %d\n", i);

		char tag;
		int id, num, sumDegree;

		fscanf(QF, "%c %d %d %d\n", &tag, &id, &num, &sumDegree);
    // printf("read: %c %d %d %d\n", tag, id, num, sumDegree);

		numQueryNode = num;

		readQueryGraph(QF, sumDegree);

		buildDAG();

	}

	fclose(QF);
	clearMemory();

	return 0;
}
