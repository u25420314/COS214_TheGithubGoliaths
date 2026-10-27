#include "ApprovalHandler.h"
#include <cstddef>

ApprovalHandler::ApprovalHandler() : next(nullptr){

}

ApprovalHandler::~ApprovalHandler(){

}

ApprovalHandler* ApprovalHandler::setNext(ApprovalHandler* handler){
    next = handler;
    return handler;
}

