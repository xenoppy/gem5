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
#include "uPIM.hh"

namespace gem5
{
  Dpu::Dpu(const DpuParams &p) : SimObject(p)
  {
    printf("enter DPU\n");
  }

  uPIM::uPIM(const uPIMParams &p) : Dpu(p), cpu_clock(p.cpu_clock), rank_clock(p.rank_clock),
                                    cycle_event([this]
                                                { processCycle(); }, name()),
                                    rank_cycle_event([this]
                                                     { process_rank_Cycle(); }, name()),
                                    cpu_cycle_event([this]
                                                    { process_cpu_Cycle(); }, name()),
                                    cpusidePort(name() + ".cpu_side", this),
                                    system(nullptr)
  {
    printf("enter uPIM\n");

    // start(argc,argv);
  }

  void uPIM::startup()
  { /*
     argument_parser = upmem_sim::init_argument_parser();
     char *argv[] = {
         (char *)"./src/uPIMulator",                                                 // argv[0]: 程序名
         (char *)"--benchmark", (char *)"RED",                                       // argv[1], argv[2]
         (char *)"--num_tasklets", (char *)"16",                                     // argv[3], argv[4]
         (char *)"--bindir", (char *)"/home/weichu/my_gem5/gem5/src/pim/bin/1_dpus", // argv[5], argv[6]
         (char *)"--logdir", (char *)"."                                             // argv[7], argv[8]
     };
     int argc = sizeof(argv) / sizeof(argv[0]);
     argument_parser->parse(argc, argv);
     system = new upmem_sim::simulator::System(argument_parser);
     system->init();*/
    // schedule(cpu_cycle_event, curTick() + cpu_clock);
    schedule(rank_cycle_event, curTick() + rank_clock);
  }
  // not used
  void uPIM::processCycle()
  {

    // printf("%s: enter processCycle\n", this->name().c_str());
    if (not system->is_finished())
    {
      // system->cycle();
      // schedule(cycle_event, curTick() + dpu_cycle_);
    }
    else
    {
      printf("uPIM: system is finished\n");

      system->fini();

      for (auto &option : argument_parser->options())
      {
        if (argument_parser->option_type(option) ==
            upmem_sim::util::ArgumentParser::INT)
        {
          std::cout << option << ": " << argument_parser->get_int_parameter(option)
                    << std::endl;
        }
        else if (argument_parser->option_type(option) ==
                 upmem_sim::util::ArgumentParser::STRING)
        {
          std::cout << option << ": "
                    << argument_parser->get_string_parameter(option) << std::endl;
        }
        else
        {
          throw std::invalid_argument("");
        }
      }

      upmem_sim::util::StatFactory *system_stat_factory = system->stat_factory();
      for (auto &stat : system_stat_factory->stats())
      {
        std::cout << stat << ": " << system_stat_factory->value(stat) << std::endl;
      }
      delete system_stat_factory;

      delete argument_parser;
      delete system;
    }
  }
  void uPIM::process_rank_Cycle()
  {
    if (system != nullptr)
    {
      system->dpu_check_cycle(); // just to check if the system is finished
      if (system->is_zombie())
      {

        Request::Flags testflag(0);
        RequestPtr req = std::make_shared<Request>(
            0, 0, testflag, 0, 0, 0);
        PacketPtr pkt = Packet::createRead(req);

        pkt->dataDynamic<upmem_sim::simulator::System>(system);

        pkt->makeTimingResponse();
        printf("uPIM: sendTimingResp---is_zombie\n");
        cpusidePort.sendTimingResp(pkt);
        // send packet to cpu, then cpu check execution
        //
      }
      // cpusidePort.trySendRetry();
      if (not system->is_finished())
      {
        system->rank_cycle();
        schedule(rank_cycle_event, curTick() + rank_clock);
      }
      else
      {
        Request::Flags testflag(0);
        RequestPtr req = std::make_shared<Request>(
            0, 0, testflag, 0, 0, 0);
        PacketPtr pkt = Packet::createRead(req);

        pkt->dataDynamic<upmem_sim::simulator::System>(system);

        pkt->makeTimingResponse();
        printf("uPIM: sendTimingResp---is_finished\n");
        cpusidePort.sendTimingResp(pkt);
      }
    }
  }

  void uPIM::process_cpu_Cycle()
  {
    if (not system->is_finished())
    {
      system->cpu_cycle();
      schedule(cpu_cycle_event, curTick() + cpu_clock);
    }

    else
    {
      printf("uPIM: system is finished\n");

      system->fini();

      for (auto &option : argument_parser->options())
      {
        if (argument_parser->option_type(option) ==
            upmem_sim::util::ArgumentParser::INT)
        {
          std::cout << option << ": " << argument_parser->get_int_parameter(option)
                    << std::endl;
        }
        else if (argument_parser->option_type(option) ==
                 upmem_sim::util::ArgumentParser::STRING)
        {
          std::cout << option << ": "
                    << argument_parser->get_string_parameter(option) << std::endl;
        }
        else
        {
          throw std::invalid_argument("");
        }
      }

      upmem_sim::util::StatFactory *system_stat_factory = system->stat_factory();
      for (auto &stat : system_stat_factory->stats())
      {
        std::cout << stat << ": " << system_stat_factory->value(stat) << std::endl;
      }
      delete system_stat_factory;

      delete argument_parser;
      delete system;
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
    // Just forward to the memobj.
    /*    if (!owner->handleRequest(pkt))
        {
          needRetry = true;
          return false;
        }
        else
        {
          return true;
        }*/
    needRetry = true;
    printf("uPIM: enter recvTimingReq\n");
    owner->system = reinterpret_cast<upmem_sim::simulator::System *>(pkt->getPtr<uint8_t>());

    printf("System updated\n");
    // pkt->makeTimingResponse();
    // sendTimingResp(pkt);
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
