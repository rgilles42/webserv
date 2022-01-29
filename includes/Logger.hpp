/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 20:04:57 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/29 22:04:16 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <sys/types.h>				// For pid_t
# include <unistd.h>				// For getpid
# include <map>						// For map
# include <string>					// For string
# include <utility>					// For pair
# include <ctime>					// For time, localtime, strftime

# include "./utils/OutStream.hpp"	// For OutStream

# define DEFAULT_DATETIME_FORMAT std::string("%d %b %Y %H:%M:%S %z")

namespace Webserv {

	class Logger {
		public:
			enum log_level_type {
				UNKNOWN = 0,
				DEBUG,
				INFO,
				WARNING,
				ERROR,
			};
			typedef pid_t						process_id_type;
			typedef Webserv::Utils::OutStream	out_stream_type;
			typedef std::map<log_level_type, out_stream_type>	map_level_type;
			typedef std::string					prefix_type;
			typedef std::string					datetime_format_type;

		protected:
			prefix_type		prefix;
			datetime_format_type	dtFormat;

			process_id_type	mainProcessPid;

			map_level_type	logMap;

			out_stream_type	cout;
			out_stream_type	cerr;

			void					fillMap(void);

		public:
			Logger(const prefix_type& prefix = "", const datetime_format_type& dtFormat = DEFAULT_DATETIME_FORMAT);
			Logger(const Logger& other);
			~Logger();

			Logger&					operator=(const Logger& other);

			void					setPrefix(const prefix_type& prefix = "");
			void					setDateTimeFormat(const datetime_format_type& dtFormat = DEFAULT_DATETIME_FORMAT);

			void					setLogMap(const log_level_type& logLevel, const out_stream_type& stream);

			const out_stream_type&	getCout(void) const;
			const out_stream_type&	getCerr(void) const;

			out_stream_type&		getOutput(const log_level_type& logLevel = Logger::UNKNOWN);

			const std::string		getDateTimePrefix(void);
			const std::string		getLevelPrefix(const log_level_type& logLevel = Logger::UNKNOWN);
			const std::string		getFullPrefix(const log_level_type& logLevel = Logger::UNKNOWN);

			template <typename T>
			void					log(const log_level_type& logLevel, const T& x) {
				out_stream_type &out = this->getOutput(logLevel);
				out << this->getFullPrefix(logLevel) << x;
			}
			template <typename T>
			void					debug(const T& x) {
				this->log(Logger::DEBUG, x);
			}
			template <typename T>
			void					info(const T& x) {
				this->log(Logger::INFO, x);
			}
			template <typename T>
			void					warning(const T& x) {
				this->log(Logger::WARNING, x);
			}
			template <typename T>
			void					error(const T& x) {
				this->log(Logger::ERROR, x);
			}

			// Default write with Unknown
			template <typename T>
			std::ostream&			operator<<(const T& x) {
				return this->cout << this->getFullPrefix() << x;
			}

			// Write with pair of level/T
			template <typename T>
			std::ostream&			operator<<(const std::pair<log_level_type, T>& x) {
				out_stream_type &out = this->getOutput(x.first);
				return out << this->getFullPrefix(x.first) << x.second;
			}

	};

} // namespace Webserv

#endif