/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Json.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 06:08:30 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/17 10:01:36 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Json.hpp"

Json::Json(void) {
	this->valueType = Json::T_NULL;
	this->values.null_value = NULL;
}

Json::Json(const boolean_type &boolean) {
	this->valueType = Json::T_BOOLEAN;
	this->values.boolean_value = boolean;
}

Json::Json(const int &number) {
	this->valueType = Json::T_NUMBER;
	this->values.number_value = number;
}

Json::Json(const number_type &number) {
	this->valueType = Json::T_NUMBER;
	this->values.number_value = number;
}

Json::Json(const char *str) {
	this->valueType = Json::T_STRING;
	this->values.string_value = new string_type(str);
}

Json::Json(const string_type &str) {
	this->valueType = Json::T_STRING;
	this->values.string_value = new string_type(str);
}

Json::Json(const array_type &array) {
	this->valueType = Json::T_ARRAY;
	this->values.array_value = new array_type(array);
}

Json::Json(const object_type &object) {
	this->valueType = Json::T_OBJECT;
	this->values.object_value = new object_type(object);
}

// It can cause some errors in valgrind..
Json::Json(const Json &x) {

	this->valueType = x.valueType;

	switch (this->valueType) {

		default:
		case Json::T_NULL: {
			this->values.null_value = x.values.null_value;
			break;
		}

		case Json::T_BOOLEAN: {
			this->values.boolean_value = x.values.boolean_value;
			break;
		}

		case Json::T_NUMBER: {
			this->values.number_value = x.values.number_value;
			break;
		}

		case Json::T_STRING: {
			this->values.string_value = x.values.string_value;
			break;
		}

		case Json::T_ARRAY: {
			this->values.array_value = x.values.array_value;
			break;
		}

		case Json::T_OBJECT: {
			this->values.object_value = x.values.object_value;
			break;
		}

	}
}

Json	&Json::operator=(const boolean_type &boolean) {
	this->valueType = Json::T_BOOLEAN;
	this->values.boolean_value = boolean;
	return (*this);
}

Json	&Json::operator=(const int &number) {
	this->valueType = Json::T_NUMBER;
	this->values.number_value = number;
	return (*this);
}

Json	&Json::operator=(const number_type &number) {
	this->valueType = Json::T_NUMBER;
	this->values.number_value = number;
	return (*this);
}

Json	&Json::operator=(const char *str) {
	this->valueType = Json::T_STRING;
	this->values.string_value = new string_type(str);
	return (*this);
}

Json	&Json::operator=(const string_type &str) {
	this->valueType = Json::T_STRING;
	this->values.string_value = new string_type(str);
	return (*this);
}

Json	&Json::operator=(const array_type &array) {
	this->valueType = Json::T_ARRAY;
	this->values.array_value = new array_type(array);
	return (*this);
}

Json	&Json::operator=(const object_type &object) {
	this->valueType = Json::T_OBJECT;
	this->values.object_value = new object_type(object);
	return (*this);
}

// It can cause some errors in valgrind..
Json	&Json::operator=(const Json &x) {
	if (this == &x) {
		return (*this);
	}
	this->valueType = x.valueType;

	switch (this->valueType) {

		default:
		case Json::T_NULL: {
			this->values.null_value = x.values.null_value;
			break;
		}

		case Json::T_BOOLEAN: {
			this->values.boolean_value = x.values.boolean_value;
			break;
		}

		case Json::T_NUMBER: {
			this->values.number_value = x.values.number_value;
			break;
		}

		case Json::T_STRING: {
			this->values.string_value = x.values.string_value;
			break;
		}

		case Json::T_ARRAY: {
			this->values.array_value = x.values.array_value;
			break;
		}

		case Json::T_OBJECT: {
			this->values.object_value = x.values.object_value;
			break;
		}

	}
	return (*this);
}

Json::~Json() {
	if (this->isString()) {
		delete this->values.string_value;
	} else if (this->isArray()) {
		Json::array_type::const_iterator it = this->values.array_value->begin();
		while (it != this->values.array_value->end()) {
			delete (*it);
			it++;
		}
		delete this->values.array_value;
	} else if (this->isObject()) {
		Json::object_type::const_iterator it = this->values.object_value->begin();
		while (it != this->values.object_value->end()) {
			delete (*it).second;
			it++;
		}
		delete this->values.object_value;
	}
}


bool	Json::isNull(void) const {
	return this->valueType == Json::T_NULL;
}

bool	Json::isBoolean(void) const {
	return this->valueType == Json::T_BOOLEAN;
}

bool	Json::isNumber(void) const {
	return this->valueType == Json::T_NUMBER;
}

bool	Json::isString(void) const {
	return this->valueType == Json::T_STRING;
}

bool	Json::isArray(void) const {
	return this->valueType == Json::T_ARRAY;
}

bool	Json::isObject(void) const {
	return this->valueType == Json::T_OBJECT;
}

Json::value_t	Json::getValueType(void) const {
	return this->valueType;
}

void			*Json::getValue(void) const {
	switch (this->valueType) {

		default:
		case Json::T_NULL: {
			return new null_type(this->values.null_value);
			break;
		}

		case Json::T_BOOLEAN: {
			return new boolean_type(this->values.boolean_value);
			break;
		}

		case Json::T_NUMBER: {
			return new number_type(this->values.number_value);
			break;
		}

		case Json::T_STRING: {
			return new string_type(*this->values.string_value);
			break;
		}

		case Json::T_ARRAY: {
			return new array_type(*this->values.array_value);
			break;
		}

		case Json::T_OBJECT: {
			return new object_type(*this->values.object_value);
			break;
		}

	}

}
Json::null_type	Json::asNull(void) const {
	return this->values.null_value;
}

Json::boolean_type	Json::asBool(void) const {
	return this->values.boolean_value;
}

Json::number_type	Json::asNumber(void) const {
	return this->values.number_value;
}

const Json::string_type	&Json::asString(void) const {
	return *this->values.string_value;
}
const Json::array_type	&Json::asArray(void) const {
	return *this->values.array_value;
}
const Json::object_type	&Json::asObject(void) const {
	return *this->values.object_value;
}

