/**   @file  
     @brief  
*/
#pragma once
#ifndef INCLUDED_MANAGERMERISTEM_LE_H
#define INCLUDED_MANAGERMERISTEM_LE_H
//---------------------------------------------------------------------------
/// Manageurs utilises pour le modele EcoMeristem_LE
///
///Description :
///
//----------------------------------------------------------------------------
/** @Author Ludovic TAMBOUR
    @Version 02/10/06 LT v0.0 version initiale; statut : en cours */

 

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
//
// INTERFACE
//
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************




  
#include "SysUtils.h"
  
#include "StrUtils.h"
  
#include "DateUtils.h"
  
#include "ClassTInstance.h"
  
#include "ClassTEntityInstance.h"
  
#include "ClassTProcInstance.h"
  
#include "ClassTModel.h"
  
#include "FunctionString.h"
  
#include "ClassTPort.h"
  
#include "ClassTAttribute.h"
  
#include "ClassTParameter.h"
  
#include "ClassTAttributeTmp.h"
  
#include "ClassTAttributeTable.h"
  
#include "ClassTAttributeTableIn.h"
  
#include "ClassTAttributeTableOut.h"
  
#include "ClassTAttributeTableInOut.h"
  
#include "FunctionTDateTime.h"
  
#include "ClassTProcInstanceInternal.h"
  
#include "ClassTProcInstanceDLL.h"
  
#include "ClassTManagerInternal.h"
  
#include "ClassTManagerDLL.h"
  
#include "UsefullFunctionsForMeristemModel.h"
  
#include "ModuleMeristem.h"
  
#include "ClassTManagerLibrary.h"
  
#include "Dialogs.h"
  
#include "GlobalVariable.h"
  
#include "DefinitionConstant.h"

int LeafManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0);
int EcoMeristemManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0);
int RootManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0);
int InternodeManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0);
int TillerManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0);
int TillerManagerPhytomer_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0);
int MeristemManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0);
int MeristemManagerPhytomers_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0);
int ThermalTimeManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0);
int PanicleManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0);
int PeduncleManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0);

