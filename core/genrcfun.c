   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  06/25/16             */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Generic Functions Internal Routines              */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Changed name of variable log to logName        */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Removed IMPERATIVE_METHODS compilation flag.   */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            Changed integer type/precision.                */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*            Fixed linkage issue when DEBUGGING_FUNCTIONS   */
/*            is set to 0 and PROFILING_FUNCTIONS is set to  */
/*            1.                                             */
/*                                                           */
/*            Fixed typing issue when OBJECT_SYSTEM          */
/*            compiler flag is set to 0.                     */
/*                                                           */
/*      6.40: Added Env prefix to GetEvaluationError and     */
/*            SetEvaluationError functions.                  */
/*                                                           */
/*            Pragma once and other inclusion changes.       */
/*                                                           */
/*************************************************************/

/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if DEFGENERIC_CONSTRUCT

#if BLOAD || BLOAD_AND_BSAVE
#include "bload.h"
#endif

#if OBJECT_SYSTEM
#include "classcom.h"
#include "classfun.h"
#endif

#include "argacces.h"
#include "constrct.h"
#include "cstrcpsr.h"
#include "envrnmnt.h"
#include "genrccom.h"
#include "genrcexe.h"
#include "memalloc.h"
#include "prccode.h"
#include "router.h"
#include "sysdep.h"

#include "genrcfun.h"

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

#if DEBUGGING_FUNCTIONS
static void DisplayGenericCore(void *,DEFGENERIC *);
#endif

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

#if ! RUN_TIME

/***************************************************
  NAME         : ClearDefgenericsReady
  DESCRIPTION  : Determines if it is safe to
                 remove all defgenerics
                 Assumes *all* constructs will be
                 deleted - only checks to see if
                 any methods are currently
                 executing
  INPUTS       : None
  RETURNS      : true if no methods are
                 executing, false otherwise
  SIDE EFFECTS : None
  NOTES        : Used by (clear) and (bload)
 ***************************************************/
bool ClearDefgenericsReady(
  void *theEnv)
  {
   return((DefgenericData(theEnv)->CurrentGeneric != NULL) ? false : true);
  }

/*****************************************************
  NAME         : AllocateDefgenericModule
  DESCRIPTION  : Creates and initializes a
                 list of defgenerics for a new module
  INPUTS       : None
  RETURNS      : The new deffunction module
  SIDE EFFECTS : Deffunction module created
  NOTES        : None
 *****************************************************/
void *AllocateDefgenericModule(
  void *theEnv)
  {
   return((void *) get_struct(theEnv,defgenericModule));
  }

/***************************************************
  NAME         : FreeDefgenericModule
  DESCRIPTION  : Removes a deffunction module and
                 all associated deffunctions
  INPUTS       : The deffunction module
  RETURNS      : Nothing useful
  SIDE EFFECTS : Module and deffunctions deleted
  NOTES        : None
 ***************************************************/
void FreeDefgenericModule(
  void *theEnv,
  void *theItem)
  {
#if (! BLOAD_ONLY)
   FreeConstructHeaderModule(theEnv,(struct defmoduleItemHeader *) theItem,DefgenericData(theEnv)->DefgenericConstruct);
#endif
   rtn_struct(theEnv,defgenericModule,theItem);
  }

#endif

#if (! BLOAD_ONLY) && (! RUN_TIME)

/************************************************************
  NAME         : ClearDefmethods
  DESCRIPTION  : Deletes all defmethods - generic headers
                   are left intact
  INPUTS       : None
  RETURNS      : true if all methods deleted, false otherwise
  SIDE EFFECTS : Defmethods deleted
  NOTES        : Clearing generic functions is done in
                   two stages

                 1) Delete all methods (to clear any
                    references to other constructs)
                 2) Delete all generic headers

                 This allows other constructs which
                   mutually refer to generic functions
                   to be cleared
 ************************************************************/
