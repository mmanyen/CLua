print("The Test Script has run")

function GiveMeFive() 
    return 5;
end

if GiveMeFive() ~= 5 then
    print("Function return error\n")
    FAIL("Lua return test")
else
    print("Gimme five! Right on brother")
end

rs = StringTest("This is a test of String Passing")
if rs ~= "This is a test of String Passing" then
    FAIL("String Test")
end

rn = NumberTest(3.14)
if rn ~= 3.14 then
    FAIL("Number Test")
end

ri = IntegerTest(42)
if ri ~= 42 then
    FAIL("Integer Test")
end

rb = BoolTest(true)
if rb ~= true then
    FAIL("Bool Test")
end

if WhatIsLove() == nil then
    print("oooh you hurt me\n");
end