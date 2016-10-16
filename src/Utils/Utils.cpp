#include <regex>
#include <boost/filesystem.hpp>
#include "Utils.hpp"

std::vector<FileInfo> getFiles(const std::string folder, const std::string filter, bool recursively){
	using namespace boost::filesystem;
	std::vector<FileInfo> out;
	// std::regex filenameFilter(filter);

	try {
		if(recursively){
			path p(folder);
			auto dir_it = recursive_directory_iterator(p);
			for(dir_it; dir_it != recursive_directory_iterator(); dir_it++){
				if( is_directory(dir_it->status()) ) continue;
				FileInfo info {};

				info.localFilePath = (*dir_it).path().relative_path().string();
				info.fileName = (*dir_it).path().filename().string();
				info.fileStem = (*dir_it).path().stem().string();
				info.fileExtension = (*dir_it).path().extension().string();

				// if(std::regex_match(info.fileName, filenameFilter))
					// cout<<"<not match>"<<endl;
					out.push_back(info);
			}
		}
		else {
			path p(folder);
			auto dir_it = directory_iterator(p);
			for(dir_it; dir_it != directory_iterator(); dir_it++){
				FileInfo info {};

				info.localFilePath = (*dir_it).path().relative_path().string();
				info.fileName = (*dir_it).path().stem().string();
				info.fileExtension = (*dir_it).path().extension().string();

				// if(std::regex_match(info.fileName, filenameFilter))
					// cout<<"<not match>"<<endl;
					out.push_back(info);
			}
		}
	}
	catch (const filesystem_error& ex){
		cout<<"boost::filesystem ex: "<<ex.what()<<endl;
	}
	return out;
}

