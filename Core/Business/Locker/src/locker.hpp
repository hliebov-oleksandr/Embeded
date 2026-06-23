#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "ssd1306.h"
#include <iostream>
#include <vector>

class Locker {
    private:
        std::string password;                

    public:
        Locker();        
        bool isLocked();
        void lock(std::string password);
        void unlock(std::string unlock_pass);
};