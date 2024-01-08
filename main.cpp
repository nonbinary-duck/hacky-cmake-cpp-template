#include <iostream>

using std::cout, std::cin, std::endl;

#include "rclcpp/rclcpp.hpp"

#include "src/utils.hpp"
#include "src/simple_controller.hpp"


/**
 * @brief Called when the program is launched
 * 
 * @param argc Count of command-line arguments
 * @param argv Args, zero is the name of the program
 * @return int An error code
 */
int main(int argc, char *argv[])
{
	//cout << "Hello, world!! I'm simple-ros2-test :)" << endl;

	rclcpp::init(argc, argv);
	rclcpp::spin( std::make_shared<simple_ros2_test::SensorSubscriber>() );
	rclcpp::shutdown();

	
	return 0;
}