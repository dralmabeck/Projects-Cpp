#include "format.h"

// DONE - TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) {
     
     // Calculate hours, minutes and seconds
     int hh = seconds / 3600;
     int mm = (seconds / 60) % 60;
     int ss = seconds % 60;

     // Stringstreams to store results as strings
     stringstream hours, mins, secs;

     // Convert integer numbers to stringstreams and pad with leading zeros if necessary
     hours << setw(2) << setfill('0') << hh;
     mins << setw(2) << setfill('0') << mm;
     secs << setw(2) << setfill('0') << ss;

     // Construct string of final result in form of HH:MM:SS
     string result = hours.str() + ":" + mins.str() + ":" + secs.str();

     return result;
}