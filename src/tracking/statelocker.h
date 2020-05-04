//==============================================================================
// Smart Eye Embedded Tracking SDK
// Copyright (c) 2017-2019 Smart Eye AB. All rights reserved.
//
// This software is provided as a toolkit for developing head and eye tracking
// applications. The software is provided "as is" without warranty of any kind.
// Unauthorized copying of this file, via any medium is strictly prohibited.
//==============================================================================

//==============================================================================
// statelocker.h
//==============================================================================

#ifndef _STATE_LOCKER_H_
#define _STATE_LOCKER_H_

#include <mutex>
#include <chrono>
#include <condition_variable>

template <typename T>
class StateLocker
{
public:
  std::unique_ptr<T> getState(bool blocking)
  {
    std::unique_lock<std::mutex> lock(m_stateMutex);
    if (blocking && !m_stateAvailable)
    {
      // If we don't get any images to track on, the state condition would never be woken up.
      // Thus we wait with a timeout so that the main program can be interrupted if need be.
      m_stateCondition.wait_for(
          lock, std::chrono::milliseconds(500), [&] { return m_stateAvailable; });
    }
    m_stateAvailable = false;
    return std::move(m_state);
  }

  void swapState(std::unique_ptr<T>& state)
  {
    std::lock_guard<std::mutex> lock(m_stateMutex);
    m_state.swap(state);
    m_stateAvailable = true;
    m_stateCondition.notify_all();
  }

private:
  std::unique_ptr<T> m_state;
  std::mutex m_stateMutex;
  bool m_stateAvailable = false;
  std::condition_variable m_stateCondition;
};

#endif  // _STATE_LOCKER_H_
