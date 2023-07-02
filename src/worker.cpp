#include "herd/mapper/worker.hpp"

#include "herd/common/model/worker/task.hpp"
#include "herd/mapper/crypto.hpp"
#include "herd/mapper/executor.hpp"
#include "herd/mapper/exception.hpp"

namespace herd::mapper
{
	common::DataFramePtr to_model(const proto::DataFramePtr& data_frame_ptr_proto)
	{
		common::DataFramePtr data_frame_ptr{};

		try
		{
			data_frame_ptr.uuid = common::UUID(data_frame_ptr_proto.data_frame_uuid());
		}
		catch (const std::invalid_argument& error)
		{
			throw mapper::MappingError(error.what());
		}

		data_frame_ptr.partition = data_frame_ptr_proto.partition();

		return data_frame_ptr;
	}

	common::InputDataFramePtr to_model(const proto::InputDataFramePtr& input_data_frame_ptr_proto)
	{
		common::InputDataFramePtr input_data_frame_ptr{};

		input_data_frame_ptr.pointer = to_model(input_data_frame_ptr_proto.pointer());
		input_data_frame_ptr.row_count = input_data_frame_ptr_proto.row_count();

		return input_data_frame_ptr;
	}


	common::CryptoKeyPtr to_model(const proto::CryptoKeyPtr& crypto_key_ptr_proto)
	{
		common::CryptoKeyPtr crypto_key_ptr{};

		crypto_key_ptr.schema_type = to_model(crypto_key_ptr_proto.schema_type());

		return crypto_key_ptr;
	}

	proto::DataFramePtr to_proto(const common::DataFramePtr& data_frame_ptr)
	{
		proto::DataFramePtr data_frame_ptr_proto{};

		data_frame_ptr_proto.set_data_frame_uuid(data_frame_ptr.uuid.as_string());
		data_frame_ptr_proto.set_partition(data_frame_ptr_proto.partition());

		return data_frame_ptr_proto;
	}

	proto::CryptoKeyPtr to_proto(const common::CryptoKeyPtr& crypto_key_ptr)
	{
		proto::CryptoKeyPtr crypto_key_ptr_proto{};

		crypto_key_ptr_proto.set_schema_type(to_proto(crypto_key_ptr.schema_type));

		return crypto_key_ptr_proto;
	}

	proto::InputDataFramePtr to_proto(const common::InputDataFramePtr& input_data_frame_ptr)
	{
		proto::InputDataFramePtr input_data_frame_ptr_proto{};

		input_data_frame_ptr_proto.set_row_count(input_data_frame_ptr.row_count);

		auto pointer = input_data_frame_ptr_proto.mutable_pointer();
		pointer->CopyFrom(to_proto(input_data_frame_ptr.pointer));


		return input_data_frame_ptr_proto;
	}

	proto::MapTask to_proto(const common::MapTask& map_task)
	{
		proto::MapTask map_task_proto{};

		map_task_proto.set_session_uuid(map_task.session_uuid.as_string());

		auto input_data_frame = map_task_proto.mutable_input_data_frame_ptr();
		input_data_frame->CopyFrom(to_proto(map_task.input_data_frame_ptr));

		auto output_data_frame = map_task_proto.mutable_output_data_frame_ptr();
		output_data_frame->CopyFrom(to_proto(map_task.output_data_frame_ptr));

		auto crypto_key_ptr = map_task_proto.mutable_crypto_key_ptr();
		crypto_key_ptr->CopyFrom(to_proto(map_task.crypto_key_ptr));

		auto circuit = map_task_proto.mutable_circuit();
		circuit->CopyFrom(to_proto(map_task.circuit));

		return map_task_proto;
	}

}