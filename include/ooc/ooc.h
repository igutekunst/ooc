#ifndef OOC_H
#define OOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * A simple calculator demonstrating basic functionality.
 */

/**
 * Add two numbers.
 */
int ooc_add(int a, int b);

/**
 * Get the library name and version.
 * Returns a statically allocated string that should not be freed.
 */
const char* ooc_version(void);

#ifdef __cplusplus
}
#endif

#endif /* OOC_H */ 