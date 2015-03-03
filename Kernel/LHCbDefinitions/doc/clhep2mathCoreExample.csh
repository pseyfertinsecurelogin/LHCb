#==============================================================================
# clhep2MathCoreExample.csh
#
# Author: Juan PALACIOS  2005-11-24
# 
# This script replaces some includes and class names from CLHEP to their 
# LHCbDefinitions equivalents. It is by no means exhaustive but can serve as
# an example. It has been used succesfully on DetDesc.
#
#==============================================================================
#
# Change CLHEP includes for Kernel/LHCbDefinitions ones
#
/afs/cern.ch/lhcb/scripts/replace "CLHEP/Geometry/Vector3D.h" "Kernel/Vector3DTypes.h" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace "CLHEP/Vector/ThreeVector.h" "Kernel/Vector3DTypes.h" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace "CLHEP/Geometry/Point3D.h" "Kernel/Point3DTypes.h" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace "CLHEP/Geometry/Plane3D.h" "Kernel/Plane3DTypes.h" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace "CLHEP/Vector/LorentzVector.h" "Kernel/Vector4DTypes.h" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace "CLHEP/Vector/LorentzRotation.h" "Kernel/Transform4DTypes.h" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace "CLHEP/Geometry/Transform3D.h" "Kernel/Transform3DTypes.h" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace "CLHEP/Vector/Rotation.h" "Kernel/Transform3DTypes.h" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace "CLHEP/Units/PhysicalConstants.h" "Kernel/PhysicalConstants.h" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace "CLHEP/Units/SystemOfUnits.h" "Kernel/SystemOfUnits.h" *.h *.icpp *.cpp

#
#==============================================================================
# Change HepPoint3D for Gaudi::Point3D etc.
#
/afs/cern.ch/lhcb/scripts/replace HepPoint3D "Gaudi::XYZPoint" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace HepVector3D "Gaudi::XYZVector" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace Hep3Vector "Gaudi::XYZVector" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace HepPlane3D "Gaudi::Plane3D" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace HepTransform3D "Gaudi::Transform3D" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace HepTranslate3D "Gaudi::TranslationXYZ" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace HepRotate3D "Gaudi::Rotation3D" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace HepRotateX3D "Gaudi::RotationX" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace HepRotateY3D "Gaudi::RotationY" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace HepRotateZ3D "Gaudi::RotationZ" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace HepRotation "Gaudi::Rotation3D" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace HepLorentzRotation "Gaudi::LorentzRotation" *.h *.icpp *.cpp

/afs/cern.ch/lhcb/scripts/replace HepLorentzVector "Gaudi::LorentzVector" *.h *.icpp *.cpp
#==============================================================================
