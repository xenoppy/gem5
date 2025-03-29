#ifndef DPU_MESSAGE_HH
#define DPU_MESSAGE_HH
#include<any>
#include"pim/uPIMulator_backend/src/encoder/byte_stream.h"
#include "pim/ringbuffer.hh"
namespace upmem_sim
{
  enum OPCODE{
    INIT,
    LOAD,
    LAUNCH,
  };
  struct Doorbells
  {
    size_t sq_tail;
    size_t cq_head;
    Doorbells(size_t sq_tail_, size_t cq_head_): sq_tail(sq_tail_), cq_head(cq_head_) {}
  };

  struct SQ_entry
  {
    OPCODE opcode;
    int tasklet_id;
    std::any data;
    SQ_entry(OPCODE opcode_, int tasklet_id_, std::any data_): opcode(opcode_), tasklet_id(tasklet_id_), data(data_) {}
  };

  struct CQ_entry
  {
    OPCODE opcode;
    int tasklet_id;
    std::any data;
    CQ_entry(OPCODE opcode_, int tasklet_id_, std::any data_): opcode(opcode_), tasklet_id(tasklet_id_), data(data_) {}
  };


  struct init_argument
  {
    char** argv;
    int argc;
    RingBuffer<SQ_entry*> *SQ;
    RingBuffer<CQ_entry*> *CQ;
    init_argument(char** argv_, int argc_, RingBuffer<SQ_entry*> *SQ_, RingBuffer<CQ_entry*> *CQ_): argv(argv_), argc(argc_), SQ(SQ_), CQ(CQ_) {}
  };

  struct binaries_to_load{
    encoder::ByteStream* atomic_byte_stream;
    encoder::ByteStream* iram_byte_stream;
    encoder::ByteStream* wram_byte_stream;
    encoder::ByteStream* mram_byte_stream;
    binaries_to_load(encoder::ByteStream* atomic_byte_stream_, encoder::ByteStream* iram_byte_stream_, encoder::ByteStream* wram_byte_stream_, encoder::ByteStream* mram_byte_stream_): atomic_byte_stream(atomic_byte_stream_), iram_byte_stream(iram_byte_stream_), wram_byte_stream(wram_byte_stream_), mram_byte_stream(mram_byte_stream_) {}

  };
  enum message_type
  {
    DPU_INIT,
    DPU_DOORBELL,
    DPU_LOAD,

  };
  struct Dpu_message{
      message_type type;
      std::any data;
      Dpu_message(message_type type_, std::any data_): type(type_), data(data_) {};

  };

}

#endif // DPU_MESSAGE_HH
