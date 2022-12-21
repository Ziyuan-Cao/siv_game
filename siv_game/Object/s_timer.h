#pragma once

class s_timer
{
private:
	s_timer();
	s_timer(const s_timer&) {};
	s_timer& operator=(const s_timer&) {};
public:

	static s_timer* get_instance()
	{
		static s_timer timer_instance;
		return &timer_instance;
	}


	float total_time()const; // total_time without stop time in seconds
	float detail_time()const; // Time difference between this frame and the previous. in seconds

	void reset(); // Call before message loop.
	void start(); // Call when unpaused.
	void stop();  // Call when paused.
	void tick();  // Call every frame.

private:
	
	double second_count;
	double detail_time_count; 

	__int64 base_time;
	__int64 paused_time;
	__int64 stop_time;
	__int64 prev_time;
	__int64 current_time;

	bool is_stop;
};
