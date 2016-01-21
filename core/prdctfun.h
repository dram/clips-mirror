   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  01/06/16             */
   /*                                                     */
   /*            PREDICATE FUNCTIONS HEADER FILE          */
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
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Support for long long integers.                */
/*                                                           */
/*            Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW and       */
/*            MAC_MCW).                                      */
/*                                                           */
/*************************************************************/

#ifndef _H_prdctfun

#pragma once

#define _H_prdctfun

   void                           PredicateFunctionDefinitions(void *);
   bool                           EqFunction(void *);
   bool                           NeqFunction(void *);
   void                           StringpFunction(UDFContext *,CLIPSValue *);
   void                           SymbolpFunction(UDFContext *,CLIPSValue *);
   void                           LexemepFunction(UDFContext *,CLIPSValue *);
   void                           NumberpFunction(UDFContext *,CLIPSValue *);
   void                           FloatpFunction(UDFContext *,CLIPSValue *);
   void                           IntegerpFunction(UDFContext *,CLIPSValue *);
   void                           MultifieldpFunction(UDFContext *,CLIPSValue *);
   void                           PointerpFunction(UDFContext *,CLIPSValue *);
   bool                           NotFunction(void *);
   bool                           AndFunction(void *);
   bool                           OrFunction(void *);
   bool                           LessThanOrEqualFunction(void *);
   bool                           GreaterThanOrEqualFunction(void *);
   bool                           LessThanFunction(void *);
   bool                           GreaterThanFunction(void *);
   bool                           NumericEqualFunction(void *);
   bool                           NumericNotEqualFunction(void *);
   void                           OddpFunction(UDFContext *,CLIPSValue *);
   void                           EvenpFunction(UDFContext *,CLIPSValue *);

#endif /* _H_prdctfun */



