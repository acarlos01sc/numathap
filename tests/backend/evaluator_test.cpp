#include <iomanip>
#include <iostream>
#include <string>

#include "numathap/Session.hpp"


int main()
{
    using namespace numathap;


    //----------------------------------------------------------
    // Session
    //----------------------------------------------------------

    Session session;


    //----------------------------------------------------------
    // Expressions
    //----------------------------------------------------------

    const std::string function = "sin(x)";

    const std::string x_values[] = {
        "0",
        "pi/2",
        "pi"
    };


    std::cout
        << std::setprecision(15);


    //----------------------------------------------------------
    // Evaluate
    //----------------------------------------------------------

    for (const auto& x_expression : x_values)
    {
        //------------------------------------------------------
        // Evaluate x expression
        //------------------------------------------------------

        core::Context context;


        auto x =
            session.evaluate(
                x_expression,
                context);


        //------------------------------------------------------
        // Set variable
        //------------------------------------------------------

        context.set(
            "x",
            x);


        //------------------------------------------------------
        // Evaluate sin(x)
        //------------------------------------------------------

        auto result =
            session.evaluate(
                function,
                context);


        std::cout
            << "sin("
            << x_expression
            << ") = "
            << result.real().value()
            << '\n';
    }


    return 0;
}