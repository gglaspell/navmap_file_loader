#ifndef NAVMAP_FILE_LOADER__NAVMAP_FILE_LOADER_HPP_
#define NAVMAP_FILE_LOADER__NAVMAP_FILE_LOADER_HPP_

#include <string>

#include "easynav_core/MapsManagerBase.hpp"
#include "navmap_ros_interfaces/msg/nav_map.hpp"
#include "rclcpp/publisher.hpp"

namespace easynav
{

class NavmapFileLoader : public MapsManagerBase
{
public:
  void on_initialize() override;
  void update(NavState & nav_state) override;

private:
  std::string map_file_;
  std::string frame_id_{"map"};
  bool loaded_{false};    // file successfully read into cached_msg_
  bool published_{false}; // at least one subscriber has received it

  navmap_ros_interfaces::msg::NavMap cached_msg_;

  // rclcpp::Publisher — matches the return type of get_node()->create_publisher()
  rclcpp::Publisher<navmap_ros_interfaces::msg::NavMap>::SharedPtr navmap_pub_;
};

}  // namespace easynav

#endif  // NAVMAP_FILE_LOADER__NAVMAP_FILE_LOADER_HPP_

