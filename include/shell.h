#ifndef __SHELL_H__
#define __SHELL_H__


#ifdef __cplusplus
extern "C" {
#endif

#define HISTORY_MAX         10
#define MAX_LINE_LEN 		80
#define MAX_COMMAND_NB		32

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

extern void shell_init (void);

#ifdef __cplusplus
}
#endif

#endif


