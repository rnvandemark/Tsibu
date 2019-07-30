#include "../include/SurroundingsAnalysisController.hpp"

void SurroundingsAnalysisController::do_process_for(
	std::string descriptor,
	unsigned int channel,
	unsigned int type,
	std::string variable_name,
	std::chrono::system_clock::time_point time_now)
{
	bool changed = false;
	
	unsigned int depth_reading = adc->read_channel(channel);
	std::cout << "\r" << depth_reading << " ";
	std::array<unsigned int, 4> constants = sensor_constants[type];
	std::queue<DepthSensorDataPoint>* queue = &recent_readings_map[descriptor];
	
	DepthSensorDataPoint* popped;
	
	float sum = dynamic_cast<FSMVariable<float>*>(
		fsm->get_variable(variable_name)
	)->get() * queue->size();
	
	if ((depth_reading > constants[2]) && (depth_reading < constants[3]))
	{
		queue->push(DepthSensorDataPoint(time_now, depth_reading));
		sum += depth_reading;
		
		if (queue->size() > SURROUNDINGS_ANALYSIS_MAX_READING_QUEUE_SIZE)
		{
			popped = &(queue->front());
			queue->pop();
			sum -= popped->value;
		}
		
		changed = true;
	}
	
	if (!queue->empty())
	{
		popped = &(queue->front());
		if (popped->is_too_old(time_now, SURROUNDINGS_ANALYSIS_DATA_POINT_RELAVANT_FOR_MS))
		{
			queue->pop();
			sum -= popped->value;
			changed = true;
		}
	}
	
	if (changed)
	{
		float* new_avg = nullptr;
		if (queue->empty())
		{
			new_avg = new float(0.0);
		}
		else
		{
			new_avg = new float(sum / queue->size());
		}
		
		fsm->set_variable<float>(variable_name, new_avg);
	}
}

float SurroundingsAnalysisController::do_lin_reg_analysis_for(
	std::string descriptor,
	std::string variable_name)
{
	double magnitude = -1.0;
	
	std::queue<DepthSensorDataPoint> queue = recent_readings_map[descriptor];
	if (queue.size() >= SURROUNDINGS_ANALYSIS_MIN_SIZE_FOR_REGRESSION_ANALYSIS)
	{
		unsigned int original_size = queue.size();
		
		DepthSensorDataPoint data_point = queue.front();
		queue.pop();
		
		std::queue<float> normalized_times;
		std::chrono::system_clock::time_point reference_start = data_point.time;
		float time_avg = 0.0;
		while (!queue.empty())
		{
			data_point = queue.front();
			queue.pop();
			
			float time = std::chrono::duration_cast<std::chrono::milliseconds>(
				data_point.time - reference_start
			).count() / 1000.0;
			
			time_avg += time;
			normalized_times.push(time);
		}
		
		time_avg /= original_size;
		
		queue = recent_readings_map[descriptor];
		data_point = queue.front();
		queue.pop();
		
		float average_sensor_reading = dynamic_cast<FSMVariable<float>*>(fsm->get_variable(variable_name))->get();
		float numerator = 0.0, denominator = 0.0;
		float normalized_time, dt;
		while (!queue.empty())
		{
			data_point = queue.front();
			queue.pop();
			
			normalized_time = normalized_times.front();
			normalized_times.pop();
			
			dt = normalized_time - time_avg;
			numerator += (dt * (data_point.value - average_sensor_reading));
			denominator += (dt * dt);
		}
		
		magnitude = numerator / denominator;
	}
	
	return magnitude;
}

void SurroundingsAnalysisController::init()
{
	fsm->set_current_state(new SurroundingsAnalysis(SurroundingsAnalysis::NOTHING_NOTICEABLE));
	
	recent_readings_map[SURROUNDINGS_ANALYSIS_FORWARD_LEFT_DESCRIPTOR] = std::queue<DepthSensorDataPoint>();
	recent_readings_map[SURROUNDINGS_ANALYSIS_FORWARD_RIGHT_DESCRIPTOR] = std::queue<DepthSensorDataPoint>();
	recent_readings_map[SURROUNDINGS_ANALYSIS_BACKWARD_LEFT_DESCRIPTOR] = std::queue<DepthSensorDataPoint>();
	recent_readings_map[SURROUNDINGS_ANALYSIS_BACKWARD_RIGHT_DESCRIPTOR] = std::queue<DepthSensorDataPoint>();
	
	fsm->set_variable<float>(
		SURROUNDINGS_ANALYSIS_VARIABLE_FORWARD_LEFT_AVG,
		new float(0.0)
	);
	
	fsm->set_variable<float>(
		SURROUNDINGS_ANALYSIS_VARIABLE_FORWARD_RIGHT_AVG,
		new float(0.0)
	);
	
	fsm->set_variable<float>(
		SURROUNDINGS_ANALYSIS_VARIABLE_BACKWARD_LEFT_AVG,
		new float(0.0)
	);
	
	fsm->set_variable<float>(
		SURROUNDINGS_ANALYSIS_VARIABLE_BACKWARD_RIGHT_AVG,
		new float(0.0)
	);
}

