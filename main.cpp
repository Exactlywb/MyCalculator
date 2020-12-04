#include <stdlib.h>
#include <stdio.h>
#include <cctype>
#include <cmath>
#include <cstring>

char* originalExpression = nullptr;
size_t symbolPosition    = 0;

const double x = 4;

//не очень понятно, в каком формате стоит хранить переменные, сколько их должно быть, да и вообще, как жить.
//Временно оставим просто const double x

double GetG (char* expression);
double GetN ();
double GetE ();
double GetT ();
double GetP ();
double GetD ();
double GetSin ();
double GetCos ();

char Require (char symbToRequire);
void SyntaxError (const char* funcName);

int main() {

    double value = GetG ("(sin(x)*cos(x*3))^2$");
    printf ("Value = %lg\n", value);

    return 0;

}

double GetG (char* expression) {

    if (expression == nullptr) {

        printf ("Null pointer on char* expression in function %s\n", __FUNCTION__);
        return 0;

    }

    originalExpression = expression;
    symbolPosition = 0;

    double value = GetE ();

    Require ('$');

    return value;

}

double GetN () {

    double value = 0;

    while ( isdigit(originalExpression[symbolPosition]) || originalExpression[symbolPosition] == 'x') {

        int addVal = 0;

        if (originalExpression[symbolPosition] == 'x')
            addVal = x;
        else
            addVal = originalExpression[symbolPosition] - '0';

        value = value * 10 + addVal;
        symbolPosition++;

    }

    return value;

}

double GetE () {

    double value = GetT ();

    while (originalExpression[symbolPosition] == '+' || originalExpression[symbolPosition] == '-') {

        char operation = originalExpression[symbolPosition];
        symbolPosition++;

        double secondValue = GetT ();

        if (operation == '+')
            value += secondValue;
        else
            value -= secondValue;

    }

    return value;

}

double GetT () {

    double value = GetD ();

    while (originalExpression[symbolPosition] == '*' || originalExpression[symbolPosition] == '/') {

        char operation = originalExpression[symbolPosition];
        symbolPosition++;

        double secondValue = GetD ();

        if (operation == '*')
            value *= secondValue;
        else
            value /= secondValue;

    }

    return value;

}

double GetP () {

    if (originalExpression[symbolPosition] == '(') {

        symbolPosition++;
        double value = GetE ();

        Require (')');

        return value;

    } else {

        return GetSin ();

    }

}

double GetD () {

    double value = GetP ();

    while (originalExpression[symbolPosition] == '^') {

        symbolPosition++;

        double secondValue = GetP ();

        value = pow (value, secondValue);

    }

    return value;

}

double GetSin () {

    double value = 0;

    if (!strncmp("sin", originalExpression + symbolPosition, 3)) {

        symbolPosition += 3;

        value = GetP ();
        value = sin (value);

    } else {

        value = GetCos ();

    }

    return value;

}

double GetCos () {

    double value = 0;

    if (!strncmp("cos", originalExpression + symbolPosition, 3)) {

        symbolPosition += 3;

        value = GetP ();
        value = cos (value);

    } else {

        value = GetN ();

    }

    return value;

}

char Require (const char symbToRequire) {

    char lastSymbol = originalExpression[symbolPosition];

    if (lastSymbol != symbToRequire)
        SyntaxError (__FUNCTION__);
    else
        symbolPosition++;

    return lastSymbol;

}

void SyntaxError (const char* funcName) {

    printf ("Syntax error! From %s function.\n", funcName);

}
