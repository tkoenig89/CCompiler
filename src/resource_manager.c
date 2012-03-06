#include <stdlib.h>

#include "diag.h"
#include "resource_manager.h"

/**
 * \var resource_mgr
 * \brief The central resource manager data structure.
 */
resource_mgr_t resource_mgr;

/**
 * \brief It will call all registered handlers in reverse
          order.
 * \param mgr The instance of the resource mgr (here we have only one).
 */
void rm_cleanup_resources (resource_mgr_t *mgr) {
  /* call the handlers in reverese order */
  for (int i = mgr->num_entries-1; i >= 0; i--) {
    /* call the registered cleanup handler */
    mgr->entries[i]->handler(mgr->entries[i]->data);
  }
}

/**
 * \brief Register a handler in the resource handler.
 * \param mgr The instance of the resource mgr.
 * \param handler The resource handler to be registered.
 * \param data Any data that should be passed to the resource handler.
 */
void rm_register_handler (resource_mgr_t *mgr, rm_cleanup_fp handler, void *data) {
  int new_num = mgr->num_entries + 1;
  mgr->entries = realloc(mgr->entries, new_num * sizeof(*mgr->entries)); 
  resource_entry_t *entry = malloc(sizeof(*entry));
  if (mgr->entries == NULL || entry == NULL) {
    /* call the handler since it already contains resources to be freed */
    handler(data);
    /* fatal_os_error will cleanup the already registered resources */
    FATAL_OS_ERROR(OUT_OF_MEMORY, 0, "rm_register_handler -> realloc");
  }

  entry->handler = handler;
  entry->data = data;
  mgr->entries[new_num-1] = entry;
  mgr->num_entries = new_num;
}

/**
 * \brief Clean-up the resource manager. This clean-up function
 *        will be part of the list of resource handlers.
 * \param data The instance of the resource manager.
 */
static void rm_cleanup (void *data) {
  resource_mgr_t *mgr = data;

  for (int i = mgr->num_entries-1; i >= 0; i--) {
    free(mgr->entries[i]);
  }
  free(mgr->entries);
}

/**
 * \brief Initialize the resource manager.
 * \param mgr The memory area to be initialized.
 */
void rm_init (resource_mgr_t *mgr) {
  mgr->num_entries = 0;
  mgr->entries = NULL;
  rm_register_handler(mgr, rm_cleanup, mgr);
}
