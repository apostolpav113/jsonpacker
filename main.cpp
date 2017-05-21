#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "rapidjson/document.h"
#include "rapidjson/error/error.h"
#include "rapidjson/error/en.h"

#include "coder.h"
#include "appoptions.h"
#include "error.h"
#include "packerstream.h"

using namespace std;

int main(int argc, char *argv[])
{
	int return_code = EXIT_SUCCESS;
	try {
		app_opt::ApplicationOptions app_options(argc, argv);
		if (app_options.Help.Exists() || !app_options.IsValid()) {
			app_options.PrintHelp();
			return EXIT_SUCCESS;
		}

		if (!boost::filesystem::exists(app_options.InputFile.Value())) {
			throw app_err::JsonPackerFileMissed(app_options.InputFile.Value());
			return EXIT_FAILURE;
		}

		if (boost::filesystem::exists(app_options.OutputFile.Value()) && !app_options.Force.Exists()) {
			throw app_err::JsonPackerFileExists(app_options.InputFile.Value());
			return EXIT_FAILURE;
		}

		if (app_options.Method.Exists()) {
			auto packer = jsonpacker_coder::GetPacker(app_options.Method.Value());

			ifstream input;
			input.open(app_options.InputFile.Value(), packer->InputOpenModeFlags());
			std::ofstream ofs(app_options.OutputFile.Value(), packer->OutputOpenModeFlags());

			jsonpacker_stream::JsonPackerFileStream stream(input, ofs);
			packer->Run(stream);
		}
	} catch (const app_err::JsonPackerError& e) {
		cout << e.what() << endl;
		return_code = EXIT_FAILURE;
	} catch (const std::exception& e) {
		cout << e.what() << endl;
		return_code = EXIT_FAILURE;
	} catch (...) {
		cout << "unhandled exception" << endl;
		return_code = EXIT_FAILURE;
	}

	return return_code;
}
