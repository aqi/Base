#include "mutexOper.h"

CMutexOper::CMutexOper()
{
}

CMutexOper::~CMutexOper()
{
}

int CMutexOper::Creat(pthread_mutex_t *pMutex)
{
    pthread_mutexattr_t attr;
    int nRet = 0;

    nRet = pthread_mutexattr_init(&attr);
    if (nRet != 0)
    {
        return -1;
    }

    nRet = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    if (nRet != 0)
    {
        return -1;
    }

    nRet = pthread_mutex_init(pMutex, &attr);
    if (nRet != 0)
    {
        return -1;
    }

    nRet = pthread_mutexattr_destroy(&attr);
    if (nRet != 0)
    {
        return -1;
    }

    return 0;
}

int CMutexOper::EnterMutex(pthread_mutex_t *pMutex)
{
    int nRet = 0;

    nRet = pthread_mutex_lock(pMutex);
    return nRet;
}

int CMutexOper::EnterMutexTimeOut(pthread_mutex_t *pMutex, int lTimeOut, int iTimeSlice)
{
    int nRet = 0;

    struct timeval start, end;
    int iUSleepInterVal = 0;

    if (iTimeSlice > 1)
    {
        iUSleepInterVal = iTimeSlice;
    }
    else
    {
        iUSleepInterVal = 10;
    }

    if (lTimeOut <= 0)
    {
        lTimeOut = 1;
    }

    (void)gettimeofday(&start, 0);

    while(1)
    {
        nRet = pthread_mutex_trylock(pMutex);
        if (nRet == 0)
        {
            return 0;
        }

        (void)gettimeofday(&end, 0);
        if (lTimeOut < ((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec + iUSleepInterVal))
        {
            break;
        }
        else
        {
            (void)usleep(iUSleepInterVal);
        }
    }

    if (nRet == EBUSY)
    {
        return -2;
    }
    else
    {
        return -1;
    }
}

int CMutexOper::TryEnterMutex(pthread_mutex_t *pMutex)
{
    int nRet = 0;

    nRet = pthread_mutex_trylock(pMutex);
    return nRet;
}

int CMutexOper::LeaveMutex(pthread_mutex_t *pMutex)
{
    int nRet = 0;

    nRet = pthread_mutex_unlock(pMutex);
    return nRet;
}

int CMutexOper::Destroy(pthread_mutex_t *pMutex)
{
    int nRet = 0;

    nRet = pthread_mutex_destroy(pMutex);
    return nRet;
}

int CMutexOper::IsLock(pthread_mutex_t *pMutex, bool &bLocked)
{
    int nRet = 0;
    bLocked = false;

    nRet = pthread_mutex_trylock(pMutex);

    if (nRet != 0)
    {
        if (nRet = EBUSY)
        {
            bLocked = true;
            nRet = 0;
            return 0;
        }
        else
        {
            return nRet;
        }
    }
    else
    {
        return pthread_mutex_unlock(pMutex);
    }
}


CCondOper::CCondOper()
{
}

CCondOper::~CCondOper()
{
}

int CCondOper::Create(pthread_cond_t *pCond)
{
    pthread_condattr_t condAttr;
    int nRet;

    nRet = pthread_condattr_init(&condAttr);
    if (nRet != 0)
    {
        return -1;
    }

    nRet = pthread_condattr_setpshared(&condAttr, PTHREAD_PROCESS_SHARED);
    if (nRet != 0)
    {
        return -1;
    }

    nRet = pthread_cond_init(pCond, &condAttr);
    if (nRet != 0)
    {
        return -1;
    }

    nRet = pthread_condattr_destroy(&condAttr);
    if (nRet != 0)
    {
        return -1;
    }

    return 0;
}

int CCondOper::Signal(pthread_cond_t *pCond)
{
    return pthread_cond_signal(pCond);
}

int CCondOper::Wait(pthread_cond_t *pCond, pthread_mutex_t *pMutex, int lTimeOut)
{
    int nRet;
    struct timespec abstime;
    struct timeval curtime;

    if (lTimeOut <= 0)
    {
        nRet = pthread_cond_wait(pCond, pMutex);
        if (nRet != 0)
        {
            return -1;
        }
    }
    else
    {
        (void)gettimeofday(&curtime, 0);
        abstime.tv_sec = curtime.tv_sec;
        abstime.tv_nsec=curtime.tv_usec*1000 + 999 + lTimeOut * 1000;
        abstime.tv_sec += abstime.tv_nsec / 1000000000;
        abstime.tv_nsec = abstime.tv_nsec % 1000000000;
        nRet = pthread_cond_timedwait(pCond, pMutex, &abstime);
        if (nRet != 0)
        {
            if (errno = ETIMEDOUT)
            {
                return -2;
            }
            else
            {
                return -1;
            }
        }
    }

    return 0;
}
