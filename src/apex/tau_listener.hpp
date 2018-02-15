//  Copyright (c) 2014 University of Oregon
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "event_listener.hpp"
#include <memory>

namespace apex {

class tau_listener : public event_listener {
private:
  void _init(void);
  bool _terminate;
public:
  tau_listener (void);
  ~tau_listener (void) { };
  static bool initialize_tau(int argc, char** avgv);
  void on_startup(startup_event_data &data);
  void on_dump(dump_event_data &data);
  void on_reset(task_identifier * id) 
      { APEX_UNUSED(id); };
  void on_shutdown(shutdown_event_data &data);
  void on_new_node(node_event_data &data);
  void on_new_thread(new_thread_event_data &data);
  void on_exit_thread(event_data &data);
  bool on_start(task_identifier *id);
  void on_stop(std::shared_ptr<profiler> &p);
  void on_yield(std::shared_ptr<profiler> &p);
  bool on_resume(task_identifier * id);
  void on_new_task(task_identifier * id, uint64_t task_id)
       { APEX_UNUSED(id); APEX_UNUSED(task_id); };
  void on_sample_value(sample_value_event_data &data);
  void on_periodic(periodic_event_data &data);
  void on_custom_event(custom_event_data &data);
  void on_send(message_event_data &data) { APEX_UNUSED(data); };
  void on_recv(message_event_data &data) { APEX_UNUSED(data); };
  void set_node_id(int node_id, int node_count);

};

int initialize_worker_thread_for_tau(void);

}

/* Weak symbols that are redefined if we load TAU at link or runtime */
extern "C" {
APEX_EXPORT APEX_WEAK_PRE int Tau_register_thread(void) APEX_WEAK_POST;
APEX_EXPORT APEX_WEAK_PRE int Tau_create_top_level_timer_if_necessary(void) APEX_WEAK_POST;
APEX_EXPORT APEX_WEAK_PRE int Tau_start(const char *) APEX_WEAK_POST;
APEX_EXPORT APEX_WEAK_PRE int Tau_stop(const char *) APEX_WEAK_POST;
APEX_EXPORT APEX_WEAK_PRE int Tau_init(int, char**) APEX_WEAK_POST;
APEX_EXPORT APEX_WEAK_PRE int Tau_exit(const char*) APEX_WEAK_POST;
APEX_EXPORT APEX_WEAK_PRE int Tau_dump(void) APEX_WEAK_POST;
APEX_EXPORT APEX_WEAK_PRE int Tau_set_node(int) APEX_WEAK_POST;
APEX_EXPORT APEX_WEAK_PRE int Tau_profile_exit_all_threads(void) APEX_WEAK_POST;
APEX_EXPORT APEX_WEAK_PRE int Tau_get_thread(void) APEX_WEAK_POST;
APEX_EXPORT APEX_WEAK_PRE int Tau_profile_exit_all_tasks(void) APEX_WEAK_POST;
APEX_EXPORT APEX_WEAK_PRE int Tau_global_stop(void) APEX_WEAK_POST;
APEX_EXPORT APEX_WEAK_PRE int Tau_trigger_context_event_thread(char*, double, int) APEX_WEAK_POST;
}
