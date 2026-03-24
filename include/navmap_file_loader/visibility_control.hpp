#ifndef NAVMAP_FILE_LOADER__VISIBILITY_CONTROL_HPP_
#define NAVMAP_FILE_LOADER__VISIBILITY_CONTROL_HPP_

#if defined _WIN32 || defined __CYGWIN__
  #define NAVMAP_FILE_LOADER_PUBLIC __declspec(dllexport)
#elif __GNUC__ >= 4
  #define NAVMAP_FILE_LOADER_PUBLIC __attribute__((visibility("default")))
#else
  #define NAVMAP_FILE_LOADER_PUBLIC
#endif

#endif  // NAVMAP_FILE_LOADER__VISIBILITY_CONTROL_HPP_
