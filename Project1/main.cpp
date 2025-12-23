#include <iostream>
#include <locale>
#include <limits>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "Money.h"
#include "Taxpayer.h"
#include "TaxpayerWithPropertyDeduction.h"

using namespace std;

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void demonstrateTemplates() {
    cout << "\n=== Демонстрация шаблонов (с копейками и без копеек) ===" << endl;

 
    Taxpayer<MoneyWithKopecks, 13> taxpayerWithKopecks("123456789012", 2024,
        MoneyWithKopecks(500000.75),
        MoneyWithKopecks(100000.25));

    Taxpayer<MoneyWithoutKopecks, 13> taxpayerWithoutKopecks("987654321098", 2024,
        MoneyWithoutKopecks(500000),
        MoneyWithoutKopecks(100000));

    cout << "\n--- Налогоплательщик с копейками ---" << endl;
    taxpayerWithKopecks.printInfo();

    cout << "\n--- Налогоплательщик без копеек ---" << endl;
    taxpayerWithoutKopecks.printInfo();


    cout << "\n--- Добавление дохода с копейками ---" << endl;
    taxpayerWithKopecks.addIncome(MoneyWithKopecks(15000.50), true);
    taxpayerWithKopecks.printTaxInfo();


    cout << "\n--- Добавление дохода без копеек ---" << endl;
    taxpayerWithoutKopecks.addIncome(MoneyWithoutKopecks(15000), true);
    taxpayerWithoutKopecks.printTaxInfo();

  
    cout << "\n--- Использование оператора >> ---" << endl;
    taxpayerWithKopecks >> MoneyWithKopecks(8700.0);
    taxpayerWithKopecks.printTaxInfo();
}

void demonstrateTaxpayerWithDeductionTemplates() {
    cout << "\n=== Демонстрация производного класса с шаблонами ===" << endl;

   
    TaxpayerWithPropertyDeduction<MoneyWithKopecks, 13> tpWithKopecks(
        "111111111111", 2024,
        MoneyWithKopecks(1000000.50),
        MoneyWithKopecks(200000.75),
        MoneyWithKopecks(3000000.25));

    TaxpayerWithPropertyDeduction<MoneyWithoutKopecks, 13> tpWithoutKopecks(
        "222222222222", 2024,
        MoneyWithoutKopecks(1000000),
        MoneyWithoutKopecks(200000),
        MoneyWithoutKopecks(3000000));

    cout << "\n--- Налогоплательщик с вычетом (с копейками) ---" << endl;
    tpWithKopecks.printInfo();

    cout << "\n--- Налогоплательщик с вычетом (без копеек) ---" << endl;
    tpWithoutKopecks.printInfo();


    cout << "\n--- Применение вычета с копейками ---" << endl;
    tpWithKopecks.applyDeduction(MoneyWithKopecks(50000.50));
    tpWithKopecks.printInfo();

    cout << "\n--- Применение вычета без копеек ---" << endl;
    tpWithoutKopecks.applyDeduction(MoneyWithoutKopecks(50000));
    tpWithoutKopecks.printInfo();
}

void demonstrateDifferentTaxRates() {
    cout << "\n=== Демонстрация разных налоговых ставок ===" << endl;

  
    Taxpayer<MoneyWithKopecks, 13> taxpayer13("333333333333", 2024, MoneyWithKopecks(500000));
    Taxpayer<MoneyWithKopecks, 15> taxpayer15("444444444444", 2024, MoneyWithKopecks(500000));
    Taxpayer<MoneyWithKopecks, 20> taxpayer20("555555555555", 2024, MoneyWithKopecks(500000));

    cout << "\n--- Ставка 13% ---" << endl;
    taxpayer13.printTaxInfo();

    cout << "\n--- Ставка 15% ---" << endl;
    taxpayer15.printTaxInfo();

    cout << "\n--- Ставка 20% ---" << endl;
    taxpayer20.printTaxInfo();
}

void demonstratePolymorphismWithTemplates() {
    cout << "\n=== Демонстрация полиморфизма с шаблонами ===" << endl;

    const int ARRAY_SIZE = 4;
    ITaxable* taxpayers[ARRAY_SIZE];

  
    taxpayers[0] = new Taxpayer<MoneyWithKopecks, 13>("111111111111", 2024,
        MoneyWithKopecks(500000.75),
        MoneyWithKopecks(100000.25));

    taxpayers[1] = new TaxpayerWithPropertyDeduction<MoneyWithKopecks, 13>(
        "222222222222", 2024,
        MoneyWithKopecks(1000000.50),
        MoneyWithKopecks(200000.75),
        MoneyWithKopecks(3000000.25));

    taxpayers[2] = new Taxpayer<MoneyWithoutKopecks, 13>("333333333333", 2024,
        MoneyWithoutKopecks(300000),
        MoneyWithoutKopecks(50000));

    taxpayers[3] = new TaxpayerWithPropertyDeduction<MoneyWithoutKopecks, 13>(
        "444444444444", 2024,
        MoneyWithoutKopecks(800000),
        MoneyWithoutKopecks(150000),
        MoneyWithoutKopecks(1500000));


    for (int i = 0; i < ARRAY_SIZE; i++) {
        
        auto deducibleWithKopecks = dynamic_cast<TaxpayerWithPropertyDeduction<MoneyWithKopecks, 13>*>(taxpayers[i]);
        if (deducibleWithKopecks) {
            deducibleWithKopecks->applyDeduction(deducibleWithKopecks->getAvailableDeduction() * 0.5);
            continue;  
        }

     
        auto deducibleWithoutKopecks = dynamic_cast<TaxpayerWithPropertyDeduction<MoneyWithoutKopecks, 13>*>(taxpayers[i]);
        if (deducibleWithoutKopecks) {
            deducibleWithoutKopecks->applyDeduction(deducibleWithoutKopecks->getAvailableDeduction() * 0.5);
        }
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

void interactiveDemo() {
    cout << "\n=== Интерактивная демонстрация шаблонов ===" << endl;

    int choice;
    cout << "Выберите тип денежных сумм:" << endl;
    cout << "1. С копейками (дробные числа)" << endl;
    cout << "2. Без копеек (целые числа)" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    clearInputBuffer();

    if (choice == 1) {
    
        TaxpayerWithPropertyDeduction<MoneyWithKopecks, 13> taxpayer(
            "666666666666", 2024,
            MoneyWithKopecks(1000000.0),
            MoneyWithKopecks(200000.0),
            MoneyWithKopecks(2000000.0));

        cout << "\nСоздан налогоплательщик с копейками" << endl;
        taxpayer.printInfo();

        taxpayer.addIncome(MoneyWithKopecks(50000.75), true);
        cout << "\nПосле добавления дохода:" << endl;
        taxpayer.printInfo();

    }
    else if (choice == 2) {
        TaxpayerWithPropertyDeduction<MoneyWithoutKopecks, 13> taxpayer(
            "777777777777", 2024,
            MoneyWithoutKopecks(1000000),
            MoneyWithoutKopecks(200000),
            MoneyWithoutKopecks(2000000));

        cout << "\nСоздан налогоплательщик без копеек" << endl;
        taxpayer.printInfo();

        taxpayer.addIncome(MoneyWithoutKopecks(50000), true);
        cout << "\nПосле добавления дохода:" << endl;
        taxpayer.printInfo();
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned>(time(nullptr)));

    try {

        demonstrateTemplates();

        demonstrateTaxpayerWithDeductionTemplates();

        demonstrateDifferentTaxRates();

   
        demonstratePolymorphismWithTemplates();

   
        interactiveDemo();

        cout << "\nПрограмма завершена." << endl;

    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}