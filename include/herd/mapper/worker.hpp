#ifndef HERD_HERD_DTO_MAPPERS_WORKER_HPP
#define HERD_HERD_DTO_MAPPERS_WORKER_HPP

#include "herd/common/model/worker/data_frame_ptr.hpp"
#include "herd/common/model/worker/crypto_key_ptr.hpp"
#include "herd/common/model/worker/task.hpp"

#include "worker.pb.h"


namespace herd::mapper
{
	[[nodiscard]] common::DataFramePtr to_model(const proto::DataFramePtr& data_frame_ptr_proto);
	[[nodiscard]] common::InputDataFramePtr to_model(const proto::InputDataFramePtr& input_data_frame_ptr_proto);
	[[nodiscard]] common::CryptoKeyPtr to_model(const proto::CryptoKeyPtr& crypto_key_ptr_proto);

	[[nodiscard]] proto::DataFramePtr to_proto(const common::DataFramePtr& data_frame_ptr);
	[[nodiscard]] proto::CryptoKeyPtr to_proto(const common::CryptoKeyPtr& crypto_key_ptr);
	[[nodiscard]] proto::InputDataFramePtr to_proto(const common::InputDataFramePtr& input_data_frame_ptr);

	[[nodiscard]] proto::MapTask to_proto(const common::MapTask& map_task);
	[[nodiscard]] proto::ReduceTask to_proto(const common::ReduceTask& reduce_task);
}

#endif //HERD_HERD_DTO_MAPPERS_WORKER_HPP
