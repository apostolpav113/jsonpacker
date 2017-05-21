#include "appoptions.h"
#include <stdlib.h>
#include <iostream>

namespace app_opt {

ApplicationOption::ApplicationOption(ApplicationOptions* parent, const string &name, const string &shortname, const string &description, bool value_required)
	: m_parent(parent)
	, m_name(name)
	, m_shortname(shortname)
	, m_description(description)
	, m_value_required(value_required)
	, m_exists(false)
{
	m_parent->m_options[m_name] = this;
}

std::map<string, ApplicationOption*> ApplicationOptions::m_options;

ApplicationOptions::ApplicationOptions(int argc, char *argv[])
	: m_options_description(OPTION_DESC)
{
	for (auto& option : m_options) {
		if (!option.second->m_value_required) {
			m_options_description.add_options()
					((option.second->m_name + "," + option.second->m_shortname).c_str(), option.second->m_description.c_str());
		} else {
			m_options_description.add_options()
					((option.second->m_name + "," + option.second->m_shortname).c_str(), boost::program_options::value<std::string>(), option.second->m_description.c_str());
		}
	}
	try {
		boost::program_options::variables_map vm;
		boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(m_options_description).run(), vm);
		boost::program_options::notify(vm);
		for (auto& option : m_options) {
			auto& value = vm[option.first];
			option.second->m_exists = !value.empty();
			if (option.second->m_exists && option.second->m_value_required)
				option.second->m_value = value.as<std::string>();
		}
	} catch (const boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::program_options::unknown_option> >& e) {
		std::cout << e.what() << std::endl;
	}
}

void ApplicationOptions::PrintHelp() {
	if (!IsValid()) {
		std::cout << "Invalid command line" << std::endl << std::endl;
	}
	std::cout << "Usage: json_packer [-f] -p|-u -i <input file name> -o <output file name>" << std::endl;
	std::cout << m_options_description << std::endl;
}

bool ApplicationOptions::IsValid() {
	return (PackJson.Exists() || UnpackTLV.Exists()) && InputFile.Exists() && OutputFile.Exists();
}

} // end of namespace app_opt
