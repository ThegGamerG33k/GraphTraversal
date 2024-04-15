#pragma once
#include <string>
#include <ostream>

namespace testing
{
    /// Just a silly class to test that our data structures are fully templatized
    /// and accept non-trivial classes
    class Foo
    {
    protected:
        /// The integer value of the Foo object
        int value;

        /// The string name of the Foo object
        std::string name;
    public:
        /// The default constructor
        Foo();

        /// The fully-qualified constructor
        Foo(int v, std::string n);

        /// <summary>
        ///  Gets the value of the value attribute
        /// </summary>
        /// <returns>current value of the value attribute</returns>
        int get_value() const;

        /// <summary>
        /// Sets the value of the value attribute
        /// </summary>
        /// <param name="v">New value for the value attribute</param>
        void set_value(int v);

        /// <summary>
        /// Gets the value of the name attribute
        /// </summary>
        /// <returns>The value of the name attribute</returns>
        std::string get_name() const;

        /// <summary>
        /// Sets the value of the name attribute
        /// </summary>
        /// <param name="s">The new value for the name attribute</param>
        void set_name(std::string s);
    };

    /// <summary>
    ///  Overload the stream operator for Foo objects
    /// </summary>
    /// <param name="os">the output stream (cout, fp, stringstream, etc.)</param>
    /// <param name="f">the Foo object we're outputting to the stream</param>
    /// <returns>the (unchanged) ostream we were given (to support stream "chaining")</returns>
    std::ostream& operator<<(std::ostream& os, const Foo& f);
}
