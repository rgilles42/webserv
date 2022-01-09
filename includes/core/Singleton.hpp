#ifndef SINGLETONS_HPP
# define SINGLETONS_HPP

namespace Webserv
{

    template<class T>
    class Singleton
	{
		protected:
            Singleton(void);
        public:
            static T &getInstance(void);
	};
}

#endif