bool ClearDefmethods(
  void *theEnv)
  {
   register DEFGENERIC *gfunc;
   bool success = true;

#if BLOAD || BLOAD_AND_BSAVE
   if (Bloaded(theEnv) == true) return(false);
#endif

   gfunc = (DEFGENERIC *) EnvGetNextDefgeneric(theEnv,NULL);
   while (gfunc != NULL)
     {
      if (RemoveAllExplicitMethods(theEnv,gfunc) == false)
        success = false;
      gfunc = (DEFGENERIC *) EnvGetNextDefgeneric(theEnv,(void *) gfunc);
     }
   return(success);
  }

/*****************************************************************
  NAME         : RemoveAllExplicitMethods
  DESCRIPTION  : Deletes all explicit defmethods - generic headers
                   are left intact (as well as a method for an
                   overloaded system function)
  INPUTS       : None
  RETURNS      : true if all methods deleted, false otherwise
  SIDE EFFECTS : Explicit defmethods deleted
  NOTES        : None
 *****************************************************************/
bool RemoveAllExplicitMethods(
  void *theEnv,
  DEFGENERIC *gfunc)
  {
   long i,j;
   unsigned systemMethodCount = 0;
   DEFMETHOD *narr;

   if (MethodsExecuting(gfunc) == false)
     {
      for (i = 0 ; i < gfunc->mcnt ; i++)
        {
         if (gfunc->methods[i].system)
           systemMethodCount++;
         else
           DeleteMethodInfo(theEnv,gfunc,&gfunc->methods[i]);
        }
      if (systemMethodCount != 0)
        {
         narr = (DEFMETHOD *) gm2(theEnv,(systemMethodCount * sizeof(DEFMETHOD)));
         i = 0;
         j = 0;
         while (i < gfunc->mcnt)
           {
            if (gfunc->methods[i].system)
              GenCopyMemory(DEFMETHOD,1,&narr[j++],&gfunc->methods[i]);
            i++;
           }
         rm(theEnv,(void *) gfunc->methods,(sizeof(DEFMETHOD) * gfunc->mcnt));
         gfunc->mcnt = (short) systemMethodCount;
         gfunc->methods = narr;
        }
      else
        {
         if (gfunc->mcnt != 0)
           rm(theEnv,(void *) gfunc->methods,(sizeof(DEFMETHOD) * gfunc->mcnt));
         gfunc->mcnt = 0;
         gfunc->methods = NULL;
        }
      return(true);
     }
   return(false);
  }

/**************************************************
  NAME         : RemoveDefgeneric
  DESCRIPTION  : Removes a generic function node
                   from the generic list along with
                   all its methods
  INPUTS       : The generic function
  RETURNS      : Nothing useful
  SIDE EFFECTS : List adjusted
                 Nodes deallocated
  NOTES        : Assumes generic is not in use!!!
 **************************************************/
void RemoveDefgeneric(
  void *theEnv,
  void *vgfunc)
  {
   DEFGENERIC *gfunc = (DEFGENERIC *) vgfunc;
   long i;

   for (i = 0 ; i < gfunc->mcnt ; i++)
     DeleteMethodInfo(theEnv,gfunc,&gfunc->methods[i]);

   if (gfunc->mcnt != 0)
     rm(theEnv,(void *) gfunc->methods,(sizeof(DEFMETHOD) * gfunc->mcnt));
   DecrementSymbolCount(theEnv,GetDefgenericNamePointer((void *) gfunc));
   EnvSetDefgenericPPForm(theEnv,(void *) gfunc,NULL);
   ClearUserDataList(theEnv,gfunc->header.usrData);
   rtn_struct(theEnv,defgeneric,gfunc);
  }

/****************************************************************
  NAME         : ClearDefgenerics
  DESCRIPTION  : Deletes all generic headers
  INPUTS       : None
  RETURNS      : true if all methods deleted, false otherwise
  SIDE EFFECTS : Generic headers deleted (and any implicit system
                  function methods)
  NOTES        : None
 ****************************************************************/
