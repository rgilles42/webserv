/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 16:38:20 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/11 19:47:55 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <iostream>
# include <fstream>

namespace Webserv {

	class Logger {
		public:
			enum mode_type {
				STREAM,
				FILE,
				STDOUT,
				STDERR,
			};
			typedef std::ostream	out_type;

		protected:
			typedef std::streambuf	buff_type;
			typedef std::ofstream	of_type;

			mode_type	mode;
			buff_type	*buf;
			of_type		of;
			out_type	out;

			void				setBuf(const out_type& stream, const mode_type& mode);

		public:
			Logger(const out_type& stream);
			Logger(const char *file);
			~Logger(void);

			const mode_type&	getMode(void) const;
			const out_type&		getOut(void) const;

			void				closeFile(void);

			void				setStream(const out_type& stream);
			void				setFile(const char *file = NULL);

			template <typename T>
			void				write(const T& x) {
				this->out << x;
			}

			template <typename T>
			std::ostream&		operator<<(const T& x) {
				return this->out << x;
			}
	};

	Logger::Logger(const out_type& stream) : out(NULL) {
		this->mode = Logger::STREAM;
		this->setStream(stream);
	}

	Logger::Logger(const char *file) : out(NULL) {
		this->mode = Logger::STREAM;
		this->setFile(file);
	}

	Logger::~Logger(void) {
		this->closeFile();
	}

	const Logger::mode_type&	Logger::getMode(void) const {
		return this->mode;
	}

	const Logger::out_type&		Logger::getOut(void) const {
		return this->out;
	}

	void	Logger::closeFile(void) {
		if (this->mode == Logger::FILE && this->of.is_open()) {
			this->of.close();
		}
	}

	void	Logger::setBuf(const out_type& stream, const mode_type& mode) {
		mode_type newMode = mode;
		if (&stream == &std::cout) {
			newMode = Logger::STDOUT;
		} else if (&stream == &std::cerr) {
			newMode = Logger::STDERR;
		}
		this->buf = stream.rdbuf();
		this->mode = newMode;
		this->out.rdbuf(this->buf);
	}

	void	Logger::setStream(const out_type& stream) {
		this->closeFile();
		this->setBuf(stream, Logger::STREAM);
	}

	void	Logger::setFile(const char *file) {
		if (file != NULL) {
			this->closeFile();
			this->of.open(file);
			this->setBuf(this->of, Logger::FILE);
		}
	}

} // namespace Webserv

#endif