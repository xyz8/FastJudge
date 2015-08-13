#define _GNU_SOURCE

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <ev.h>
#include <sys/utsname.h>

#define KERNEL_MAJOR(v) ( ( (v) & 0xFF0000L ) >> 16 )
#define KERNEL_MINOR(v) ( ( (v) & 0xFF00L) >> 8 )
#define KERNEL_PATCH(v) ( (v) & 0xFF )
#define KERNEL_VERSION(major, minor, patch) ( ( (major) << 16 ) | ( (minor) << 8 ) | (patch) )

#define L_DEBUG 0
#define L_INFO 1
#define L_WARN 2
#define L_ERROR 3
#define L_FATAL 4

typedef long kernel_version_t;

const static char *level_str[] = {
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL",
};

__attribute__((format(printf, 4, 5)))
static inline void _logf(int level, const char *file, const long line, const char *fmt, ...) {
	fprintf(stderr, "[%s:%d %s] ", file, line, level_str[level]);
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fputs("\n", stderr);
}

#ifdef DEBUG
# define debugf(fmt, ...) _logf(L_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
# define debugf(fmt, ...) (void) 0
#endif /* DEBUG */

#define infof(errfmt, ...) _logf(L_INFO, __FILE__, __LINE__, errfmt, ##__VA_ARGS__)
#define warnf(errfmt, ...) _logf(L_WARN, __FILE__, __LINE__, errfmt, ##__VA_ARGS__)
#define errorf(errfmt, ...) _logf(L_ERROR, __FILE__, __LINE__, errfmt, ##__VA_ARGS__)
#define fatalf(errfmt, ...) do { \
	_logf(L_FATAL, __FILE__, __LINE__, errfmt, ##__VA_ARGS__); \
	exit(EXIT_FAILURE); \
} while(0)
#define must_success(func_call, errfmt, ...) do { \
	int fres = (func_call); \
	debugf("%s = %d", #func_call, fres); \
	if ( (fres) < 0 ) fatalf(errfmt, ##__VA_ARGS__); \
} while(0)

void check_kernel_version(kernel_version_t *version) {
	static struct utsname buf;
	static long major, minor, patch, ver = -1;
	if (ver == -1) {
		must_success(uname(&buf), "uname failed");
		if (buf.release[0] < '2')
			goto fail;
		sscanf(buf.release, "%d.%d.%d", &major, &minor, &patch);
		debugf("Kernel version: %d.%d.%d", major, minor, patch);
		ver = KERNEL_VERSION(major, minor, patch);
		if (ver < KERNEL_VERSION(2, 6, 32))
			goto fail;
	}
	*version = ver;
	return;
fail:
	fatalf("Kernel version < 2.6.32 is not supported!");
}

void spawn_child() {
}

int main(void) {
	infof("fastjudge 0.0.1");
	kernel_version_t version;
	check_kernel_version(&version);
	EV_P = EV_DEFAULT;

	return 0;
}