bool ClearDefgenerics(
  void *theEnv)
  {
   register DEFGENERIC *gfunc,*gtmp;
   bool success = true;

#if BLOAD || BLOAD_AND_BSAVE
   if (Bloaded(theEnv) == true) return(false);
#endif

   gfunc = (DEFGENERIC *) EnvGetNextDefgeneric(theEnv,NULL);
   while (gfunc != NULL)
     {
      gtmp = gfunc;
      gfunc = (DEFGENERIC *) EnvGetNextDefgeneric(theEnv,(void *) gfunc);
      if (RemoveAllExplicitMethods(theEnv,gtmp) == false)
        {
         CantDeleteItemErrorMessage(theEnv,"generic function",EnvGetDefgenericName(theEnv,gtmp));
         success = false;
        }
      else
        {
         RemoveConstructFromModule(theEnv,(struct constructHeader *) gtmp);
         RemoveDefgeneric(theEnv,(void *) gtmp);
        }
     }
   return(success);
  }

/********************************************************
  NAME         : MethodAlterError
  DESCRIPTION  : Prints out an error message reflecting
                   that a generic function's methods
                   cannot be altered while any of them
                   are executing
  INPUTS       : The generic function
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : None
 ********************************************************/
void MethodAlterError(
  void *theEnv,
  DEFGENERIC *gfunc)
  {
   PrintErrorID(theEnv,"GENRCFUN",1,false);
   EnvPrintRouter(theEnv,WERROR,"Defgeneric ");
   EnvPrintRouter(theEnv,WERROR,EnvGetDefgenericName(theEnv,(void *) gfunc));
   EnvPrintRouter(theEnv,WERROR," cannot be modified while one of its methods is executing.\n");
  }

/***************************************************
  NAME         : DeleteMethodInfo
  DESCRIPTION  : Deallocates all the data associated
                  w/ a method but does not release
                  the method structure itself
  INPUTS       : 1) The generic function address
                 2) The method address
  RETURNS      : Nothing useful
  SIDE EFFECTS : Nodes deallocated
  NOTES        : None
 ***************************************************/
void DeleteMethodInfo(
  void *theEnv,
  DEFGENERIC *gfunc,
  DEFMETHOD *meth)
  {
   short j,k;
   RESTRICTION *rptr;

   SaveBusyCount(gfunc);
   ExpressionDeinstall(theEnv,meth->actions);
   ReturnPackedExpression(theEnv,meth->actions);
   ClearUserDataList(theEnv,meth->usrData);
   if (meth->ppForm != NULL)
     rm(theEnv,(void *) meth->ppForm,(sizeof(char) * (strlen(meth->ppForm)+1)));
   for (j = 0 ; j < meth->restrictionCount ; j++)
     {
      rptr = &meth->restrictions[j];

      for (k = 0 ; k < rptr->tcnt ; k++)
#if OBJECT_SYSTEM
        DecrementDefclassBusyCount(theEnv,rptr->types[k]);
#else
        DecrementIntegerCount(theEnv,(INTEGER_HN *) rptr->types[k]);
#endif

      if (rptr->types != NULL)
        rm(theEnv,(void *) rptr->types,(sizeof(void *) * rptr->tcnt));
      ExpressionDeinstall(theEnv,rptr->query);
      ReturnPackedExpression(theEnv,rptr->query);
     }
   if (meth->restrictions != NULL)
     rm(theEnv,(void *) meth->restrictions,
        (sizeof(RESTRICTION) * meth->restrictionCount));
   RestoreBusyCount(gfunc);
  }
  
/***************************************************
  NAME         : DestroyMethodInfo
  DESCRIPTION  : Deallocates all the data associated
                  w/ a method but does not release
                  the method structure itself
  INPUTS       : 1) The generic function address
                 2) The method address
  RETURNS      : Nothing useful
  SIDE EFFECTS : Nodes deallocated
  NOTES        : None
 ***************************************************/
