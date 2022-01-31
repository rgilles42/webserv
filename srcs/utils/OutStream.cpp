/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutStream.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 19:49:49 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/29 22:07:56 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/OutStream.hpp"

namespace Webserv {

	namespace Utils {

		OutStream::OutStream(void) : out(NULL), mode(OutStream::UNKNOWN) {}

		OutStream::OutStream(const out_type& stream) : out(NULL), mode(OutStream::UNKNOWN) {
			this->setStream(stream);
		}

		OutStream::OutStream(const char *file) : out(NULL), mode(OutStream::UNKNOWN) {
			this->setFile(file);
		}

		OutStream::OutStream(const OutStream& src) : out(NULL), mode(OutStream::UNKNOWN) {
			this->setBuf(src.getOut(), src.getMode());
		}

		OutStream::~OutStream(void) {
			this->closeFile();
		}

		OutStream&					OutStream::operator=(const OutStream& src) {
			if (this != &src) {
				this->closeFile();
				this->setBuf(src.getOut(), src.getMode());
			}
			return (*this);
		}

		const OutStream::mode_type&	OutStream::getMode(void) const {
			return this->mode;
		}

		const OutStream::out_type&		OutStream::getOut(void) const {
			return this->out;
		}

		void	OutStream::closeFile(void) {
			if (this->mode == OutStream::FILE && this->of.is_open()) {
				this->of.close();
			}
		}

		void	OutStream::setBuf(const out_type& stream, const mode_type& mode) {
			mode_type newMode = mode;
			if (&stream == &std::cout) {
				newMode = OutStream::STDOUT;
			} else if (&stream == &std::cerr) {
				newMode = OutStream::STDERR;
			}
			this->buf = stream.rdbuf();
			this->mode = newMode;
			this->out.rdbuf(this->buf);
		}

		void	OutStream::setStream(const out_type& stream) {
			this->closeFile();
			this->setBuf(stream, OutStream::STREAM);
		}

		void	OutStream::setFile(const char *file) {
			this->closeFile();
			if (file != NULL) {
				this->of.open(file);
				this->setBuf(this->of, OutStream::FILE);
			}
		}

	} // namespace Utils

} // namespace Webserv
