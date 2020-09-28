#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

using namespace std::chrono;

template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 

    std::unique_lock<std::mutex> lck(_mutex); // FP.5
    // Wait until an element enters the queue
    _condition.wait(lck, [this] { return !_queue.empty(); }); // FP.5
    // Pull msg from queue
    T message = std::move(_queue.front()); // FP.5
    // Clear queue
    _queue.pop_front(); // FP.5

    // Return message
    return message; // FP.5
}

template <typename T>
void MessageQueue<T>::send(T &&message)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    std::lock_guard<std::mutex> lock_guard(_mutex); // FP.4
    _queue.emplace_back(std::move(message)); // FP.4
    _condition.notify_one(); // FP.4
}


/* Implementation of class "TrafficLight" */

 
TrafficLight::TrafficLight()
{
    _currentPhase = red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.

    for(;/*ever*/;) // FP.5
    {
        if (_messages.receive() == green) // FP.5
        {
            return; // FP.5
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called.
    // To do this, use the thread queue in the base class. 

    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this)); // FP.2
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.

    // Store time at start of cycle
    high_resolution_clock::time_point start_cycle = high_resolution_clock::now(); // FP.2

    // Minimum and maximum cycle lengths in milliseconds
    double min_cycle = 4000.0; // FP.2
    double max_cycle = 6000.0; // FP.2

    // C++11 way to generate random numbers (Stackoverflow)

    // Type of random number distribution
    std::uniform_real_distribution<double> dist_cycle(min_cycle, max_cycle); // FP.2
    // Mersenne Twister random number generator
    std::mt19937 rng; // FP.2
    // See the random number generator with a random device
    rng.seed(std::random_device{}()); // FP.2

    // Randomly draw length of cycle
    double duration_cycle = dist_cycle(rng); // FP.2

    //std::cout << std::to_string(duration_cycle) << std::endl;

    for(;/*ever*/;)
    {
        // Put to sleep for 1 ms as requested
        std::this_thread::sleep_for(milliseconds(1)); // FP.2

        // Current time from high-resolution clock
        high_resolution_clock::time_point now_cycle = high_resolution_clock::now(); // FP.2
        // calculate time difference between current and start time of cycle
        double elapsed_cycle = static_cast<double>(duration_cast<milliseconds>(now_cycle - start_cycle).count()); // FP.2

        //std::cout << std::to_string(elapsed_cycle) << std::endl;

        // If cycle is over, which traffic lights
        if (elapsed_cycle > duration_cycle) // FP.2
        {
            if (_currentPhase == red) // FP.2
            {
                _currentPhase = green; // FP.2
            }
            else {
                _currentPhase = red; // FP.2
            }
            
            // Update message queue
            _messages.send(std::move(getCurrentPhase())); // FP.4

            // Set values for next cylce
            start_cycle = high_resolution_clock::now(); // FP.2
            duration_cycle = dist_cycle(rng); // FP.2
        }
    }
}
