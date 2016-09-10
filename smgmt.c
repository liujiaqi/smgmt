/* a Simple Memory manaGeMent Tool
 *
 * Powered by TiH liujiaqi
 * tihljq@gmail.com
 *
 */

#include <stddef.h>
#include <stdint.h>
#include "smgmt.h"

#define NODE_HEAD_LEN sizeof(ts_memnode)
#define GET_NODE_HEAD(p) ((ts_memnode *)(((uint8_t *)p) - NODE_HEAD_LEN))

typedef struct _ts_memnode {
  uint16_t size;
  struct _ts_memnode * prev;
  struct _ts_memnode * next;
  uint8_t free;
  uint8_t * pmagic;
  uint8_t data[0];
}ts_memnode;

static ts_memnode * pfirst_node;

void smgmt_init(void *p, uint16_t size){
  pfirst_node = p;
  pfirst_node->size = size - NODE_HEAD_LEN;
  pfirst_node->prev = NULL;
  pfirst_node->next = NULL;
  pfirst_node->free = 1;
  pfirst_node->pmagic = pfirst_node->data;
}

void * smgmt_malloc(uint16_t size){
  ts_memnode * pnode;
  ts_memnode * pnew_node;
  pnode = pfirst_node;

// first fit
  while(pnode && !(pnode->free && pnode->size>=size)){
    pnode = pnode->next;
  }
  if(!pnode) return NULL;

  if(pnode->size > size+NODE_HEAD_LEN){
    pnew_node = (ts_memnode *)(pnode->data + size);
    pnew_node->size = pnode->size - size - NODE_HEAD_LEN;
    pnew_node->prev = pnode;
    pnew_node->next = pnode->next;
    pnew_node->free = 1;
    pnew_node->pmagic = pnew_node->data;
    if(pnew_node->next) pnew_node->next->prev = pnew_node;

    pnode->size = size;
    pnode->next = pnew_node;
  }
  pnode->free = 0;
  return pnode->data;
}

void smgmt_free(void *p){
  ts_memnode * pnode;
  if(p == NULL) return;
  if((uint8_t *)p < pfirst_node->data) return;
  if(p != pnode->pmagic) return;
  pnode = GET_NODE_HEAD(p);
  pnode->free = 1;
//  pnode->pmagic = NULL;  //increase safety for refree
  if(pnode->next && pnode->next->free){
    pnode->size += pnode->next->size + NODE_HEAD_LEN;
    pnode->next = pnode->next->next;
    if(pnode->next) pnode->next->prev = pnode;
  }
  if(pnode->prev && pnode->prev->free){
    pnode->prev->size += pnode->size + NODE_HEAD_LEN;
    pnode->prev->next = pnode->next;
    if(pnode->next) pnode->next->prev = pnode->prev;
  }
}
