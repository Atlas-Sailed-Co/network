/*
 * Carrie Segal
 * Network Simulation
*/

#include <stdio.h>
 #include <stdlib.h>

#define C 2
#define N 9
#define T 100 // Simulate for 100 time Steps
//#define TIGGERFILE 

struct node {
	int id;
	int trigger;
	int event;
	int bufferSize;
	int bufferFill;
	int *buffer;
	int neighbors[C];
	int overFlow;
};

FILE fr;
char line[80];

FILE* fp;
FILE* fh;
FILE* fb;

int main (void) {
	int i,j,t;
	// Declare all of the nodes on this sheet.
	struct node nodes[N];	
	int triggerEvents[T][N]; // Trigger events are able to enter the network from connector nodes, and make their way to the request node, where they then ask for event data. 
	int happeningEvents[T][N]; // Events that can strike any detector at any time. 
	

#ifdef TIGGERFILE
	// Need to update this to read in Happening Events to, if we want to read from a file
	// Need to read in triggers @ some times for simulation
	fr=fopen("triggeredEvents.txt", "rt");  /* open the file for reading */
   /* elapsed.dta is the name of the file */
   /* "rt" means open the file for reading text */
	i = 0;
	while(fgets(line, 80, fr) != NULL) {
   		// Skip the first row, it's for the titles
   		if( i != 0 ) {
		 	/* get a line, up to 80 chars from fr.  done if NULL */
		 	for( j = 0; j < N; j++) {
		 		if (line[i] == 1) { 
		 			// It's a trigger event
		 		}
		 	}
		 	//sscanf (line, "%ld", &elapsed_seconds);
		 	/* convert the string to a long int */
		 	//printf ("%ld\n", elapsed_seconds);
		}
		i++;
   }
   fclose(fr);  /* close the file prior to exiting the routine */
#else
   // Make some Data to use for Trigger Events
   	fp = fopen("triggeredEvents.txt", "w");
	fprintf(fp, "t\t");
	for (i = 0; i < N; i++) {
		fprintf(fp, "%d\t",i); }
	fprintf(fp,"\n");


	// Make some data to use for Happening Events
   	fh=fopen("happeningEvents.txt", "w");
	fprintf(fh, "t\t");
	for (i = 0; i < N; i++) {
		fprintf(fh, "%d\t",i); }
	fprintf(fh,"\n");

	for (t = 0; t < T; t++) {
		fprintf(fp, "%d\t",t); 
		fprintf(fh, "%d\t",t); 
		for(i = 0; i < N; i++) {
			int someData = rand() % 2000;
			
			if (t == 0) { // nothing happens on first time step.
				someData = 0;
			}

			if (someData > 1000 ) {
				happeningEvents[t][i] = 1;
				if (someData > 1800 ) {
					triggerEvents[t][i] = 1;
				}	else {
					triggerEvents[t][i] = 0;
				}
			}	else {
				happeningEvents[t][i] = 0;
				triggerEvents[t][i] = 0;
			}
			fprintf(fh, "%d\t",happeningEvents[t][i]);
			fprintf(fp, "%d\t",triggerEvents[t][i]); 
		}
		fprintf(fh,"\n");
		fprintf(fp,"\n");
	}
#endif

	// Initialize all of the nodes.
	for (i = 0; i < N; i++) {
		nodes[i].id = i; // Everyone needs an ID.
		nodes[i].trigger = 0; /// Wat... probably not necessary.
		nodes[i].event = 0; // Wat again....
		nodes[i].bufferSize = 8; // Why not? Lets start with a buffer size of 8 integers.
		nodes[i].bufferFill = 0; // nothing in the buffer to start out
		nodes[i].buffer = malloc(nodes[i].bufferSize * sizeof(int) );
		nodes[i].overFlow = 0;
		for (j = 0; j < nodes[i].bufferSize; j++) {
			nodes[i].buffer[j] = 0; // All the buffer is holding is empty time stamps. 
		}

		// Connectivity map definitions... each node can see it's two neighbors.
		// It's a ring!
		if( (i > 0) && (i < N-1) ) {
			nodes[i].neighbors[0] = i-1;
			nodes[i].neighbors[1] = i+1;
		} else if (i == 0 ) {
			nodes[i].neighbors[0] = N-1;
			nodes[i].neighbors[1] = i+1;
		} else if (i == N-1) {
			nodes[i].neighbors[0] = i-1;
			nodes[i].neighbors[1] = 0;
		}
	}

	// Store The buffer Data Here.
	fb = fopen("buffers.txt", "w");

	for(t = 0; t < T; t++) {
		// Send information into the system
		// Right now all the info is already generated in the earler section of code.
		// printf("%d %d\n",t,t);
		for(i = 0; i < N; i++) {
			// Update the state of the nodes.
			if ( happeningEvents[t][i] ) {
				//printf("Happened!\n");
				if (nodes[i].bufferFill < nodes[i].bufferSize ) {
					nodes[i].buffer[nodes[i].bufferFill] = t;
					nodes[i].bufferFill++;
				} else {
					nodes[i].overFlow = 1;
				}
			} 

		}
		// Write data out of the system
		for (i = 0; i < N; i++) {
		fprintf(fb, "%d\t", t);
			for(j=0; j< nodes[i].bufferSize; j++) {
			fprintf(fb, "%d\t", nodes[i].buffer[j] );
			}
		fprintf(fb,"\n");
		}
	}
	return 0;
}
