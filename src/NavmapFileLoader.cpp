#include "rclcpp/rclcpp.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_msgs/msg/state.hpp"
#include "easynav_maps_manager/MapsManagerNode.hpp"
#include "easynav_core/NavState.hpp"

using namespace std::chrono_literals;

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = easynav::MapsManagerNode::make_shared();
  auto nav_state = std::make_shared<easynav::NavState>();

  node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);
  if (node->get_current_state().id() !=
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE)
  {
    RCLCPP_ERROR(node->get_logger(), "Failed to configure MapsManagerNode");
    rclcpp::shutdown();
    return 1;
  }

  node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
  if (node->get_current_state().id() !=
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE)
  {
    RCLCPP_ERROR(node->get_logger(), "Failed to activate MapsManagerNode");
    rclcpp::shutdown();
    return 1;
  }

  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());

  rclcpp::WallRate rate(10);
  while (rclcpp::ok()) {
    node->cycle(nav_state);
    executor.spin_some();
    rate.sleep();
  }

  rclcpp::shutdown();
  return 0;
}
