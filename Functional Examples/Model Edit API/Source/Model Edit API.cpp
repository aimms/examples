//This example illustrates the use of the Model Edit functions in 
//the AIMMS API by creating an AIMMS transport model.

#define _CRT_SECURE_NO_WARNINGS
#include "aimmsapi.h"
#include <stdio.h>

#ifdef UNICODE
    #define FPRINTF _ftprintf
    #define PRINTF _tprintf
    #define STRCPY _tcscpy
    #define FOPEN _tfopen
#else
    #define FPRINTF fprintf
    #define PRINTF printf
    #define STRCPY strcpy
    #define FOPEN fopen
#endif

//CHECK(m) is used to easily check the return value of all statements 
//and to print all actions to a log file.
#define                                                             \
    CHECK(m){                                                       \
        if ( res != AIMMSAPI_SUCCESS ) {                            \
            FPRINTF( Log,_T( "\nError: \n") );                      \
            FPRINTF( Log,_T("%s\n"), AimmsErrorMessage( 1 ) );      \
            FPRINTF( Log,_T("when trying: %s\n"), _T(m) );          \
            res = AimmsErrorClear( );                               \
        } else {                                                    \
            FPRINTF( Log,_T("\n%s\n"), _T(m) );                     \
        }                                                           \
    } 

