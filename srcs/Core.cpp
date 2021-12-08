/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:05:38 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/08 19:13:21 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Core.hpp"

namespace Webserv {

	Core::Core(void) {
		this->isInit = false;
	}

	Core::~Core() {}

	void		Core::setCustomConfigFile(const std::string &configFile) {
		this->customConfigFile = configFile;
	}

	void		Core::setCustomMimeTypesFile(const std::string &mimeTypesFile) {
		this->customMimeTypesFile = mimeTypesFile;
	}

	bool		Core::preInit(int argc, char *argv[], char *envp[]) {
		this->args.fromArg(argc, argv);

		args_type::map_type::const_iterator it = this->args.getArgs().begin();
		while (it != this->args.getArgs().end()) {
			if (it->first == "-c") {
				if (it->second.empty()) {
					std::cout << args_type::BadFormatException(it->first).what() << std::endl;
					exit(EXIT_FAILURE);
				}
				this->setCustomConfigFile(it->second);
			} else if (it->first == "-?" || it->first == "-h") {
				std::cout << this->getHelp() << std::endl;
				exit(EXIT_SUCCESS);
			} else {
				std::cout << args_type::UnknownArgException(it->first).what() << std::endl;
				exit(EXIT_FAILURE);
			}
			it++;
		}
		this->env.fromEnvp(envp);
		return true;
	}

	bool		Core::init(void) {
		std::string configFile = DEFAULT_CONFIG_LOCATION;
		std::string mimeTypesFile = DEFAULT_MIME_TYPES_LOCATION;
		if (!this->customConfigFile.empty()) {
			configFile = this->customConfigFile;
		}
		if (!this->customMimeTypesFile.empty()) {
			mimeTypesFile = this->customMimeTypesFile;
		}
		this->config.reset();
		this->config.addConfigFile(configFile);
		this->config.addConfigFile(mimeTypesFile);
		if (!this->config.processFiles()) {
			return false;
		}
		this->isInit = true;
		return true;
	}

	const bool	&Core::isReady(void) const {
		return this->isInit;
	}

	std::string		Core::getHelp(void) const {
		std::ostringstream ss;

		ss << "webserv version: webserv/1.0.0" << std::endl;
		ss << "Usage: webserv [-?h] [-c filename] [-c=filename]" << std::endl << std::endl;
		ss << "Options:" << std::endl;
		ss << " -?,-h		: this help" << std::endl;
		ss << " -c filename	: set configuration file (default: ./conf/webserv.conf)" << std::endl;
		ss << " -c=filename	: set configuration file (default: ./conf/webserv.conf)" << std::endl;

		return ss.str();
	}

} // namespace Webserv
