#include "herd/mapper/crypto.hpp"
#include "herd/mapper/exception.hpp"


namespace herd::mapper
{
	common::SchemaType to_model(proto::SchemaType type)
	{
		switch(type)
		{
			case proto::BINFHE:
				return common::SchemaType::BINFHE;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	proto::SchemaType to_proto(common::SchemaType type)
	{
		switch(type)
		{
			case common::SchemaType::BINFHE:
				return proto::BINFHE;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}
}