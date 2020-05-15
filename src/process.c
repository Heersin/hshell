#include "process.h"

/* process2.c   - version 2 - supports builtins
 * command processing layer
 * 
 * The process(char **arglist) function is called by the main loop
 * It sits in front of the execute() function.  This layer handles
 * two main classes of processing:
 * 	a) built-in functions (e.g. exit(), set, =, read, .. )
 * 	b) control structures (e.g. if, while, for)
 */


int process(char **args, char *cmdline)
/*
 * purpose: process user command
 * returns: result of processing command
 * details: if a built-in then call appropriate function, if not execute()
 *  errors: arise from subroutines, handled there
 */
{
	int	rv = 0;

	if ( args[0] == NULL )
		rv = 0;
	else if ( is_control_command(args[0]) )
		rv = do_control_command(args);
	else if (is_pipe_cmd(cmdline))
		rv = start_pipe(args);
	else if (is_redirect_cmd(cmdline))
		rv = start_redirect(args);
	else if ( ok_to_execute() )
		if ( !builtin_command(args,&rv) )
			rv = execute(args);
			
	return rv;
}

