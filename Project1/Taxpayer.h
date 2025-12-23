#pragma once
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include "ITaxable.h"
#include "Money.h"

template<typename MoneyType = MoneyWithKopecks, int TaxPercent = 13>
class Taxpayer : public ITaxable {
protected:
    static const int INN_LENGTH = 12;
    static const int MIN_YEAR = 1900;
    static const int MAX_YEAR = 2100;
    static constexpr double TAX_RATE = TaxPercent / 100.0;  

    char* inn;
    int year;
    MoneyType taxable_income;
    MoneyType non_taxable_income;
    MoneyType tax_amount;
    MoneyType total_income;

    void validateINN(const char* inn) const;
    void validateYear(int year) const;
    void validateIncome(MoneyType income) const;
    virtual void calculateTax();

public:
   
    Taxpayer(const char* i, int y, MoneyType ti = MoneyType(0.0), MoneyType nti = MoneyType(0.0));
    Taxpayer(const Taxpayer& other);
    Taxpayer& operator=(const Taxpayer& other);
    virtual ~Taxpayer();

   
    virtual void addIncome(MoneyType amount, bool isTaxable);
    void addIncomeFromNet(MoneyType net_income_after_tax);
    virtual void printInfo() const;

    virtual double getNonRefundableTax() const override;
    virtual void printTaxInfo() const override;

   
    const char* getInn() const { return inn; }
    int getYear() const { return year; }
    MoneyType getTaxableIncome() const { return taxable_income; }
    MoneyType getNonTaxableIncome() const { return non_taxable_income; }
    MoneyType getTaxAmount() const { return tax_amount; }
    MoneyType getTotalIncome() const { return total_income; }

  
    Taxpayer& operator>>(MoneyType net_income_after_tax);
};


template<typename MoneyType, int TaxPercent>
MoneyType& operator+=(MoneyType& sum, const Taxpayer<MoneyType, TaxPercent>& taxpayer) {
    sum += taxpayer.getTaxAmount();
    return sum;
}


template<typename MoneyType, int TaxPercent>
Taxpayer<MoneyType, TaxPercent>::Taxpayer(const char* i, int y, MoneyType ti, MoneyType nti) {
    validateINN(i);
    validateYear(y);
    validateIncome(ti);
    validateIncome(nti);

    inn = new char[INN_LENGTH + 1];
    strcpy_s(inn, INN_LENGTH + 1, i);
    year = y;
    taxable_income = ti;
    non_taxable_income = nti;
    calculateTax();
}

template<typename MoneyType, int TaxPercent>
Taxpayer<MoneyType, TaxPercent>::Taxpayer(const Taxpayer& other) {
    inn = new char[INN_LENGTH + 1];
    strcpy_s(inn, INN_LENGTH + 1, other.inn);
    year = other.year;
    taxable_income = other.taxable_income;
    non_taxable_income = other.non_taxable_income;
    tax_amount = other.tax_amount;
    total_income = other.total_income;
}

template<typename MoneyType, int TaxPercent>
Taxpayer<MoneyType, TaxPercent>& Taxpayer<MoneyType, TaxPercent>::operator=(const Taxpayer& other) {
    if (this != &other) {
        delete[] inn;
        inn = new char[INN_LENGTH + 1];
        strcpy_s(inn, INN_LENGTH + 1, other.inn);
        year = other.year;
        taxable_income = other.taxable_income;
        non_taxable_income = other.non_taxable_income;
        tax_amount = other.tax_amount;
        total_income = other.total_income;
    }
    return *this;
}

template<typename MoneyType, int TaxPercent>
Taxpayer<MoneyType, TaxPercent>::~Taxpayer() {
    delete[] inn;
}

template<typename MoneyType, int TaxPercent>
void Taxpayer<MoneyType, TaxPercent>::validateINN(const char* inn) const {
    if (!inn) {
        throw std::invalid_argument("ИНН не может быть nullptr");
    }

    size_t length = 0;
    while (inn[length] != '\0') {
        length++;
    }

    if (length != INN_LENGTH) {
        throw std::invalid_argument("ИНН должен содержать 12 символов");
    }

    for (int i = 0; i < INN_LENGTH; ++i) {
        if (inn[i] < '0' || inn[i] > '9') {
            throw std::invalid_argument("ИНН должен содержать только цифры");
        }
    }
}

template<typename MoneyType, int TaxPercent>
void Taxpayer<MoneyType, TaxPercent>::validateYear(int year) const {
    if (year < MIN_YEAR || year > MAX_YEAR) {
        throw std::invalid_argument("Некорректный год");
    }
}

template<typename MoneyType, int TaxPercent>
void Taxpayer<MoneyType, TaxPercent>::validateIncome(MoneyType income) const {
    if (income < MoneyType(0)) {
        throw std::invalid_argument("Доход не может быть отрицательным");
    }
}

template<typename MoneyType, int TaxPercent>
void Taxpayer<MoneyType, TaxPercent>::calculateTax() {
    tax_amount = taxable_income * TAX_RATE;
    total_income = taxable_income + non_taxable_income - tax_amount;
}

template<typename MoneyType, int TaxPercent>
void Taxpayer<MoneyType, TaxPercent>::addIncome(MoneyType amount, bool isTaxable) {
    validateIncome(amount);
    if (isTaxable) {
        taxable_income += amount;
    }
    else {
        non_taxable_income += amount;
    }
    calculateTax();
}

template<typename MoneyType, int TaxPercent>
void Taxpayer<MoneyType, TaxPercent>::addIncomeFromNet(MoneyType net_income_after_tax) {
    validateIncome(net_income_after_tax);

 
    MoneyType gross_income = net_income_after_tax / (1 - TAX_RATE);

    taxable_income += gross_income;
    calculateTax();

    std::cout << "Добавлен доход после вычета налога: " << net_income_after_tax << std::endl;
    std::cout << "Рассчитанный налогооблагаемый доход: " << gross_income << std::endl;
    std::cout << "Уплаченный налог с этой суммы: " << gross_income * TAX_RATE << std::endl;
}

template<typename MoneyType, int TaxPercent>
double Taxpayer<MoneyType, TaxPercent>::getNonRefundableTax() const {

    return static_cast<double>(tax_amount);
}

template<typename MoneyType, int TaxPercent>
Taxpayer<MoneyType, TaxPercent>& Taxpayer<MoneyType, TaxPercent>::operator>>(MoneyType net_income_after_tax) {
    addIncomeFromNet(net_income_after_tax);
    return *this;
}

template<typename MoneyType, int TaxPercent>
void Taxpayer<MoneyType, TaxPercent>::printInfo() const {
    std::cout << "ИНН: " << inn << std::endl;
    std::cout << "Год: " << year << std::endl;
    std::cout << "Налогооблагаемый доход: " << taxable_income << std::endl;
    std::cout << "Неналогооблагаемый доход: " << non_taxable_income << std::endl;
    std::cout << "Налог (" << TaxPercent << "%): " << tax_amount << std::endl;
    std::cout << "Доход после уплаты налога: " << total_income << std::endl;
}

template<typename MoneyType, int TaxPercent>
void Taxpayer<MoneyType, TaxPercent>::printTaxInfo() const {
    std::cout << "\n=== Информация о налоге (" << TaxPercent << "%) ===" << std::endl;
    std::cout << "Налог, не подлежащий возврату: " << tax_amount << std::endl;
    std::cout << "Общий доход после уплаты налогов: " << total_income << std::endl;
}
