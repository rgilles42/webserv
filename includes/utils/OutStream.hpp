/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutStream.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 16:38:20 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/11 19:47:55 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTSTREAM_HPP
# define OUTSTREAM_HPP

# include <iostream>
# include <fstream>

namespace Webserv {

	namespace Utils {
		class OutStream {
			public:
				enum mode_type {
					UNKNOWN = 0,
					STREAM,
					FILE,
					STDOUT,
					STDERR,
				};
				typedef std::ostream	out_type;

			protected:
				typedef std::streambuf	buff_type;
				typedef std::ofstream	of_type;

				buff_type	*buf;
				of_type		of;
				out_type	out;
				mode_type	mode;

				void				setBuf(const out_type& stream, const mode_type& mode);

			public:
				OutStream(void);
				OutStream(const out_type& stream);
				OutStream(const char *file);
				OutStream(const OutStream& src);
				~OutStream(void);

				OutStream&			operator=(const OutStream& src);

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

	} // namespace Utils

} // namespace Webserv

#endif