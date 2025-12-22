#pragma once

class ITaxable {
public:
    virtual double getNonRefundableTax() const = 0;
    virtual ~ITaxable() = default;
    virtual void printTaxInfo() const = 0;
};
