   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  06/17/16            */
   /*                                                     */
   /*          MISCELLANEOUS FUNCTIONS HEADER FILE        */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
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
/*            Changed name of variable exp to theExp         */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Removed CONFLICT_RESOLUTION_STRATEGIES,        */
/*            DYNAMIC_SALIENCE, INCREMENTAL_RESET,           */
/*            LOGICAL_DEPENDENCIES, IMPERATIVE_METHODS       */
/*            INSTANCE_PATTERN_MATCHING,                     */
/*            IMPERATIVE_MESSAGE_HANDLERS, and               */
/*            AUXILIARY_MESSAGE_HANDLERS compilation flags.  */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Support for long long integers.                */
/*                                                           */
/*            Used gensprintf instead of sprintf.            */
/*                                                           */
/*            Removed conditional code for unsupported       */
/*            compilers/operating systems.                   */
/*                                                           */
/*            Renamed EX_MATH compiler flag to               */
/*            EXTENDED_MATH_FUNCTIONS.                       */
/*                                                           */
/*            Combined BASIC_IO and EXT_IO compilation       */
/*            flags into the IO_FUNCTIONS compilation flag.  */
/*                                                           */    
/*            Removed code associated with HELP_FUNCTIONS    */
/*            and EMACS_EDITOR compiler flags.               */
/*                                                           */    
/*            Added operating-system function.               */
/*                                                           */ 
/*            Added new function (for future use).           */
/*                                                           */ 
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*      6.31: Added local-time and gm-time functions.        */
/*                                                           */
/*      6.40: Refactored code to reduce header dependencies  */
/*            in sysdep.c.                                   */
/*                                                           */
/*************************************************************/

#ifndef _H_miscfun

#define _H_miscfun

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _MISCFUN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           MiscFunctionDefinitions(void *);
   LOCALE void                           CreateFunction(void *,DATA_OBJECT_PTR);
   LOCALE long long                      SetgenFunction(void *);
   LOCALE void                          *GensymFunction(void *);
   LOCALE void                          *GensymStarFunction(void *);
   LOCALE long long                      RandomFunction(void *);
   LOCALE void                           SeedFunction(void *);
   LOCALE long long                      LengthFunction(void *);
   LOCALE void                           ConserveMemCommand(void *);
   LOCALE long long                      ReleaseMemCommand(void *);
   LOCALE long long                      MemUsedCommand(void *);
   LOCALE long long                      MemRequestsCommand(void *);
   LOCALE void                           OptionsCommand(void *);
   LOCALE void                          *OperatingSystemFunction(void *);
   LOCALE void                           ExpandFuncCall(void *,DATA_OBJECT *);
   LOCALE void                           DummyExpandFuncMultifield(void *,DATA_OBJECT *);
   LOCALE void                          *CauseEvaluationError(void *);
   LOCALE intBool                        SetSORCommand(void *);
   LOCALE void                          *GetFunctionRestrictions(void *);
   LOCALE void                           AproposCommand(void *);
   LOCALE void                          *GensymStar(void *);
   LOCALE void                           GetFunctionListFunction(void *,DATA_OBJECT *);
   LOCALE void                           FuncallFunction(void *,DATA_OBJECT *);
   LOCALE void                           NewFunction(void *,DATA_OBJECT *);
   LOCALE void                           CallFunction(void *,DATA_OBJECT *);
   LOCALE double                         TimerFunction(void *);
   LOCALE double                         TimeFunction(void *);
   LOCALE void                           SystemCommand(void *);
   LOCALE void                           LocalTimeFunction(void *,DATA_OBJECT *);
   LOCALE void                           GMTimeFunction(void *,DATA_OBJECT *);

#endif /* _H_miscfun */






