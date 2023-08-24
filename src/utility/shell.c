#include "inc_files.h"

#include "vt100.h"

#ifdef __cplusplus
extern "C" {
#endif

struct queue_t rxShell;

struct command_t cliCmds[MAX_COMMAND_NB];
struct cmd_history_t hisCmd;


void command_add (const char *command, const char *help, uint8_t (*exec)(int argc, char **argv))
{
	int i = 0;
	for (; i < MAX_COMMAND_NB; i++) {
		if (strcmp(cliCmds[i].pCmd, "") == 0) {
			cliCmds[i].pCmd = command;
			cliCmds[i].pFun = exec;
			cliCmds[i].pHelp = help;
			break;
		}
	}
}

uint8_t cli_help (int argc, char **argv)
{
	int i = 0;

	for(i = 0; i < MAX_COMMAND_NB; i++) {
		if(strcmp(cliCmds[i].pCmd, "") != 0){
	    	printf("%s - ", cliCmds[i].pCmd);
	        if (cliCmds[i].pHelp) {
	            printf(cliCmds[i].pHelp);
	        }
			printf("\n");
		}
	}
	return 0;

}

uint8_t cli_clear (int argc, char **argv)
{
	TERMINAL_BACK_DEFAULT(); /* set terminal background color: black */
	TERMINAL_FONT_DEFAULT(); /* set terminal display color: green */

	/* This prints the clear screen and move cursor to top-left corner control
	 * characters for VT100 terminals. This means it will not work on
	 * non-VT100 compliant terminals, namely Windows' cmd.exe, but should
	 * work on anything unix-y. */
	TERMINAL_RESET_CURSOR();
	TERMINAL_DISPLAY_CLEAR();

	return 0;
}

uint8_t cli_reboot (int argc, char **argv)
{
	log_info("Reboot myself.");

	NVIC_SystemReset();

	return 0;
}

uint8_t cli_log (int argc, char **argv)
{
	if (argc != 2) {
		return 1;
	}

	if (strcmp(argv[1], "on") == 0) {
		log_info("set log on.");

	} else if(strcmp(argv[1], "off") == 0) {
		log_info("set log off.");

	}


	return 0;
}

void shell_init (void)
{
	memset((uint8_t *)&hisCmd, 0, sizeof(struct cmd_history_t));
	memset((uint8_t *)&cliCmds, 0, sizeof(struct command_t)*MAX_COMMAND_NB);

	queue_init(&rxShell);

    command_add("help", "show commands", cli_help);
    command_add("cls", "clear screen", cli_clear);
    command_add("reset", "reboot", cli_reboot);
    command_add("log", "control log show", cli_log);



}



#ifdef __cplusplus
}
#endif
