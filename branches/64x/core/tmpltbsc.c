   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  08/25/16             */
   /*                                                     */
   /*          DEFTEMPLATE BASIC COMMANDS MODULE          */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements core commands for the deftemplate     */
/*   construct such as clear, reset, save, undeftemplate,    */
/*   ppdeftemplate, list-deftemplates, and                   */
/*   get-deftemplate-list.                                   */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Corrected compilation errors for files         */
/*            generated by constructs-to-c. DR0861           */
/*                                                           */
/*            Changed name of variable log to logName        */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Corrected code to remove compiler warnings     */
/*            when ENVIRONMENT_API_ONLY flag is set.         */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*      6.40: Pragma once and other inclusion changes.       */
/*                                                           */
/*            Added support for booleans with <stdbool.h>.   */
/*                                                           */
/*            Removed use of void pointers for specific      */
/*            data structures.                               */
/*                                                           */
/*            ALLOW_ENVIRONMENT_GLOBALS no longer supported. */
/*                                                           */
/*            UDF redesign.                                  */
/*                                                           */
/*************************************************************/

#include "setup.h"

#if DEFTEMPLATE_CONSTRUCT

#include <stdio.h>
#include <string.h>

#include "argacces.h"
#include "constrct.h"
#include "cstrccom.h"
#include "cstrcpsr.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "factrhs.h"
#include "memalloc.h"
#include "router.h"
#include "scanner.h"
#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE
#include "tmpltbin.h"
#endif
#if CONSTRUCT_COMPILER && (! RUN_TIME)
#include "tmpltcmp.h"
#endif
#include "tmpltdef.h"
#include "tmpltpsr.h"
#include "tmpltutl.h"

#include "tmpltbsc.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if ! DEFFACTS_CONSTRUCT
   static void                    ResetDeftemplates(Environment *);
#endif
   static void                    ClearDeftemplates(Environment *);
   static void                    SaveDeftemplates(Environment *,Defmodule *,const char *);

/*********************************************************************/
/* DeftemplateBasicCommands: Initializes basic deftemplate commands. */
/*********************************************************************/
void DeftemplateBasicCommands(
  Environment *theEnv)
  {
#if ! DEFFACTS_CONSTRUCT
   EnvAddResetFunction(theEnv,"deftemplate",ResetDeftemplates,0);
#endif
   EnvAddClearFunction(theEnv,"deftemplate",ClearDeftemplates,0);
   AddSaveFunction(theEnv,"deftemplate",SaveDeftemplates,10);

#if ! RUN_TIME
   EnvAddUDF(theEnv,"get-deftemplate-list","m",0,1,"y",GetDeftemplateListFunction,"GetDeftemplateListFunction",NULL);
   EnvAddUDF(theEnv,"undeftemplate","v",1,1,"y",UndeftemplateCommand,"UndeftemplateCommand",NULL);
   EnvAddUDF(theEnv,"deftemplate-module","y",1,1,"y",DeftemplateModuleFunction,"DeftemplateModuleFunction",NULL);

#if DEBUGGING_FUNCTIONS
   EnvAddUDF(theEnv,"list-deftemplates","v",0,1,"y",ListDeftemplatesCommand,"ListDeftemplatesCommand",NULL);
   EnvAddUDF(theEnv,"ppdeftemplate","v",1,1,"y",PPDeftemplateCommand,"PPDeftemplateCommand",NULL);
#endif

#if (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE)
   DeftemplateBinarySetup(theEnv);
#endif

#if CONSTRUCT_COMPILER && (! RUN_TIME)
   DeftemplateCompilerSetup(theEnv);
#endif

#endif
  }

/*************************************************************/
/* ResetDeftemplates: Deftemplate reset routine for use with */
/*   the reset command. Asserts the initial-fact fact when   */
/*   the deffacts construct has been disabled.               */
/*************************************************************/
#if ! DEFFACTS_CONSTRUCT
static void ResetDeftemplates(
  Environment *theEnv)
  {
   Fact *factPtr;

   factPtr = StringToFact(theEnv,"(initial-fact)");

   if (factPtr == NULL) return;

   EnvAssert(theEnv,factPtr);
 }
#endif

/*****************************************************************/
/* ClearDeftemplates: Deftemplate clear routine for use with the */
/*   clear command. Creates the initial-facts deftemplate.       */
/*****************************************************************/
static void ClearDeftemplates(
  Environment *theEnv)
  {
#if (! RUN_TIME) && (! BLOAD_ONLY)

   CreateImpliedDeftemplate(theEnv,(SYMBOL_HN *) EnvAddSymbol(theEnv,"initial-fact"),false);
#else
#if MAC_XCD
#pragma unused(theEnv)
#endif
#endif
  }

/**********************************************/
/* SaveDeftemplates: Deftemplate save routine */
/*   for use with the save command.           */
/**********************************************/
static void SaveDeftemplates(
  Environment *theEnv,
  Defmodule *theModule,
  const char *logicalName)
  {
   SaveConstruct(theEnv,theModule,logicalName,DeftemplateData(theEnv)->DeftemplateConstruct);
  }

/**********************************************/
/* UndeftemplateCommand: H/L access routine   */
/*   for the undeftemplate command.           */
/**********************************************/
void UndeftemplateCommand(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   UndefconstructCommand(context,"undeftemplate",DeftemplateData(theEnv)->DeftemplateConstruct);
  }

