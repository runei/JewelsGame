#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer {
public:
    // Constructor
    Timer();

    // Start the timer with a specified duration
    void start(double duration);

    // Add time to the current timer duration
    void addTime(double duration);

    // Check if the timer is still running
    bool isRunning() const;

    // Get the remaining time on the timer
    double getTimeRemaining() const;

    // Stop the timer
    void stop();

private:
    using Clock = std::chrono::high_resolution_clock;

    // Start time of the timer
    std::chrono::high_resolution_clock::time_point m_startTime;

    // Duration of the timer
    double m_duration;

    // Calculate the elapsed time since start
    double calculateElapsedTime() const;
};

#endif // TIMER_HPP

