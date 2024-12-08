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

#ifndef __UPIM_HH__
#define __UPIM_HH__

#include "mem/port.hh"
#include "simulator/system.h"
#include "util/argument_parser.h"
#include "params/Dpu.hh"
#include "params/uPIM.hh"
#include "sim/sim_object.hh"

namespace gem5
{
  class Dpu : public SimObject
  {

  public:
    Dpu(const DpuParams &p);
  };

  class uPIM : public Dpu
  {
  private:
    upmem_sim::util::ArgumentParser *argument_parser;
    upmem_sim::simulator::System *system;
    void processCycle();
    EventFunctionWrapper cycle_event;
    void process_rank_Cycle();
    EventFunctionWrapper rank_cycle_event;
    void process_cpu_Cycle();
    EventFunctionWrapper cpu_cycle_event;
    const Tick rank_clock, cpu_clock;

    class CPUSidePort : public ResponsePort
    {
    private:
      /// The object that owns this object (SimpleMemobj)
      uPIM *owner;

      /// True if the port needs to send a retry req.
      bool needRetry;

      /// If we tried to send a packet and it was blocked, store it here
      PacketPtr blockedPacket;

    public:
      /**
       * Constructor. Just calls the superclass constructor.
       */
      CPUSidePort(const std::string &name, uPIM *owner) : ResponsePort(name), owner(owner), needRetry(false), blockedPacket(nullptr)
      {
      }

      /**
       * Send a packet across this port. This is called by the owner and
       * all of the flow control is hanled in this function.
       *
       * @param packet to send.
       */
      void sendPacket(PacketPtr pkt);

      /**
       * Get a list of the non-overlapping address ranges the owner is
       * responsible for. All response ports must override this function
       * and return a populated list with at least one item.
       *
       * @return a list of ranges responded to
       */
      AddrRangeList getAddrRanges() const override;

      /**
       * Send a retry to the peer port only if it is needed. This is called
       * from the SimpleMemobj whenever it is unblocked.
       */
      void trySendRetry();

    protected:
      /**
       * Receive an atomic request packet from the request port.
       * No need to implement in this simple memobj.
       */
      Tick recvAtomic(PacketPtr pkt) override
      {
        panic("recvAtomic unimpl.");
      }

      /**
       * Receive a functional request packet from the request port.
       * Performs a "debug" access updating/reading the data in place.
       *
       * @param packet the requestor sent.
       */
      void recvFunctional(PacketPtr pkt) override;

      /**
       * Receive a timing request from the request port.
       *
       * @param the packet that the requestor sent
       * @return whether this object can consume the packet. If false, we
       *         will call sendRetry() when we can try to receive this
       *         request again.
       */
      bool recvTimingReq(PacketPtr pkt) override;

      /**
       * Called by the request port if sendTimingResp was called on this
       * response port (causing recvTimingResp to be called on the request
       * port) and was unsuccesful.
       */
      void recvRespRetry() override;
    };
    CPUSidePort cpusidePort;

  public:
    uPIM(const uPIMParams &p);

    /**
     * Get a port with a given name and index. This is used at
     * binding time and returns a reference to a protocol-agnostic
     * port.
     *
     * @param if_name Port name
     * @param idx Index in the case of a VectorPort
     *
     * @return A reference to the given port
     */
    Port &getPort(const std::string &if_name,
                  PortID idx = InvalidPortID) override;
    void startup() override;
  };

} // namespace gem5

#endif // __UPIM_HH__
