#ifndef DATETIME_HPP
#define DATETIME_HPP

#include <ctime>
#include <string>

class Datetime
{
public:
    Datetime() = default;
    Datetime(std::time_t epoch)
        : time_(epoch)
    { }

    std::string dump(void) const
    {
        return std::string{std::asctime(std::localtime(&time_))};
    }

    void set(std::time_t epoch)
    {
        time_ = epoch;
    }

    time_t epoch(void) const
    {
        return time_;
    }

    bool operator==(const Datetime& d)
    {
        return time_ == d.time_;
    }

    bool operator!=(const Datetime& d)
    {
        return time_ != d.time_;
    }

    bool operator<=(const Datetime& d)
    {
        return time_ <= d.time_;
    }

    bool operator<(const Datetime& d)
    {
        return time_ < d.time_;
    }

    bool operator>=(const Datetime& d)
    {
        return time_ >= d.time_;
    }

    bool operator>(const Datetime& d)
    {
        return time_ > d.time_;
    }

private:
    time_t time_ = 0;
};

#endif
