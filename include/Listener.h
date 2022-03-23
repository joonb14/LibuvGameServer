#ifndef __LISTENER_H__
#define __LISTENER_H__
#include <memory>
#include "CorePch.h"
#include "Session.h"

class Listener : public std::enable_shared_from_this<Listener>
{
public:
  Listener() = default;
  ~Listener();
  
  bool startAccept(ServerServiceRef service);
    
private:
  ServerServiceRef _service;
};

#endif