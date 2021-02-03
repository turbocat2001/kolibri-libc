#include <time.h>
#include <ksys.h>

struct tm buffertime;

struct tm * localtime (const time_t * timer)
/* non-standard!  ignore parameter and return just time now */
{
	int kos_date, kos_time;
    kos_date = _ksys_get_date();
    kos_time = _ksys_get_clock();
   
    int bcd_day = (kos_date >> 16);
    int bcd_mon = ((kos_date & 0xFF00) >> 8);
    int bcd_year = (kos_date & 0xFF);
    buffertime.tm_mday = ((bcd_day & 0xF0)>>4)*10 + (bcd_day & 0x0F);
    buffertime.tm_mon = ((bcd_mon & 0xF0)>>4)*10 + (bcd_mon & 0x0F) - 1;
    buffertime.tm_year = ((bcd_year & 0xF0)>>4)*10 + (bcd_year & 0x0F) + 100;
   
    buffertime.tm_wday = buffertime.tm_yday = buffertime.tm_isdst = -1; /* temporary */
   
    int bcd_sec = (kos_time >> 16);
    int bcd_min = ((kos_time & 0xFF00) >> 8);
    int bcd_hour = (kos_time & 0xFF);

    buffertime.tm_sec = ((bcd_sec & 0xF0)>>4)*10 + (bcd_sec & 0x0F);
    buffertime.tm_min = ((bcd_min & 0xF0)>>4)*10 + (bcd_min & 0x0F);
    buffertime.tm_hour = ((bcd_hour & 0xF0)>>4)*10 + (bcd_hour & 0x0F);

	return &buffertime;
}

time_t time (time_t* timer)
{
	time_t t = mktime(localtime(0));
	
	if (timer) *timer = t;
	
	return t;
}

time_t mktime (struct tm * timeptr)
{
    int utcdiff = -3;
    const int mon_days [] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    unsigned long int tyears, tdays, leaps, utc_hrs;
    int i;
    
    tyears = timeptr->tm_year - 70 ;// tm->tm_year is from 1900.
    leaps = (tyears + 2) / 4; // no of next two lines until year 2100.
    i = (timeptr->tm_year - 100) / 100;
    leaps -= ( (i/4)*3 + i%4 );
    tdays = 0;
    for (i=0; i < timeptr->tm_mon; i++) tdays += mon_days[i];
    
    tdays += timeptr->tm_mday-1; // days of month passed.
    tdays = tdays + (tyears * 365) + leaps;

    utc_hrs = timeptr->tm_hour + utcdiff; // for your time zone.
    return (tdays * 86400) + (utc_hrs * 3600) + (timeptr->tm_min * 60) + timeptr->tm_sec;
}

double difftime (time_t end, time_t beginning)
{
	return end - beginning;
}
