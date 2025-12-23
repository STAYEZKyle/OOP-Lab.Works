#pragma once
#include <iostream>
#include <iomanip>
#include <type_traits>

template<typename T>
class Money {
private:
    T amount;  

public:
    Money() : amount(0) {}
    explicit Money(T value) : amount(value) {}


    Money(const Money& other) : amount(other.amount) {}

   
    Money& operator=(const Money& other) {
        if (this != &other) {
            amount = other.amount;
        }
        return *this;
    }

    
    operator T() const { return amount; }

 
    Money operator+(const Money& other) const { return Money(amount + other.amount); }
    Money operator-(const Money& other) const { return Money(amount - other.amount); }
    Money operator*(double factor) const { return Money(static_cast<T>(amount * factor)); }
    Money operator/(double divisor) const { return Money(static_cast<T>(amount / divisor)); }

  
    bool operator<(const Money& other) const { return amount < other.amount; }
    bool operator>(const Money& other) const { return amount > other.amount; }
    bool operator<=(const Money& other) const { return amount <= other.amount; }
    bool operator>=(const Money& other) const { return amount >= other.amount; }
    bool operator==(const Money& other) const { return amount == other.amount; }
    bool operator!=(const Money& other) const { return amount != other.amount; }

  
    Money& operator+=(const Money& other) { amount += other.amount; return *this; }
    Money& operator-=(const Money& other) { amount -= other.amount; return *this; }
    Money& operator*=(double factor) { amount = static_cast<T>(amount * factor); return *this; }
    Money& operator/=(double divisor) { amount = static_cast<T>(amount / divisor); return *this; }

    
    friend std::ostream& operator<<(std::ostream& os, const Money& money) {
        if constexpr (std::is_integral_v<T>) {
        
            os << money.amount << " руб.";
        }
        else if constexpr (std::is_floating_point_v<T>) {
       
            os << std::fixed << std::setprecision(2) << money.amount << " руб.";
        }
        else {
            os << money.amount;
        }
        return os;
    }
};


using MoneyWithKopecks = Money<double>;    
using MoneyWithoutKopecks = Money<int>;