/**************************************/
/* EnvUndeftemplate: C access routine */
/*   for the undeftemplate command.   */
/**************************************/
bool EnvUndeftemplate(
  Environment *theEnv,
  Deftemplate *theDeftemplate)
  {
   return(Undefconstruct(theEnv,theDeftemplate,DeftemplateData(theEnv)->DeftemplateConstruct));
  }

/****************************************************/
/* GetDeftemplateListFunction: H/L access routine   */
/*   for the get-deftemplate-list function.         */
/****************************************************/
void GetDeftemplateListFunction(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   GetConstructListFunction(context,returnValue,DeftemplateData(theEnv)->DeftemplateConstruct);
  }

/***********************************************/
/* EnvGetDeftemplateList: C access routine for */
/*   the get-deftemplate-list function.        */
/***********************************************/
void EnvGetDeftemplateList(
  Environment *theEnv,
  CLIPSValue *returnValue,
  Defmodule *theModule)
  {
   GetConstructList(theEnv,returnValue,DeftemplateData(theEnv)->DeftemplateConstruct,theModule);
  }

/***************************************************/
/* DeftemplateModuleFunction: H/L access routine   */
/*   for the deftemplate-module function.          */
/***************************************************/
void DeftemplateModuleFunction(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   returnValue->type = SYMBOL;
   returnValue->value = GetConstructModuleCommand(context,"deftemplate-module",DeftemplateData(theEnv)->DeftemplateConstruct);
  }

#if DEBUGGING_FUNCTIONS

/**********************************************/
/* PPDeftemplateCommand: H/L access routine   */
/*   for the ppdeftemplate command.           */
/**********************************************/
void PPDeftemplateCommand(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   PPConstructCommand(context,"ppdeftemplate",DeftemplateData(theEnv)->DeftemplateConstruct);
  }

/***************************************/
/* PPDeftemplate: C access routine for */
/*   the ppdeftemplate command.        */
/***************************************/
bool PPDeftemplate(
  Environment *theEnv,
  const char *deftemplateName,
  const char *logicalName)
  {
   return(PPConstruct(theEnv,deftemplateName,logicalName,DeftemplateData(theEnv)->DeftemplateConstruct));
  }

/*************************************************/
/* ListDeftemplatesCommand: H/L access routine   */
/*   for the list-deftemplates command.          */
/*************************************************/
void ListDeftemplatesCommand(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   ListConstructCommand(context,DeftemplateData(theEnv)->DeftemplateConstruct);
  }

/*****************************************/
/* EnvListDeftemplates: C access routine */
/*   for the list-deftemplates command.  */
/*****************************************/
void EnvListDeftemplates(
  Environment *theEnv,
  const char *logicalName,
  Defmodule *theModule)
  {
   ListConstruct(theEnv,DeftemplateData(theEnv)->DeftemplateConstruct,logicalName,theModule);
  }

/***********************************************************/
/* EnvGetDeftemplateWatch: C access routine for retrieving */
/*   the current watch value of a deftemplate.             */
/***********************************************************/
bool EnvGetDeftemplateWatch(
  Environment *theEnv,
  Deftemplate *theTemplate)
  {
#if MAC_XCD
#pragma unused(theEnv)
#endif

   return theTemplate->watch;
  }

/*********************************************************/
/* EnvSetDeftemplateWatch:  C access routine for setting */
/*   the current watch value of a deftemplate.           */
/*********************************************************/
void EnvSetDeftemplateWatch(
  Environment *theEnv,
  bool newState,
  Deftemplate *theTemplate)
  {
#if MAC_XCD
#pragma unused(theEnv)
#endif

   theTemplate->watch = newState;
  }

/**********************************************************/
/* DeftemplateWatchAccess: Access routine for setting the */
/*   watch flag of a deftemplate via the watch command.   */
/**********************************************************/
bool DeftemplateWatchAccess(
  Environment *theEnv,
  int code,
  bool newState,
  EXPRESSION *argExprs)
  {
#if MAC_XCD
#pragma unused(code)
#endif

   return(ConstructSetWatchAccess(theEnv,DeftemplateData(theEnv)->DeftemplateConstruct,newState,argExprs,
                                  ((bool (*)(Environment *,void *)) EnvGetDeftemplateWatch),
                                  ((void (*)(Environment *,bool,void *)) EnvSetDeftemplateWatch)));
  }

/*************************************************************************/
/* DeftemplateWatchPrint: Access routine for printing which deftemplates */
/*   have their watch flag set via the list-watch-items command.         */
/*************************************************************************/
bool DeftemplateWatchPrint(
  Environment *theEnv,
  const char *logName,
  int code,
  EXPRESSION *argExprs)
  {
#if MAC_XCD
#pragma unused(code)
#endif

   return(ConstructPrintWatchAccess(theEnv,DeftemplateData(theEnv)->DeftemplateConstruct,logName,argExprs,
                                    ((bool (*)(Environment *,void *)) EnvGetDeftemplateWatch),
                                    ((void (*)(Environment *,bool,void *)) EnvSetDeftemplateWatch)));
  }

#endif /* DEBUGGING_FUNCTIONS */

#endif /* DEFTEMPLATE_CONSTRUCT */


