#ifndef HERD_HERD_DTO_MAPPERS_STORAGE_HPP
#define HERD_HERD_DTO_MAPPERS_STORAGE_HPP

#include "herd/common/model/column_meta.hpp"
#include "herd/common/model/data_type.hpp"

#include "storage.pb.h"


namespace herd::mapper
{
	[[nodiscard]] common::DataType to_model(proto::DataType data_type);
	[[nodiscard]] std::vector<common::ColumnMeta> to_model(const google::protobuf::RepeatedPtrField<proto::ColumnDescriptor>& columns_proto);

	[[nodiscard]] proto::DataType to_proto(common::DataType data_type);
	[[nodiscard]] google::protobuf::RepeatedPtrField<proto::ColumnDescriptor> to_proto(const std::vector<common::ColumnMeta>& columns);
}

#endif //HERD_HERD_DTO_MAPPERS_STORAGE_HPP
