#include "latch_guard.h"

const int nLockTimeOut = 100;

latch_guard::latch_guard(LOCK_INFO *a_pLock)
{
    m_Lock = a_pLock;
}

latch_guard::~latch_guard()
{
    if (0 == m_Lock)
    {
        return;
    }

    try
    {
        (void)CMutexOper::LeaveMutex(m_Lock->LockAddr);
    }
    catch (...)
    {
    }
}

int latch_guard::lock()
{
    if (0 == m_Lock)
    {
        return -1;
    }

    m_Lock->LockPID = getpid();
    (void)gettimeofday(&(m_Lock->LockTime), NULL);

    int iRet = CMutexOper::EnterMutex(m_Lock->LockAddr);
    if (iRet != 0)
    {
        if (iRet == -2)
        {
            if (kill(m_Lock->LockPID, 0) == -1)
            {
                printf("进程[%d]不存在，锁强制释放\n", m_Lock->LockPID);
                iRet = CMutexOper::LeaveMutex(m_Lock->LockAddr);
                if (iRet != 0)
                {
                    return iRet;
                }

                iRet = CMutexOper::EnterMutexTimeOut(m_Lock->LockAddr, nLockTimeOut);
                if (iRet != 0)
                {
                    m_Lock = 0;
                    return iRet;
                }
            }
            else
            {
                m_Lock = 0;
                return iRet;
            }
        }
        else
        {
            m_Lock = 0;
            return iRet;
        }
    }
    
    return iRet;
}

int latch_guard::reset(LOCK_INFO *a_pLock)
{
    if (a_pLock == NULL)
    {
        return -1;
    }

    memset(a_pLock, 0, sizeof(LOCK_INFO));
    (void)CMutexOper::Create(a_pLock->LockAddr);
    (void)CCondOper::Create(a_pLock->CondAddr);
    return 0;
}

int latch_guard::lock(LOCK_INFO *a_pLock, int nTimeOut)
{
    int iRet = 0;
    if (0 == a_pLock)
    {
        return -1;
    }

    if (nTimeOut <= 0)
    {
        a_pLock->LockPID = getpid();
        iRet = CMutexOper::EnterMutex(a_pLock->LockAddr);
    }
    else
    {
        iRet = CMutexOper::EnterMutexTimeOut(a_pLock->LockAddr, nTimeOut);
    }

    if (iRet != 0)
    {
        return iRet;
    }

    a_pLock->LockPID = getpid();
    (void)gettimeofday(&(a_plock->LockTime), NULL);
    return iRet;
}

int latch_guard::unlock(LOCK_INFO *a_pLock)
{
    if (0 = a_pLock)
    {
        return -1;
    }

    int iRet = CMutexOper::LeaveMutex(a_pLock->LockAddr);
    if (iRet != 0)
    {
        return iRet;
    }

    a_pLock->LockPID = 0;
    (void)gettimeofday(&(a_pLock->LockTime), NULL);
    return iRet;
}

int latch_guard::lockStatus(LOCK_INFO *a_pLock, bool &bLock)
{
    if (0 == a_pLock)
    {
        return -1;
    }

    int iRet = CMutexOper::IsLock(a_pLock->LockAddr, bLock);
    return iRet;
}

int latch_guard::waitSignal(LOCK_INFO *a_pLock, int nTimeOut)
{
    if (0 == a_pLock)
    {
        return -1;
    }

    int iRet = CCondOper::Wait(a_pLock->CondAddr, a_pLock->LockAddr, nTimeOut);
    return iRet;
}

int latch_guard::signal(LOCK_INFO *a_pLock)
{
    if (0 == a_pLock)
    {
        return -1;
    }

    int iRet = CCondOper::Signal(a_pLock->CondAddr);
    return iRet;
}


