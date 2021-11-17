/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Json.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 06:07:49 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/17 09:52:04 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSON_HPP
# define JSON_HPP

# include <cstdio>
# include <string>
# include <map>
# include <vector>

# include <iostream>

class Json {

	public:
		enum value_t {
			T_NULL,		// null value
			T_BOOLEAN,	// boolean value
			T_NUMBER,	// number value
			T_STRING,	// string value
			T_ARRAY,	// array (ordered collection of values)
			T_OBJECT,	// object (unordered set of name/value pairs)
		};

		typedef void*							null_type;
		typedef bool							boolean_type;
		typedef double							number_type;
		typedef std::string						string_type;
		typedef std::vector<Json*>				array_type;
		typedef std::map<string_type, Json*>	object_type;

	protected:
		union values_type {
			null_type 		null_value;
			boolean_type	boolean_value;
			number_type		number_value;
			string_type		*string_value;
			array_type		*array_value;
			object_type		*object_value;
		};

		values_type	values;
		value_t		valueType;

	public:
		Json(void);
		Json(const boolean_type &boolean);
		Json(const int &number);
		Json(const number_type &number);
		Json(const string_type &str);
		Json(const char *str);
		Json(const array_type &array);
		Json(const object_type &object);
		Json(const Json &x);

		Json	&operator=(const boolean_type &boolean);
		Json	&operator=(const int &number);
		Json	&operator=(const number_type &number);
		Json	&operator=(const char *str);
		Json	&operator=(const string_type &str);
		Json	&operator=(const array_type &array);
		Json	&operator=(const object_type &object);
		Json	&operator=(const Json &x);

		virtual ~Json();

		bool	isNull(void) const;
		bool	isBoolean(void) const;
		bool	isNumber(void) const;
		bool	isString(void) const;
		bool	isArray(void) const;
		bool	isObject(void) const;

		value_t	getValueType(void) const;
		void	*getValue(void) const;

		null_type			asNull(void) const;
		boolean_type		asBool(void) const;
		number_type			asNumber(void) const;
		const string_type	&asString(void) const;
		const array_type	&asArray(void) const;
		const object_type	&asObject(void) const;

};

#endif