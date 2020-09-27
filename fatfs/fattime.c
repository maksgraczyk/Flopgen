#include "ff.h"
#include <time.h>

DWORD get_fattime (void)
{
	time_t timer = time(NULL);
	struct tm *time_data = localtime(&timer);
  
	return (time_data->tm_year - 80) << 25 | (time_data->tm_mon + 1) << 21 | time_data->tm_mday << 16;
}
