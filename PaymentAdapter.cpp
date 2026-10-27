#include "PaymentAdapter.h"
#include <cmath>

bool PaymentAdapter::pay(const std::string& payee, double amount) {
    long cents = std::lround(amount * 100.0);
    lastId_ = service_.makeTransfer(payee, cents);
    return lastId_ > 0;
}