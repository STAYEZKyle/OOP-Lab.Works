#include "TaxpayerWithPropertyDeduction.h"
#include <iostream>
#include <algorithm>

using namespace std;

double TaxpayerWithPropertyDeduction::getPropertyCost() const {
    return property_cost;
}

double TaxpayerWithPropertyDeduction::getDeductionAmount() const {
    return deduction_amount;
}

double TaxpayerWithPropertyDeduction::getUsedDeduction() const {
    return used_deduction;
}

double TaxpayerWithPropertyDeduction::getAvailableDeduction() const {
    return deduction_amount - used_deduction;
}

double TaxpayerWithPropertyDeduction::getRefundedTax() const {
    return refunded_tax;
}

TaxpayerWithPropertyDeduction::TaxpayerWithPropertyDeduction(
    const char* inn, int year,
    double taxable_income, double non_taxable_income,
    double property_cost)
    : Taxpayer(inn, year, taxable_income, non_taxable_income),
    property_cost(property_cost), deduction_amount(0.0),
    used_deduction(0.0), refunded_tax(0.0) {
    validateIncome(property_cost);
    calculateDeduction();
    calculateTax();
}

TaxpayerWithPropertyDeduction::TaxpayerWithPropertyDeduction(const TaxpayerWithPropertyDeduction& other)
    : Taxpayer(other), property_cost(other.property_cost),
    deduction_amount(other.deduction_amount), used_deduction(other.used_deduction),
    refunded_tax(other.refunded_tax) {
}

TaxpayerWithPropertyDeduction& TaxpayerWithPropertyDeduction::operator=(const TaxpayerWithPropertyDeduction& other) {
    if (this != &other) {
        Taxpayer::operator=(other);
        property_cost = other.property_cost;
        deduction_amount = other.deduction_amount;
        used_deduction = other.used_deduction;
        refunded_tax = other.refunded_tax;
    }
    return *this;
}

TaxpayerWithPropertyDeduction::~TaxpayerWithPropertyDeduction() {}

void TaxpayerWithPropertyDeduction::calculateDeduction() {
    const double MAX_PROPERTY_COST_FOR_DEDUCTION = 2000000.0;
    const double DEDUCTION_RATE = 0.13;

    double cost_for_calculation = property_cost;
    if (cost_for_calculation > MAX_PROPERTY_COST_FOR_DEDUCTION) {
        cost_for_calculation = MAX_PROPERTY_COST_FOR_DEDUCTION;
    }

    deduction_amount = cost_for_calculation * DEDUCTION_RATE;
}

void TaxpayerWithPropertyDeduction::setPropertyCost(double cost) {
    validateIncome(cost);
    property_cost = cost;
    calculateDeduction();
    calculateTax();

    cout << "Стоимость жилья установлена: " << property_cost << endl;
    cout << "Доступный налоговый вычет: " << deduction_amount << endl;
}

void TaxpayerWithPropertyDeduction::addIncome(double amount, bool isTaxable) {
    Taxpayer::addIncome(amount, isTaxable);

    if (isTaxable && getAvailableDeduction() > 0) {
        calculateTax();
    }
}

void TaxpayerWithPropertyDeduction::applyDeduction(double amount) {
    if (amount < 0) {
        throw invalid_argument("Сумма вычета не может быть отрицательной");
    }

    double available = getAvailableDeduction();
    if (amount > available) {
        cout << "Внимание: запрошенная сумма вычета (" << amount
            << ") превышает доступную (" << available
            << "). Будет применен максимальный доступный вычет." << endl;
        amount = available;
    }

    used_deduction += amount;
    refunded_tax += amount;
    calculateTax();

    cout << "Применен налоговый вычет в размере: " << amount << endl;
    cout << "Использовано вычета всего: " << used_deduction << " из " << deduction_amount << endl;
    cout << "Возвращено налогов: " << refunded_tax << endl;
}

double TaxpayerWithPropertyDeduction::getNonRefundableTax() const {
    return tax_amount;
}

void TaxpayerWithPropertyDeduction::calculateTax() {
    double base_tax = taxable_income * TAX_RATE;

    double available_deduction = deduction_amount - used_deduction;

    if (available_deduction > 0 && base_tax > 0) {
        double deduction_to_apply = min(available_deduction, base_tax);
        tax_amount = base_tax - deduction_to_apply;
        used_deduction += deduction_to_apply;
    }
    else {
        tax_amount = base_tax;
    }

    total_income = taxable_income + non_taxable_income - tax_amount;
}

void TaxpayerWithPropertyDeduction::printInfo() const {
    Taxpayer::printInfo();

    cout << "=== Информация об имущественном вычете ===" << endl;
    cout << "Стоимость жилья: " << property_cost << endl;
    cout << "Доступный налоговый вычет: " << deduction_amount << endl;
    cout << "Использовано вычета: " << used_deduction << endl;
    cout << "Остаток вычета: " << getAvailableDeduction() << endl;
    cout << "Возвращено налогов (получено вычета): " << refunded_tax << endl;
    cout << "Налог, не подлежащий возврату: " << getNonRefundableTax() << endl;
}

void TaxpayerWithPropertyDeduction::printTaxInfo() const {
    cout << "\n=== Информация о налоге (с учетом вычета) ===" << endl;
    cout << "Налог, не подлежащий возврату: " << getNonRefundableTax() << endl;
    cout << "Возвращено налогов (получено вычета): " << refunded_tax << endl;
    cout << "Общий доход после уплаты налогов: " << total_income << endl;
}