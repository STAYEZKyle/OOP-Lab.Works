#include "Taxpayer.h"
#include <iostream>

const double Taxpayer::TAX_RATE = 0.13;

const char* Taxpayer::getInn() const {
    return inn;
}

int Taxpayer::getYear() const {
    return year;
}

double Taxpayer::getTaxableIncome() const {
    return taxable_income;
}

double Taxpayer::getNonTaxableIncome() const {
    return non_taxable_income;
}

double Taxpayer::getTaxAmount() const {
    return tax_amount;
}

double Taxpayer::getTotalIncome() const {
    return total_income;
}

Taxpayer& Taxpayer::operator>>(double net_income_after_tax) {
    addIncomeFromNet(net_income_after_tax);
    return *this;
}

void Taxpayer::validateINN(const char* inn) const {
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

void Taxpayer::validateYear(int year) const {
    if (year < MIN_YEAR || year > MAX_YEAR) {
        throw std::invalid_argument("Некорректный год");
    }
}

void Taxpayer::validateIncome(double income) const {
    if (income < 0) {
        throw std::invalid_argument("Доход не может быть отрицательным");
    }
}

void Taxpayer::calculateTax() {
    tax_amount = taxable_income * TAX_RATE;
    total_income = taxable_income + non_taxable_income - tax_amount;
}

Taxpayer::Taxpayer(const char* inn, int year, double taxable_income, double non_taxable_income) {
    validateINN(inn);
    validateYear(year);
    validateIncome(taxable_income);
    validateIncome(non_taxable_income);

    this->inn = new char[INN_LENGTH + 1];
    strcpy_s(this->inn, INN_LENGTH + 1, inn);
    this->year = year;
    this->taxable_income = taxable_income;
    this->non_taxable_income = non_taxable_income;
    calculateTax();
}

Taxpayer::Taxpayer(const Taxpayer& other) {
    inn = new char[INN_LENGTH + 1];
    strcpy_s(inn, INN_LENGTH + 1, other.inn);
    year = other.year;
    taxable_income = other.taxable_income;
    non_taxable_income = other.non_taxable_income;
    tax_amount = other.tax_amount;
    total_income = other.total_income;
}

Taxpayer& Taxpayer::operator=(const Taxpayer& other) {
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

Taxpayer::~Taxpayer() {
    delete[] inn;
}

void Taxpayer::addIncome(double amount, bool isTaxable) {
    validateIncome(amount);
    if (isTaxable) {
        taxable_income += amount;
    }
    else {
        non_taxable_income += amount;
    }
    calculateTax();
}

void Taxpayer::addIncomeFromNet(double net_income_after_tax) {
    validateIncome(net_income_after_tax);

    double gross_income = net_income_after_tax / (1 - TAX_RATE);

    taxable_income += gross_income;

    calculateTax();

    std::cout << "Добавлен доход после вычета налога: " << net_income_after_tax << std::endl;
    std::cout << "Рассчитанный налогооблагаемый доход: " << gross_income << std::endl;
    std::cout << "Уплаченный налог с этой суммы: " << gross_income * TAX_RATE << std::endl;
}

double Taxpayer::getNonRefundableTax() const {
    return tax_amount;
}

void Taxpayer::printInfo() const {
    std::cout << "ИНН: " << inn << std::endl;
    std::cout << "Год: " << year << std::endl;
    std::cout << "Налогооблагаемый доход: " << taxable_income << std::endl;
    std::cout << "Неналогооблагаемый доход: " << non_taxable_income << std::endl;
    std::cout << "Доход после уплаты налога: " << total_income << std::endl;
}

void Taxpayer::printTaxInfo() const {
    std::cout << "\n=== Информация о налоге ===" << std::endl;
    std::cout << "Налог, не подлежащий возврату: " << getNonRefundableTax() << std::endl;
    std::cout << "Общий доход после уплаты налогов: " << total_income << std::endl;
}

double& operator+=(double& sum, const Taxpayer& taxpayer) {
    sum += taxpayer.getTaxAmount();
    return sum;
}