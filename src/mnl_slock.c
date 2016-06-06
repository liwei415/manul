#include "mnl_slock.h"

void mnl_slock_init(mnl_slock_t *lock, long *flag)
{
  __sync_and_and_fetch((long*)&lock->spin_, 0);
  //__sync_lock_test_and_set((long*)&lock->spin_,flag?(long)flag:(long)&lock->flag_);
}

void mnl_slock_lock(mnl_slock_t *lock)
{
  for (;0!=__sync_fetch_and_or((long*)&lock->spin_, 1);) {
    ;
  }
}

void mnl_slock_unlock(mnl_slock_t *lock)
{
  __sync_and_and_fetch((long*)&lock->spin_, 0);
}

int mnl_slock_trylock(mnl_slock_t *lock)
{
  return !__sync_fetch_and_or((long*)&lock->spin_, 1);
}

int mnl_slock_islock(mnl_slock_t *lock)
{
  return __sync_add_and_fetch((long*)&lock->spin_, 0);
}
