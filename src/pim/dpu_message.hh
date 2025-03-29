#ifndef DPU_MESSAGE_HH
#define DPU_MESSAGE_HH
namespace upmem_sim
{
  enum message_type
  {
    DPU_INIT,
    DPU_DOORBELL,
    DPU_LOAD,
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
      Dpu_message(message_type type_, size_t data_count_, message_data** data_ptr_): type(type_), data_ptrs(data_ptr_),data_count(data_count_) {};

  };

}

#endif // DPU_MESSAGE_HH
