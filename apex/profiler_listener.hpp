//  Copyright (c) 2014 University of Oregon
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef PROFILER_LISTENER_HPP
#define PROFILER_LISTENER_HPP

#include "event_listener.hpp"
#include <boost/atomic.hpp>

using namespace std;

namespace apex {

class profiler_listener : public event_listener {
private:
  void _init(void);
  bool _terminate;
  static boost::atomic<int> active_tasks;
  static profiler * main_timer;
  static void finalize_profiles(void);
  static void write_profile(void);
  static void delete_profiles(void);
  static void process_profiles(void);
  static void process_profile(profiler * p, bool sample);
  static int node_id;
public:
  profiler_listener (void)  : _terminate(false) { };
  ~profiler_listener (void) { };
  void on_startup(startup_event_data &data);
  void on_shutdown(shutdown_event_data &data);
  void on_new_node(node_event_data &data);
  void on_new_thread(new_thread_event_data &data);
  void on_start(timer_event_data &data);
  void on_stop(timer_event_data &data);
  void on_resume(timer_event_data &data);
  void on_sample_value(sample_value_event_data &data);
  void on_periodic(periodic_event_data &data);
};

}

#endif // PROFILER_LISTENER_HPP
