#ifndef HERD_HERD_DTO_MAPPERS_EXECUTOR_HPP
#define HERD_HERD_DTO_MAPPERS_EXECUTOR_HPP

#include "herd/common/model/job.hpp"
#include "herd/common/model/executor/execution_plan.hpp"
#include "herd/common/model/executor/stage.hpp"

#include "execution.pb.h"


namespace herd::mapper
{
	[[nodiscard]] common::JobStatus to_model(proto::JobStatus columns_proto);
	[[nodiscard]] common::Operation to_model(proto::Operation columns_proto);
	[[nodiscard]] common::node_t to_model(const proto::Node& node_proto);
	[[nodiscard]] common::Circuit to_model(const proto::Circuit& columns_proto);
	[[nodiscard]] common::Policy to_model(proto::Policy policy_proto);
	[[nodiscard]] common::stage_t to_model(const proto::Stage& columns_proto);
	[[nodiscard]] common::ExecutionPlan to_model(const proto::ExecutionPlan& columns_proto);

	[[nodiscard]] herd::proto::JobStatus to_proto(herd::common::JobStatus status);
	[[nodiscard]] proto::Operation to_proto(common::Operation operation);
	[[nodiscard]] proto::Node to_proto(const common::node_t& node);
	[[nodiscard]] proto::OutputColumn to_proto(const common::Circuit::OutputColumn& column);
	[[nodiscard]] proto::Circuit to_proto(const common::Circuit& circuit);
	[[nodiscard]] proto::Policy to_proto(const common::Policy& policy);
	[[nodiscard]] proto::Stage to_proto(const common::stage_t& stage);
	[[nodiscard]] proto::ExecutionPlan to_proto(const common::ExecutionPlan& plan);
}

#endif //HERD_HERD_DTO_MAPPERS_EXECUTOR_HPP
