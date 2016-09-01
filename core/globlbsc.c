   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  08/25/16             */
   /*                                                     */
   /*         DEFGLOBAL BASIC COMMANDS HEADER FILE        */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements core commands for the defglobal       */
/*   construct such as clear, reset, save, undefglobal,      */
/*   ppdefglobal, list-defglobals, and get-defglobals-list.  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*            Corrected compilation errors for files         */
/*            generated by constructs-to-c. DR0861           */
/*                                                           */
/*            Changed name of variable log to logName        */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Moved WatchGlobals global to defglobalData.    */
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

#if DEFGLOBAL_CONSTRUCT

#include "constrct.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE
#include "globlbin.h"
#endif
#if CONSTRUCT_COMPILER && (! RUN_TIME)
#include "globlcmp.h"
#endif
#include "globlcom.h"
#include "globldef.h"
#include "watch.h"

#include "globlbsc.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                    SaveDefglobals(Environment *,Defmodule *,const char *);
   static void                    ResetDefglobalAction(Environment *,struct constructHeader *,void *);
#if DEBUGGING_FUNCTIONS && (! RUN_TIME)
   static bool                    DefglobalWatchAccess(Environment *,int,bool,struct expr *);
   static bool                    DefglobalWatchPrint(Environment *,const char *,int,struct expr *);
#endif

/*****************************************************************/
/* DefglobalBasicCommands: Initializes basic defglobal commands. */
/*****************************************************************/
void DefglobalBasicCommands(
  Environment *theEnv)
  {
   AddSaveFunction(theEnv,"defglobal",SaveDefglobals,40);
   EnvAddResetFunction(theEnv,"defglobal",ResetDefglobals,50);

#if ! RUN_TIME
   EnvAddUDF(theEnv,"get-defglobal-list","m",0,1,"y",GetDefglobalListFunction,"GetDefglobalListFunction",NULL);
   EnvAddUDF(theEnv,"undefglobal","v",1,1,"y",UndefglobalCommand,"UndefglobalCommand",NULL);
   EnvAddUDF(theEnv,"defglobal-module","y",1,1,"y",DefglobalModuleFunction,"DefglobalModuleFunction",NULL);

#if DEBUGGING_FUNCTIONS
   EnvAddUDF(theEnv,"list-defglobals","v",0,1,"y",ListDefglobalsCommand,"ListDefglobalsCommand",NULL);
   EnvAddUDF(theEnv,"ppdefglobal","v",1,1,"y",PPDefglobalCommand,"PPDefglobalCommand",NULL);
   AddWatchItem(theEnv,"globals",0,&DefglobalData(theEnv)->WatchGlobals,0,DefglobalWatchAccess,DefglobalWatchPrint);
#endif

#if (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE)
   DefglobalBinarySetup(theEnv);
#endif

#if CONSTRUCT_COMPILER && (! RUN_TIME)
   DefglobalCompilerSetup(theEnv);
#endif

#endif
  }

/*************************************************************/
/* ResetDefglobals: Defglobal reset routine for use with the */
/*   reset command. Restores the values of the defglobals.   */
/*************************************************************/
void ResetDefglobals(
  Environment *theEnv)
  {
   if (! EnvGetResetGlobals(theEnv)) return;
   DoForAllConstructs(theEnv,ResetDefglobalAction,
                      DefglobalData(theEnv)->DefglobalModuleIndex,true,NULL);
  }

/******************************************************/
/* ResetDefglobalAction: Action to be applied to each */
/*   defglobal construct during a reset command.      */
/******************************************************/
static void ResetDefglobalAction(
  Environment *theEnv,
  struct constructHeader *theConstruct,
  void *buffer)
  {
#if MAC_XCD
#pragma unused(buffer)
#endif
   Defglobal *theDefglobal = (Defglobal *) theConstruct;
   CLIPSValue assignValue;

   if (EvaluateExpression(theEnv,theDefglobal->initial,&assignValue))
     {
      assignValue.type = SYMBOL;
      assignValue.value = EnvFalseSymbol(theEnv);
     }

   QSetDefglobalValue(theEnv,theDefglobal,&assignValue,false);
  }

/******************************************/
/* SaveDefglobals: Defglobal save routine */
/*   for use with the save command.       */
/******************************************/
static void SaveDefglobals(
  Environment *theEnv,
  Defmodule *theModule,
  const char *logicalName)
  {
   SaveConstruct(theEnv,theModule,logicalName,DefglobalData(theEnv)->DefglobalConstruct); 
  }

/********************************************/
/* UndefglobalCommand: H/L access routine   */
/*   for the undefglobal command.           */
/********************************************/
void UndefglobalCommand(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   UndefconstructCommand(context,"undefglobal",DefglobalData(theEnv)->DefglobalConstruct);
  }

