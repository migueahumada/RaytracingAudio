#pragma once
#include "Prerequisites.h"
#include <CL/opencl.h>
#include <iostream>

#define THROW_ON_ERROR(x, msg)                     \
{                                                  \
  if(err != CL_SUCCESS)                            \
  {                                                \
    std::cerr << msg << std::endl;                 \
    return;                                      \
  }                                                \
}                                                  \

class ComputeAPI
{
 public:
  ComputeAPI() = default;
  ~ComputeAPI()
  {
    clReleaseDevice(m_device);
    clReleaseContext(m_context);
    clReleaseCommandQueue(m_cmdQueue);
    clReleaseProgram(m_program);
  }

  void Init();
  void DecodeProgram(const String& filePath);
  

 private:

  cl_device_id m_device {0};
  cl_context m_context  {0};
  cl_command_queue m_cmdQueue {0};
  cl_program m_program;
  //Vector<cl_program> m_programs;
  
};

