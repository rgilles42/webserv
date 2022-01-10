#include "../../includes/core/Singleton.hpp"

namespace Webserv
{
	Singleton::Singleton(void) {}

	template <class T>
	static T	Singleton::getInstance(void)
    {
    	static T instance;

        return (instance);
    }
}