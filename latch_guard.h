#ifndef _LATCH_GUARD_H
#define _LATCH_GUARD_H

typedef pthread_mutex_t latch_lock_t;

typedef struct
{
    latch_lock_t LockAddr[1];
    pthread_cond_t CondAddr[1];
    pid_t LockPID;
    struct timeval LockTime;
}LOCK_INFO;

class latch_guard
{
    public:
        latch_guard(LOCK_INFO *);
        ~latch_guard();
        int lock();
        static int reset(LOCK_INFO *a_pLock);
        static int lock(LOCK_INFO *a_pLock, int nTimeOut = -1);
        static int unlock(LOCK_INFO *a_pLock);
        static int lockStatus(LOCK_INFO *a_pLock, bool &bLock);
        static int signal(LOCK_INFO *a_pLock);
        static int waitSignal(LOCK_INFO *a_pLock, int nTimeOut);
    private:
        LOCK_INFO *m_Lock;
};


#define acquire_latch_guard(vname, lock_addr) \
    latch_guard vname(lock_addr);   \
int iRet##vname = vname.lock(); \
if (iRet##vname != 0)   \
return -1;

#endif
