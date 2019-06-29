#include "cLua.h"
#include <stdio.h>
#include <math.h>

static CLua* pLua = NULL;
static int sLuaBasedTestResult = 0;
LuaGlue(FAIL)
{
    const char* r = pLua->GetStringArgument(1, "message fail");
    printf("FAIL: %s\n", r);
    sLuaBasedTestResult += -3;
    return 0;
}
LuaGlue(StringTest)
{
    if (pLua->GetArgumentType(1) != LuaArgType::ARG_TYPE_STRING)
    {
        printf("Fail: StringTest argument is not a String\n");
        sLuaBasedTestResult += -4;
    }
    const char* r = pLua->GetStringArgument(1, "fail");
    pLua->PushString(r);
    return 1;
}
LuaGlue(NumberTest)
{
    if (pLua->GetArgumentType(1) != LuaArgType::ARG_TYPE_NUMBER)
    {
        printf("Fail: NumberTest argument is not a Number\n");
        sLuaBasedTestResult += -5;
    }
    double r = pLua->GetNumberArgument(1, INFINITY);
    pLua->PushNumber(r);
    return 1;
}
LuaGlue(IntegerTest)
{
    if (pLua->GetArgumentType(1) != LuaArgType::ARG_TYPE_NUMBER)
    {
        printf("Fail: IntegerTest argument is not a Number\n");
        sLuaBasedTestResult += -6;
    }
    int r = (int) pLua->GetNumberArgument(1, 0xFFFFFFFF);
    pLua->PushNumber(r);
    return 1;
}
LuaGlue(BoolTest)
{
    if (pLua->GetArgumentType(1) != LuaArgType::ARG_TYPE_BOOL)
    {
        printf("Fail: BoolTest argument is not a Boolean\n");
        sLuaBasedTestResult += -7;
    }
    bool r = pLua->GetBoolArgument(1);
    pLua->PushBool(r);
    return 1;
}
LuaGlue(WhatIsLove)
{
    pLua->PushNil();
    return 1;
}
static const luaDef myFunctions[] = {
    {"StringTest", StringTest},
    {"NumberTest", NumberTest},
    {"IntegerTest", IntegerTest},
    {"BoolTest", BoolTest},
    {"WhatIsLove", WhatIsLove},

    {NULL, NULL}
};

int main()
{
    int retval = 0;
    printf("Hello World!\n");

    pLua = new CLua();
    pLua->AddFunction("FAIL", FAIL);
    pLua->AddFunction(myFunctions);

    if (!pLua->RunString("print('Test String has run')"))
    {
        printf("Failed to run c string\n");
        retval += -1;
    }
    if (!pLua->RunScript("scripttest.lua"))
    {
        printf("Failed to run script file\n");
        retval += -2;
    }

    retval += sLuaBasedTestResult;

    if (retval == 0)
    {
        printf("All tests passed\n");
    }
    else
    {
        printf("Some test failed: Guru Medition:%d", retval);
    }
    
    return retval;
}