/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 16:38:20 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 18:08:14 by ppaglier         ###   ########.fr       */
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
				STDOUT,
				FILE,
				STREAM,
			};
			typedef std::ostream	out_type;

		protected:
			typedef std::streambuf	buff_type;
			typedef std::ofstream	of_type;

			mode_type	mode;
			buff_type	*buf;
			of_type		of;
			out_type	out;

		public:
			Logger(const char *file = NULL);
			Logger(const out_type& stream);
			~Logger(void);

			const mode_type&	getMode(void) const;
			const out_type&		getOut(void) const;

			void				closeFile(void);

			void				setStdOut(void);
			void				setStream(const out_type& stream);
			void				setFile(const char *file = NULL);

			template <typename T>
			void				write(const T &x) {
				this->out << x;
			}

			template <typename T>
			std::ostream&		operator<<(T &x) {
				return this->out << x;
			}
	};

	Logger::Logger(const char *file) : out(NULL) {
		this->mode = Logger::STDOUT;
		this->setFile(file);
	}

	Logger::Logger(const out_type& stream) : out(NULL) {
		this->mode = Logger::STDOUT;
		this->setStream(stream);
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

	void	Logger::setStdOut(void) {
		this->setFile(NULL);
	}

	void	Logger::setStream(const out_type& stream) {
		this->closeFile();
		this->buf = stream.rdbuf();
		this->mode = Logger::STREAM;
		this->out.rdbuf(this->buf);
	}

	void	Logger::setFile(const char *file) {
		this->closeFile();
		if (file != NULL) {
			this->of.open(file);
			this->buf = this->of.rdbuf();
			this->mode = Logger::FILE;
		} else {
			this->buf = std::cout.rdbuf();
			this->mode = Logger::STDOUT;
		}
		this->out.rdbuf(this->buf);
	}

} // namespace Webserv

#endif