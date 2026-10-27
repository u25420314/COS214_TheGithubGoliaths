#pragma once
#include <string>
#include "StubServices.h"

/** Target interface the rest of the system uses to pay people (amounts in currency units). */
class PaymentGateway {
public:
    virtual ~PaymentGateway() = default;
    virtual bool pay(const std::string& payee, double amount) = 0;
};

/** Adapter: PaymentGateway -> PaymentServiceStub::makeTransfer (converts to cents). */
class PaymentAdapter : public PaymentGateway {
public:
    explicit PaymentAdapter(PaymentServiceStub& service) : service_(service) {}
    bool pay(const std::string& payee, double amount) override;
    int lastTransactionId() const { return lastId_; }
private:
    PaymentServiceStub& service_;
    int lastId_ = -1;
};