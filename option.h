#define VERBOSE if (verbose && !silent)
#define NSILENT if (!silent)

/* Verbose option; if it's on, program will print out almost every info of execution. */
extern int verbose;

/* Silent option; if it's on, program will not print anything.
   This option is superior of `verbose`;
   if this is on, verbose option is ignored. */
extern int silent;

/* Input file name of this program. */
extern const char* input;

/* Output file name of this program. */
extern const char* output;

/* Initialize the options with main arguments. */
int init (int argc, char *argv[]);

/* Print details of options. */
void printOptions ();