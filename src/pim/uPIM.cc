/*
 * Copyright (c) 2024 The Regents of the University of California
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "pim/uPIM.hh"
#include "pim/uPIMulator_backend/src/external.hh"
#include "pim/uPIM.hh"
#include "pim/dpu_message.hh"


namespace gem5
{

  inline upmem_sim::Dpu_message* makeSingleDataDpuMessage(
    upmem_sim::message_type type,
    size_t data_size,
    void* data_ptr)
  {
    size_t argc=1;
    upmem_sim::message_data** argv_ptr = new upmem_sim::message_data*[argc];
    argv_ptr[0] = new upmem_sim::message_data(data_size,data_ptr);
    return new upmem_sim::Dpu_message(type, argc,argv_ptr);
  }

  PacketPtr make_pkt()
  {
    Request::Flags testflag(0);
    RequestPtr req = std::make_shared<Request>(
        0, 0, testflag, 0, 0, 0);
    return Packet::createRead(req);

  }

  Dpu::Dpu(const DpuParams &p) : SimObject(p)
  {
    printf("enter DPU\n");
  }

  uPIM::uPIM(const uPIMParams &p) : Dpu(p), cpu_clock(p.cpu_clock), rank_clock(p.rank_clock),
                                    rank_cycle_event([this]
                                                     { process_rank_Cycle(); }, name()),
                                    cpusidePort(name() + ".cpu_side", this),
                                    system(nullptr)
  {
    printf("enter uPIM\n");
  }

  void uPIM::startup()
  {
   printf("uPIM: startup called\n");
  }

  void uPIM::process_rank_Cycle()
  {
    //printf("%s: enter process_rank_Cycle\n", this->name().c_str());
    if(system != nullptr && system->is_benchmark_set)
    {
      system->dpu_check_cycle(); // just to check if the system is finished

      // cpusidePort.trySendRetry();
      if (not system->is_finished())
      {
        system->rank_cycle();
        schedule(rank_cycle_event, curTick() + rank_clock);
      }
      else
      {
        printf("uPIM: sendTimingResp---is_finished\n");
        PacketPtr pkt = make_pkt();
        upmem_sim::Dpu_message* msg = makeSingleDataDpuMessage(upmem_sim::DPU_UPDATE_SYSTEM,
                                  sizeof(upmem_sim::simulator::System*),
                                  system);
        pkt->dataDynamic<upmem_sim::Dpu_message>(msg);
        pkt->makeTimingResponse();
        cpusidePort.sendTimingResp(pkt);
        return;
      }
      if (system->is_zombie())
      {
        printf("uPIM: sendTimingResp---is_zombie\n");
        PacketPtr pkt = make_pkt();
        upmem_sim::Dpu_message* msg = makeSingleDataDpuMessage(upmem_sim::DPU_UPDATE_SYSTEM,
                                  sizeof(upmem_sim::simulator::System*),
                                  system);
        pkt->dataDynamic<upmem_sim::Dpu_message>(msg);
        pkt->makeTimingResponse();
        cpusidePort.sendTimingResp(pkt);
      }
    }
    else
    {
      // printf("uPIM: system is not initialized\n");
      schedule(rank_cycle_event, curTick() + rank_clock);
    }
  }

  Port &
  uPIM::getPort(const std::string &if_name, PortID idx)
  {
    panic_if(idx != InvalidPortID, "This object doesn't support vector ports");

    // This is the name from the Python SimObject declaration (SimpleMemobj.py)
    if (if_name == "cpu_side")
    {
      return cpusidePort;
    }
    else
    {
      // pass it along to our super class
      return SimObject::getPort(if_name, idx);
    }
  }

  void
  uPIM::CPUSidePort::sendPacket(PacketPtr pkt)
  {
    // Note: This flow control is very simple since the memobj is blocking.

    panic_if(blockedPacket != nullptr, "Should never try to send if blocked!");

    // If we can't send the packet across the port, store it for later.
    if (!sendTimingResp(pkt))
    {
      blockedPacket = pkt;
    }
  }

  // try: just return null list?
  AddrRangeList
  uPIM::CPUSidePort::getAddrRanges() const
  {
    return AddrRangeList();
  }

  void
  uPIM::CPUSidePort::trySendRetry()
  {
    if (needRetry && blockedPacket == nullptr)
    {
      // Only send a retry if the port is now completely free
      needRetry = false;
      printf("uPIM: Sending retry req for %d\n", id);
      // DPRINTF(SimpleMemobj, "Sending retry req for %d\n", id);
      sendRetryReq();
    }
  }

  void
  uPIM::CPUSidePort::recvFunctional(PacketPtr pkt)
  {
    printf("uPIM: enter recvFunctional\n");
    // Just doing nothing.
  }

  bool
  uPIM::CPUSidePort::recvTimingReq(PacketPtr pkt)
  {
    needRetry = true;
    printf("uPIM: enter recvTimingReq\n");
    upmem_sim::Dpu_message* msg = reinterpret_cast<upmem_sim::Dpu_message *>(pkt->getPtr<uint8_t>());
    printf("uPIM: pkg received. \n");
    printf("uPIM: msg type: %d\n", msg->type);
    switch (msg->type){
      case upmem_sim::DPU_INIT:
        {
          printf("uPIM: DPU_INIT received\n");
          const char** argv= new const char*[msg->data_count];
          for (size_t i = 0; i < msg->data_count; ++i) {
            // Read the data from the message_data structure
            // Convert to string
            argv[i] = static_cast<const char*>(msg->data_ptrs[i]->data);
            std::cout<<"m"<<"uPIM: argv["<<i<<"]: "<<argv[i]<<std::endl;
          }
          int argc = msg->data_count;
          for(int i = 0; i < argc; i++) {
            printf("uPIM: argv[%d]: %s\n", i, argv[i]);
          }
          upmem_sim::util::ArgumentParser* argument_parser = upmem_sim::init_argument_parser();
          owner->argument_parser = argument_parser;
          argument_parser->parse(argc, argv);
          owner->system = new upmem_sim::simulator::System(argument_parser);

         //owner->system->init();
          printf("uPIM: DPU_INIT done\n");

          break;
        }
      case upmem_sim::DPU_LOAD:
        {
          // Handle DPU_LOAD message
          // Load binary or whatever is needed
          printf("uPIM: DPU_LOAD received\n");
          if (owner->system != nullptr) {
            // Assuming system has a method to load binary
            // This is just a placeholder for actual loading logic
            std::string binary(static_cast<const char*>(msg->data_ptrs[0]->data));
            owner->system->set_benchmark(binary); // Set the benchmark name
            owner->system->init();
            printf("uPIM: Loading binary done: %s\n", binary.c_str());

          } else {
            printf("uPIM: System not initialized, cannot load binary\n");
          }
          break;
        }
      case upmem_sim::DPU_LAUNCH:
        {
          // Handle DPU_LAUNCH message
          // Launch the DPU execution
          printf("uPIM: DPU_LAUNCH received\n");
          if (owner->system != nullptr) {
            if(owner->system->is_benchmark_set != false){
              // Assuming system has a method to launch execution
              // This is just a placeholder for actual launch logic

              std::memcpy(&(owner->system->launch_policy), msg->data_ptrs[0]->data, sizeof(owner->system->launch_policy));
              printf("uPIM: Launching DPU with policy: %d\n", owner->system->launch_policy);
              owner->start_working();

            }
            else{
              printf("uPIM: Benchmark not set, cannot launch DPU\n");
            }
          } else {
            printf("uPIM: System not initialized, cannot launch DPU\n");
          }
          //only after dpu workdone, then is_finished will be set to true
          return false;
        }
      case upmem_sim::DPU_UPDATE:
        {
          // Handle DPU_UPDATE message
          // Update the DPU state or whatever is needed
          printf("uPIM: DPU_UPDATE received\n");
          if (owner->system != nullptr) {
            //Not elegant at all...
            owner->system=reinterpret_cast<upmem_sim::simulator::System *>(const_cast<void *>(msg->data_ptrs[0]->data));
            printf("uPIM: DPU state updated\n");

          } else {
            printf("uPIM: System not initialized, cannot update DPU\n");
          }
          break;
        }

      default:
        {
          // Handle unknown message type
          printf("uPIM: Unknown message type received: %d\n", msg->type);
          exit(2);
        }
    }
    //response
    PacketPtr resp_pkt = make_pkt();
    upmem_sim::Dpu_message* resp_msg = new upmem_sim::Dpu_message(upmem_sim::DPU_FINISHED,0,nullptr);
    resp_pkt->dataDynamic<upmem_sim::Dpu_message>(resp_msg);
    resp_pkt->makeTimingResponse();
    sendTimingResp(resp_pkt);
    printf("Message addressed done\n");
    return false;
  }

  void
  uPIM::CPUSidePort::recvRespRetry()
  {
    // We should have a blocked packet if this function is called.
    assert(blockedPacket != nullptr);

    // Grab the blocked packet.
    PacketPtr pkt = blockedPacket;
    blockedPacket = nullptr;

    // Try to resend it. It's possible that it fails again.
    sendPacket(pkt);
  }

} // namespace gem5
