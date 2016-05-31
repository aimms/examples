#ifdef _WIN64
#import "C:\\Users\\Marcel\\AppData\\Local\\AIMMS\\IFA\\AIMMSLauncher\\1.0.0.50\\x64\\AimmsSelector.dll" named_guids
#else
#import "C:\\Users\\Marcel\\AppData\\Local\\AIMMS\\IFA\\AIMMSLauncher\\1.0.0.50\\x86\\AimmsSelector.dll" named_guids
#endif

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <stdlib.h>
#include <comutil.h>

#define NR_DEPOTS    3
#define NR_CUSTOMERS 10

double Supply[NR_DEPOTS]    =
{ 10.0, 15.0, 20.0 };

double Demand[NR_CUSTOMERS] =
{ 3.0, 6.0, 3.0, 3.5, 5.5, 5.0, 6.0, 1.5, 7.0, 4.5 };

double UnitTransportCost[NR_DEPOTS][NR_CUSTOMERS] =
{ { 1.0, 0.7, 0.3, 0.8, 1.2, 0.3, 1.1, 0.9, 1.0, 0.5 },
  { 0.7, 0.9, 1.3, 0.9, 0.9, 1.3, 1.2, 0.5, 0.7, 1.5 },
  { 1.2, 0.5, 0.9, 1.8, 1.1, 0.7, 0.5, 1.4, 0.5, 1.4 } };

int    TransportModelStatus;
double Transport[NR_DEPOTS][NR_CUSTOMERS];
double TotalCost;

/*
CreateSupplyArray fills a "_variant_t" structure with a SafeArrays that
contains the supply values. This structure can be passed to AIMMS.
*/
void CreateSupplyArray(_variant_t *resvt, double Supply[NR_DEPOTS])
{
    long j;
    SAFEARRAY *sa   = SafeArrayCreateVector(VT_R8,0,NR_DEPOTS);
    for (j = 0; j < NR_DEPOTS; j++)
    {
        SafeArrayPutElement(sa,&j,&Supply[j]);
    }
    (*resvt).vt = VT_ARRAY | VT_R8 ;
    (*resvt).parray = sa;
}

/*
CreateDemandArray fills a "_variant_t" structure with a SafeArrays that
contains the demand values. This structure can be passed to AIMMS.
*/
void CreateDemandArray(_variant_t *resvt, double Demand[NR_CUSTOMERS])
{
    long j;
    SAFEARRAY *sa   = SafeArrayCreateVector(VT_R8,0,NR_CUSTOMERS);
    for (j = 0; j < NR_CUSTOMERS; j++)
    {
        SafeArrayPutElement(sa,&j,&Demand[j]);
    }
    (*resvt).vt = VT_ARRAY | VT_R8 ;
    (*resvt).parray = sa;
}


/*
CreateCostArray fills a "_variant_t" structure with a SafeArrays that
contains the unit cost values. This structure can be passed to AIMMS.
*/
void CreateCostArray(_variant_t *resvt, double UnitCost[NR_DEPOTS][NR_CUSTOMERS])
{
    SAFEARRAYBOUND rgsabound[2];
    long i,j;
    long index[2];

    rgsabound[0].lLbound = 0;
    rgsabound[0].cElements = NR_DEPOTS;
    rgsabound[1].lLbound = 0;
    rgsabound[1].cElements = NR_CUSTOMERS;

    SAFEARRAY *sa   = SafeArrayCreate(VT_R8,2,rgsabound);

    for (i = 0; i < NR_DEPOTS; i++)
    {
        index[0] = i;
        for ( j = 0; j < NR_CUSTOMERS; j++)
            {
                index[1] = j;
                SafeArrayPutElement(sa,index,&UnitCost[i][j]);
            }
    }
    (*resvt).vt = VT_ARRAY | VT_R8 ;
    (*resvt).parray = sa;
}

/*
CreateCostArray fills a double with the transport value from a SafeArray via a VARIANT
This SafeArray structure can be passed from AIMMS.
*/
void GetCostArray(SAFEARRAY *sa, double Transport[NR_DEPOTS][NR_CUSTOMERS])
{
    long i,j;
    long index[2];
    VARIANT Value;

    for (i = 0; i < NR_DEPOTS; i++)
    {
        index[0] = i;
        for ( j = 0; j < NR_CUSTOMERS; j++)
        {
            index[1] = j;
            SafeArrayGetElement(sa,index,&Value);
            Transport[i][j] = Value.dblVal;
		}
    }
}