void DestroyMethodInfo(
  void *theEnv,
  DEFGENERIC *gfunc,
  DEFMETHOD *meth)
  {
   register int j;
   register RESTRICTION *rptr;
#if MAC_XCD
#pragma unused(gfunc)
#endif

   ReturnPackedExpression(theEnv,meth->actions);
   
   ClearUserDataList(theEnv,meth->usrData);
   if (meth->ppForm != NULL)
     rm(theEnv,(void *) meth->ppForm,(sizeof(char) * (strlen(meth->ppForm)+1)));
   for (j = 0 ; j < meth->restrictionCount ; j++)
     {
      rptr = &meth->restrictions[j];

      if (rptr->types != NULL)
        rm(theEnv,(void *) rptr->types,(sizeof(void *) * rptr->tcnt));
      ReturnPackedExpression(theEnv,rptr->query);
     }

   if (meth->restrictions != NULL)
     rm(theEnv,(void *) meth->restrictions,
        (sizeof(RESTRICTION) * meth->restrictionCount));
  }

/***************************************************
  NAME         : MethodsExecuting
  DESCRIPTION  : Determines if any of the methods of
                   a generic function are currently
                   executing
  INPUTS       : The generic function address
  RETURNS      : true if any methods are executing,
                   false otherwise
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
bool MethodsExecuting(
  DEFGENERIC *gfunc)
  {
   long i;

   for (i = 0 ; i < gfunc->mcnt ; i++)
     if (gfunc->methods[i].busy > 0)
       return(true);
   return(false);
  }
  
#endif

#if ! OBJECT_SYSTEM

/**************************************************************
  NAME         : SubsumeType
  DESCRIPTION  : Determines if the second type subsumes
                 the first type
                 (e.g. INTEGER is subsumed by NUMBER_TYPE_CODE)
  INPUTS       : Two type codes
  RETURNS      : true if type 2 subsumes type 1, false
                 otherwise
  SIDE EFFECTS : None
  NOTES        : Used only when COOL is not present
 **************************************************************/
bool SubsumeType(
  int t1,
  int t2)
  {
   if ((t2 == OBJECT_TYPE_CODE) || (t2 == PRIMITIVE_TYPE_CODE))
     return(true);
   if ((t2 == NUMBER_TYPE_CODE) && ((t1 == INTEGER) || (t1 == FLOAT)))
     return(true);
   if ((t2 == LEXEME_TYPE_CODE) && ((t1 == STRING) || (t1 == SYMBOL)))
     return(true);
   if ((t2 == ADDRESS_TYPE_CODE) && ((t1 == EXTERNAL_ADDRESS) ||
       (t1 == FACT_ADDRESS) || (t1 == INSTANCE_ADDRESS)))
     return(true);
   if ((t2 == LEXEME_TYPE_CODE) &&
       ((t1 == INSTANCE_NAME) || (t1 == INSTANCE_ADDRESS)))
     return(true);
   return(false);
  }

#endif

/*****************************************************
  NAME         : FindMethodByIndex
  DESCRIPTION  : Finds a generic function method of
                   specified index
  INPUTS       : 1) The generic function
                 2) The index
  RETURNS      : The position of the method in the
                   generic function's method array,
                   -1 if not found
  SIDE EFFECTS : None
  NOTES        : None
 *****************************************************/
long FindMethodByIndex(
  DEFGENERIC *gfunc,
  long theIndex)
  {
   long i;

   for (i = 0 ; i < gfunc->mcnt ; i++)
     if (gfunc->methods[i].index == theIndex)
       return(i);
   return(-1);
  }

#if DEBUGGING_FUNCTIONS || PROFILING_FUNCTIONS

/******************************************************************
  NAME         : PrintMethod
  DESCRIPTION  : Lists a brief description of methods for a method
  INPUTS       : 1) Buffer for method info
                 2) Size of buffer (not including space for '\0')
                 3) The method address
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : A terminating newline is NOT included
 ******************************************************************/
