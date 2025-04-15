#ifndef DPU_MESSAGE_HH
#define DPU_MESSAGE_HH
#include"pim/ring_buffer.hh"
namespace upmem_sim
{
  #define SQ_SIZE 16
  #define CQ_SIZE 16
  struct Doorbells
  {
    size_t sq_tail;
    size_t cq_head;
    Doorbells():sq_tail(0), cq_head(0) {}
    Doorbells(size_t sq_tail_, size_t cq_head_):sq_tail(sq_tail_), cq_head(cq_head_) {}
  };
  enum message_type
  {
    DPU_INIT,
    DPU_LOAD,
    DPU_TRANS,
    DPU_LAUNCH,
    DPU_UPDATE,
    DPU_CHECK,
    //ASYNCHRONOUS
    DPU_INIT_ASYNCHRONOUS = 100,
    DPU_LOAD_ASYNCHRONOUS,
    DPU_LAUNCH_ASYNCHRONOUS,
    DPU_DOORBELL,
    //RESPONSE
    DPU_FINISHED=200,
    DPU_UPDATE_SYSTEM,

  };
  enum dpu_check_target
  {
    DPU_CHECK_INIT,
    DPU_CHECK_MEM,
    DPU_CHECK_GROUP,
    DPU_CHECK_ALL,
  };
  enum dpu_launch_policy
  {
    DPU_DEFAULT,
    DPU_ASYNCHRONOUS,
    DPU_SYNCHRONOUS,
  };
  struct message_data{
    size_t size;
    const void* data;
    message_data(size_t size_, const void* data_):
      size(size_), data(data_)
    {
    };
  };
  struct Dpu_message{
      message_type type;
      size_t data_count; // number of data items
      message_data** data_ptrs;
      Dpu_message(message_type type_, size_t data_count_, message_data** data_ptr_): type(type_),data_count(data_count_), data_ptrs(data_ptr_) {};

  };




}

#endif // DPU_MESSAGE_HH
