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


namespace gem5
{
Dpu::Dpu(const DpuParams &p) :
    SimObject(p)
{
    printf("enter DPU\n");
}

uPIM::uPIM(const uPIMParams &p) :
    Dpu(p) ,dpu_cycle_(p.dpu_cycle),cycle_event([this]{processCycle();},name())
{
    printf("enter uPIM\n");\
    char *argv[] = {
        (char*)"./src/uPIMulator",                                // argv[0]: 程序名
        (char*)"--benchmark", (char*)"RED",                               // argv[1], argv[2]
        (char*)"--num_tasklets", (char*)"16",                            // argv[3], argv[4]
        (char*)"--bindir", (char*)"/home/weichu/my_gem5/gem5/src/pim/bin/1_dpus", // argv[5], argv[6]
        (char*)"--logdir", (char*)"."                                    // argv[7], argv[8]
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    //start(argc,argv);
    
}

void uPIM::startup()
{
    schedule(cycle_event, dpu_cycle_);
}

void uPIM::processCycle()
{
    printf("%s: enter processCycle\n",this->name().c_str());    
    schedule(cycle_event, curTick() + dpu_cycle_);
}


} // namespace gem5
