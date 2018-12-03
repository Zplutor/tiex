#pragma once

#include <ctime>

namespace tiex {
namespace internal {

class Time {
public:
    Time(std::time_t timet) : timet_(timet), has_tm_(false) {
        tm_ = { 0 };
    }
    
    std::time_t GetTimet() const {
        return timet_;
    }
    
    const std::tm* GetTm() const {
        
        if (has_tm_) {
            return &tm_;
        }
        
        auto tm = std::localtime(&timet_);
        if (tm != nullptr) {
            
            tm_ = *tm;
            has_tm_ = true;
            return &tm_;
        }
        
        return nullptr;
    }

private:
    std::time_t timet_;
    mutable bool has_tm_;
    mutable std::tm tm_;
};
    
}
}
