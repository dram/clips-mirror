   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  08/05/14            */
   /*                                                     */
   /*                DEFGLOBAL HEADER FILE                */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*            Moved WatchGlobals global to defglobalData.    */
/*                                                           */
/*************************************************************/

#ifndef _H_globldef
#define _H_globldef

#ifndef _H_conscomp
#include "conscomp.h"
#endif
#ifndef _H_symbol
#include "symbol.h"
#endif
#ifndef _H_expressn
#include "expressn.h"
#endif
#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_constrct
#include "constrct.h"
#endif
#ifndef _H_moduldef
#include "moduldef.h"
#endif
#ifndef _H_cstrccom
#include "cstrccom.h"
#endif

#define DEFGLOBAL_DATA 1

struct defglobalData
  { 
   struct construct *DefglobalConstruct;
   int DefglobalModuleIndex;  
   int ChangeToGlobals;
#if DEBUGGING_FUNCTIONS
   unsigned WatchGlobals;
#endif
   intBool ResetGlobals;
   struct entityRecord GlobalInfo;
   struct entityRecord DefglobalPtrRecord;
   long LastModuleIndex;
   struct defmodule *TheDefmodule;
#if CONSTRUCT_COMPILER && (! RUN_TIME)
   struct CodeGeneratorItem *DefglobalCodeItem;
#endif
  };

struct defglobal
  {
   struct constructHeader header;
   unsigned int watch   : 1;
   unsigned int inScope : 1;
   long busyCount;
   DATA_OBJECT current;
   struct expr *initial;
  };

struct defglobalModule
  {
   struct defmoduleItemHeader header;
  };

#define DefglobalData(theEnv) ((struct defglobalData *) GetEnvironmentData(theEnv,DEFGLOBAL_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _GLOBLDEF_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           InitializeDefglobals(void *);
   LOCALE void                          *EnvFindDefglobal(void *,const char *);
   LOCALE void                          *EnvGetNextDefglobal(void *,void *);
   LOCALE void                           CreateInitialFactDefglobal(void);
   LOCALE intBool                        EnvIsDefglobalDeletable(void *,void *);
   LOCALE struct defglobalModule        *GetDefglobalModuleItem(void *,struct defmodule *);
   LOCALE void                           QSetDefglobalValue(void *,struct defglobal *,DATA_OBJECT_PTR,int);
   LOCALE struct defglobal              *QFindDefglobal(void *,struct symbolHashNode *);
   LOCALE void                           EnvGetDefglobalValueForm(void *,char *,unsigned,void *);
   LOCALE int                            EnvGetGlobalsChanged(void *);
   LOCALE void                           EnvSetGlobalsChanged(void *,int);
   LOCALE intBool                        EnvGetDefglobalValue(void *,const char *,DATA_OBJECT_PTR);
   LOCALE intBool                        EnvSetDefglobalValue(void *,const char *,DATA_OBJECT_PTR);
   LOCALE void                           UpdateDefglobalScope(void *);
   LOCALE void                          *GetNextDefglobalInScope(void *,void *);
   LOCALE int                            QGetDefglobalValue(void *,void *,DATA_OBJECT_PTR);
   LOCALE char                          *EnvDefglobalModule(void *,void *);
   LOCALE char                          *EnvGetDefglobalName(void *,void *);
   LOCALE char                          *EnvGetDefglobalPPForm(void *,void *);

#if ALLOW_ENVIRONMENT_GLOBALS

   LOCALE char                          *DefglobalModule(void *);
   LOCALE void                          *FindDefglobal(const char *);
   LOCALE char                          *GetDefglobalName(void *);
   LOCALE char                          *GetDefglobalPPForm(void *);
   LOCALE intBool                        GetDefglobalValue(const char *,DATA_OBJECT_PTR);
   LOCALE void                           GetDefglobalValueForm(char *,unsigned,void *);
   LOCALE int                            GetGlobalsChanged();
   LOCALE void                          *GetNextDefglobal(void *);
   LOCALE intBool                        IsDefglobalDeletable(void *);
   LOCALE intBool                        SetDefglobalValue(const char *,DATA_OBJECT_PTR);
   LOCALE void                           SetGlobalsChanged(int);

#endif /* ALLOW_ENVIRONMENT_GLOBALS */

#endif /* _H_globldef */


