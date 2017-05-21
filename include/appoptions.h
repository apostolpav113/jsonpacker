#ifndef APPOPTIONS_H
#define APPOPTIONS_H

#include <string>
#include <map>
#include <boost/program_options.hpp>

using namespace std;

#define OPTION_DESC "JSON Packer Options"

namespace app_opt {

class ApplicationOptions;

class ApplicationOption {
public:
	ApplicationOption(ApplicationOptions* parent, const string& name, const string& shortname, const string& description, bool value_required = true);

	bool Exists() {return m_exists;}
	string Value() {return m_value;}
private:
	ApplicationOptions* m_parent;
	string m_name;
	string m_shortname;
	string m_description;
	bool m_value_required;
	bool m_exists;
	string m_value;

	friend class ApplicationOptions;
};

class ApplicationOptions {
public:
	ApplicationOptions(int argc, char *argv[]);
	void PrintHelp();
	bool IsValid();

	ApplicationOption Help {this, "help", "h", "Show help", false};
	ApplicationOption PackJson {this, "pack", "p", "Pack JSON input file into TLV format", false};
	ApplicationOption UnpackTLV {this, "unpack", "u", "Unpack TLV input file into JSON format", false};
	ApplicationOption InputFile {this, "input", "i", "Input file name"};
	ApplicationOption OutputFile {this, "output", "o", "Output file name"};
	ApplicationOption Force {this, "force", "f", "Owerwrite output file if exists", false};
	ApplicationOption Method {this, "method", "m", "Method of file proceed"};
private:
	boost::program_options::options_description m_options_description;
	static std::map<string, ApplicationOption*> m_options;


	friend class ApplicationOption;
};

} // end of namespace app_opt

#endif // APPOPTIONS_H
