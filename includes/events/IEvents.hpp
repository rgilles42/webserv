#ifndef IEVENTS_HPP
# define IEVENTS_HPP

namespace Webserv
{
	/* Interface use for all events class */
	class IEvents
	{
		public:
			virtual			~IEvents() {}
			virtual void	read_event() = 0;
			virtual void	write_event() = 0;
			virtual short	getEventsFlags() = 0; 
			virtual int		getFD() = 0;
	};
}

#endif
