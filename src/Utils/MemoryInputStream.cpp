#include "MemoryInputStream.hpp"
#include <cstring>

namespace Audio
{
MemoryInputStream::MemoryInputStream() :
m_data  (NULL),
m_size  (0),
m_offset(0){
}

void MemoryInputStream::open(const void* data, std::size_t sizeInBytes){
	m_data = static_cast<const char*>(data);
	m_size = sizeInBytes;
	m_offset = 0;
}

i64 MemoryInputStream::read(void* data, i64 size){
	if(!m_data)
		return -1;

	i64 endPosition = m_offset + size;
	i64 count = endPosition <= m_size ? size : m_size - m_offset;

	if(count > 0)
	{
		std::memcpy(data, m_data + m_offset, static_cast<std::size_t>(count));
		m_offset += count;
	}

	return count;
}

i64 MemoryInputStream::seek(i64 position){
	if(!m_data)
		return -1;

	m_offset = position < m_size ? position : m_size;
	return m_offset;
}

i64 MemoryInputStream::tell(){
	if(!m_data)
		return -1;

	return m_offset;
}

i64 MemoryInputStream::getSize(){
	if(!m_data)
		return -1;

	return m_size;
}

}
