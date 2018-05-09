#include "FileInputStream.hpp"

namespace Audio
{
FileInputStream::FileInputStream()
: m_file(NULL){

}

FileInputStream::~FileInputStream(){

	if(m_file)
		std::fclose(m_file);
}

bool FileInputStream::open(const std::string& filename){
	if(m_file)
		std::fclose(m_file);

	m_file = std::fopen(filename.c_str(), "rb");

	return m_file != NULL;
}

i64 FileInputStream::read(void* data, i64 size){
	if(m_file)
		return std::fread(data, 1, static_cast<std::size_t>(size), m_file);
	else
		return -1;
}

i64 FileInputStream::seek(i64 position){
	if(m_file) {
		std::fseek(m_file, static_cast<std::size_t>(position), SEEK_SET);
		return tell();
	}
	else {
		return -1;
	}
}

i64 FileInputStream::tell(){
	if(m_file)
		return std::ftell(m_file);
	else
		return -1;
}

i64 FileInputStream::getSize(){
	if(m_file){
		i64 position = tell();
		std::fseek(m_file, 0, SEEK_END);
		i64 size = tell();
		seek(position);
		return size;
	}
	else {
		return -1;
	}
}

}
