#include <gtest/gtest.h>

#include <EchoEngine/FixedPoint.h>

namespace EchoEngine
{
    TEST(FixedPoint, Initialize) // From string ctr
    {
        FixedPoint<256> value;
        value.fromString("321.123f");
        EXPECT_EQ(value.getRaw(), 82207); // About 321.121 in float
    }

    TEST(FixedPoint, GetAsString) // to_string()
    {
        FixedPoint<> value;

        //Leave this in till getAsString is fully functional.

        //std::string str;
        //for (uint32_t i = 0; i < 10; i++) {
        //    for (uint32_t j = 0; j < 10; j++) {
        //        str = "2.";
        //        //value.fromString("2.85f");
        //        str += std::to_string(i);
        //        str += std::to_string(j);
        //        str += 'f';
        //        value.fromString(str);
        //        FixedPoint<> t;
        //        t.fromString(value.getAsString());
        //        std::cout << ((float)value.getRaw()) / (float)(256 * 256) 
        //            << " " << value.getAsString() << " " << t.getAsString() << std::endl;
        //        str.clear();
        //    }
        //}

        //Seems getAsString() has a very slight error with most numbers.
        value.fromString("2.75f");
        FixedPoint<> value2;
        value2.fromString(value.getAsString());
        //FixedPoint<> value3;
        //value3.fromString(value2.getAsString());
        //std::cout << value.getAsString() 
        //    << " " << value2.getAsString() << " " << value3.getAsString() << std::endl;
        EXPECT_EQ(value.getAsString(), value2.getAsString());
    }

    TEST(FixedPoint, multiplication_assignment_operator) 
    {
        FixedPoint<> value;
        value.fromString("5.5f");
        value *= FixedPoint<>(2);
        EXPECT_EQ(value.getAsString(), "11.0f");
    }

    TEST(FixedPoint, division_assignment_operator)
    {
        FixedPoint<> value;
        value.fromString("5.5f");
        value /= FixedPoint<>(2);
        EXPECT_EQ(value.getAsString(), "2.75f");
    }

    TEST(FixedPoint, addition_assignment_operator) 
    {
        FixedPoint<> value;
        value.fromString("5.5f");
        value += value;
        EXPECT_EQ(value.getAsString(), "11.0f");
    }

    TEST(FixedPoint, subtraction_assignment_operator)
    {
        FixedPoint<> value;
        value.fromString("11.0f");
        FixedPoint<> value2;
        value2.fromString("5.5f");
        value -= value2;
        EXPECT_EQ(value.getAsString(), "5.5f");
    }

    TEST(FixedPoint, multiplication_operator)
    {
        FixedPoint<> value;
        value.fromString("5.5f");
        auto result = value * FixedPoint<>(2);
        EXPECT_EQ(result.getAsString(), "11.0f");
    }

    TEST(FixedPoint, division_operator)
    {
        FixedPoint<> value;
        value.fromString("11.0f");
        auto result = value / FixedPoint<>(2);
        EXPECT_EQ(result.getAsString(), "5.5f");
    }

    TEST(FixedPoint, addition_operator)
    {
        FixedPoint<> value;
        value.fromString("5.5f");
        auto result = value + value;
        EXPECT_EQ(result.getAsString(), "11.0f");
    }

    TEST(FixedPoint, subtraction_operator)
    {
        FixedPoint<> value;
        value.fromString("11.0f");
        FixedPoint<> value2;
        value2.fromString("5.5f");
        auto result = value - value2;
        EXPECT_EQ(result.getAsString(), "5.5f");
    }
}

// TODO: 
/*
inline std::string T_FixedPointNegativeArithmatic()
{
    FixedPoint<> value;
    std::string retValue = "Within...\n";
    {
        retValue += "=MULTIPLY\n";
        value.fromString("5.5f");
        value = -value;
        value *= FixedPoint<>(2);
        if (value.getAsString() != "-11.0f")
        {
            retValue += "-5.5f*2 should = -11.0f not ";
            retValue += std::to_string(value.getAsFloat());
            retValue += '\n';
        }
    }
    {
        retValue += "=DIVISION\n";
        value.fromString("11.0f");
        value = -value;
        value /= FixedPoint<>(2);
        if (value.getAsString() != "-5.5f")
        {
            retValue += "-11.0f/2 should = -5.5f not ";
            retValue += std::to_string(value.getAsFloat());
            retValue += '\n';
        }
    }
    {
        retValue += "=ADDITION\n";
        value.fromString("5.5f");
        value = -value;
        value += value;
        if (value.getAsString() != "11.0f")
        {
            retValue += "-5.5f+-5.5f should = -11.0f not ";
            retValue += std::to_string(value.getAsFloat());
            retValue += '\n';
        }
    }
    {
        retValue += "=SUBTRACTION\n";
        value.fromString("11.0f");
        FixedPoint<> value2;
        value2.fromString("5.5f");
        value = -value;
        value -= value2;
        if (value.getAsString() != "-16.5f")
        {
            retValue += "-11.0f-5.5f should = -16.5f not ";
            retValue += std::to_string(value.getAsFloat());
            retValue += '\n';
        }
    }
    return retValue;
}
inline std::string T_FixedPointSqrt()
{
    FixedPoint<> value;
    value.fromString("9.5f");
    value = sqrt(value);
    float comparedTo = sqrt(9.5f);
    if (value.getAsString() != std::to_string(comparedTo))
    {
        std::string retValue = "Expected value ";
        retValue += std::to_string(comparedTo);
        retValue += " instead got ";
        retValue += value.getAsString();
        return retValue;
    }
    return "";
}
*/