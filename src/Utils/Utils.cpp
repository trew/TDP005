#include <Utils/Utils.h>

namespace Utils
{

std::string itos(int i)
{
	std::stringstream s_stream;
	s_stream << i;
	return s_stream.str();
}

std::string ftos(float f)
{
	std::stringstream ss;
	ss << f;
	return ss.str();
}

}