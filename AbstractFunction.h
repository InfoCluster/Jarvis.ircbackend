#ifndef ABSTRACTFUNCTION_H
#define ABSTRACTFUNCTION_H

#include "Framework/Arithmetic/AbstractArithmetic.h"
#include <string>

class AbstractFunction
{
public:
    virtual bool matches(std::string) = 0;
    virtual CAS::AbstractArithmetic *parse(CAS::AbstractArithmetic *) = 0;
};

#endif //ABSTRACTFUNCTION_H