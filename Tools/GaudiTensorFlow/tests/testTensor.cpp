/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "GaudiTensorFlow/Predictor.h"
#include "GaudiTensorFlow/Tensor.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>

using namespace std;
namespace gtf = GaudiTensorFlow;

void splash( void );
void sectionTitle( std::string s );

int main() {
  splash();

  sectionTitle( "Constructor of the tensor" );
  std::cout << "Creating a floating point 1D tensor... \n";
  gtf::Tensor<float, 1> tensorF1( {3} );
  std::cout << "Creating a floating point 2D tensor... \n";
  gtf::Tensor<float, 2> tensorF2( {3, 2} );
  std::cout << "Creating a floating point 3D tensor... \n";
  gtf::Tensor<float, 3> tensorF3( {1, 3, 2} );
  std::cout << "Creating a double precision 2D tensor... \n";
  gtf::Tensor<double, 2> tensorD2( {1, 3} );
  std::cout << "Creating an integer 2D tensor... \n";
  gtf::Tensor<int, 2> tensorI2( {3, 2} );

  std::cout << "Creating and initialize with Tensor::make\n";
  auto tf1 = gtf::make_tensor<float>( {1, 2, 3} );
  std::cout << "Created tensor:" << tf1 << std::endl;
  auto tf2 = gtf::make_tensor<float>( {{1, 2, 3}, {2, 3, 4}} );
  std::cout << "Created tensor:\n" << tf2 << std::endl;
  auto tf2z = gtf::zip_tensor<float>( {{1, 2, 3}, {1, 2, 3}} );
  std::cout << "Created tensor:\n" << tf2z << std::endl;

  sectionTitle( "Setting the tensor elements and reading them back" );
  std::cout << "Print the content of the tensor... \n";
  std::cout << "tensorF1: " << tensorF1 << std::endl;
  std::cout << "Setting the tensor to [1 2 3]\n";
  tensorF1 = gtf::make_tensor<float>( {1, 2, 3} );
  std::cout << "tensorF1: " << tensorF1 << std::endl;
  std::cout << "Setting the first element to -1\n";
  tensorF1[0] = -1;
  std::cout << "tensorF1: " << tensorF1 << std::endl;
  std::cout << "Checking the first element is properly set to -1\n";
  // Explicit cast of indices is needed in this case,
  // otherwise 0 is read as integer and operator[] returns a tensor instead
  // of a number.
  if ( tensorF1[0] != -1 ) throw std::runtime_error( "Something went wrong either writing or reading" );

  std::cout << "Set up a 2D tensor line by line\n";
  tensorF2[0] = gtf::make_tensor<float>( {1, 10} );
  tensorF2[1] = gtf::make_tensor<float>( {2, 20} );
  tensorF2[2] = gtf::make_tensor<float>( {3, 30} );
  std::cout << "tensorF2:\n" << tensorF2 << std::endl;

  std::cout << "Setting up the first row to (8, 9) \n";
  tensorF2[0] = gtf::make_tensor<float>( {8, 9} );
  std::cout << "tensorF2: \n" << tensorF2 << std::endl;

  std::cout << "Replace element (1,1) with 123 \n";
  tensorF2( {1, 1} ) = 123;
  std::cout << "tensorF2:\n" << tensorF2 << std::endl;

  ////////////////////////////////////////////////////////////////////////////////
  sectionTitle( "Loading Predictor" );
  const size_t nEntries = 10;

  std::cout << "Creating the predictor unique pointer... \n";
  std::unique_ptr<gtf::Predictor> predictor;
  try {
    std::cout << "Loading saved model from file handling possible exceptions\n";
    predictor.reset(
        new gtf::Predictor( "/afs/cern.ch/user/l/landerli/DiamondGAN/src/DiamondGan-2018-11-20_15_06_39.793797-TEST/",
                            {"X_gen"}, {"Y_gen_1"}, 1 ) );
  } catch ( const std::runtime_error& ) { abort(); }

  std::cout << "Creating a second predictor to check consistency\n";
  std::unique_ptr<gtf::Predictor> predictor2(
      new gtf::Predictor( "/afs/cern.ch/user/l/landerli/DiamondGAN/src/DiamondGan-2018-11-20_14_26_32.562304-TEST/",
                          {"X_gen"}, {"Y_gen_1"} ) );

  // Allocate the input tensor
  auto tensorIn = gtf::zip_tensor<float>(
      {std::vector<float>( nEntries, 100. ), std::vector<float>( nEntries, 2. ), std::vector<float>( nEntries, 30. )} );

  // Allocate the output tensor
  auto tensorOut = gtf::Tensor<float, 2>( {nEntries, 2} );

  std::cout << "Executing predictor 1...\n";
  predictor->exec( {&tensorIn}, {&tensorOut} );
  std::cout << tensorOut << std::endl;

  std::cout << "Executing predictor 2...\n";
  predictor2->exec( {&tensorIn}, {&tensorOut} );
  std::cout << tensorOut << std::endl;
}

void splash( void ) {
  std::cout << "///////////////////////////////////////////////////////////////////\n"
            << "////                                                          /////\n"
            << "////  Testing probe for the Gaudi-TensorFlow interface.       /////\n"
            << "////                                                          /////\n"
            << "////  TensorFlow C API - version:   " << TF_Version() << "                    /////\n"
            << "////                                                          /////\n"
            << "////  Interface maintained by: Lucio.Anderlini@cern.ch        /////\n"
            << "////                                                          /////\n"
            << "///////////////////////////////////////////////////////////////////\n";
}

void sectionTitle( std::string title ) {
  std::cout << "--------------------------------------------------------------\n"
            << " >>  " << title << std::endl
            << "--------------------------------------------------------------\n";
}
