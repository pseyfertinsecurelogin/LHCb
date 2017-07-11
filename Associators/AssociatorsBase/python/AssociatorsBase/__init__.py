import cppyy
cppyy.gbl.gSystem.Load("libGaudiKernel.so")
cppyy.gbl.gSystem.Load("libLinkerEvent.so")
cppyy.gbl.gInterpreter.Declare("#include <Associators/InputLinks.h>")
InputLinks = cppyy.gbl.InputLinks
