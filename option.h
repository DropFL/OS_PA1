#define VERBOSE if (verbose)

/* Verbose option; if it's on, program will print out almost every info of execution. */
extern int verbose;

/* Input file name of this program. */
extern const char* file;

/* Initialize the options with main arguments. */
int init (int argc, char *argv[]);

/* Print details of options. */
void printOptions ();