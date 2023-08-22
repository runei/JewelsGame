#include "Timer.hpp"

// Constructor
Timer::Timer() : m_duration(0.0), m_startTime(Clock::now()) {}

// Start the timer with a specified duration
void Timer::start(double duration) {
    m_startTime = Clock::now();
    m_duration = duration;
}

// Check if the timer is still running
bool Timer::isRunning() const {
    return calculateElapsedTime() < m_duration;
}

// Get the remaining time on the timer
double Timer::getTimeRemaining() const {
    return m_duration - calculateElapsedTime();
}

// Add time to the current timer duration
void Timer::addTime(double duration) {
    m_duration += duration;
}

// Stop the timer
void Timer::stop() {
    m_duration = 0.0;
}

// Calculate the elapsed time since start
double Timer::calculateElapsedTime() const {
    return std::chrono::duration<double>(Clock::now() - m_startTime).count();
}
