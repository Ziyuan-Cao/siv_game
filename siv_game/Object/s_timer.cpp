#include <windows.h>
#include "s_timer.h"

s_timer::s_timer()
: second_count(0.0), detail_time_count(-1.0), base_time(0), 
  paused_time(0), prev_time(0), current_time(0), is_stop(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	second_count = 1.0 / (double)countsPerSec;
}

// Returns the total time elapsed since reset() was called, NOT counting any
// time when the clock is stopped.
float s_timer::total_time()const
{
	// If we are stopped, do not count the time that has passed since we stopped.
	// Moreover, if we previously already had a pause, the distance 
	// stop_time - base_time includes paused time, which we do not want to count.
	// To correct this, we can subtract the paused time from stop_time:  
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  base_time       stop_time        startTime     stop_time    current_time

	if( is_stop )
	{
		return (float)(((stop_time - paused_time)-base_time)*second_count);
	}

	// The distance current_time - base_time includes paused time,
	// which we do not want to count.  To correct this, we can subtract 
	// the paused time from current_time:  
	//
	//  (current_time - paused_time) - base_time 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	//  base_time       stop_time        startTime     current_time
	
	else
	{
		return (float)(((current_time-paused_time)-base_time)*second_count);
	}
}

float s_timer::detail_time()const
{
	return (float)detail_time_count;
}

void s_timer::reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	base_time = currTime;
	prev_time = currTime;
	stop_time = 0;
	is_stop  = false;
}

void s_timer::start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  base_time       stop_time        startTime     

	if( is_stop )
	{
		paused_time += (startTime - stop_time);	

		prev_time = startTime;
		stop_time = 0;
		is_stop  = false;
	}
}

void s_timer::stop()
{
	if( !is_stop )
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		stop_time = currTime;
		is_stop  = true;
	}
}

void s_timer::tick()
{
	if( is_stop )
	{
		detail_time_count = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	current_time = currTime;

	// Time difference between this frame and the previous.
	detail_time_count = (current_time - prev_time)*second_count;

	// Prepare for next frame.
	prev_time = current_time;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then detail_time_count can be negative.
	if(detail_time_count < 0.0)
	{
		detail_time_count = 0.0;
	}
}

