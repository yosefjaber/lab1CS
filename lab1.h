/* 
 * lab4.h - Prototypes for Lab4 helper functions
 */

#ifndef LAB_TOOLS_H
#define LAB_TOOLS_H

/* 
 * printSummary - This function provides a standard way for your cache
 * simulator * to display its final hit and miss statistics
 */ 
void printSummary(int hits,  /* number of  hits */
				  int misses, /* number of misses */
				  int evictions); /* number of evictions */

#endif /* LAB_TOOLS_H */
