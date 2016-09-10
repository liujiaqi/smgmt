
#ifndef  __MEMGMT_H_
#define  __MEMGMT_H_

#if defined __cplusplus
extern "C" {
#endif

#include <stdint.h>

void smgmt_init(void *p, uint16_t size);
void * smgmt_malloc(uint16_t size);
void smgmt_free(void *p);

#if defined __cplusplus
}
#endif

#endif
