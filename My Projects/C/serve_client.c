pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

int numDowntimes = 0;

void *downtime() {
  Node * balance;
  while(numDowntimes != 3){
  numDowntimes ++;
  pthread_rwlock_wrlock(&lock);
  sleep(2);
  balance = balanceTree(root);
  freeSubtree(root);
  root = balance;
  pthread_rwlock_unlock(&lock);
  }
  return NULL;
}

void *ServeClient(char *client) {  	
  FILE *fptr;
  fptr = fopen(client, "r");
  char getline [30];
  char storeString[16];
  strcpy(storeString, client);
  char *idkman = strtok(storeString, "_");
  char * command;
  char * imtired;
  int i = 0;
  while(fgets(getline, sizeof(getline), fptr) != NULL){
  	command = strtok_r(getline, " ", &imtired);
  	i++;
  	printf("%d \n", numDowntimes);
  	if (strcmp(command, "addNode") == 0){
  		command = strtok_r(NULL, " ", &imtired);
  		pthread_rwlock_wrlock(&lock);
  		root = addNode(root, atoi(command));
  		printf("%sinsertNode %d \n", idkman, atoi(command));
  		pthread_rwlock_unlock(&lock);
  	}
  	else if(strcmp(command, "removeNode") == 0){
  		command = strtok_r(NULL, " ", &imtired);
  		pthread_rwlock_wrlock(&lock);
  		root = removeNode(root, atoi(command));
  		printf("%sdeleteNode %d \n", idkman, atoi(command));
  		pthread_rwlock_unlock(&lock);
  		
  	}
        else if(strcmp(command, "avgSubtree") == 0) {
          pthread_rwlock_rdlock(&lock);
	  printf("%savgSubtree = %f\n", idkman, avgSubtree(root));
	  pthread_rwlock_unlock(&lock);
	}
	else{
		pthread_rwlock_rdlock(&lock);
		printf("%scountNodes = %d\n", idkman, countNodes(root));
		pthread_rwlock_unlock(&lock);	
	}
  }

  fclose(fptr);
  return NULL; 
}
