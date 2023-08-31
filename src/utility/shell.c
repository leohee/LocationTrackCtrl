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
	    	printf("%s\t- ", cliCmds[i].pCmd);
	        if (cliCmds[i].pHelp) {
	            printf(cliCmds[i].pHelp);
	        }
			printf("\n\r");
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
	DelayMs(10);

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

uint8_t cli_power (int argc, char **argv)
{
	if (argc != 2) {
		return 1;
	}

	if (strcmp(argv[1], "idle") == 0) {
		log_info("lowpower mode idle.");

	} else if(strcmp(argv[1], "halt") == 0) {
		log_info("lowpower mode halt.");

	} else if(strcmp(argv[1], "slp") == 0) {
		log_info("lowpower mode sleep.");

	} else if(strcmp(argv[1], "stdn") == 0) {
		log_info("lowpower mode shutdown.");

	}


	return 0;
}

static void cli_history_add (char* buff)
{
    uint16_t len;
    uint8_t index = hisCmd.latest;

    if (NULL == buff) return;

    len = strlen((const char *)buff);
    if (len >= MAX_LINE_LEN) return;  /* command too long */

    /* find the latest one */
    if (0 != index) {
        index--;
    } else {
        index = HISTORY_MAX - 1;
    }

    if (0 != memcmp(hisCmd.cmd[index], buff, len)) {
        /* if the new one is different with the latest one, the save */
        memset((void *)hisCmd.cmd[hisCmd.latest], 0x00, MAX_LINE_LEN);
        memcpy((void *)hisCmd.cmd[hisCmd.latest], (const void *)buff, len);
        if (hisCmd.count < HISTORY_MAX) {
            hisCmd.count++;
        }

        hisCmd.latest++;
        if (hisCmd.latest >= HISTORY_MAX) {
            hisCmd.latest = 0;
        }
    }

    hisCmd.show = 0;
}

static uint8_t cli_history_show(uint8_t mode, char** p_history)
{
    uint8_t err = true;
    uint8_t num;
    uint8_t index;

    if (0 == hisCmd.count) return err;

    if (true == mode) {
        /* look up */
        if (hisCmd.show < hisCmd.count) {
            hisCmd.show++;
        }
    } else {
        /* look down */
        if (hisCmd.show > 1) {
            hisCmd.show--;
        }
    }

    num = hisCmd.show;
    index = hisCmd.latest;
    while (num) {
        if (0 != index) {
            index--;
        } else {
            index = HISTORY_MAX - 1;
        }
        num--;
    }

    err = false;
    *p_history = hisCmd.cmd[index];

    return err;
}