void PrintMethod(
  void *theEnv,
  char *buf,
  size_t buflen,
  DEFMETHOD *meth)
  {
#if MAC_XCD
#pragma unused(theEnv)
#endif
   long j,k;
   register RESTRICTION *rptr;
   char numbuf[15];

   buf[0] = '\0';
   if (meth->system)
     genstrncpy(buf,"SYS",(STD_SIZE) buflen);
   gensprintf(numbuf,"%-2d ",meth->index);
   genstrncat(buf,numbuf,(STD_SIZE) buflen-3);
   for (j = 0 ; j < meth->restrictionCount ; j++)
     {
      rptr = &meth->restrictions[j];
      if ((((int) j) == meth->restrictionCount-1) && (meth->maxRestrictions == -1))
        {
         if ((rptr->tcnt == 0) && (rptr->query == NULL))
           {
            genstrncat(buf,"$?",buflen-strlen(buf));
            break;
           }
         genstrncat(buf,"($? ",buflen-strlen(buf));
        }
      else
        genstrncat(buf,"(",buflen-strlen(buf));
      for (k = 0 ; k < rptr->tcnt ; k++)
        {
#if OBJECT_SYSTEM
         genstrncat(buf,EnvGetDefclassName(theEnv,rptr->types[k]),buflen-strlen(buf));
#else
         genstrncat(buf,TypeName(theEnv,ValueToInteger(rptr->types[k])),buflen-strlen(buf));
#endif
         if (((int) k) < (((int) rptr->tcnt) - 1))
           genstrncat(buf," ",buflen-strlen(buf));
        }
      if (rptr->query != NULL)
        {
         if (rptr->tcnt != 0)
           genstrncat(buf," ",buflen-strlen(buf));
         genstrncat(buf,"<qry>",buflen-strlen(buf));
        }
      genstrncat(buf,")",buflen-strlen(buf));
      if (((int) j) != (((int) meth->restrictionCount)-1))
        genstrncat(buf," ",buflen-strlen(buf));
     }
  }

#endif /* DEBUGGING_FUNCTIONS || PROFILING_FUNCTIONS */

#if DEBUGGING_FUNCTIONS

/*************************************************************
  NAME         : PreviewGeneric
  DESCRIPTION  : Allows the user to see a printout of all the
                   applicable methods for a particular generic
                   function call
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Any side-effects of evaluating the generic
                   function arguments
                 and evaluating query-functions to determine
                   the set of applicable methods
  NOTES        : H/L Syntax: (preview-generic <func> <args>)
 *************************************************************/
void PreviewGeneric(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   DEFGENERIC *gfunc;
   DEFGENERIC *previousGeneric;
   int oldce;
   CLIPSValue theArg;
   Environment *theEnv = UDFContextEnvironment(context);

   EvaluationData(theEnv)->EvaluationError = false;
   if (! UDFFirstArgument(context,SYMBOL_TYPE,&theArg)) return;

   gfunc = LookupDefgenericByMdlOrScope(theEnv,mCVToString(&theArg));
   if (gfunc == NULL)
     {
      PrintErrorID(theEnv,"GENRCFUN",3,false);
      EnvPrintRouter(theEnv,WERROR,"Unable to find generic function ");
      EnvPrintRouter(theEnv,WERROR,mCVToString(&theArg));
      EnvPrintRouter(theEnv,WERROR," in function preview-generic.\n");
      return;
     }
   oldce = ExecutingConstruct(theEnv);
   SetExecutingConstruct(theEnv,true);
   previousGeneric = DefgenericData(theEnv)->CurrentGeneric;
   DefgenericData(theEnv)->CurrentGeneric = gfunc;
   EvaluationData(theEnv)->CurrentEvaluationDepth++;
   PushProcParameters(theEnv,GetFirstArgument()->nextArg,
                          CountArguments(GetFirstArgument()->nextArg),
                          EnvGetDefgenericName(theEnv,(void *) gfunc),"generic function",
                          UnboundMethodErr);
   if (EvaluationData(theEnv)->EvaluationError)
     {
      PopProcParameters(theEnv);
      DefgenericData(theEnv)->CurrentGeneric = previousGeneric;
      EvaluationData(theEnv)->CurrentEvaluationDepth--;
      SetExecutingConstruct(theEnv,oldce);
      return;
     }
   gfunc->busy++;
   DisplayGenericCore(theEnv,gfunc);
   gfunc->busy--;
   PopProcParameters(theEnv);
   DefgenericData(theEnv)->CurrentGeneric = previousGeneric;
   EvaluationData(theEnv)->CurrentEvaluationDepth--;
   SetExecutingConstruct(theEnv,oldce);
  }

