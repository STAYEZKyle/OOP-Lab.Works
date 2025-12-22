#pragma once
#include "Taxpayer.h"

class TaxpayerWithPropertyDeduction : public Taxpayer {
private:
    double property_cost;
    double deduction_amount;
    double used_deduction;
    double refunded_tax;

    void calculateDeduction();

public:
    TaxpayerWithPropertyDeduction(const char* inn, int year,
        double taxable_income = 0.0,
        double non_taxable_income = 0.0,
        double property_cost = 0.0);
    TaxpayerWithPropertyDeduction(const TaxpayerWithPropertyDeduction& other);
    TaxpayerWithPropertyDeduction& operator=(const TaxpayerWithPropertyDeduction& other);
    virtual ~TaxpayerWithPropertyDeduction();

    void setPropertyCost(double cost);

    double getPropertyCost() const;
    double getDeductionAmount() const;
    double getUsedDeduction() const;
    double getAvailableDeduction() const;
    double getRefundedTax() const;

    void addIncome(double amount, bool isTaxable) override;
    void applyDeduction(double amount);
    virtual void printInfo() const override;
    virtual void printTaxInfo() const override;
    virtual double getNonRefundableTax() const override;

private:
    virtual void calculateTax() override;
};