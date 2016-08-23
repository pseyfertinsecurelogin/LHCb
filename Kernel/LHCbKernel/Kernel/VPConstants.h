#ifndef KERNEL_VPCONSTANTS_H
#define KERNEL_VPCONSTANTS_H 1


namespace VP {

static const unsigned int NModules = 52;
static const unsigned int NSensorsPerModule = 4;
static const unsigned int NSensors = NModules * NSensorsPerModule;
static const unsigned int NChipsPerSensor = 3;
static const unsigned int NRows = 512;
static const unsigned int NColumns = 512;
static const unsigned int NRowBits = 9;
static const unsigned int NColumnBits = 9;
static const unsigned int NChipBits = 2;
//static const unsigned int NSensorBits = 10; // not used
static const unsigned int NSensorColumns = NColumns * NChipsPerSensor; 
static const unsigned int NPixelsPerSensor = NSensorColumns * NRows;

static const double Pitch = 0.0275;

}

#endif
