#include "herd/mapper/worker.hpp"

#include "herd/mapper/crypto.hpp"
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

		data_frame_ptr.block_id = data_frame_ptr_proto.block_id();

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
		data_frame_ptr_proto.set_block_id(data_frame_ptr_proto.block_id());

		return data_frame_ptr_proto;
	}

	proto::CryptoKeyPtr to_proto(const common::CryptoKeyPtr& crypto_key_ptr)
	{
		proto::CryptoKeyPtr crypto_key_ptr_proto{};

		crypto_key_ptr_proto.set_schema_type(to_proto(crypto_key_ptr.schema_type));

		return crypto_key_ptr_proto;
	}
}