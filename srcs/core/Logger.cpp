/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 16:50:41 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/30 02:58:50 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/Logger.hpp"

namespace Webserv {

	Logger::Logger(const prefix_type& prefix, const datetime_format_type& dtFormat) {
		this->setPrefix(prefix);
		this->setDateTimeFormat(dtFormat);
		this->cout.setStream(std::cout);
		this->cerr.setStream(std::cerr);
		this->fillMap();
	}

	Logger::Logger(const Logger& other) {
		*this = other;
	}

	Logger::~Logger() {}

	Logger&	Logger::operator=(const Logger& other) {
		if (this != &other) {
			this->prefix = other.prefix;
			this->dtFormat = other.dtFormat;
			this->mainProcessPid = other.mainProcessPid;
			this->logMap = other.logMap;
			this->cout = other.cout;
			this->cerr = other.cerr;
		}
		return *this;
	}

	void							Logger::fillMap(void) {
		this->logMap[Logger::UNKNOWN] = this->cout;
		this->logMap[Logger::DEBUG] = this->cout;
		this->logMap[Logger::INFO] = this->cout;
		this->logMap[Logger::WARNING] = this->cerr;
		this->logMap[Logger::ERROR] = this->cerr;
	}

	void							Logger::setPrefix(const prefix_type &prefix) {
		this->prefix = prefix;
	}

	void							Logger::setDateTimeFormat(const datetime_format_type &dtFormat) {
		this->dtFormat = dtFormat;
	}

	void							Logger::setLogMap(const log_level_type& logLevel, const out_stream_type& stream) {
		this->logMap[logLevel] = stream;
	}

	const Logger::out_stream_type&	Logger::getCout(void) const {
		return this->cout;
	}

	const Logger::out_stream_type&	Logger::getCerr(void) const {
		return this->cerr;
	}

	Logger::out_stream_type&		Logger::getOutput(const log_level_type& logLevel) {
		if (this->logMap.count(logLevel) <= 0) {
			return this->cout;
		}
		return this->logMap.at(logLevel);
	}

	const std::string				Logger::getDateTimePrefix(void) {
		struct tm *timeinfo;
		char buffer[80];
		time_t t;

		time(&t);
		timeinfo = localtime(&t);
		strftime(buffer, sizeof(buffer), this->dtFormat.c_str(), timeinfo);
		return ("[" + std::string(buffer) + "]");
	}

	const std::string				Logger::getLevelPrefix(const log_level_type& logLevel) {
		std::string strLogLevel;
		switch (logLevel) {
			default:
			case Logger::UNKNOWN:
				strLogLevel = "UNKNOWN";
				break;
			case Logger::DEBUG:
				strLogLevel = "DEBUG";
				break;
			case Logger::INFO:
				strLogLevel = "\x1b[34;1mINFO\x1b[0m";
				break;
			case Logger::WARNING:
				strLogLevel = "\x1b[31;33mWARNING\x1b[0m";
				break;
			case Logger::ERROR:
				strLogLevel = "\x1b[31;1mERROR\x1b[0m";
				break;
		}
		return strLogLevel;
	}

	const std::string				Logger::getFullPrefix(const log_level_type& logLevel) {
		std::string prefix = "";
		prefix += this->getDateTimePrefix() + " ";
		if (this->prefix.length() > 0) {
			prefix += this->prefix + " ";
		}
		prefix += this->getLevelPrefix(logLevel) + ": ";
		return prefix;
	}

} // namespace Webserv