int main() {
	
//Create a Log file.
FILE *Log = FOPEN( _T( "Log.txt" ), _T( "w" ) );
if (Log == NULL) {

	PRINTF(_T("Error when opening Log file. Press return to exit."));
	while (!getchar()) ; 
	exit(-1);
}
FPRINTF( Log, _T( "\"Model Edit API\" Log - Transport Model\n" ) );
	
//res is used to store the return value of the functions.
int res = 0;

//Open the project; note that the additional double quotes are needed because of spaces in the project and path.
int PROJECT_HANDLE = 0;
res = AimmsProjectOpen( _T( "\"Empty AIMMS Project\\Empty AIMMS Project.aimms\"" ), &PROJECT_HANDLE);
CHECK("opening project");

//Start building the model.

//SECTION Transport_Model
int main_model_MEH = 0;
res = AimmsMeOpenRoot(0, &main_model_MEH);
CHECK("getting handle 'MEH' to main model");

//runtime Transport_Model
int runtime_library_MEH = 0;
res = AimmsMeCreateRuntimeLibrary(_T("Transport_Model_Library"), _T("tml"), &runtime_library_MEH);
CHECK("creating runtime library Transport_Model_Library");

int TM_MEH = 0;
res = AimmsMeCreateNode(_T("Transport_Model_Section"), AIMMSAPI_ME_IDTYPE_SECTION, runtime_library_MEH, 0, &TM_MEH);
CHECK("creating Transport_Model_Section");

//DECLARATION SECTION Model_Declaration
int mod_decl_MEH = 0;
res = AimmsMeCreateNode(_T("Model_Declaration"), AIMMSAPI_ME_IDTYPE_DECLARATION_SECTION, TM_MEH, 0, &mod_decl_MEH);
CHECK("creating Model_Declaration section");

//SET:
//       identifier   :  Plants
//       index        :  p ;
int plants_MEH = 0;
res = AimmsMeCreateNode(_T("Plants"), AIMMSAPI_ME_IDTYPE_SET, mod_decl_MEH, 0, &plants_MEH);
CHECK("creating set Plants");
res = AimmsMeSetAttribute(plants_MEH, AIMMSAPI_ME_ATTR_INDEX, _T("p"));
CHECK("set index");

//SET:
//       identifier   :  Depots
//       index        :  d ;
int depots_MEH = 0;
res = AimmsMeCreateNode(_T("Depots"), AIMMSAPI_ME_IDTYPE_SET, mod_decl_MEH, 0, &depots_MEH);
CHECK("creating set Depots");
res = AimmsMeSetAttribute(depots_MEH, AIMMSAPI_ME_ATTR_INDEX, _T("d"));
CHECK("set index");

//PARAMETER:
//       identifier   :  Supply
//       index domain :  p
int supply_MEH = 0;
res = AimmsMeCreateNode(_T("Supply"), AIMMSAPI_ME_IDTYPE_PARAMETER_NUMERIC, mod_decl_MEH, 0, &supply_MEH);
CHECK("creating parameter Supply");
res = AimmsMeSetAttribute(supply_MEH, AIMMSAPI_ME_ATTR_INDEX_DOMAIN, _T("p"));
CHECK("set index domain");

//PARAMETER:
//       identifier   :  Demand
//       index domain :  d
int demand_MEH = 0;
res = AimmsMeCreateNode(_T("Demand"), AIMMSAPI_ME_IDTYPE_PARAMETER_NUMERIC, mod_decl_MEH, 0, &demand_MEH);
CHECK("creating parameter Demand");
res = AimmsMeSetAttribute(demand_MEH, AIMMSAPI_ME_ATTR_INDEX_DOMAIN, _T("d"));
CHECK("set index domain");

//PARAMETER:
//       identifier   :  Unit_Transport_Costs
//       index domain :  (p,d)
int unit_transport_costs_MEH = 0;
res = AimmsMeCreateNode(_T("Unit_Transport_Costs"), AIMMSAPI_ME_IDTYPE_PARAMETER_NUMERIC, mod_decl_MEH, 0, &unit_transport_costs_MEH);
CHECK("creating parameter Unit_Transport_Costs");
res = AimmsMeSetAttribute(unit_transport_costs_MEH, AIMMSAPI_ME_ATTR_INDEX_DOMAIN, _T("(p,d)"));
CHECK("set index domain");

//VARIABLE:
//       identifier   :  Transport
//       index domain :  (p,d)
//       range        :  nonnegative
int transport_MEH = 0;
res = AimmsMeCreateNode(_T("Transport"), AIMMSAPI_ME_IDTYPE_VARIABLE_NUMERIC, mod_decl_MEH, 0, &transport_MEH);
CHECK("creating variable Transport");
res = AimmsMeSetAttribute(transport_MEH, AIMMSAPI_ME_ATTR_INDEX_DOMAIN, _T("(p,d)"));
CHECK("set index domain");
res = AimmsMeSetAttribute(transport_MEH, AIMMSAPI_ME_ATTR_RANGE, _T("nonnegative"));
CHECK("set binary range");

//VARIABLE:
//       identifier   :  Total_Transport_Cost
//       definition   :  sum((p,d), Transport(p,d) * Unit_Transport_Costs(p,d)) ;
int total_transport_cost_MEH = 0;
res = AimmsMeCreateNode(_T("Total_Transport_Cost"), AIMMSAPI_ME_IDTYPE_VARIABLE_NUMERIC, mod_decl_MEH, 0, &total_transport_cost_MEH);
CHECK("creating variable Total_Transport_Cost");
res = AimmsMeSetAttribute(total_transport_cost_MEH, AIMMSAPI_ME_ATTR_DEFINITION, _T("sum((p,d), Transport(p,d) * Unit_Transport_Costs(p,d))"));
CHECK("set definition");

//CONSTRAINT:
//       identifier   :  SatisfyDemand
//       index domain :  d
//       definition   :  sum(p,Transport(p,d)) >= Demand(d) ;
int Satisfy_Demand_MEH = 0;
res = AimmsMeCreateNode(_T("SatisfyDemand"), AIMMSAPI_ME_IDTYPE_CONSTRAINT, mod_decl_MEH, 0, &Satisfy_Demand_MEH);
CHECK("creating SatisfyDemand constraint");
res = AimmsMeSetAttribute(Satisfy_Demand_MEH, AIMMSAPI_ME_ATTR_INDEX_DOMAIN, _T("d"));
CHECK("set index domain");
res = AimmsMeSetAttribute(Satisfy_Demand_MEH, AIMMSAPI_ME_ATTR_DEFINITION, _T("sum(p,Transport(p,d)) >= Demand(d)"));
CHECK("set definition");

//CONSTRAINT:
//       identifier   :  SatisfySupply
//       index domain :  p
//       definition   :  sum(d,Transport(p,d)) <= Supply(p) ;
int Satisfy_Supply_MEH = 0;
res = AimmsMeCreateNode(_T("SatisfySupply"), AIMMSAPI_ME_IDTYPE_CONSTRAINT, mod_decl_MEH, 0, &Satisfy_Supply_MEH);
CHECK("creating SatisfySupply constraint");
res = AimmsMeSetAttribute(Satisfy_Supply_MEH, AIMMSAPI_ME_ATTR_INDEX_DOMAIN, _T("p"));
CHECK("set index domain");
res = AimmsMeSetAttribute(Satisfy_Supply_MEH, AIMMSAPI_ME_ATTR_DEFINITION, _T("sum(d,Transport(p,d)) <= Supply(p)"));
CHECK("set definition");

//MATHEMATICAL PROGRAM:
//       identifier   :  Transport_Model
//       objective    :  Total_Transport_Cost
//       direction    :  minimize ;
int Transport_Model_MEH = 0;
res = AimmsMeCreateNode(_T("Transport_Model"), AIMMSAPI_ME_IDTYPE_MATHEMATICAL_PROGRAM, mod_decl_MEH, 0, &Transport_Model_MEH);
CHECK("creating math program Transport_Model");
res = AimmsMeSetAttribute(Transport_Model_MEH, AIMMSAPI_ME_ATTR_OBJECTIVE, _T("Total_Transport_Cost"));
CHECK("set objective");
res = AimmsMeSetAttribute(Transport_Model_MEH, AIMMSAPI_ME_ATTR_DIRECTION, _T("Minimize"));
CHECK("set direction");

//FILE:
//       identifier   :  Data_Output_File
//       name         :  "Data Output.txt" ;
int Data_Output_MEH = 0;
res = AimmsMeCreateNode(_T("Data_Output_File"), AIMMSAPI_ME_IDTYPE_FILE, mod_decl_MEH, 0, &Data_Output_MEH);
CHECK("creating Data_Output_File");
res = AimmsMeSetAttribute(Data_Output_MEH, AIMMSAPI_ME_ATTR_NAME, _T("\"Data Output.txt\""));
CHECK("setting file name");

//PROCEDURE
//      identifier :  Solve_Transport
//      body       :  ! a procedure that initializes the demo data and solves the model.
int Solve_Transport_MEH = 0;
res = AimmsMeCreateNode(_T("Solve_Transport"), AIMMSAPI_ME_IDTYPE_PROCEDURE, TM_MEH, 0, &Solve_Transport_MEH);
CHECK("creating procedure Solve_Transport");
res = AimmsMeSetAttribute(Solve_Transport_MEH, AIMMSAPI_ME_ATTR_BODY, 
        _T("Plants    := data{Haarlem,Zwolle};\n")
        _T("Depots    := data{Groningen, Rotterdam, Maastricht, Arnhem, Amsterdam};\n")
        _T("Demand(d) := Uniform(0,100);\n")
        _T("Supply(p) := Uniform(500,1000);\n")
        _T("Unit_Transport_Costs(p,d) := Uniform(0,1);\n")
        _T("solve Transport_Model;\n")
        _T("put Data_Output_File ;\n")
        _T("display Plants, Depots, Demand, Supply, Unit_Transport_Costs, Transport, Total_Transport_Cost;\n")
        _T("putclose;\n")
           );
CHECK("set body");

//Now that the model has been created, compile it.
res = AimmsMeCompile(runtime_library_MEH);
//If there are errors, we print the first error to the Log file.
if (AimmsErrorLine(1,1)) {
    FPRINTF(Log,_T("Error on line %d: %s - %s"), AimmsErrorLine(1,1), AimmsErrorNode(1,1), AimmsErrorMessage(1));
}
CHECK("compiling model");

//Export the Transport Model section, such that we can inspect it if we want to.
res = AimmsMeExportNode(TM_MEH, _T("TransportModel.ams"), _T("") );
CHECK("exporting Transport Model section");

//Run the created procedure.
int Solve_Transport_PH = 0;
int nargs = 0;
int argtype[16];
int result = 0;
res = AimmsProcedureHandleCreate(_T("Solve_Transport"), &Solve_Transport_PH, &nargs, argtype);
CHECK("getting procedure handle");
res = AimmsProcedureRun(Solve_Transport_PH, NULL, NULL, &result);
CHECK("running procedure");	

//The project can be closed now.
res = AimmsProjectClose(PROJECT_HANDLE, 0);
CHECK("closing project");

//Close	the Log file.
FPRINTF(Log, _T("\nEnd of \"Model Edit API\" Log - Transport Model\n"));
fclose(Log);

return 1;

}