/*
The results will be printed to a file "RunAIMMS.out"
*/
void print_results(void)
{
    FILE *f;
    int  i,j;

    if ( ! (f = fopen( "RunAIMMS.out", "w" )) ) {
        fprintf( stderr, "Cannot open output file" );
        return;
    }

    fprintf( f, "Model Status: %d\n", TransportModelStatus );
    fprintf( f, "Total Cost  : %12.5f\n\n", TotalCost );
    fprintf( f, "Transports  :\n" );

    fprintf( f, "            " );
    for ( j = 0; j < NR_CUSTOMERS; j++ ) {
        fprintf( f, "        c-%02d", j+1 );
    }
    fprintf( f, "\n" );

    for ( i = 0; i < NR_DEPOTS; i++ ) {
        fprintf( f, "        d-%02d", i+1 );
        for ( j = 0; j < NR_CUSTOMERS; j++ ) {
            fprintf( f, "%12.5f", Transport[i][j] );
        }
        fprintf( f, "\n" );
    }
    fclose(f);
}

int main(int argc, char **argv)
	{
    using namespace AimmsSelector;
    long i;
    int control = 0;
    char buffer[_MAX_PATH];
    char buf[50], message[1024];


   /*
    * For reasons of clarity of code, we assume that all calls succeed.
    * In any practical application, it would be wise to verify the return
    * value of all calls, because further execution will probably lead
    * to havoc anyway.
    */

    _getcwd( buffer, _MAX_PATH );
    strcat(buffer,"\\RunAIMMS.aimms");
    CoInitialize(0);

    // Create handle to Project
    ISelectorPtr hSelector(CLSID_CSelector, NULL, CLSCTX_SERVER);
    IProjectPtr hProject = hSelector->GetAimmsProject(buffer);

    try
    {
        hProject->StartupMode = STARTUP_MINIMIZED;
        hProject->ProjectOpen(buffer, 0);
        hProject->GetControl(-1);

        SAFEARRAY *sa;
		_variant_t vsa;

	    ISetPtr hDepots		= hProject->GetSet("Depots");
		ISetPtr hCustomers  = hProject->GetSet("Customers");

	    sa  = SafeArrayCreateVector(VT_VARIANT,1,NR_DEPOTS);

		/*
		This fills the set "Depots" with the appropriate elements.
		The elements will be communicated at once as an array
		*/
		hDepots->ElementPassMode = ELEMENT_BY_NAME;
        for ( i = 1;i <= NR_DEPOTS; i++)
        {
            sprintf(buf,"d-%d",i);
            _variant_t v(buf);
            SafeArrayPutElement(sa,&i,&v);
		};
		vsa.vt = VT_ARRAY | VT_VARIANT ;
		vsa.parray = sa;
		hDepots->AssignElementArray(&vsa, REPLACE);
        hDepots = 0;

		/*
		This fills the set "Customers" with the appropriate elements.
		The elements will be communicated individually
		*/
		for ( i = 1;i <= NR_CUSTOMERS; i++)
		{
		    sprintf(buf,"c-%d",i);
            hCustomers->AddElement(buf,TRUE);
		};
        hCustomers = 0;

		// This fills the parameter Supply.
		CreateSupplyArray(&vsa,Supply);
		IIdentifierPtr hSupply = hProject->GetIdentifier("Supply");
		hSupply->AssignArray(&vsa,(long)0);
        hSupply = 0;

		// This fills the parameter Demand.
		CreateDemandArray(&vsa,Demand);
		IIdentifierPtr hDemand = hProject->GetIdentifier("Demand");
		hDemand->AssignArray(&vsa,(long)0);
        hDemand = 0;

		// This fills the parameter UnitTransportCost.
		CreateCostArray(&vsa,UnitTransportCost);
		IIdentifierPtr hUnitCost = hProject->GetIdentifier("UnitTransportCost");
		hUnitCost->AssignArray(&vsa,(long)0);
        hUnitCost = 0;

		// This executes the procedure "MainExecution"
		SAFEARRAY *no_args = SafeArrayCreateVector(VT_VARIANT,0,0);
		IProcedurePtr Main = hProject->GetProcedure("MainExecution");
		Main->Run(&no_args);
		SafeArrayDestroy(no_args);
        Main = 0;

		// This retrieves the values of variable Transport
		sa = hProject ->CreateArray("Transport",0,0);
		GetCostArray(sa, Transport);

		// This retrieves the model status.
		IScalarPtr hStatus = hProject->GetScalar("TransportModelStatus");
		TransportModelStatus = (long)hStatus->Value;
        hStatus = 0;

		IScalarPtr hCost = hProject->GetScalar("TotalCost");
		TotalCost = hCost->Value;
        hCost = 0;

		SafeArrayDestroy(sa);

        hProject->ReleaseControl();
        hProject->ProjectClose();
        hProject = 0;
	}catch(...)
	{
		// Some basic error handling
		IErrorInfo *perrinfo;
		BSTR err;

		GetErrorInfo(0,&perrinfo);
		if (perrinfo)
		{
			perrinfo->GetDescription(&err);

			_bstr_t errmsg(err);
			sprintf(message, "Aimms com failed: %s", (char *)errmsg);
			perrinfo->Release();
		}else
		{
			sprintf(message, "Aimms com failed.");
		}

	}

    hSelector = 0;
	CoUninitialize();

	print_results();

	return 0;
}

