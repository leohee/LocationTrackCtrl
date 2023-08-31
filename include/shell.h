#ifndef __SHELL_H__
#define __SHELL_H__


#ifdef __cplusplus
extern "C" {
#endif

#define HISTORY_MAX         10
#define MAX_LINE_LEN 		80
#define MAX_COMMAND_NB		32
#define MAX_ARGC			8

#define NL1()           do { printf("\n\r"); } while(0)
#define NL2()           do { printf("\n\r\n\r"); } while(0)
#define NL3()           do { printf("\n\r\n\r\n\r"); } while(0)

#define SHELL_NAME()	do { printf(CLI_FONT_DEFAULT"\n\r -> "); } while(0)


struct handle_type_t {
	uint8_t		buff[MAX_LINE_LEN];
	uint8_t		len;
};

struct command_t {
	const char	*pCmd;
	const char	*pHelp;
	uint8_t		(*pFun)(int argc, char **argv);
};

struct cmd_history_t {
	char		cmd[HISTORY_MAX][MAX_LINE_LEN];
	uint8_t		count;
	uint8_t		latest;
	uint8_t		show;
};

extern struct queue_t rxShell;

extern int shell_run (void);

extern void shell_init (void);

#ifdef __cplusplus
}
#endif

#endif


