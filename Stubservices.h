#pragma once
#include <string>

/** Fake email service with its own (deliberately different) API. Header-only. */
struct EmailServiceStub {
    int sentCount = 0;
    std::string lastAddress, lastText;
    bool sendRaw(const std::string& address, const std::string& text) {
        if (address.empty()) return false;
        ++sentCount; lastAddress = address; lastText = text;
        return true;
    }
};

/** Fake payment service: takes whole cents, returns a transaction id or -1. */
struct PaymentServiceStub {
    int transfers = 0;
    long lastCents = 0;
    std::string lastAccount;
    int makeTransfer(const std::string& account, long cents) {
        if (account.empty() || cents <= 0) return -1;
        ++transfers; lastCents = cents; lastAccount = account;
        return 1000 + transfers;
    }
};