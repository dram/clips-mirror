#pragma once

#include "clipscpp.h"
#include "CLIPSNET_Values.h"
#include "CLIPSNET_Router.h"

using namespace System;
using namespace CLIPS;

namespace CLIPSNET
  {
   /*###################*/
   /* Environment class */
   /*###################*/

   public ref class Environment 
     {
      public:
        Environment();
        ~Environment();

        void CommandLoop();
        long long Run();
        long long Run(long long);
        void Reset();
        bool Build(String ^);
        bool GetHaltExecution();
        void SetHaltExecution(bool);
        bool GetHaltRules();
        void SetHaltRules(bool);
        bool GetEvaluationError();
        void SetEvaluationError(bool);
        int Load(String ^);
        void LoadFromString(String ^);
        bool LoadFromResource(String ^,String ^);
        PrimitiveValue ^ Eval(String ^);
        void AddRouter(String ^,int ,Router ^);
        FactAddressValue ^ AssertString(String ^);
        size_t InputBufferCount();
        virtual String^ ToString() override;

      protected:
        !Environment();

      private:
        CLIPSCPPEnv *m_Env;
     };
  };