enum {
  ADL_TYPE_INT,
  ADL_TYPE_DOUBLE,
  ADL_TYPE_STRING
};

int   ADLNrOfErrors( void );
char *ADLErrorMessage( int n );

int ADLAttachToProject( void );
int ADLProjectOpen( char *Commandline );
int ADLProjectClose( int interactive );

int ADLGetControl( int timeout );
int ADLReleaseControl( void );

int ADLRun( char *procedure, int *retval );

int ADLGetSetCard( char *setname, int *n );

int ADLGetElements( char *setname, char **elements, int recs, int size, int *card );
int ADLPutElements( char *setname, char **elements, int card );
int ADLGenerateElements( char *setname, char *prefix, int card );

int ADLGetArraySize( char *id, int *dim, int *cards );

int ADLGetArray( char *id, void *val, int recs, int size, int type, int dim, int *cards );
int ADLPutArray( char *id, void *val, int type, int dim, int *cards );

int ADLMultiToOne( int dim, int *ords, int *cards );

#define ADLGetScalar(id,val,size,type)             ADLGetArray(id,val,1,size,type,0,NULL)
#define ADLPutScalar(id,val,type)                  ADLPutArray(id,&val,type,0,NULL)

#define ADLGetInt(id,val)                          ADLGetArray(id,val,1,sizeof(int),ADL_TYPE_INT,0,NULL)
#define ADLPutInt(id,val)                          ADLPutArray(id,&val,ADL_TYPE_INT,0,NULL)

#define ADLGetDouble(id,val)                       ADLGetArray(id,val,1,sizeof(double),ADL_TYPE_DOUBLE,0,NULL)
#define ADLPutDouble(id,val)                       ADLPutArray(id,&val,ADL_TYPE_DOUBLE,0,NULL)

#define ADLGetDoubleArray(id,val,recs,dim,cards)   ADLGetArray(id,val,recs,sizeof(double),ADL_TYPE_DOUBLE,dim,cards)
#define ADLPutDoubleArray(id,val,dim,cards)        ADLPutArray(id,val,ADL_TYPE_DOUBLE,dim,cards)

#define ADLGetIntArray(id,val,recs,dim,cards)      ADLGetArray(id,val,recs,sizeof(int),ADL_TYPE_INT,dim,cards)
#define ADLPutIntArray(id,val,dim,cards)           ADLPutArray(id,val,ADL_TYPE_INT,dim,cards)



