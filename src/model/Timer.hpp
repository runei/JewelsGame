#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer {
public:
    Timer();
    void start(double duration);
    bool isRunning() const;
    double getTimeRemaining() const;
    void stop();

private:
    std::chrono::high_resolution_clock::time_point m_startTime;
    double m_duration;
};

#endif // TIMER_HPP
