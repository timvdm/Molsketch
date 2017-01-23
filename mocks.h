#ifndef MOCKS_H
#define MOCKS_H

/**
 * Use to create mock function with an arbitrary return value and arguments.
 * Will provide a callback ("Callback" appended to the function name) and
 * a default return value ("DefaultReturn" appended to the function name).
 * The default return value will be returned if the callback is undefined.
 *
 * To pass multiple arguments, it is suggested to define a macro COMMA.
 */
#define MOCK(RETURNTYPE, FUNCTIONNAME, ARGUMENTS, ARGUMENTNAMES) \
    std::function<RETURNTYPE(ARGUMENTS)> FUNCTIONNAME##Callback; \
    RETURNTYPE FUNCTIONNAME##DefaultReturn; \
    RETURNTYPE FUNCTIONNAME(ARGUMENTS) { \
        if(FUNCTIONNAME##Callback) \
            return FUNCTIONNAME##Callback(ARGUMENTNAMES) ; \
        return FUNCTIONNAME##DefaultReturn; \
        }

/**
 * Use to create mock function of void type.
 * Creates a callback ("Callback" appended to the function name)
 * which will be executed in the function if it has been defined.
 */
#define VOIDMOCK(FUNCTIONNAME, ARGUMENTS, ARGUMENTNAMES) \
    std::function<void(ARGUMENTS)> FUNCTIONNAME##Callback; \
    void FUNCTIONNAME(ARGUMENTS) { \
        if(FUNCTIONNAME##Callback) \
            FUNCTIONNAME##Callback(ARGUMENTNAMES) ; \
        }

#endif // MOCKS_H
