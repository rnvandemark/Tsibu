#ifndef TSIBU_SURROUNDINGS_ANALYSIS_CONTROLLER_HPP
#define TSIBU_SURROUNDINGS_ANALYSIS_CONTROLLER_HPP

#include <string>
#include <array>
#include <unordered_map>
#include <queue>
#include <chrono>

#include "../../common/include/FSMController.hpp"
#include "SurroundingsAnalysis.hpp"
#include "SurroundingsAnalysisConstants.hpp"
#include "DepthSensorDataPoint.hpp"

#include "../../common/include/MCP3008SPI.hpp"

/*
 *  This class represents the controller for the SurroundingsAnalysis FSM.
 */
class SurroundingsAnalysisController : public FSMController<SurroundingsAnalysis>
{
	private:
		
		/*
		 *  A map for the depth sensor constants, depending on the type.
		 */
		std::unordered_map<unsigned int, std::array<unsigned int, 4>> sensor_constants;
		
		/*
		 *  A pointer to the ADC that is handling normalizing the depth sensor data.
		 */
		MCP3008SPI* adc;
		
		/*
		 *  A map of queues, where each queue has the recent readings for each depth sensor.
		 */
		std::unordered_map<std::string, std::queue<DepthSensorDataPoint>> recent_readings_map;
		
		/*
		 *  A helper function to check the processing for a depth sensor.
		 *  @param descriptor The descriptor of the depth sensor.
		 *  @param channel The channel number, to read the value from.
		 *  @param type The type ID of the depth sensor model being used.
		 *  @param variable_name The string name of the variable, in case an overwrite needs to occur.
		 *  @param time_now The time point to consider as "now".
		 */
		void do_process_for(
			std::string descriptor,
			unsigned int channel,
			unsigned int type,
			std::string variable_name,
			std::chrono::system_clock::time_point time_now
		);
		
		/*
		 *  A helper function to perform the linear regression on the readings from a sensor.
		 *  @param descriptor The descriptor of the depth sensor.
		 *  @param variable_name The string name of the variable, in case an overwrite needs to occur.
		 *  @return The regression's magnitude.
		 */
		float do_lin_reg_analysis_for(std::string descriptor, std::string variable_name);
	
	protected:

		/*
		 *  Override of FSMController's definition.
		 *  See that definition for more information.
		 */
		void init();

		/*
		 *  Override of FSMController's definition.
		 *  See that definition for more information.
		 */
		void update_inputs();

		/*
		 *  Override of FSMController's definition.
		 *  See that definition for more information.
		 */
		bool process();

	public:

		/*
		 *  The constructor.
		 *  Simply calls the base constructor.
		 *  @param f The FSM for surroundings analyses.
		 *  @param fsc The system communicator that can contact other FSM instances to observe their current states.
		 */
		explicit SurroundingsAnalysisController(FSM<SurroundingsAnalysis>* f, FSMSystemCommunicator* fsc);

		/*
		 *  The destructor.
		 *  Cleans up the ADC container.
		 */
		~SurroundingsAnalysisController();
};

#endif /* TSIBU_SURROUNDINGS_ANALYSIS_CONTROLLER_HPP */
