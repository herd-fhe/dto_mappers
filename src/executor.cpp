#include "herd/mapper/executor.hpp"
#include "herd/mapper/crypto.hpp"
#include "herd/mapper/storage.hpp"
#include "herd/mapper/exception.hpp"


namespace herd::mapper
{
	common::JobStatus to_model(proto::JobStatus job_status_proto)
	{
		switch(job_status_proto)
		{
			case proto::WAITING_FOR_EXECUTION:
				return common::JobStatus::WAITING_FOR_EXECUTION;
			case proto::PENDING:
				return common::JobStatus::PENDING;
			case proto::COMPLETED:
				return common::JobStatus::COMPLETED;
			case proto::FAILED:
				return common::JobStatus::FAILED;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}


	common::Operation to_model(proto::Operation operation_proto)
	{
		switch(operation_proto)
		{
			case proto::AND:
				return common::Operation::AND;
			case proto::OR:
				return common::Operation::OR;
			case proto::NOT:
				return common::Operation::NOT;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	common::node_t to_model(const proto::Node& node_proto)
	{
		if(node_proto.has_input())
		{
			const auto& input_node_proto = node_proto.input();
			return common::InputNode{
					input_node_proto.tuple_index(),
					input_node_proto.field_index(),
					input_node_proto.bit_index()
			};
		}
		else if(node_proto.has_output())
		{
			const auto& output_node_proto = node_proto.output();
			return common::OutputNode{
					output_node_proto.field_index(),
					output_node_proto.bit_index()
			};
		}
		else if(node_proto.has_constant())
		{
			const auto& constant_node_proto = node_proto.constant();
			return common::ConstantNode{
					constant_node_proto.value()
			};
		}
		else if(node_proto.has_operation())
		{
			const auto& operation_node_proto = node_proto.operation();
			return common::OperationNode{
					to_model(operation_node_proto.type())
			};
		}
		else
		{
			throw MappingError("Proto schema, model mismatch");
		}
	}

	common::Circuit to_model(const proto::Circuit& circuit_proto)
	{
		common::Circuit circuit{};

		circuit.inputs.resize(static_cast<std::size_t>(circuit_proto.inputs().size()));
		circuit.output.reserve(static_cast<std::size_t>(circuit_proto.output().size()));

		for(int i = 0; i < circuit_proto.inputs().size(); ++i)
		{
			circuit.inputs[static_cast<std::size_t>(i)].reserve(static_cast<std::size_t>(circuit_proto.inputs(i).fields().size()));
			for(int j = 0; j < circuit_proto.inputs(i).fields().size(); ++j)
			{
				circuit.inputs[static_cast<std::size_t>(i)].emplace_back(to_model(circuit_proto.inputs(i).fields(j)));
			}
		}

		for(int i = 0; i < circuit_proto.output().size(); ++i)
		{
			const auto& field_proto = circuit_proto.output(i);
			circuit.output.emplace_back(field_proto.name(), to_model(field_proto.data_type()));
		}

		auto& graph = circuit.circuit_graph;

		for(const auto& node: circuit_proto.nodes())
		{
			graph.insert(to_model(node));
		}

		for(const auto& edge: circuit_proto.edges())
		{
			const auto start = edge.start();
			const auto end = edge.end();

			const auto begin = std::begin(graph);

			graph.add_edge(begin + start, begin + end);
		}

		return circuit;
	}

	common::Policy to_model(proto::Policy policy_proto)
	{
		switch(policy_proto)
		{
			case proto::SEQUENCED:
				return common::Policy::SEQUENCED;
			case proto::PARALLEL:
				return common::Policy::PARALLEL;
			case proto::PARALLEL_FULL:
				return common::Policy::PARALLEL_FULL;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	common::stage_t to_model(const proto::Stage& stage_proto)
	{
		if(stage_proto.has_input())
		{
			const auto& input_stage_proto = stage_proto.input();

			common::InputStage input_stage;
			input_stage.data_frame_uuid = common::UUID(input_stage_proto.data_frame_uuid());
			return input_stage;
		}
		else if(stage_proto.has_output())
		{
			const auto& output_stage_proto = stage_proto.output();

			common::OutputStage output_stage;
			if(output_stage_proto.has_name())
			{
				output_stage.name = output_stage_proto.name();
			}

			return output_stage;
		}
		else if(stage_proto.has_mapper())
		{
			const auto& mapper_stage_proto = stage_proto.mapper();

			common::MapperStage mapper_stage;
			mapper_stage.circuit = to_model(mapper_stage_proto.circuit());
			return mapper_stage;
		}
		else if(stage_proto.has_reduce())
		{
			const auto& reduce_stage_proto = stage_proto.reduce();

			common::ReduceStage reduce_stage;
			reduce_stage.circuit = to_model(reduce_stage_proto.circuit());
			reduce_stage.policy = to_model(reduce_stage_proto.policy());

			if(reduce_stage_proto.has_per_node_count())
			{
				reduce_stage.per_node_count = reduce_stage_proto.per_node_count();
			}

			return reduce_stage;
		}

		throw MappingError("Proto schema, model mismatch");
	}

	common::ExecutionPlan to_model(const proto::ExecutionPlan& execution_plan_proto)
	{
		common::ExecutionPlan execution_plan{};

		execution_plan.schema_type = to_model(execution_plan_proto.schema_type());

		const auto execution_graph_proto = execution_plan_proto.execution_graph();
		auto& execution_graph = execution_plan.execution_graph;
		for(const auto& stage_proto: execution_graph_proto.stages())
		{
			execution_graph.insert(to_model(stage_proto));
		}

		for(const auto& edge: execution_graph_proto.edges())
		{
			const auto start = edge.start();
			const auto end = edge.end();

			const auto begin_iter = std::begin(execution_graph);

			execution_graph.add_edge(begin_iter+start, begin_iter+end);
		}

		return execution_plan;
	}

	herd::proto::JobStatus to_proto(herd::common::JobStatus status)
	{
		using herd::common::JobStatus;
		switch(status)
		{
			case JobStatus::WAITING_FOR_EXECUTION:
				return herd::proto::WAITING_FOR_EXECUTION;
			case JobStatus::PENDING:
				return herd::proto::PENDING;
			case JobStatus::COMPLETED:
				return herd::proto::COMPLETED;
			case JobStatus::FAILED:
				return herd::proto::FAILED;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	proto::Operation to_proto(common::Operation operation)
	{
		switch(operation)
		{
			using enum common::Operation;
			case AND:
				return proto::Operation::AND;
			case OR:
				return proto::Operation::OR;
			case NOT:
				return proto::Operation::NOT;
			case MUX:
			case NAND:
			case XOR:
				throw MappingError("Not supported");
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	proto::Node to_proto(const common::node_t& node)
	{
		proto::Node node_proto;

		if(std::holds_alternative<common::InputNode>(node))
		{
			auto input_proto = node_proto.mutable_input();
			const auto& input = std::get<common::InputNode>(node);

			input_proto->set_tuple_index(input.tuple_index);
			input_proto->set_field_index(input.field_index);
			input_proto->set_bit_index(input.bit_index);
		}
		else if(std::holds_alternative<common::OutputNode>(node))
		{
			auto output_proto = node_proto.mutable_output();
			const auto& output = std::get<common::OutputNode>(node);

			output_proto->set_bit_index(output.bit_index);
			output_proto->set_field_index(output.field_index);
		}
		else if(std::holds_alternative<common::ConstantNode>(node))
		{
			auto constant_proto = node_proto.mutable_constant();
			const auto& constant = std::get<common::ConstantNode>(node);

			constant_proto->set_value(constant.value);
		}
		else if(std::holds_alternative<common::OperationNode>(node))
		{
			auto operation_proto = node_proto.mutable_operation();
			const auto& operation = std::get<common::OperationNode>(node);

			operation_proto->set_type(to_proto(operation.type));
		}
		else
		{
			throw MappingError("Proto schema, model mismatch");
		}
		return node_proto;
	}

	proto::OutputColumn to_proto(const common::Circuit::OutputColumn& column)
	{
		proto::OutputColumn column_proto{};

		column_proto.set_name(column.name);
		column_proto.set_data_type(to_proto(column.data_type));

		return column_proto;
	}

	proto::Circuit to_proto(const common::Circuit& circuit)
	{
		proto::Circuit circuit_proto{};

		for(const auto& input_struct: circuit.inputs)
		{
			auto input_struct_proto = circuit_proto.add_inputs();
			for(const auto data_type: input_struct)
			{
				input_struct_proto->add_fields(to_proto(data_type));
			}
		}

		for(const auto& output_column: circuit.output)
		{
			circuit_proto.mutable_output()->Add(to_proto(output_column));
		}

		const auto operations_proto = circuit_proto.mutable_nodes();
		for(const auto& operation: circuit.circuit_graph)
		{
			operations_proto->Add(to_proto(operation.value()));
		}

		const auto edges_proto = circuit_proto.mutable_edges();
		for(const auto& [start, end]: circuit.circuit_graph.edges())
		{
			const auto edge_proto = edges_proto->Add();
			edge_proto->set_start(static_cast<unsigned int>(start));
			edge_proto->set_end(static_cast<unsigned int>(end));
		}

		return circuit_proto;
	}

	proto::Policy to_proto(const common::Policy& policy)
	{
		switch(policy)
		{
			using enum common::Policy;
			case SEQUENCED:
				return proto::SEQUENCED;
			case PARALLEL:
				return proto::PARALLEL;
			case PARALLEL_FULL:
				return proto::PARALLEL_FULL;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	proto::Stage to_proto(const common::stage_t& stage)
	{
		proto::Stage stage_proto{};

		if(std::holds_alternative<common::InputStage>(stage))
		{
			const auto& input_stage = std::get<common::InputStage>(stage);
			stage_proto.mutable_input()->set_data_frame_uuid(input_stage.data_frame_uuid.as_string());
		}
		else if(std::holds_alternative<common::OutputStage>(stage))
		{
			const auto& output_stage = std::get<common::OutputStage>(stage);
			if(output_stage.name.has_value())
			{
				stage_proto.mutable_output()->set_name(output_stage.name.value());
			}
		}
		else if(std::holds_alternative<common::MapperStage>(stage))
		{
			const auto& mapper_stage = std::get<common::MapperStage>(stage);
			const auto circuit_proto = stage_proto.mutable_mapper()->mutable_circuit();
			circuit_proto->CopyFrom(to_proto(mapper_stage.circuit));
		}
		else if(std::holds_alternative<common::ReduceStage>(stage))
		{
			const auto& reduce_stage = std::get<common::ReduceStage>(stage);
			const auto reduce = stage_proto.mutable_reduce();

			const auto circuit_proto = reduce->mutable_circuit();
			circuit_proto->CopyFrom(to_proto(reduce_stage.circuit));

			reduce->set_policy(to_proto(reduce_stage.policy));

			if(reduce_stage.per_node_count.has_value())
			{
				reduce->set_per_node_count(reduce_stage.per_node_count.value());
			}
		}
		else
		{
			throw MappingError("Proto schema, model mismatch");
		}

		return stage_proto;
	}

	proto::ExecutionPlan to_proto(const common::ExecutionPlan& plan)
	{
		proto::ExecutionPlan proto_plan;
		proto_plan.set_schema_type(to_proto(plan.schema_type));

		const auto execution_graph_proto = proto_plan.mutable_execution_graph();
		const auto stages_proto = execution_graph_proto->mutable_stages();

		for(const auto& stage: plan.execution_graph)
		{
			const auto& stage_details = stage.value();
			stages_proto->Add(to_proto(stage_details));
		}

		const auto edges_proto = execution_graph_proto->mutable_edges();
		for(const auto& [start, end]: plan.execution_graph.edges())
		{
			const auto edge_proto = edges_proto->Add();
			edge_proto->set_start(static_cast<unsigned int>(start));
			edge_proto->set_end(static_cast<unsigned int>(end));
		}

		return proto_plan;
	}
}