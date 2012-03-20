#ifndef _DHBWCC_RESOURCE_MANAGER_H
#define _DHBWCC_RESOURCE_MANAGER_H

typedef void (*rm_cleanup_fp)(void *);

/** 
 * \struct resource_entry
 * \brief Resource entry data structure used for
 *        each resource handler registered with the
 *        resource manager.
 */
typedef struct resource_entry {
  rm_cleanup_fp handler;                        /**< The resource handler function pointer */
  void *data;                                   /**< The anonymous data passed to the handler */
} resource_entry_t;

/**
 * \struct resource_mgr
 * \brief Resource manager data structure.
 */
typedef struct resource_mgr {
  int num_entries;                              /**< The number of registered handlers */
  resource_entry_t **entries;                   /**< The list of registered handlers */
} resource_mgr_t;

extern void rm_register_handler (resource_mgr_t *mgr, rm_cleanup_fp handler, void *data);
extern void rm_cleanup_resources (resource_mgr_t *mgr);
extern void rm_init (resource_mgr_t *mgr);

extern resource_mgr_t resource_mgr;

#endif
