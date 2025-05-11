/**
 * preprocessor.c - Examples of working with preprocessor
 *
 * Based on https://gcc.gnu.org/onlinedocs/cpp/
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CONST_STR "string"
#define CONST_INT 123

void basic_const_and_conditionals(void)
{
	printf("  Basic const and conditionals:\n\n");

#if defined(CONST_STR) && defined(CONST_INT)
	printf("%s(): CONST_STR = %s, CONST_INT = %d\n", __FUNCTION__, CONST_STR, CONST_INT);
#elif defined(CONST_STR)
	printf("%s(): CONST_STR = %s\n", __FUNCTION__, CONST_STR);
#elif defined(CONST_INT)
	printf("%s(): CONST_INT = %s\n", __FUNCTION__, CONST_INT);
#else
	printf("%s(): CONSTs not defined\n", __FUNCTION__);
#endif

#if defined(CONST_INT) && CONST_INT >= 123
	printf("%s(): true\n", __FUNCTION__);
#else
	printf("%s(): false\n", __FUNCTION__);
#endif
}

void def_undef(void)
{
	printf("\n  Define, undefine example:\n\n");

#define BUFSIZE   1024
#define TABLESIZE BUFSIZE
#undef BUFSIZE
#define BUFSIZE 64
	printf("%s(): TABLESIZE = %d, BUFSIZE = %d\n", __FUNCTION__, TABLESIZE, BUFSIZE);
}

void macros_and_stringizing(void)
{
	int var1, var2;

	printf("\n  Create macros, stringizing:\n\n");

#define UPDATE_PARAM_WITH_TRACE(param, arg)                                                                            \
	param = arg;                                                                                                       \
	printf("%s(): %s = %d\n", __FUNCTION__, #param, param);

	UPDATE_PARAM_WITH_TRACE(var1, 123)
	UPDATE_PARAM_WITH_TRACE(var2, 321)
}

void debug_funcs(void)
{
	printf("\n  Create debug functions:\n\n");

#define DEBUG
#ifdef DEBUG
/* both options below are acceptable */
#define print_err(fmt, arg...) printf("[ERR][%s()][%d] " fmt "\n", __FUNCTION__, __LINE__, ##arg); /*exit(-1);*/
#define print_dbg(fmt, ...)    printf("[DBG][%s()][%d] " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define print_err(fmt, arg...)
#define print_dbg(fmt, ...)
#endif /* DEBUG */

	print_err("This %s", "Error");
	print_dbg("This %s", "Debug");
}

void builtin_macros(void)
{
	printf("\n  Built-in macros:\n\n");

	printf("Filename:        %s\n", __FILE__);
	printf("Function:        %s\n", __FUNCTION__); /* __func__*/
	printf("Line:            %d\n", __LINE__);
	printf("Build date:      %s\n", __DATE__);
	printf("Build time:      %s\n", __TIME__);

#ifdef __x86_64__
	printf("Arch:            Intel x86 64bit\n");
#elif __i386__
	printf("Arch:            Intel x86 32bit\n");
#else
	printf("Arch:            Unknown\n");
#endif

#if defined(__BIG_ENDIAN__)
	printf("Endian:          Big\n");
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	printf("Endian:          Big (GCC)\n");
#elif defined(__LITTLE_ENDIAN__)
	printf("Endian:          Little\n");
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	printf("Endian:          Little (GCC)\n");
#else
	printf("Endian:          Unknown\n");
#endif

#ifdef __WINDOWS__
	printf("OS:              Windows\n");
#elif __APPLE__
	printf("OS:              Mac\n");
#elif __linux__
	printf("OS:              Linux\n");
#else
	printf("OS:              Other");
#endif
}

int main()
{
	basic_const_and_conditionals();

	def_undef();

	macros_and_stringizing();

	debug_funcs();

	builtin_macros();

	return 0;
}
