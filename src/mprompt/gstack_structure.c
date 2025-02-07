#include <sys/types.h>
#include <stdbool.h>
#include <pthread.h>   // use pthread local storage keys to detect thread ending

//----------------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------------

pthread_key_t mp_pthread_key = 0;
//----------------------------------------------------------------------------------
// Configuration
//----------------------------------------------------------------------------------

// size of page?
static ssize_t os_page_size               = 0;             // initialized at startup
static bool    os_use_gpools              = true;          // reuse gstacks in-process

// ----------------------------------------------------
// Signal handler for gpools to commit-on-demand
// ----------------------------------------------------

// Install a signal handler that handles page faults in gpool allocated stacks
// by making them accessible (PROT_READ|PROT_WRITE).
static struct sigaction mp_sig_segv_prev_act;
static struct sigaction mp_sig_bus_prev_act;
#define mp_decl_thread          __thread
static mp_decl_thread stack_t* mp_sig_stack;  // every thread needs a signal stack in order do demand commit stack pages

//----------------------------------------------------------------------------------
// Initialization
//----------------------------------------------------------------------------------

// 1. mp_gstack_init
//      1. some configurations setup
//      2. mp_gstack_os_init --> OS setup
//          1. get page size
//          2. check if os supports overcommit
//          3. register pthread key to detect thread termination (pthread_key_create)
//          4. set process cleanup of the gpools
//          5. mp_gpools_process_init -->
//      mp_gpools_process_init
//          1. mp_gpools_thread_init -->
//          2. if no gpools and OS uses overcommit, exit
//          3. install signal handler (which grows stack on demand)
//      mp_gpools_thread_init: Register signal stack (for page fault handler to run) for each thread.
//          1. if no gpools and OS uses overcommit, exit
//          2. create signal stack to handle page fault if not yet installed.
//      3. ensure stack sizes are page aligned
//      4. register exit routine
//      5. mp_gstack_thread_init
//          1. pthread_setspecific(mp_pthread_key, (void*)(1));  // set to non-zero value