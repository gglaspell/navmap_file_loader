#include "navmap_file_loader/NavmapFileLoader.hpp"

#include <system_error>

#include "navmap_ros/conversions.hpp"
#include "navmap_ros/navmap_io.hpp"
#include "pluginlib/class_list_macros.hpp"
#include "rclcpp/rclcpp.hpp"

namespace easynav
{

void NavmapFileLoader::on_initialize()
{
  auto node = get_node();
  const auto & plugin_name = get_plugin_name();

  map_file_ = node->declare_parameter(plugin_name + ".map_file", "");
  frame_id_ = node->declare_parameter(plugin_name + ".frame_id", frame_id_);

  navmap_pub_ = node->create_publisher<navmap_ros_interfaces::msg::NavMap>(
    plugin_name + "/map",
    rclcpp::QoS(1));

  RCLCPP_INFO(
    node->get_logger(),
    "[NavmapFileLoader] initialized. map_file='%s', frame_id='%s', topic='%s/map'",
    map_file_.c_str(), frame_id_.c_str(), plugin_name.c_str());
}

void NavmapFileLoader::update(NavState & /*nav_state*/)
{
  auto node = get_node();

  if (!loaded_) {
    if (map_file_.empty()) {
      RCLCPP_ERROR_THROTTLE(
        node->get_logger(),
        *node->get_clock(),
        5000,
        "[NavmapFileLoader] Parameter '%s.map_file' is empty -- nothing to publish",
        get_plugin_name().c_str());
      return;
    }

    navmap::NavMap navmap;
    std::error_code ec;
    if (!navmap_ros::io::load_from_file(map_file_, navmap, &ec)) {
      RCLCPP_ERROR_THROTTLE(
        node->get_logger(),
        *node->get_clock(),
        5000,
        "[NavmapFileLoader] Failed to load NavMap from '%s': %s",
        map_file_.c_str(),
        ec.message().c_str());
      return;
    }

    cached_msg_ = navmap_ros::to_msg(navmap);
    cached_msg_.header.frame_id = frame_id_;
    cached_msg_.header.stamp = node->now();
    loaded_ = true;

    RCLCPP_INFO(
      node->get_logger(),
      "[NavmapFileLoader] NavMap loaded from '%s', stamped at load time, waiting for subscriber...",
      map_file_.c_str());
  }

  const bool has_subscribers = navmap_pub_->get_subscription_count() > 0;
  if (!has_subscribers) {
    had_subscribers_ = false;
    return;
  }

  if (had_subscribers_) {
    return;
  }

  navmap_pub_->publish(cached_msg_);
  had_subscribers_ = true;

  RCLCPP_INFO(
    node->get_logger(),
    "[NavmapFileLoader] Published NavMap on '%s/map'",
    get_plugin_name().c_str());
}

}  // namespace easynav

PLUGINLIB_EXPORT_CLASS(easynav::NavmapFileLoader, easynav::MapsManagerBase)
