#pragma once
#include <cstring>
#include <stdexcept>
#include "ITaxable.h"

class Taxpayer : public ITaxable {
private:
    static const int INN_LENGTH = 12;
    static const int MIN_YEAR = 1900;
    static const int MAX_YEAR = 2100;

protected:
    static const double TAX_RATE;

    char* inn;
    int year;
    double taxable_income;
    double non_taxable_income;
    double tax_amount;
    double total_income;

    void validateINN(const char* inn) const;
    void validateYear(int year) const;
    void validateIncome(double income) const;
    virtual void calculateTax();

public:
    Taxpayer(const char* inn, int year, double taxable_income = 0.0, double non_taxable_income = 0.0);
    Taxpayer(const Taxpayer& other);
    Taxpayer& operator=(const Taxpayer& other);
    virtual ~Taxpayer();

    virtual void addIncome(double amount, bool isTaxable);
    void addIncomeFromNet(double net_income_after_tax);
    virtual void printInfo() const;
    virtual double getNonRefundableTax() const override;
    virtual void printTaxInfo() const override;


    const char* getInn() const;
    int getYear() const;
    double getTaxableIncome() const;
    double getNonTaxableIncome() const;
    double getTaxAmount() const;
    double getTotalIncome() const;

    Taxpayer& operator>>(double net_income_after_tax);
};

double& operator+=(double& sum, const Taxpayer& taxpayer);