/************************************/
/* EnvUndefglobal: C access routine */
/*   for the undefglobal command.   */
/************************************/
bool EnvUndefglobal(
  Environment *theEnv,
  Defglobal *theDefglobal)
  {
   return(Undefconstruct(theEnv,theDefglobal,DefglobalData(theEnv)->DefglobalConstruct)); 
  }

/**************************************************/
/* GetDefglobalListFunction: H/L access routine   */
/*   for the get-defglobal-list function.         */
/**************************************************/
void GetDefglobalListFunction(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  { 
   GetConstructListFunction(context,returnValue,DefglobalData(theEnv)->DefglobalConstruct); 
  }

/******************************************/
/* EnvGetDefglobalList: C access routine  */
/*   for the get-defglobal-list function. */
/******************************************/
void EnvGetDefglobalList(
  Environment *theEnv,
  CLIPSValue *returnValue,
  Defmodule *theModule)
  {
   GetConstructList(theEnv,returnValue,DefglobalData(theEnv)->DefglobalConstruct,theModule); 
  }

/*************************************************/
/* DefglobalModuleFunction: H/L access routine   */
/*   for the defglobal-module function.          */
/*************************************************/
void DefglobalModuleFunction(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CVSetCLIPSSymbol(returnValue,GetConstructModuleCommand(context,"defglobal-module",DefglobalData(theEnv)->DefglobalConstruct));
  }

#if DEBUGGING_FUNCTIONS

/********************************************/
/* PPDefglobalCommand: H/L access routine   */
/*   for the ppdefglobal command.           */
/********************************************/
void PPDefglobalCommand(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   PPConstructCommand(context,"ppdefglobal",DefglobalData(theEnv)->DefglobalConstruct);
  }

/*************************************/
/* PPDefglobal: C access routine for */
/*   the ppdefglobal command.        */
/*************************************/
bool PPDefglobal(
  Environment *theEnv,
  const char *defglobalName,
  const char *logicalName)
  {
   return(PPConstruct(theEnv,defglobalName,logicalName,DefglobalData(theEnv)->DefglobalConstruct)); 
  }

/***********************************************/
/* ListDefglobalsCommand: H/L access routine   */
/*   for the list-defglobals command.          */
/***********************************************/
void ListDefglobalsCommand(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   ListConstructCommand(context,DefglobalData(theEnv)->DefglobalConstruct);
  }

/***************************************/
/* EnvListDefglobals: C access routine */
/*   for the list-defglobals command.  */
/***************************************/
void EnvListDefglobals(
  Environment *theEnv,
  const char *logicalName,
  Defmodule *theModule)
  {
   ListConstruct(theEnv,DefglobalData(theEnv)->DefglobalConstruct,logicalName,theModule);
  }

/*********************************************************/
/* EnvGetDefglobalWatch: C access routine for retrieving */
/*   the current watch value of a defglobal.             */
/*********************************************************/
bool EnvGetDefglobalWatch(
  Environment *theEnv,
  Defglobal *theGlobal)
  { 
#if MAC_XCD
#pragma unused(theEnv)
#endif

   return theGlobal->watch;
  }

/******************************************************/
/* EnvSetDefglobalWatch: C access routine for setting */
/*   the current watch value of a defglobal.          */
/******************************************************/
void EnvSetDefglobalWatch(
  Environment *theEnv,
  bool newState,
  Defglobal *theGlobal)
  {  
#if MAC_XCD
#pragma unused(theEnv)
#endif

   theGlobal->watch = newState;
  }

#if ! RUN_TIME

/********************************************************/
/* DefglobalWatchAccess: Access routine for setting the */
/*   watch flag of a defglobal via the watch command.   */
/********************************************************/
static bool DefglobalWatchAccess(
  Environment *theEnv,
  int code,
  bool newState,
  EXPRESSION *argExprs)
  {
#if MAC_XCD
#pragma unused(code)
#endif

   return(ConstructSetWatchAccess(theEnv,DefglobalData(theEnv)->DefglobalConstruct,newState,argExprs,
                                  (bool (*)(Environment *,void *)) EnvGetDefglobalWatch,
                                  (void (*)(Environment *,bool,void *)) EnvSetDefglobalWatch));
  }

/*********************************************************************/
/* DefglobalWatchPrint: Access routine for printing which defglobals */
/*   have their watch flag set via the list-watch-items command.     */
/*********************************************************************/
static bool DefglobalWatchPrint(
  Environment *theEnv,
  const char *logName,
  int code,
  EXPRESSION *argExprs)
  {
#if MAC_XCD
#pragma unused(code)
#endif
   return(ConstructPrintWatchAccess(theEnv,DefglobalData(theEnv)->DefglobalConstruct,logName,argExprs,
                                    (bool (*)(Environment *,void *)) EnvGetDefglobalWatch,
                                    (void (*)(Environment *,bool,void *)) EnvSetDefglobalWatch));
  }

#endif /* ! RUN_TIME */

#endif /* DEBUGGING_FUNCTIONS */

#endif /* DEFGLOBAL_CONSTRUCT */


