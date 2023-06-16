#ifndef HERD_HERD_DTO_MAPPERS_CRYPTO_HPP
#define HERD_HERD_DTO_MAPPERS_CRYPTO_HPP

#include "herd/common/model/schema_type.hpp"

#include "common.pb.h"


namespace herd::mapper
{
	[[nodiscard]] common::SchemaType to_model(proto::SchemaType type);

	[[nodiscard]] proto::SchemaType to_proto(common::SchemaType type);
}

#endif //HERD_HERD_DTO_MAPPERS_CRYPTO_HPP
