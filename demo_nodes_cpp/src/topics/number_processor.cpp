
#include <chrono>
#include <cstdio>
#include <memory>
#include <utility>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"

#include "std_msgs/msg/string.hpp"

#include "demo_nodes_cpp/visibility_control.h"
using std::placeholders::_1;


using namespace std::chrono_literals;

namespace demo_nodes_cpp{

class NumberProcessor: public rclcpp::Node {
    public:
    explicit NumberProcessor(const rclcpp::NodeOptions & options)
: Node("number_processor", options), count_(0)
{
    publisher_ = this->create_publisher<std_msgs::msg::String>("processed_data", 10);
    
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "chatter", 10, std::bind(&NumberProcessor::topic_callback, this, _1));
}

  private:
    
    void topic_callback(const std_msgs::msg::String & msg) const
    {

      std::string messageChatter=msg.data.c_str();

      //Split y parse del string
      auto pos=messageChatter.find(":");
      if(pos != std::string::npos){
        std::string numeroStr=messageChatter.substr(pos+1);
        int value=std::stoi(numeroStr);
        int pow2=value * value;
        messageChatter=std::to_string(value)+"** 2 = " + std::to_string(pow2);

      }else{
        messageChatter="No se encontro separador";
      }

      auto message = std_msgs::msg::String();
      message.data = messageChatter;
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      publisher_->publish(message);

    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
    
};

}

RCLCPP_COMPONENTS_REGISTER_NODE(demo_nodes_cpp::NumberProcessor);