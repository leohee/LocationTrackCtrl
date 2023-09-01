#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include "vt100.h"

#define MAX_LINE					(256)		///< 每行最大字符数

extern const char Hex2Ascii[17];

extern int hex_printf (const uint8_t *buff, int count);

extern void log_do (const char *fmt, ...);

#if defined(DEBUG)
  #define log_err(fmt, ...) log_do(CLI_FONT_RED"%lu[_E_]: %s(%d): "CLI_FONT_DEFAULT fmt, \
		gLT->ticks, __func__, __LINE__, ##__VA_ARGS__)

  #define log_info(fmt, ...) log_do(CLI_FONT_GREEN"%lu[_I_]: %s(%d): "CLI_FONT_DEFAULT fmt, \
		gLT->ticks, __func__, __LINE__, ##__VA_ARGS__)

  #define log_debug(fmt, ...) log_do(CLI_FONT_YELLOW"%lu[_D_]: %s(%d): "CLI_FONT_DEFAULT fmt, \
		gLT->ticks, __func__, __LINE__, ##__VA_ARGS__)

  #define log_hex(type, buff, len, fmt, ...) do {\
  			log_do("%lu: %s(%d): " fmt, gLT->ticks, \
  				__func__, __LINE__, ##__VA_ARGS__); \
  			hex_printf(buff, len); } while(0)

#else
  #define log_err(fmt, ...)
  #define log_info(fmt, ...)
  #define log_debug(fmt, ...)
  #define log_hex(type, buff, len, fmt, ...)

#endif


#ifdef __cplusplus
}
#endif

#endif /* __LOG_H__ */

