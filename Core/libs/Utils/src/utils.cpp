#include "utils.h"
#include "string.h"
#include "stdio.h"


int Utils::split(char *data, char *delimetr, char **splitedDataBuf, int maxBufRows)
{
    const char *start = data;
    const char *foundStr;
    int segmentLength;
    int rows = 0;

    while ((foundStr = strstr(start, delimetr)) != NULL && rows < maxBufRows)
    {
        segmentLength = foundStr - start;
        memcpy(splitedDataBuf[rows], start, sizeof(char) * segmentLength);
        splitedDataBuf[rows][segmentLength] = '\0';        
        start = foundStr + strlen(delimetr);
        rows++;
    }
    return rows;
}