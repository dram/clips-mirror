   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.30  01/25/15          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Corrected compilation errors for files         */
/*            generated by constructs-to-c. DR0861           */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Borland C (IBM_TBC) and Metrowerks CodeWarrior */
/*            (MAC_MCW, IBM_MCW) are no longer supported.    */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*            Changed find construct functionality so that   */
/*            imported modules are search when locating a    */
/*            named construct.                               */
/*                                                           */
/*************************************************************/

#ifndef _H_classcom
#define _H_classcom

#define CONVENIENCE_MODE  0
#define CONSERVATION_MODE 1

#ifndef _H_cstrccom
#include "cstrccom.h"
#endif
#ifndef _H_moduldef
#include "moduldef.h"
#endif
#ifndef _H_object
#include "object.h"
#endif
#ifndef _H_symbol
#include "symbol.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _CLASSCOM_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE const char             *EnvGetDefclassName(void *,void *);
   LOCALE const char             *EnvGetDefclassPPForm(void *,void *);
   LOCALE struct defmoduleItemHeader 
                                 *EnvGetDefclassModule(void *,void *);
   LOCALE const char             *EnvDefclassModule(void *,void *);
   LOCALE SYMBOL_HN              *GetDefclassNamePointer(void *);
   LOCALE void                    SetNextDefclass(void *,void *);
   LOCALE void                    EnvSetDefclassPPForm(void *,void *,char *);

   LOCALE void                   *EnvFindDefclass(void *,const char *);
   LOCALE void                   *EnvFindDefclassInModule(void *,const char *);
   LOCALE DEFCLASS               *LookupDefclassByMdlOrScope(void *,const char *);
   LOCALE DEFCLASS               *LookupDefclassInScope(void *,const char *);
   LOCALE DEFCLASS               *LookupDefclassAnywhere(void *,struct defmodule *,const char *);
   LOCALE intBool                 DefclassInScope(void *,DEFCLASS *,struct defmodule *);
   LOCALE void                   *EnvGetNextDefclass(void *,void *);
   LOCALE intBool                 EnvIsDefclassDeletable(void *,void *);

   LOCALE void                    UndefclassCommand(void *);
   LOCALE unsigned short          EnvSetClassDefaultsMode(void *,unsigned short);
   LOCALE unsigned short          EnvGetClassDefaultsMode(void *);
   LOCALE void                   *GetClassDefaultsModeCommand(void *);
   LOCALE void                   *SetClassDefaultsModeCommand(void *);

#if DEBUGGING_FUNCTIONS
   LOCALE void                    PPDefclassCommand(void *);
   LOCALE void                    ListDefclassesCommand(void *);
   LOCALE void                    EnvListDefclasses(void *,const char *,struct defmodule *);
   LOCALE unsigned                EnvGetDefclassWatchInstances(void *,void *);
   LOCALE void                    EnvSetDefclassWatchInstances(void *,unsigned,void *);
   LOCALE unsigned                EnvGetDefclassWatchSlots(void *,void *);
   LOCALE void                    EnvSetDefclassWatchSlots(void *,unsigned,void *);
   LOCALE unsigned                DefclassWatchAccess(void *,int,unsigned,EXPRESSION *);
   LOCALE unsigned                DefclassWatchPrint(void *,const char *,int,EXPRESSION *);
#endif

   LOCALE void                    GetDefclassListFunction(void *,DATA_OBJECT *);
   LOCALE void                    EnvGetDefclassList(void *,DATA_OBJECT *,struct defmodule *);
   LOCALE intBool                 EnvUndefclass(void *,void *);
   LOCALE intBool                 HasSuperclass(DEFCLASS *,DEFCLASS *);

   LOCALE SYMBOL_HN              *CheckClassAndSlot(void *,const char *,DEFCLASS **);

#if (! BLOAD_ONLY) && (! RUN_TIME)
   LOCALE void                    SaveDefclasses(void *,void *,const char *);
#endif

#if ALLOW_ENVIRONMENT_GLOBALS

   LOCALE const char             *DefclassModule(void *);
   LOCALE void                   *FindDefclass(const char *);
   LOCALE void                    GetDefclassList(DATA_OBJECT *,struct defmodule *);
   LOCALE unsigned short          GetClassDefaultsMode(void);
   LOCALE struct defmoduleItemHeader 
                                 *GetDefclassModule(void *);
   LOCALE const char             *GetDefclassName(void *);
   LOCALE const char             *GetDefclassPPForm(void *);
   LOCALE unsigned                GetDefclassWatchInstances(void *);
   LOCALE unsigned                GetDefclassWatchSlots(void *);
   LOCALE void                   *GetNextDefclass(void *,void *);
   LOCALE intBool                 IsDefclassDeletable(void *);
   LOCALE void                    ListDefclasses(const char *,struct defmodule *);
   LOCALE unsigned short          SetClassDefaultsMode(unsigned short);
   LOCALE void                    SetDefclassWatchInstances(unsigned,void *);
   LOCALE void                    SetDefclassWatchSlots(unsigned,void *);
   LOCALE intBool                 Undefclass(void *);

#endif /* ALLOW_ENVIRONMENT_GLOBALS */

#endif /* _H_classcom */
