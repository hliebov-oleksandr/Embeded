#pragma once

typedef void (*funcHandler)(char* param);

class Utils {

    public:        
        static int split(char *data, char *delimetr, char **splitedDataBuf, int maxBufRows);
};