#endif /* DEBUGGING_FUNCTIONS */

/***************************************************
  NAME         : CheckGenericExists
  DESCRIPTION  : Finds the address of named
                  generic function and prints out
                  error message if not found
  INPUTS       : 1) Calling function
                 2) Name of generic function
  RETURNS      : Generic function address (NULL if
                   not found)
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
DEFGENERIC *CheckGenericExists(
  void *theEnv,
  const char *fname,
  const char *gname)
  {
   DEFGENERIC *gfunc;

   gfunc = LookupDefgenericByMdlOrScope(theEnv,gname);
   if (gfunc == NULL)
     {
      PrintErrorID(theEnv,"GENRCFUN",3,false);
      EnvPrintRouter(theEnv,WERROR,"Unable to find generic function ");
      EnvPrintRouter(theEnv,WERROR,gname);
      EnvPrintRouter(theEnv,WERROR," in function ");
      EnvPrintRouter(theEnv,WERROR,fname);
      EnvPrintRouter(theEnv,WERROR,".\n");
      EnvSetEvaluationError(theEnv,true);
     }
   return(gfunc);
  }

/***************************************************
  NAME         : CheckMethodExists
  DESCRIPTION  : Finds the array index of the
                  specified method and prints out
                  error message if not found
  INPUTS       : 1) Calling function
                 2) Generic function address
                 3) Index of method
  RETURNS      : Method array index (-1 if not found)
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
long CheckMethodExists(
  void *theEnv,
  const char *fname,
  DEFGENERIC *gfunc,
  long mi)
  {
   long fi;

   fi = FindMethodByIndex(gfunc,mi);
   if (fi == -1)
     {
      PrintErrorID(theEnv,"GENRCFUN",2,false);
      EnvPrintRouter(theEnv,WERROR,"Unable to find method ");
      EnvPrintRouter(theEnv,WERROR,EnvGetDefgenericName(theEnv,(void *) gfunc));
      EnvPrintRouter(theEnv,WERROR," #");
      PrintLongInteger(theEnv,WERROR,mi);
      EnvPrintRouter(theEnv,WERROR," in function ");
      EnvPrintRouter(theEnv,WERROR,fname);
      EnvPrintRouter(theEnv,WERROR,".\n");
      EnvSetEvaluationError(theEnv,true);
     }
   return(fi);
  }

#if ! OBJECT_SYSTEM

/*******************************************************
  NAME         : TypeName
  DESCRIPTION  : Given an integer type code, this
                 function returns the string name of
                 the type
  INPUTS       : The type code
  RETURNS      : The name-string of the type, or
                 "<???UNKNOWN-TYPE???>" for unrecognized
                 types
  SIDE EFFECTS : EvaluationError set and error message
                 printed for unrecognized types
  NOTES        : Used only when COOL is not present
 *******************************************************/
