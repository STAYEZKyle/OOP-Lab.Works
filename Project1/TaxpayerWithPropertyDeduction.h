#pragma once
#include "Taxpayer.h"

template<typename MoneyType = MoneyWithKopecks, int TaxPercent = 13>
class TaxpayerWithPropertyDeduction : public Taxpayer<MoneyType, TaxPercent> {
private:
    MoneyType property_cost;
    MoneyType deduction_amount;
    MoneyType used_deduction;
    MoneyType refunded_tax;

    void calculateDeduction();

public:

    TaxpayerWithPropertyDeduction(const char* inn, int year,
        MoneyType taxable_income = MoneyType(0.0),
        MoneyType non_taxable_income = MoneyType(0.0),
        MoneyType property_cost = MoneyType(0.0));

    TaxpayerWithPropertyDeduction(const TaxpayerWithPropertyDeduction& other);
    TaxpayerWithPropertyDeduction& operator=(const TaxpayerWithPropertyDeduction& other);
    virtual ~TaxpayerWithPropertyDeduction() override;

  
    void setPropertyCost(MoneyType cost);

    MoneyType getPropertyCost() const { return property_cost; }
    MoneyType getDeductionAmount() const { return deduction_amount; }
    MoneyType getUsedDeduction() const { return used_deduction; }
    MoneyType getAvailableDeduction() const { return deduction_amount - used_deduction; }
    MoneyType getRefundedTax() const { return refunded_tax; }

 
    void addIncome(MoneyType amount, bool isTaxable) override;
    void applyDeduction(MoneyType amount);
    virtual void printInfo() const override;
    virtual void printTaxInfo() const override;
    virtual double getNonRefundableTax() const override;

private:
 
    virtual void calculateTax() override;
};


template<typename MoneyType, int TaxPercent>
TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>::TaxpayerWithPropertyDeduction(
    const char* inn, int year, MoneyType taxable_income, MoneyType non_taxable_income, MoneyType property_cost)
    : Taxpayer<MoneyType, TaxPercent>(inn, year, taxable_income, non_taxable_income),
    property_cost(property_cost), deduction_amount(MoneyType(0)),
    used_deduction(MoneyType(0)), refunded_tax(MoneyType(0)) {
    this->validateIncome(property_cost);
    calculateDeduction();
    this->calculateTax();
}

template<typename MoneyType, int TaxPercent>
TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>::TaxpayerWithPropertyDeduction(
    const TaxpayerWithPropertyDeduction& other)
    : Taxpayer<MoneyType, TaxPercent>(other), property_cost(other.property_cost),
    deduction_amount(other.deduction_amount), used_deduction(other.used_deduction),
    refunded_tax(other.refunded_tax) {
}

template<typename MoneyType, int TaxPercent>
TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>&
TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>::operator=(const TaxpayerWithPropertyDeduction& other) {
    if (this != &other) {
        Taxpayer<MoneyType, TaxPercent>::operator=(other);
        property_cost = other.property_cost;
        deduction_amount = other.deduction_amount;
        used_deduction = other.used_deduction;
        refunded_tax = other.refunded_tax;
    }
    return *this;
}

template<typename MoneyType, int TaxPercent>
TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>::~TaxpayerWithPropertyDeduction() {
}

template<typename MoneyType, int TaxPercent>
void TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>::calculateDeduction() {
   
    const MoneyType MAX_PROPERTY_COST_FOR_DEDUCTION(2000000.0);
    const double DEDUCTION_RATE = 0.13;

    MoneyType cost_for_calculation = property_cost;
    if (cost_for_calculation > MAX_PROPERTY_COST_FOR_DEDUCTION) {
        cost_for_calculation = MAX_PROPERTY_COST_FOR_DEDUCTION;
    }

    deduction_amount = cost_for_calculation * DEDUCTION_RATE;
}
template<typename MoneyType, int TaxPercent>
void TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>::setPropertyCost(MoneyType cost) {
    this->validateIncome(cost);
    property_cost = cost;
    calculateDeduction();
    this->calculateTax();

    std::cout << "Стоимость жилья установлена: " << property_cost << std::endl;
    std::cout << "Доступный налоговый вычет: " << deduction_amount << std::endl;
}

template<typename MoneyType, int TaxPercent>
void TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>::addIncome(MoneyType amount, bool isTaxable) {
    Taxpayer<MoneyType, TaxPercent>::addIncome(amount, isTaxable);

    if (isTaxable && getAvailableDeduction() > MoneyType(0)) {
        this->calculateTax();
    }
}

template<typename MoneyType, int TaxPercent>
void TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>::applyDeduction(MoneyType amount) {
    if (amount < MoneyType(0)) {
        throw std::invalid_argument("Сумма вычета не может быть отрицательной");
    }

    MoneyType available = getAvailableDeduction();
    if (amount > available) {
        std::cout << "Внимание: запрошенная сумма вычета (" << amount
            << ") превышает доступную (" << available
            << "). Будет применен максимальный доступный вычет." << std::endl;
        amount = available;
    }

    used_deduction += amount;
    refunded_tax += amount;
    this->calculateTax();

    std::cout << "Применен налоговый вычет в размере: " << amount << std::endl;
    std::cout << "Использовано вычета всего: " << used_deduction << " из " << deduction_amount << std::endl;
    std::cout << "Возвращено налогов: " << refunded_tax << std::endl;
}

template<typename MoneyType, int TaxPercent>
double TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>::getNonRefundableTax() const {
    return static_cast<double>(this->tax_amount);
}

template<typename MoneyType, int TaxPercent>
void TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>::calculateTax() {
    MoneyType base_tax = this->taxable_income * this->TAX_RATE;

    MoneyType available_deduction = deduction_amount - used_deduction;

    if (available_deduction > MoneyType(0) && base_tax > MoneyType(0)) {
        MoneyType deduction_to_apply = (available_deduction < base_tax) ? available_deduction : base_tax;
        this->tax_amount = base_tax - deduction_to_apply;
        used_deduction += deduction_to_apply;
    }
    else {
        this->tax_amount = base_tax;
    }

    this->total_income = this->taxable_income + this->non_taxable_income - this->tax_amount;
}

template<typename MoneyType, int TaxPercent>
void TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>::printInfo() const {
    Taxpayer<MoneyType, TaxPercent>::printInfo();

    std::cout << "\n=== Информация об имущественном вычете ===" << std::endl;
    std::cout << "Стоимость жилья: " << property_cost << std::endl;
    std::cout << "Доступный налоговый вычет: " << deduction_amount << std::endl;
    std::cout << "Использовано вычета: " << used_deduction << std::endl;
    std::cout << "Остаток вычета: " << getAvailableDeduction() << std::endl;
    std::cout << "Возвращено налогов (получено вычета): " << refunded_tax << std::endl;
    std::cout << "Налог, не подлежащий возврату: " << this->tax_amount << std::endl;
}

template<typename MoneyType, int TaxPercent>
void TaxpayerWithPropertyDeduction<MoneyType, TaxPercent>::printTaxInfo() const {
    std::cout << "\n=== Информация о налоге (с учетом вычета) ===" << std::endl;
    std::cout << "Налог, не подлежащий возврату: " << this->tax_amount << std::endl;
    std::cout << "Возвращено налогов (получено вычета): " << refunded_tax << std::endl;
    std::cout << "Общий доход после уплаты налогов: " << this->total_income << std::endl;
}