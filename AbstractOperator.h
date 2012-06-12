#ifndef ABSTRACTOPERATOR_H
#define ABSTRACTOPERATOR_H

#include "Framework/Arithmetic/AbstractArithmetic.h"
#include <string>

class AbstractOperator
{
public:
    virtual bool matches(std::string) = 0;
    virtual unsigned int priority() = 0;
    virtual CAS::AbstractArithmetic *parse(CAS::AbstractArithmetic *, CAS::AbstractArithmetic *) = 0;
};

#endif //ABSTRACTFUNCTION_H