void SurroundingsAnalysisController::update_inputs()
{
	std::chrono::system_clock::time_point time_now = std::chrono::system_clock::now();
	
	do_process_for(
		SURROUNDINGS_ANALYSIS_FORWARD_LEFT_DESCRIPTOR,
		SURROUNDINGS_ANALYSIS_FORWARD_LEFT_DEPTH_DIN_CHANNEL,
		SURROUNDINGS_ANALYSIS_FORWARD_LEFT_TYPE,
		SURROUNDINGS_ANALYSIS_VARIABLE_FORWARD_LEFT_AVG,
		time_now
	);
	
	/*
	do_process_for(
		SURROUNDINGS_ANALYSIS_FORWARD_RIGHT_DESCRIPTOR,
		SURROUNDINGS_ANALYSIS_FORWARD_RIGHT_DEPTH_DIN_CHANNEL,
		SURROUNDINGS_ANALYSIS_FORWARD_RIGHT_TYPE,
		SURROUNDINGS_ANALYSIS_VARIABLE_FORWARD_RIGHT_AVG,
		time_now
	);
	
	do_process_for(
		SURROUNDINGS_ANALYSIS_BACKWARD_LEFT_DESCRIPTOR,
		SURROUNDINGS_ANALYSIS_BACKWARD_LEFT_DEPTH_DIN_CHANNEL,
		SURROUNDINGS_ANALYSIS_BACKWARD_LEFT_TYPE,
		SURROUNDINGS_ANALYSIS_VARIABLE_BACKWARD_LEFT_AVG,
		time_now
	);
	
	do_process_for(
		SURROUNDINGS_ANALYSIS_BACKWARD_RIGHT_DESCRIPTOR,
		SURROUNDINGS_ANALYSIS_BACKWARD_RIGHT_DEPTH_DIN_CHANNEL,
		SURROUNDINGS_ANALYSIS_BACKWARD_RIGHT_TYPE,
		SURROUNDINGS_ANALYSIS_VARIABLE_BACKWARD_RIGHT_AVG,
		time_now
	);
	*/
}

bool SurroundingsAnalysisController::process()
{
	SurroundingsAnalysis current_state = *(fsm->get_current_state());
	SurroundingsAnalysis next_state = SurroundingsAnalysis::NOTHING_NOTICEABLE;
	
	std::queue<DepthSensorDataPoint>* queue = &recent_readings_map[SURROUNDINGS_ANALYSIS_FORWARD_LEFT_DESCRIPTOR];
	if (!queue->empty())
	{
		next_state = SurroundingsAnalysis::STATIC_OBJECT_GETTING_TOO_CLOSE;
		
		std::array<unsigned int, 4> constants = sensor_constants[SURROUNDINGS_ANALYSIS_FORWARD_LEFT_TYPE];
		float magnitude = do_lin_reg_analysis_for(
			SURROUNDINGS_ANALYSIS_FORWARD_LEFT_DESCRIPTOR,
			SURROUNDINGS_ANALYSIS_VARIABLE_FORWARD_LEFT_AVG
		);
		
		unsigned int most_recent = queue->back().value;
		if (most_recent > constants[0])
		{
			next_state = SurroundingsAnalysis::STATIC_OBJECT_IS_TOO_CLOSE;
		}
		
		if (magnitude >= SURROUNDINGS_ANALYSIS_OBJECT_IS_KINETIC_THRESHOLD)
		{
			if (next_state == SurroundingsAnalysis::STATIC_OBJECT_IS_TOO_CLOSE)
			{
				next_state = SurroundingsAnalysis::KINETIC_OBJECT_IS_TOO_CLOSE;
			}
			else if (next_state == SurroundingsAnalysis::STATIC_OBJECT_GETTING_TOO_CLOSE)
			{
				next_state = SurroundingsAnalysis::KINETIC_OBJECT_GETTING_TOO_CLOSE;
			}
		}
	}
	
	fsm->set_current_state(new SurroundingsAnalysis(next_state));
	return static_cast<int>(current_state) != static_cast<int>(next_state);
}

SurroundingsAnalysisController::SurroundingsAnalysisController(FSM<SurroundingsAnalysis>* f, FSMSystemCommunicator* fsc)
: FSMController(SURROUNDINGS_ANALYSIS_FSM_NAME, f, SURROUNDINGS_ANALYSIS_REEVALUATION_RATE_MILLISECONDS, fsc)
{
	sensor_constants[SURROUNDINGS_ANALYSIS_ID_2Y0A41SK0F] = {
		SURROUNDINGS_ANALYSIS_IS_TOO_CLOSE_THRESHOLD_2Y0A41SK0F,
		SURROUNDINGS_ANALYSIS_GETTING_TOO_CLOSE_THRESHOLD_2Y0A41SK0F,
		SURROUNDINGS_ANALYSIS_FILTER_LOW_2Y0A41SK0F,
		SURROUNDINGS_ANALYSIS_FILTER_HIGH_2Y0A41SK0F
	};
	
	sensor_constants[SURROUNDINGS_ANALYSIS_ID_GP2Y0A21YK0F] = {
		SURROUNDINGS_ANALYSIS_IS_TOO_CLOSE_THRESHOLD_GP2Y0A21YK0F,
		SURROUNDINGS_ANALYSIS_GETTING_TOO_CLOSE_THRESHOLD_GP2Y0A21YK0F,
		SURROUNDINGS_ANALYSIS_FILTER_LOW_GP2Y0A21YK0F,
		SURROUNDINGS_ANALYSIS_FILTER_HIGH_GP2Y0A21YK0F
	};
	
	adc = new MCP3008SPI();
}

SurroundingsAnalysisController::~SurroundingsAnalysisController()
{
	if (adc != nullptr)
	{
		delete adc;
	}
}