//
// procedures interne non appelée par d'autres fonctions que celle de cette unite
//
void MeristemManagerLeafCreation( TEntityInstance & instance,    TDateTime const& date = 0,    double const& n = 0,    bool const& LL_BL_MODIFIED = false);
void MeristemManagerPhytomerLeafCreation( TEntityInstance & instance,    TDateTime const& date = 0,    double const& n = 0,    bool const& LL_BL_MODIFIED = false);
void MeristemManagerPhytomerLeafActivation( TEntityInstance & instance,    TDateTime const& date = 0,    double const& n = 0);
void MeristemManagerInternodeCreation( TEntityInstance & instance,    TDateTime const& date = 0,   bool isFirstInternode = true,    double const& n = 0,    double const& stock = 0);
void MeristemManagerPhytomerInternodeCreation( TEntityInstance & instance,    TDateTime const& date = 0,   bool isFirstInternode = true,    double const& n = 0,    double const& stock = 0);
void MeristemManagerPanicleCreation( TEntityInstance & instance,    TDateTime const& date = 0);
void MeristemManagerPhytomerPanicleCreation( TEntityInstance & instance,    TDateTime const& date = 0);
void MeristemManagerPeduncleCreation( TEntityInstance & instance,    TDateTime const& date = 0,   bool isFirstInternode = true,    double const& n = 0,    double const& stock = 0);
void MeristemManagerPhytomerPeduncleCreation( TEntityInstance & instance,    TDateTime const& date = 0,   bool isFirstInternode = true,    double const& n = 0,    double const& stock = 0);
void MeristemManagerUpdatePlasto( TEntityInstance & instance);
void MeristemManagerStartInternodeElongation( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0,    bool const& isFirstInternode = false);
void MeristemManagerPhytomerPanicleActivation( TEntityInstance & instance);
void MeristemManagerPhytomerPeduncleActivation( TEntityInstance & instance,   TDateTime const& date);
void TillerManagerLeafCreation( TEntityInstance & instance,   TDateTime const& date,    bool const& IsInitiation = false,    bool const& LL_BL_MODIFIED = false);
void TillerManagerPhytomerLeafCreation( TEntityInstance & instance,   TDateTime const& date,    bool const& IsInitiation = false,    bool const& LL_BL_MODIFIED = false);
void TillerManagerPhytomerLeafActivation( TEntityInstance & instance,   TDateTime const& date);
void TillerManagerInternodeCreation( TEntityInstance & instance,   TDateTime const& date,    bool const& IsFirstDayOfPI = false,    double const& plantStock = 0);
void TillerManagerPhytomerInternodeCreation( TEntityInstance & instance,   TDateTime const& date,    bool const& IsFirstDayOfPI = false,    double const& plantStock = 0);
void TillerManagerPanicleCreation( TEntityInstance & instance,    TDateTime const& date = 0);
void TillerManagerPhytomerPanicleCreation( TEntityInstance & instance,    TDateTime const& date = 0);
void TillerManagerPeduncleCreation( TEntityInstance & instance,   TDateTime const& date,    bool const& IsFirstDayOfPI = false,    double const& plantStock = 0);
void TillerManagerPhytomerPeduncleCreation( TEntityInstance & instance,   TDateTime const& date,    bool const& IsFirstDayOfPI = false,    double const& plantStock = 0);
void TillerManagerCreateFirstPhytomers( TEntityInstance & instance,    TDateTime const& date = 0);
void TillerManagerCreateOthersPhytomers( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0);
void SetAllActiveStateToValue(TEntityInstance instance,  int value);
void TillerManagerCreateFirstLeaf( TEntityInstance & instance,    TDateTime const& date = 0);
void TillerManagerCreateFirstIN( TEntityInstance & instance,    TDateTime const& date = 0);
void TillerManagerCreateOthersLeaf( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0);
void TillerManagerCreateOthersIN( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0);
void TillersTransitionToEndfilling_LE( TEntityInstance & instance,   int const& oldState);
void TillersTransitionToMaturity_LE( TEntityInstance & instance,   int const& oldState);
void TillersTransitionToPre_PI_LE( TEntityInstance & instance,   int const& oldState);
void TillersTransitionToPI_LE( TEntityInstance & instance,   int const& oldState);
void TillersTransitionToState( TEntityInstance & instance,   int const& state,   int const& oldState);
void TillersTransitionToElong( TEntityInstance & instance);
void TillersStockIndividualization( TEntityInstance & instance,   TDateTime const& date);
void TillerManagerPhytomerPanicleActivation( TEntityInstance & instance);
void TillerManagerPhytomerPeduncleActivation( TEntityInstance & instance,   TDateTime const& date);
void TillerManagerStartInternodeElongation( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0);
void RootTransitionToState( TEntityInstance & instance,   int const& state);
void RootTransitionToElong( TEntityInstance & instance);
void RootTransitionToPI( TEntityInstance & instance);
void RootTransitionToFLO( TEntityInstance & instance);
void PanicleTransitionToFLO( TEntityInstance & instance);
void PeduncleTransitionToFLO( TEntityInstance & instance);
void PanicleTransitionToEndFilling( TEntityInstance & instance);
void PeduncleTransitionToEndfilling( TEntityInstance & instance);
void PanicleTransitionToMaturity( TEntityInstance & instance);
void PeduncleTransitionToMaturity( TEntityInstance & instance);
void CreateInitialPhytomers_LE( TEntityInstance & instance,    TDateTime const& date = 0);
void EcoMeristemFirstINCreation( TEntityInstance & instance,    TDateTime const& date = 0);
void EcoMeristemFirstLeafCreation( TEntityInstance & instance,    TDateTime const& date = 0);
void CreateFirstPhytomer_LE( TEntityInstance & instance,    TDateTime const& date = 0);
void EcoMeristemInitialOtherINCreation( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0);
void EcoMeristemInitialOtherLeafCreation( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0);
void CreateOtherPhytomers_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0);
void StoreThermalTimeAtPI( TEntityInstance & instance,    TDateTime const& date = 0);
void StorePhenostageAtPre_Flo( TEntityInstance & instance,    TDateTime const& date = 0,    double const& phenostage = 0);
void SetAllInstanceToEndFilling( TEntityInstance & instance);
void TillerStorePhenostageAtPre_Flo( TEntityInstance & instance,    TDateTime const& date = 0,    double const& phenostage = 0);
void TillerStorePhenostageAtPI( TEntityInstance & instance,    TDateTime const& date = 0,    double const& phenostage = 0);
std::string FindLastLigulatedLeafNumberOnCulm( TEntityInstance & instance);
double FindLeafOnSameRankWidth( TEntityInstance & instance,  std::string rank);
double FindLeafOnSameRankLength( TEntityInstance & instance,  std::string rank);
TEntityInstance FindLeafAtRank( TEntityInstance & instance,  std::string rank);
TEntityInstance FindInternodeAtRank( TEntityInstance & instance,  std::string rank);
TEntityInstance FindPeduncle( TEntityInstance & instance);
TEntityInstance FindPanicle( TEntityInstance & instance);

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
//
// IMPLEMENTATION
//
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************



#endif//INCLUDED_MANAGERMERISTEM_LE_H
//END
