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
        FixedPoint<100> value;
        value.fromString("2.5f");
        value.setRaw(250);
        EXPECT_EQ(value.getAsString(), "2.5f");
    }

    TEST(FixedPoint, multiplication_assignment_operator) 
    {
        FixedPoint<> value;
        value.fromString("5.5f");
        std::cout << value.getAsString();
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