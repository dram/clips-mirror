   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  08/16/14            */
   /*                                                     */
   /*              DEFFUNCTION HEADER FILE                */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
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
/*            Corrected code to remove run-time program      */
/*            compiler warning.                              */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            Changed integer type/precision.                */
/*                                                           */
/*            Added missing initializer for ENTITY_RECORD.   */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*************************************************************/

#ifndef _H_dffnxfun
#define _H_dffnxfun

typedef struct deffunctionStruct DEFFUNCTION;
typedef struct deffunctionModule DEFFUNCTION_MODULE;

#ifndef _H_conscomp
#include "conscomp.h"
#endif
#ifndef _H_cstrccom
#include "cstrccom.h"
#endif
#ifndef _H_moduldef
#include "moduldef.h"
#endif
#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_expressn
#include "expressn.h"
#endif
#ifndef _H_symbol
#include "symbol.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif
#ifdef _DFFNXFUN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

struct deffunctionModule
  {
   struct defmoduleItemHeader header;
  };

struct deffunctionStruct
  {
   struct constructHeader header;
   unsigned busy,
            executing;
   unsigned short trace;
   EXPRESSION *code;
   int minNumberOfParameters,
       maxNumberOfParameters,
       numberOfLocalVars;
  };
  
#define DEFFUNCTION_DATA 23

struct deffunctionData
  { 
   struct construct *DeffunctionConstruct;
   int DeffunctionModuleIndex;
   ENTITY_RECORD DeffunctionEntityRecord;
#if DEBUGGING_FUNCTIONS
   unsigned WatchDeffunctions;
#endif
   struct CodeGeneratorItem *DeffunctionCodeItem;
   DEFFUNCTION *ExecutingDeffunction;
#if (! BLOAD_ONLY) && (! RUN_TIME)
   struct token DFInputToken;
#endif
  };

#define DeffunctionData(theEnv) ((struct deffunctionData *) GetEnvironmentData(theEnv,DEFFUNCTION_DATA))

   LOCALE int                            CheckDeffunctionCall(void *,void *,int);
   LOCALE void                           DeffunctionGetBind(DATA_OBJECT *);
   LOCALE void                           DFRtnUnknown(DATA_OBJECT *);
   LOCALE void                           DFWildargs(DATA_OBJECT *);
   LOCALE char                          *EnvDeffunctionModule(void *,void *);
   LOCALE void                          *EnvFindDeffunction(void *,const char *);
   LOCALE void                           EnvGetDeffunctionList(void *,DATA_OBJECT *,struct defmodule *);
   LOCALE char                          *EnvGetDeffunctionName(void *,void *);
   LOCALE SYMBOL_HN                     *EnvGetDeffunctionNamePointer(void *,void *);
   LOCALE char                          *EnvGetDeffunctionPPForm(void *,void *);
   LOCALE void                          *EnvGetNextDeffunction(void *,void *);
   LOCALE int                            EnvIsDeffunctionDeletable(void *,void *);
   LOCALE void                           EnvSetDeffunctionPPForm(void *,void *,char *);
   LOCALE intBool                        EnvUndeffunction(void *,void *);
   LOCALE void                           GetDeffunctionListFunction(void *,DATA_OBJECT *);
   LOCALE void                          *GetDeffunctionModuleCommand(void *);
   LOCALE DEFFUNCTION                   *LookupDeffunctionByMdlOrScope(void *,const char *);
   LOCALE DEFFUNCTION                   *LookupDeffunctionInScope(void *,const char *);
#if (! BLOAD_ONLY) && (! RUN_TIME)
   LOCALE void                           RemoveDeffunction(void *,void *);
#endif
   LOCALE void                           SetupDeffunctions(void *);
   LOCALE void                           UndeffunctionCommand(void *);
#if DEBUGGING_FUNCTIONS
   LOCALE unsigned                       EnvGetDeffunctionWatch(void *,void *);
   LOCALE void                           EnvListDeffunctions(void *,const char *,struct defmodule *);
   LOCALE void                           EnvSetDeffunctionWatch(void *,unsigned,void *);
   LOCALE void                           ListDeffunctionsCommand(void *);
   LOCALE void                           PPDeffunctionCommand(void *);
#endif

#if ALLOW_ENVIRONMENT_GLOBALS

   LOCALE char                          *DeffunctionModule(void *);
   LOCALE void                          *FindDeffunction(const char *);
   LOCALE void                           GetDeffunctionList(DATA_OBJECT *,struct defmodule *);
   LOCALE char                          *GetDeffunctionName(void *);
   LOCALE char                          *GetDeffunctionPPForm(void *);
   LOCALE void                          *GetNextDeffunction(void *);
   LOCALE intBool                        IsDeffunctionDeletable(void *);
   LOCALE intBool                        Undeffunction(void *);
#if DEBUGGING_FUNCTIONS
   LOCALE unsigned                       GetDeffunctionWatch(void *);
   LOCALE void                           ListDeffunctions(const char *,struct defmodule *);
   LOCALE void                           SetDeffunctionWatch(unsigned,void *);
#endif 

#endif /* ALLOW_ENVIRONMENT_GLOBALS */

#endif /* _H_dffnxfun */






