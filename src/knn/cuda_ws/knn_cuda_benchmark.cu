/*
 * Hetero-mark
 *
 * Copyright (c) 2015 Northeastern University
 * All rights reserved.
 *
 * Developed by:
 *   Northeastern University Computer Architecture Research (NUCAR) Group
 *   Northeastern University
 *   http://www.ece.neu.edu/groups/nucar/
 *
 * Author: Yifan Sun (yifansun@coe.neu.edu)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal with the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 *   Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimers.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimers in the
 *   documentation and/or other materials provided with the distribution.
 *
 *   Neither the names of NUCAR, Northeastern University, nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this Software without specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS WITH THE SOFTWARE.
 */
#include "src/knn/cuda_ws/knn_cuda_benchmark.h"
#include "src/knn/cuda_ws/knn_gpu_partitioner.h"
#include "src/knn/cuda_ws/knn_cpu_partitioner.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


__global__ void knn_cuda(LatLong *latLong,float *d_distances,int num_records,float lat,float lng,int *worklist) {
  
  int *position = (int*)&latLong[num_records];
  GpuPartitioner p = gpu_partitioner_create(num_records,worklist,position);

  for(int tid = gpu_initialize(&p); gpu_more(&p); tid = gpu_increment(&p))
  {
  d_distances[tid] = (float)sqrt((lat - latLong[tid].lat)*(lat-latLong[tid].lat)+(lng-latLong[tid].lng)*(lng-latLong[tid].lng)); 
   }
}


void KnnCudaBenchmark::Initialize() {
 KnnBenchmark::Initialize();
 cudaMallocManaged(&h_locations_,sizeof(LatLong) * num_records_);
 cudaMallocManaged(&h_distances_,sizeof(float) * num_records_);
 cudaMallocManaged(&worklist_,sizeof(std::atomic_uint));
 for(int i = 0; i < num_records_; i++)
 {
   h_locations_[i].lat  = locations_.at(i).lat;
   h_locations_[i].lng  = locations_.at(i).lng;

 }
 worklist_[0].store(0);
}

void KnnCudaBenchmark::Run() {
 dim3 block_size(256);
 dim3 grid_size(16);
 
 knn_cuda<<<grid_size, block_size>>>(h_locations_,h_distances_,num_records_,latitude_,longitude_, (int*)worklist_);
 KnnCPU(h_locations_, h_distances_,num_records_,latitude_,longitude_,worklist_);
  cudaDeviceSynchronize();
 
 for(int i = 0; i < 10; i++)
        printf("Distances are %f \n", h_distances_[i]);
  
 // find the resultsCount least distances
 findLowest(records_,h_distances_,num_records_,k_value_);

 for(int i = 0;i < k_value_;i++) {
      printf("%s --> Distance=%f\n",records_[i].recString,records_[i].distance);
    }
}

void KnnCudaBenchmark::Cleanup() {
  KnnBenchmark::Cleanup();
}
