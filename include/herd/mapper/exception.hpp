#ifndef HERD_HERD_DTO_MAPPERS_EXCEPTION_HPP
#define HERD_HERD_DTO_MAPPERS_EXCEPTION_HPP

#include <stdexcept>


namespace herd::mapper
{
	struct MappingError: public std::runtime_error {
		using std::runtime_error::runtime_error;
	};
}

#endif //HERD_HERD_DTO_MAPPERS_EXCEPTION_HPP
