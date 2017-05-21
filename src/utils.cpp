#include "utils.h"

#include <boost/algorithm/string/replace.hpp>

namespace str {

std::string Replace(const std::string &source, const std::string &from, const std::string &to) {
	std::string rs = source;
	boost::algorithm::replace_all(rs, from, to);
	return rs;
}

std::string Replace(const std::string &source, const std::map<std::string, std::string> &replace_map) {
	std::string rs = source;
	for (auto& replace_pair : replace_map) {
		boost::algorithm::replace_all(rs, replace_pair.first, replace_pair.second);
	}
	return rs;
}

} // end of namespace str
