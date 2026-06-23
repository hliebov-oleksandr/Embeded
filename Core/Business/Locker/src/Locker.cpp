#include "locker.hpp"

Locker::Locker() : password("")
{
}

bool Locker::isLocked()
{
    return !password.empty();
}

void Locker::lock(std::string pass)
{
    password = pass;
}

void Locker::unlock(std::string unlock_pass)
{
    if (unlock_pass == password)
    {
        memset(&password[0], 0, password.size());
        password.clear();
    }
}