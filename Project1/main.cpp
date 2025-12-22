#include <iostream>
#include <locale>
#include <limits>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "Taxpayer.h"
#include "TaxpayerWithPropertyDeduction.h"

using namespace std;

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void demonstratePolymorphism() {
    cout << "\n=== Демонстрация полиморфизма (массив указателей) ===" << endl;

    const int ARRAY_SIZE = 4;
    ITaxable* taxpayers[ARRAY_SIZE];

    taxpayers[0] = new Taxpayer("111111111111", 2024, 500000, 100000);
    taxpayers[1] = new TaxpayerWithPropertyDeduction("222222222222", 2024, 1000000, 200000, 3000000);
    taxpayers[2] = new Taxpayer("333333333333", 2024, 300000, 50000);
    taxpayers[3] = new TaxpayerWithPropertyDeduction("444444444444", 2024, 800000, 150000, 1500000);

    TaxpayerWithPropertyDeduction* deducible1 = dynamic_cast<TaxpayerWithPropertyDeduction*>(taxpayers[1]);
    if (deducible1) {
        deducible1->applyDeduction(100000);
    }

    TaxpayerWithPropertyDeduction* deducible3 = dynamic_cast<TaxpayerWithPropertyDeduction*>(taxpayers[3]);
    if (deducible3) {
        deducible3->applyDeduction(50000);
    }

    cout << "\n=== Налоги, не подлежащие возврату (по всем объектам) ===" << endl;
    double totalNonRefundableTax = 0.0;

    for (int i = 0; i < ARRAY_SIZE; i++) {
        cout << "\nНалогоплательщик " << i + 1 << ":" << endl;
        taxpayers[i]->printTaxInfo();
        totalNonRefundableTax += taxpayers[i]->getNonRefundableTax();
    }

    cout << "\n=== Итоговая статистика ===" << endl;
    cout << "Общая сумма налогов, не подлежащих возврату: " << totalNonRefundableTax << endl;

    for (int i = 0; i < ARRAY_SIZE; i++) {
        delete taxpayers[i];
    }
}

void demonstrateSmartPointers() {
    cout << "\n=== Демонстрация работы с вектором умных указателей ===" << endl;

    vector<unique_ptr<ITaxable>> taxpayerVector;

    taxpayerVector.push_back(make_unique<Taxpayer>("555555555555", 2024, 400000, 80000));
    taxpayerVector.push_back(make_unique<TaxpayerWithPropertyDeduction>("666666666666", 2024, 1200000, 300000, 2500000));
    taxpayerVector.push_back(make_unique<Taxpayer>("777777777777", 2024, 250000, 40000));
    taxpayerVector.push_back(make_unique<TaxpayerWithPropertyDeduction>("888888888888", 2024, 900000, 180000, 1800000));

    for (auto& taxpayer : taxpayerVector) {
        auto deducible = dynamic_cast<TaxpayerWithPropertyDeduction*>(taxpayer.get());
        if (deducible) {
            deducible->applyDeduction(30000 + rand() % 70000);
        }
    }

    cout << "\n=== Вывод через интерфейс ITaxable ===" << endl;
    for (size_t i = 0; i < taxpayerVector.size(); i++) {
        cout << "\nЭлемент " << i + 1 << ":" << endl;
        taxpayerVector[i]->printTaxInfo();
    }

    double total = 0.0;
    for (const auto& taxpayer : taxpayerVector) {
        total += taxpayer->getNonRefundableTax();
    }
    cout << "\nОбщая сумма не возвращаемых налогов: " << total << endl;
}

void createTaxpayerCollection() {
    cout << "\n=== Создание коллекции налогоплательщиков ===" << endl;

    int count;
    cout << "Сколько налогоплательщиков создать? ";
    cin >> count;
    clearInputBuffer();

    if (count <= 0) {
        cout << "Неверное количество." << endl;
        return;
    }

    vector<unique_ptr<Taxpayer>> collection;

    for (int i = 0; i < count; i++) {
        cout << "\n=== Налогоплательщик " << i + 1 << " ===" << endl;

        char type;
        cout << "Тип налогоплательщика (1 - обычный, 2 - с имущественным вычетом): ";
        cin >> type;
        clearInputBuffer();

        char inn[13];
        int year;
        double taxable_income, non_taxable_income;
        cout << "Введите ИНН (12 цифр): ";
        cin.getline(inn, 13);

        cout << "Введите год: ";
        cin >> year;

        cout << "Введите налогооблагаемый доход: ";
        cin >> taxable_income;

        cout << "Введите неналогооблагаемый доход: ";
        cin >> non_taxable_income;
        clearInputBuffer();
        if (type == '2') {
            double property_cost;
            cout << "Введите стоимость жилья для налогового вычета: ";
            cin >> property_cost;
            clearInputBuffer();

            collection.push_back(make_unique<TaxpayerWithPropertyDeduction>(
                inn, year, taxable_income, non_taxable_income, property_cost));
        }
        else {
            collection.push_back(make_unique<Taxpayer>(
                inn, year, taxable_income, non_taxable_income));
        }
    }

    cout << "\n=== Информация о всех налогоплательщиках ===" << endl;
    for (size_t i = 0; i < collection.size(); i++) {
        cout << "\n--- Налогоплательщик " << i + 1 << " ---" << endl;
        collection[i]->printInfo();
        collection[i]->printTaxInfo();
    }

    cout << "\n=== Анализ через интерфейс ITaxable ===" << endl;
    double totalTax = 0.0;
    int withDeduction = 0;

    for (const auto& taxpayer : collection) {
        totalTax += taxpayer->getNonRefundableTax();

        if (dynamic_cast<TaxpayerWithPropertyDeduction*>(taxpayer.get())) {
            withDeduction++;
        }
    }

    cout << "Всего налогоплательщиков: " << collection.size() << endl;
    cout << "С имущественным вычетом: " << withDeduction << endl;
    cout << "Без имущественного вычета: " << collection.size() - withDeduction << endl;
    cout << "Общая сумма налогов, не подлежащих возврату: " << totalTax << endl;
    cout << "Средний налог на одного налогоплательщика: " << (collection.empty() ? 0 : totalTax / collection.size()) << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned>(time(nullptr)));

    try {
        demonstratePolymorphism();
        demonstrateSmartPointers();
        createTaxpayerCollection();

        cout << "\nПрограмма завершена." << endl;

    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
