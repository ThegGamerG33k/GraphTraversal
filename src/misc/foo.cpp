#include <foo.h>

std::ostream& testing::operator<<(std::ostream& os, const Foo& f)
{
    os << "<FOO:" << f.get_value() << ":" << f.get_name() << ">";
    return os;
}

testing::Foo::Foo() : value(0), name("")            // Make sure to ask about this if I haven't already "volunteered" this info
{
    // intentionally empty (everything was done by the initializers above)
}


testing::Foo::Foo(int v, std::string n) : value(v), name(n) 
{
    // intentionally empty
}


int testing::Foo::get_value() const
{
    return value;
}


void testing::Foo::set_value(int v)
{
    value = v;
}


std::string testing::Foo::get_name() const
{ 
    return name; 
}
    

void testing::Foo::set_name(std::string s)
{
    name = s;
}