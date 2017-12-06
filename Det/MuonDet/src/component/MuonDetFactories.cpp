// Factories for converters for classes in MuonDetLib
#include "DetDescCnv/XmlUserConditionCnv.h"
#include "DetDescCnv/XmlUserDetElemCnv.h"

#include "MuonDet/MuonChamberGrid.h"
typedef XmlUserConditionCnv<MuonChamberGrid> XmlMuonGridCnv;

#include "MuonDet/DeMuonChamber.h"
typedef XmlUserDetElemCnv<DeMuonChamber> XmlDeMuonChamber;


#include "MuonDet/DeMuonRegion.h"
typedef XmlUserDetElemCnv<DeMuonRegion> XmlDeMuonRegion;

#include "MuonDet/DeMuonDetector.h"
typedef XmlUserDetElemCnv<DeMuonDetector> XmlDeMuonDetector;

DECLARE_CONVERTER( XmlMuonGridCnv )
DECLARE_CONVERTER( XmlDeMuonChamber )
DECLARE_CONVERTER( XmlDeMuonRegion )
DECLARE_CONVERTER( XmlDeMuonDetector )
