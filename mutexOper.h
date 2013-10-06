#ifndef _OPER_MUTEX_H
#define _OPER_MUTEX_H

#include <pthread.h>

class CMutexOper
{
    public:
        CMutexOper();
        ~CMutexOper();

        static int Create(pthread_mutex_t *pMutex);
        static int EnterMutex(pthread_mutex_t *pMutex);
        static int EnterMutexTimeOut(pthread_mutex_t *pMutex, long lTimeOut, int iTimeSlice = 10);
        static int TryEnterMutex(pthread_mutex_t *pMutex);
        static int LeaveMutex(pthread_mutex_t *pMutex);
        static int Destroy(pthread_mutex_t *pMutex);
        static int IsLock(pthread_mutex_t *pMutex, bool &bLocked);
};

class CCondOper
{
    public:
        CCondOper();
        ~CCondOper();
        static int Create(pthread_cond_t *pCond);
        static int Signal(pthread_cond_t *pCond);
        static int Wait(pthread_cond_t *pCond, pthread_mutex_t *pMutex, long lTimeOut = -1);
};

#endif