static void shell_handle (struct queue_t *rx_buff)
{
	static struct handle_type_t Handle = {.len = 0, .buff = {0}};
	uint8_t i = Handle.len;
	uint8_t cmd_match = false;
	uint8_t exec_req = false;

	// Step1: save chars from the terminal
	bool newChar = true;
	while (newChar) {
		if (Handle.len >= MAX_LINE_LEN) {
			break;
		}

		newChar = queue_out(rx_buff, Handle.buff+Handle.len);
		if (newChar) {
		    /* KEY_BACKSPACE -->get DELETE key from keyboard */
		    if ((Handle.buff[Handle.len] == KEY_BACKSPACE) 
			  || (Handle.buff[Handle.len] == KEY_DEL)) {
		        if (Handle.len > 0) {
		            /* delete a char in terminal */
		            TERMINAL_MOVE_LEFT(1);
		            TERMINAL_CLEAR_END();
		            Handle.buff[Handle.len] = '\0';
		            Handle.len--;
		        }
		    } else if(Handle.buff[Handle.len] == KEY_ENTER) {
		    	exec_req = true;
		    	Handle.len++;
		    } else if (strstr((const char *)Handle.buff, KEY_DELETE) != NULL){
		    	strcpy((char *)&Handle.buff[Handle.len-3], (char *)&Handle.buff[Handle.len+1]);
		    	Handle.len -= 3;
			} else {
		        Handle.len++;
		    }

			continue;
		}

	    /* all chars copied to Handle.buff */
	    uint8_t key = 0;
	    uint8_t err = 0xff;
	    char *p_hist_cmd = 0;

	    if (Handle.len >= 3) {
	        if (strstr((const char *)Handle.buff, KEY_UP) != NULL) {
	            key = 1;
	            TERMINAL_MOVE_LEFT(Handle.len-3);
	            TERMINAL_CLEAR_END();
	            err = cli_history_show(true, &p_hist_cmd);
	        } else if (strstr((const char *)Handle.buff, KEY_DOWN) != NULL) {
	            key = 2;
	            TERMINAL_MOVE_LEFT(Handle.len-3);
	            TERMINAL_CLEAR_END();
	            err = cli_history_show(false, &p_hist_cmd);
	        } else if (strstr((const char *)Handle.buff, KEY_RIGHT) != NULL) {
	            key = 3;
	        } else if (strstr((const char *)Handle.buff, KEY_LEFT) != NULL) {
	            key = 4;
	        }

	        if (key != 0) {
	            if (!err) {
	                memset(&Handle, 0x00, sizeof(Handle));
	                memcpy(Handle.buff, p_hist_cmd, strlen(p_hist_cmd));
	                Handle.len = strlen(p_hist_cmd);
	                Handle.buff[Handle.len] = '\0';
	                printf("%s", Handle.buff);  /* display history command */
	            } else if (err && (0 != key)) {
	                /* no history found */
	                TERMINAL_MOVE_LEFT(Handle.len-3);
	                TERMINAL_CLEAR_END();
	                memset(&Handle, 0x00, sizeof(Handle));
	            }
	        }
	    }

	    if ((key == 0) && (Handle.len > i)) {
	        /* display char in terminal */
	        for (; i < Handle.len; i++) {
	        	printf("%c", Handle.buff[i]);

	        }
	    }
	    break;
    }

    // Step2: handle the commands
    if(exec_req && (Handle.len == 1)) {
        /* KEY_ENTER -->ENTER key from terminal */
    	SHELL_NAME();
        Handle.len = 0;
    } else if(exec_req && Handle.len > 1) {  /* check for the length of command */
		NL1();
		Handle.buff[Handle.len - 1] = '\0';
		cli_history_add((char *)Handle.buff);
		char *command = strtok((char *)Handle.buff, " \t");

		/* looking for a match */
		for(i = 0; i < MAX_COMMAND_NB; i++) {
			if(0 == strcmp(command, cliCmds[i].pCmd)) {
				cmd_match = true;

				//Split arguments string to argc/argv
				uint8_t argc = 1;
				char 	*argv[MAX_ARGC];
				argv[0] = command;

				char *token = strtok(NULL, " \t");
				while(token != NULL){
					if(argc >= MAX_ARGC){
						printf(CLI_FONT_RED "Too many args." CLI_FONT_DEFAULT);NL1();
						break;
					}
					argv[argc] = token;
					argc++;
					token = strtok(NULL, " \t");
				}

				if (cliCmds[i].pFun != NULL) {
					/* call the func. */
					TERMINAL_HIDE_CURSOR();
					uint8_t result = cliCmds[i].pFun(argc, argv);

					if(result == 0){
						printf(CLI_FONT_GREEN "(%s returned %d)" CLI_FONT_DEFAULT, command, result);NL1();
					}else{
						printf(CLI_FONT_RED "(%s returned %d)" CLI_FONT_DEFAULT, command, result);NL1();
					}
					TERMINAL_SHOW_CURSOR();
					break;
				} else {
					/* func. is void */
					printf(CLI_FONT_RED "Command %s exists but no function is associated to it.", command);NL1();
				}
			}
		}

		if(!cmd_match) {
			/* no matching command */
			printf("Command \"%s\" unknown, try: help", Handle.buff);NL1();
		}

		Handle.len = 0;
		SHELL_NAME();

    }

    if(Handle.len >= MAX_LINE_LEN) {
        /* full, so restart the count */
    	printf(CLI_FONT_RED "\r\nMax command length is %d.\r\n" CLI_FONT_DEFAULT, MAX_LINE_LEN-1);
    	SHELL_NAME();
        Handle.len = 0;
    }

	fflush(stdout);
}

int shell_run (void)
{
	shell_handle(&rxShell);

	return 0;
}

void shell_init (void)
{
	SHELL_NAME();

	memset((uint8_t *)&hisCmd, 0, sizeof(struct cmd_history_t));
	memset((uint8_t *)&cliCmds, 0, sizeof(struct command_t)*MAX_COMMAND_NB);

	queue_init(&rxShell);

    command_add("help", "show commands", cli_help);
    command_add("cls", "clear screen", cli_clear);
    command_add("reset", "reboot", cli_reboot);
    command_add("log", "control log show", cli_log);
	command_add("power", "low power mode: idle,halt,slp,stdn", cli_power);


}



#ifdef __cplusplus
}
#endif
