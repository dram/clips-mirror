   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  06/23/16             */
   /*                                                     */
   /*                DEFFACTS PARSER MODULE               */
   /*******************************************************/

/*************************************************************/
/* Purpose: Parses a deffacts construct.                     */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW and       */
/*            MAC_MCW).                                      */
/*                                                           */
/*            GetConstructNameAndComment API change.         */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Changed find construct functionality so that   */
/*            imported modules are search when locating a    */
/*            named construct.                               */
/*                                                           */
/*      6.40: Pragma once and other inclusion changes.       */
/*                                                           */
/*************************************************************/

#include "setup.h"

#if DEFFACTS_CONSTRUCT

#if BLOAD || BLOAD_AND_BSAVE
#include "bload.h"
#endif

#include "cstrcpsr.h"
#include "dffctbsc.h"
#include "dffctdef.h"
#include "envrnmnt.h"
#include "factrhs.h"
#include "memalloc.h"
#include "router.h"

#include "dffctpsr.h"

/************************************************************/
/* ParseDeffacts: Coordinates all actions necessary for the */
/*   addition of a deffacts construct into the current      */
/*   environment. Called when parsing a construct after the */
/*   deffacts keyword has been found.                       */
/************************************************************/
int ParseDeffacts(
  void *theEnv,
  const char *readSource)
  {
#if (! RUN_TIME) && (! BLOAD_ONLY)
   SYMBOL_HN *deffactsName;
   struct expr *temp;
   struct deffacts *newDeffacts;
   int deffactsError;
   struct token inputToken;

   /*=========================*/
   /* Parsing initialization. */
   /*=========================*/

   deffactsError = FALSE;
   SetPPBufferStatus(theEnv,ON);

   FlushPPBuffer(theEnv);
   SetIndentDepth(theEnv,3);
   SavePPBuffer(theEnv,"(deffacts ");

   /*==========================================================*/
   /* Deffacts can not be added when a binary image is loaded. */
   /*==========================================================*/

#if BLOAD || BLOAD_AND_BSAVE
   if ((Bloaded(theEnv) == TRUE) && (! ConstructData(theEnv)->CheckSyntaxMode))
     {
      CannotLoadWithBloadMessage(theEnv,"deffacts");
      return(TRUE);
     }
#endif

   /*============================*/
   /* Parse the deffacts header. */
   /*============================*/

   deffactsName = GetConstructNameAndComment(theEnv,readSource,&inputToken,"deffacts",
                                             EnvFindDeffactsInModule,EnvUndeffacts,"$",TRUE,
                                             TRUE,TRUE,FALSE);
   if (deffactsName == NULL) { return(TRUE); }

   /*===============================================*/
   /* Parse the list of facts in the deffacts body. */
   /*===============================================*/

   temp = BuildRHSAssert(theEnv,readSource,&inputToken,&deffactsError,FALSE,FALSE,"deffacts");

   if (deffactsError == TRUE) { return(TRUE); }

   if (ExpressionContainsVariables(temp,FALSE))
     {
      LocalVariableErrorMessage(theEnv,"a deffacts construct");
      ReturnExpression(theEnv,temp);
      return(TRUE);
     }

   SavePPBuffer(theEnv,"\n");

   /*==============================================*/
   /* If we're only checking syntax, don't add the */
   /* successfully parsed deffacts to the KB.      */
   /*==============================================*/

   if (ConstructData(theEnv)->CheckSyntaxMode)
     {
      ReturnExpression(theEnv,temp);
      return(FALSE);
     }

   /*==========================*/
   /* Create the new deffacts. */
   /*==========================*/

   ExpressionInstall(theEnv,temp);
   newDeffacts = get_struct(theEnv,deffacts);
   newDeffacts->header.name = deffactsName;
   IncrementSymbolCount(deffactsName);
   newDeffacts->assertList = PackExpression(theEnv,temp);
   newDeffacts->header.whichModule = (struct defmoduleItemHeader *)
                              GetModuleItem(theEnv,NULL,FindModuleItem(theEnv,"deffacts")->moduleIndex);

   newDeffacts->header.next = NULL;
   newDeffacts->header.usrData = NULL;
   ReturnExpression(theEnv,temp);

   /*=======================================================*/
   /* Save the pretty print representation of the deffacts. */
   /*=======================================================*/

   if (EnvGetConserveMemory(theEnv) == TRUE)
     { newDeffacts->header.ppForm = NULL; }
   else
     { newDeffacts->header.ppForm = CopyPPBuffer(theEnv); }

   /*=============================================*/
   /* Add the deffacts to the appropriate module. */
   /*=============================================*/

   AddConstructToModule(&newDeffacts->header);

#endif /* (! RUN_TIME) && (! BLOAD_ONLY) */

   /*================================================================*/
   /* Return FALSE to indicate the deffacts was successfully parsed. */
   /*================================================================*/

   return(FALSE);
  }

#endif /* DEFFACTS_CONSTRUCT */