const char *TypeName(
  void *theEnv,
  int tcode)
  {
   switch (tcode)
     {
      case INTEGER             : return(INTEGER_TYPE_NAME);
      case FLOAT               : return(FLOAT_TYPE_NAME);
      case SYMBOL              : return(SYMBOL_TYPE_NAME);
      case STRING              : return(STRING_TYPE_NAME);
      case MULTIFIELD          : return(MULTIFIELD_TYPE_NAME);
      case EXTERNAL_ADDRESS    : return(EXTERNAL_ADDRESS_TYPE_NAME);
      case FACT_ADDRESS        : return(FACT_ADDRESS_TYPE_NAME);
      case INSTANCE_ADDRESS    : return(INSTANCE_ADDRESS_TYPE_NAME);
      case INSTANCE_NAME       : return(INSTANCE_NAME_TYPE_NAME);
      case OBJECT_TYPE_CODE    : return(OBJECT_TYPE_NAME);
      case PRIMITIVE_TYPE_CODE : return(PRIMITIVE_TYPE_NAME);
      case NUMBER_TYPE_CODE    : return(NUMBER_TYPE_NAME);
      case LEXEME_TYPE_CODE    : return(LEXEME_TYPE_NAME);
      case ADDRESS_TYPE_CODE   : return(ADDRESS_TYPE_NAME);
      case INSTANCE_TYPE_CODE  : return(INSTANCE_TYPE_NAME);
      default                  : PrintErrorID(theEnv,"INSCOM",1,false);
                                 EnvPrintRouter(theEnv,WERROR,"Undefined type in function type.\n");
                                 EnvSetEvaluationError(theEnv,true);
                                 return("<UNKNOWN-TYPE>");
     }
  }

#endif

/******************************************************
  NAME         : PrintGenericName
  DESCRIPTION  : Prints the name of a gneric function
                 (including the module name if the
                  generic is not in the current module)
  INPUTS       : 1) The logical name of the output
                 2) The generic functions
  RETURNS      : Nothing useful
  SIDE EFFECTS : Generic name printed
  NOTES        : None
 ******************************************************/
void PrintGenericName(
  void *theEnv,
  const char *logName,
  DEFGENERIC *gfunc)
  {
   if (gfunc->header.whichModule->theModule != ((struct defmodule *) EnvGetCurrentModule(theEnv)))
     {
      EnvPrintRouter(theEnv,logName,EnvGetDefmoduleName(theEnv,(void *)
                        gfunc->header.whichModule->theModule));
      EnvPrintRouter(theEnv,logName,"::");
     }
   EnvPrintRouter(theEnv,logName,ValueToString((void *) gfunc->header.name));
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

#if DEBUGGING_FUNCTIONS

/*********************************************************
  NAME         : DisplayGenericCore
  DESCRIPTION  : Prints out a description of a core
                   frame of applicable methods for
                   a particular call of a generic function
  INPUTS       : The generic function
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : None
 *********************************************************/
static void DisplayGenericCore(
  void *theEnv,
  DEFGENERIC *gfunc)
  {
   long i;
   char buf[256];
   bool rtn = false;

   for (i = 0 ; i < gfunc->mcnt ; i++)
     {
      gfunc->methods[i].busy++;
      if (IsMethodApplicable(theEnv,&gfunc->methods[i]))
        {
         rtn = true;
         EnvPrintRouter(theEnv,WDISPLAY,EnvGetDefgenericName(theEnv,(void *) gfunc));
         EnvPrintRouter(theEnv,WDISPLAY," #");
         PrintMethod(theEnv,buf,255,&gfunc->methods[i]);
         EnvPrintRouter(theEnv,WDISPLAY,buf);
         EnvPrintRouter(theEnv,WDISPLAY,"\n");
        }
      gfunc->methods[i].busy--;
     }
   if (rtn == false)
     {
      EnvPrintRouter(theEnv,WDISPLAY,"No applicable methods for ");
      EnvPrintRouter(theEnv,WDISPLAY,EnvGetDefgenericName(theEnv,(void *) gfunc));
      EnvPrintRouter(theEnv,WDISPLAY,".\n");
     }
  }

#endif

#endif

