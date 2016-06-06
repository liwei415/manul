#ifndef _MNL_SLOCK_
#define _MNL_SLOCK_

#include <sched.h>

typedef struct mnl_slock_s mnl_slock_t;

struct mnl_slock_s{
    volatile long spin_;
    volatile long flag_;
};

void mnl_slock_init(mnl_slock_t *lock, long *flag);
void mnl_slock_lock(mnl_slock_t *lock);
void mnl_slock_unlock(mnl_slock_t *lock);
int  mnl_slock_trylock(mnl_slock_t *lock);
int  mnl_slock_islock(mnl_slock_t *lock);

#endif
