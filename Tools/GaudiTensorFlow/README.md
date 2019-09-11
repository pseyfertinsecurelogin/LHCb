# GaudiTensorFlow
GaudiTensorFlow brings to the C++ LHCb software the power of
TensorFlow through a dedicated Predictor interface.

## Introduction
TensorFlow is a framework widely adopted to employ large-scale 
High-Performance Computing (HPC) infrastructures to train and evaluate 
Neural Networks. In TensorFlow, a Neural Network is described as 
a generic algorithm transforming multi-dimensional tensors into other 
tensors through operations (ops) connected to form a computational graph. 
The tensors representing the input and output variables are distributed 
through the computing system, possibly composed of multiple servers hosting 
CPU and GPU cores, and processed through kernels representing the low-level
implementation of the operation (op) for a given architecture. 
The description of the algorihm and the tuning of the constants, usually 
achieve through the minimization of the loss function, is usually performed 
using the Python APIs of TensorFlow. The obtained graph can then be 
stored as a "SavedModel", distributed through the LHCb Grid, and loaded 
from the GaudiAlgorithms used in the simulation and reconstruction of the 
LHCb experiment for low-latency evaluation. 

This library provides an interface between the C API of TensorFlow and 
the Gaudi infrastructure, relying on C++17 and STL and limiting as much as 
possible the dependencies on Gaudi and specific LHCb software.  
The interface is designed to provide a simple interface to TensorFlow, at
the cost of some overhead due to memory copies, together with a lower-level 
access to the tensor memory for the most computing-intesive applications. 

The interface is composed of 
 - `GaudiTensorFlow::Tensor`: providing an interface to the memory allocation
   and access;
 - `GaudiTenorFlow::Predictor`: providing an interface to load the 
  computation graph from cvmfs and setting up the multiprocessing options. 

In order to keep a frequent pace in the upgrade to the most recent versions 
of TensorFlow, the C api are chosen. A CMake script downloads and links to 
GaudiTensorFlow a TensorFlow version defined by the CMake variable 
`TENSORFLOW_TAG`. 


The C++ classes defined in GaudiTensorFlow are enclosed in a C++ namespace 
named GaudiTensorFlow. It is recomended to rename it
("namespace gtf=GaudiTensorFlow;") rather than 
importing its content with "using namespace". 


## Tensor
Tensor is the memory interface to TensorFlow. 
`GaudiTensorFlow::Tensor<type, rank>` is a templated class that 
can be used instead of raw `TF_Tensor` pointer in client applications. 
`type` is a C type (int, float, double...) defining the representation of 
the tensor; `rank` is an integer defining the tensor rank. 

### Memory allocation and initialization
To allocate memory for a new object: 
```
namespace gtf=GaudiTensorFlow ;
const size_t nRows = 50, nCols = 3; 
auto newTensor = gtf::Tensor<float,2>({ nRows, nCols }); 
```
This allocates unitialized memory and provides an interface to 
read and write a 50x3 matrix of float32 entries. 

A tensor can be allocated and initialized copying the memory
from STL vectors:
```
const size_t nEntries = 50;
std::vector < std::vector < float > > dataset; 
while (some_conditions())
  dataset.emplace_back ( { 1., 2., 3.} ); 

auto newTensor = gtf::make_tensor<float> ( dataset ); 
```
This is useful when the tensor dimensions are not 
known a priori. In this case you can prepare the dataset 
using STL containers, and then import the memory 
into a contiguous and properly aligned array. 
Note that `make_tensor` copies the memory. 

Finally, if your code follows the SoA structure, you can zip
`std::vector`s into a `Tensor` object: 
```
struct Tracks{
  std::vector <float> p, px, py, pz, eta, nTracks; 
};

... 

void processTracks ( Tracks tracks )
{
  auto trackTensor = gtf::zip_tensor<float> ( {tracks.p, tracks.eta} ); 
  ... 
}
```
As in the previous example, `Tensor::zip` copies the memory and therefore
may result in a small overhead in the computation.  


### Accessing tensor elements 
Once the Tensor is allocated, memory can be set by row or by single element,
not by column.
The tensor elements can be accessed through slicing (obtaining a new 
Tensor object) or by addressing the element with a vector of indices. 

```
auto t = gtf::make_tensor<float> ( {{100, 101, 102}, {110, 111, 110}} ); 
t[0] 	                // return TensorF{100, 101, 102} 
t({0, 1})             // return float(101)
t[0][1]               // return float(101)
```

## Predictor
The Predictor class implements an interface to load and execute computational
graphs using `gtf::Tensor`s. 

In order to decouple the loading and unzipping of the model (once per Gaudi
instance or at most once per thread) from the execution time (once per event),
the `Predictor` interface provides a constructor to load the model and an 
`exec` function to evaluate it. 

Construction: 
```
std::unique_ptr<gtf::Predictor> predictor ( 
    new gtf::Predictor(
        <SavedModel_dir>, 
        { <InputTensorName_1>  [, <InputTensorName_2>  [, ...]] } , 
        { <OutputTensorName_1> [, <OutputTensorName_2> [, ...]] } 
      )
    ); 
```

Execution: 
```
// Creates the input tensor 
auto input1 = gtf::make_tensor<float> ( { { 1, 2, 3 } , { 4, 5, 6 } } ); 
// Allocates the output tensor with the right shape 
auto output1 = gtf::make_tensor<float> ( {2, 1} ); 

predictor->exec ( {&intput1}, {&output1} ); 
```


## Known issues
TensorFlow is designed to take control over a High-Performance Computing 
system including GPU and CPU servers. This sets some limitations in the 
integration with Gaudi, which is designed to run in parsllel instances 
sharing resources with other processes on the computing resources of the 
LCG. 

#### Nodes with GPUs 
If the node is equipped with a GPU it is possible that TensorFlow, and therefore
the Gaudi instance relying on it, takes control of the GPU allocating the whole 
memory. This has not been tested yet, but it is expected that since the 
second instance of Gaudi running on a machine with a GPU either crashes or 
does not exploit the GPU at all. 

#### Multithreading 
The constructor of the `gtf::Predictor` allows to define a number of threads. 
Due to an internal state of the TensorFlow C API, all the constructor calls 
following the first one are ignored. 
By default, the number of threads is set to one in order to make the execution 
of GaudiTensorFlow consistent with the Dirac paradigm. If working with more 
threads (for example with GaudiMP) it is important to set the desired number 
of threads in all the constructors. 

## Conclusions
GaudiTensorFlow defines a high-level interface to load and evaluate 
TensorFlow models in the Gaudi environment. 
The number of clients and therefore of tests is still very limited, 
and should therefore considered as an "alpha" version. 


