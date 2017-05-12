/**   @file  
     @brief  
*/
#include "ManagerMeristem_LE.h"




  
#include "EntityLeaf_LE.h" 
#include "EntityInternode_LE.h" 
#include "EntityPanicle_LE.h" 
#include "EntityPeduncle_LE.h"

// ----------------------------------------------------------------------------
//  fonction LeafManager_LE
//  ------------------------
//
/// gere le comportement d'une feuille
///
/// description : TODO
///
///
/// @param instance instance d'entité portant le manageur
/// @param date date actuelle de simulation  (par défaut, MIN_DATE)
/// @param state état actuel de l'instance portant le manageur (par défaut, 0)
/// @return nouvelle etat
// ---------------------------------------------------------------------------

int LeafManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0)
{   int result;
  static double/*?*/ const PREDIM = 1;
  static double/*?*/ const TRANSITION = 10;
  static double/*?*/ const TRANSITION_TO_LIGULE = 20;
  static double/*?*/ const REALIZATION_WITH_STOCK = 2;
  static double/*?*/ const REALIZATION_WITHOUT_STOCK = 3;
  static double/*?*/ const LIGULE_WITH_STOCK = 4;
  static double/*?*/ const LIGULE_WITHOUT_STOCK = 5;
  static double/*?*/ const ENDFILLING = 12;
  static double/*?*/ const DEAD_BY_SENESCENCE = 500;
  static double/*?*/ const DEAD = 1000;
  static double/*?*/ const VEGETATIVE = 2000;
  static double/*?*/ const UNDEFINED = -1;

   int newState;
   double stock;
   std::string newStateStr; std::string oldStateStr;


   switch( state ) {
    case VEGETATIVE: 
      {
        oldStateStr = "VEGETATIVE";
        newState = VEGETATIVE;
        newStateStr = "VEGETATIVE";
      }

    // ETAT PREDIMENSIONNEMENT :
    // -----------------------
    case PREDIM: 
      {
        oldStateStr = "PREDIM";
        newState = TRANSITION;
        newStateStr = "TRANSITION";
      }

    // ETAT TRANSITION : (evite que etat 1 et 2 soit fait pendant le meme pas de temps)
    // ---------------
    case TRANSITION: 
      {
        oldStateStr = "TRANSITION";
        stock = instance.GetTAttribute( "stock" ).GetSample( date ).value;
        if(  ( stock > 0 ) )
        {
          // ETAT STOCK DISPONIBLE
          newState = REALIZATION_WITH_STOCK;
          newStateStr = "REALIZATION_WITH_STOCK";
        }
        else
        {
          // ETAT STOCK INDISPONIBLE
        newState = REALIZATION_WITHOUT_STOCK;
        newStateStr = "REALIZATION_WITHOUT_STOCK";
        }
      }

    // ETAT REALIZATION et STOCK DISPONIBLE
    // ------------------------------------
    case REALIZATION_WITH_STOCK: 
      {
        oldStateStr = "REALIZATION_WITH_STOCK";
        stock = instance.GetTAttribute( "stock" ).GetSample( date ).value;

        if(  ( stock > 0 ) )
        {
          // ETAT REALIZATION et STOCK DISPONIBLE
          newState = REALIZATION_WITH_STOCK;
          newStateStr = "REALIZATION_WITH_STOCK";
        }
        else
        {
          // ETAT REALIZATION et STOCK INDISPONIBLE
          newState = REALIZATION_WITHOUT_STOCK;
          newStateStr = "REALIZATION_WITHOUT_STOCK";
        }
      }

    // ETAT REALIZATION et STOCK INDISPONIBLE
    // ------------------------------------
    case REALIZATION_WITHOUT_STOCK: 
      {
        oldStateStr = "REALIZATION_WITHOUT_STOCK";
        stock = instance.GetTAttribute( "stock" ).GetSample( date ).value;

        if(  ( stock > 0 ) )
        {
          // ETAT REALIZATION et STOCK DISPONIBLE
          newState = REALIZATION_WITH_STOCK;
          newStateStr = "REALIZATION_WITH_STOCK";
        }
        else
        {
          // ETAT REALIZATION et STOCK INDISPONIBLE
          newState = REALIZATION_WITHOUT_STOCK;
          newStateStr = "REALIZATION_WITHOUT_STOCK";
        }
      }

    // ETAT TRANSITION_TO_LIGULE
    // -------------------------

    case TRANSITION_TO_LIGULE: 
      {
        oldStateStr = "TRANSITION_TO_LIGULE";
        stock = instance.GetTAttribute( "stock" ).GetSample( date ).value;

        if(  ( stock > 0 ) )
        {
          newState = LIGULE_WITH_STOCK;
          newStateStr = "LIGULE_WITH_STOCK";
        }
        else
        {
          newState = LIGULE_WITHOUT_STOCK;
          newStateStr = "LIGULE_WITHOUT_STOCK";
        }  
      }

    // ETAT LIGULE et STOCK DISPONIBLE
    // ------------------------------------
    case LIGULE_WITH_STOCK: 
      {
        oldStateStr = "LIGULE_WITH_STOCK";
        stock = instance.GetTAttribute( "stock" ).GetSample( date ).value;

        if(  ( stock > 0 ) )
        {
          // ETAT LIGULE et STOCK DISPONIBLE
          newState = LIGULE_WITH_STOCK;
          newStateStr = "LIGULE_WITH_STOCK";
        }
        else
        {
          // ETAT LIGULE et STOCK INDISPONIBLE
          newState = LIGULE_WITHOUT_STOCK;
          newStateStr = "LIGULE_WITHOUT_STOCK";
        }
      }

    // ETAT LIGULE et STOCK INDISPONIBLE
    // ------------------------------------
    case LIGULE_WITHOUT_STOCK: 
      {
        oldStateStr = "LIGULE_WITHOUT_STOCK";
        stock = instance.GetTAttribute( "stock" ).GetSample( date ).value;

        if(  ( stock > 0 ) )
        {
          // ETAT LIGULE et STOCK DISPONIBLE
          newState = LIGULE_WITH_STOCK;
          newStateStr = "LIGULE_WITH_STOCK";
        }
        else
        {
          // ETAT LIGULE et STOCK INDISPONIBLE
          newState = LIGULE_WITHOUT_STOCK;
          newStateStr = "LIGULE_WITHOUT_STOCK";
        }
      }

    // ETAT MORT
    // ---------
    case DEAD: 
      {
        oldStateStr = "DEAD";
        newState = DEAD;
        newStateStr = "DEAD";
      }

    // ETAT MORT PAR SENESCENCE
    // ------------------------
    case DEAD_BY_SENESCENCE: 
      {
        oldStateStr = "DEAD_BY_SENESCENCE";
        newState = DEAD_BY_SENESCENCE;
        newStateStr = "DEAD_BY_SENESCENCE";
      }

    case ENDFILLING: 
      {
        oldStateStr = "ENDFILLING";
        newState = ENDFILLING;
        newStateStr = "ENDFILLING";
      }

      // ETAT NON DEFINIT :
      // ----------------
      default:
      {
        result = UNDEFINED;
        return result;
      }
    }

  // retourne l'état (pas de modification d'état)
  SRwriteln( "leaf, etat de : " + oldStateStr + " --> " + newStateStr );
  result = newState;
return result;
}


// ----------------------------------------------------------------------------
//  fonction RootManager_LE
//  ------------------------
//
/// gere le comportement des racines
///
/// description : TODO
///
///
/// @param instance instance d'entité portant le manageur
/// @param date date actuelle de simulation  (par défaut, MIN_DATE)
/// @param state état actuel de l'instance portant le manageur (par défaut, 0)
/// @return nouvelle etat
// ---------------------------------------------------------------------------

int RootManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0)
{   int result;
  static double/*?*/ const INIT = 1;
  static double/*?*/ const STOCK_AVAILABLE = 2;
  static double/*?*/ const NO_STOCK = 3;
  static double/*?*/ const PI_STOCK_AVAILABLE = 4;
  static double/*?*/ const PI_NO_STOCK = 5;
  static double/*?*/ const FLO = 6;
  static double/*?*/ const ELONG_STOCK_AVAILABLE = 9;
  static double/*?*/ const ELONG_NO_STOCK = 10;
  static double/*?*/ const ENDFILLING = 12;
  static double/*?*/ const DEAD = 1000;

   int newState;
   double stock;
   std::string newStateStr; std::string oldStateStr;


  newState = 0;

  if(  ( state != DEAD ) )
  {

    stock = instance.GetTAttribute( "stock" ).GetSample( date ).value;

    SRwriteln( "stock = " + floattostr( stock ) );

     switch( state ) {
      case INIT: 
      {
        oldStateStr = "INIT";
        newState = STOCK_AVAILABLE;
        newStateStr = "STOCK_AVAILABLE";
      }
      
      case STOCK_AVAILABLE: 
      {
        oldStateStr = "STOCK_AVAILABLE";
        if(  ( stock > 0 ) )
        {
          // ETAT STOCK DISPONIBLE
          // ---------------------
          newState = STOCK_AVAILABLE;
          newStateStr = "STOCK_AVAILABLE";
        }
        else
        {
          // ETAT STOCK INDISPONIBLE
          // -----------------------
          newState = NO_STOCK;
          newStateStr = "NO_STOCK";
        }
      }

      case NO_STOCK: 
      {
        oldStateStr = "NO_STOCK";
        if(  ( stock > 0 ) )
        {
          // ETAT STOCK DISPONIBLE
          // ---------------------
          newState = STOCK_AVAILABLE;
          newStateStr = "STOCK_AVAILABLE";
        }
        else
        {
          // ETAT STOCK INDISPONIBLE
          // -----------------------
          newState = NO_STOCK;
          newStateStr = "NO_STOCK";
        }
      }

      case PI_STOCK_AVAILABLE: 
      {
        oldStateStr = "PI_STOCK_AVAILABLE";
        if(  ( stock > 0 ) )
        {
          // ETAT STOCK DISPONIBLE
          // ---------------------
          newState = PI_STOCK_AVAILABLE;
          newStateStr = "PI_STOCK_AVAILABLE";
        }
        else
        {
          // ETAT STOCK INDISPONIBLE
          // -----------------------
          newState = PI_NO_STOCK;
          newStateStr = "PI_NO_STOCK";
        }
      }

      case PI_NO_STOCK: 
      {
        oldStateStr = "PI_NO_STOCK";
        if(  ( stock > 0 ) )
        {
          // ETAT STOCK DISPONIBLE
          // ---------------------
          newState = PI_STOCK_AVAILABLE;
          newStateStr = "PI_STOCK_AVAILABLE";
        }
        else
        {
          // ETAT STOCK INDISPONIBLE
          // -----------------------
          newState = PI_NO_STOCK;
          newStateStr = "PI_NO_STOCK";
        }
      }

      case FLO: 
      {
        oldStateStr = "FLO";
        newState = FLO;
        newStateStr = "FLO";
      }

      case ELONG_STOCK_AVAILABLE: 
      {
        oldStateStr = "ELONG_STOCK_AVAILABLE";
        if(  ( stock > 0 ) )
        {
          newState = ELONG_STOCK_AVAILABLE;
          newStateStr = "ELONG_STOCK_AVAILABLE";
        }
        else
        {
          newState = ELONG_NO_STOCK;
          newStateStr = "ELONG_NO_STOCK";
        }
      }

      case ELONG_NO_STOCK: 
      {
        oldStateStr = "ELONG_NO_STOCK";
        if(  ( stock > 0 ) )
        {
          newState = ELONG_STOCK_AVAILABLE;
          newStateStr = "ELONG_STOCK_AVAILABLE";
        }
        else
        {
          newState = ELONG_NO_STOCK;
          newStateStr = "ELONG_NO_STOCK";
        }
      }

      case ENDFILLING: 
      {
        oldStateStr = "ENDFILLING";
        newState = ENDFILLING;
        newStateStr = "ENDFILLING";
      }
    }
  }
  else
  {
    newState = DEAD;
    newStateStr = "DEAD";
  }

  // retourne le nouvel état (pas de modification d'état)
  SRwriteln( "root, etat " + oldStateStr + " --> " + newStateStr );
  result = newState;
return result;
}


// ----------------------------------------------------------------------------
//  fonction InternodeManager_LE
//  ------------------------
//
/// gere le comportement des inter-noeuds
///
/// description : TODO
///
///
/// @param instance instance d'entité portant le manageur
/// @param date date actuelle de simulation  (par défaut, MIN_DATE)
/// @param state état actuel de l'instance portant le manageur (par défaut, 0)
/// @return nouvelle etat
// ---------------------------------------------------------------------------

int InternodeManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0)
{   int result;
  static double/*?*/ const PREDIM = 1;
  static double/*?*/ const TRANSITION = 10;
  static double/*?*/ const REALIZATION = 2;
  static double/*?*/ const REALIZATION_NOSTOCK = 3;
  static double/*?*/ const MATURITY = 4;
  static double/*?*/ const MATURITY_NOSTOCK = 5;
  static double/*?*/ const ENDFILLING = 12;
  static double/*?*/ const UNKNOWN = -1;
  static double/*?*/ const DEAD = 1000;
  static double/*?*/ const VEGETATIVE = 2000;

   int newState;
   double stock;
   std::string name; std::string oldStateStr; std::string newStateStr;

  name = instance.GetName(  );
  if(  ( AnsiContainsStr( name , "T" ) ) )
  {
    SRwriteln( "Sur talle" );
    stock = ( instance.GetFather(  ).GetFather(  ) as TEntityInstance ).GetTAttribute( "stock" ).GetSample( date ).value;}
  else
  {
    SRwriteln( "Mainstem" );
    stock = ( instance.GetFather(  ) as TEntityInstance ).GetTAttribute( "stock" ).GetSample( date ).value;}
  SRwriteln( "stock --> " + floattostr( stock ) );
   switch( state ) {
    case VEGETATIVE: 
      {
        oldStateStr = "VEGETATIVE";
        newState = VEGETATIVE;
        newStateStr = "VEGETATIVE";
      }

    // ETAT PREDIMENSIONNEMENT :
    // -----------------------
    case PREDIM: 
      {
        oldStateStr = "PREDIM";
        newState = TRANSITION;
        newStateStr = "TRANSITION";
      }

    // ETAT TRANSITION : (evite que etat 1 et 2 soit fait pendant le meme pas de temps)
    // ---------------
    case TRANSITION: 
      {
        oldStateStr = "TRANSITION";
        if(  ( stock == 0 ) )
        {
          newState = REALIZATION_NOSTOCK;
          newStateStr = "REALIZATION_NOSTOCK";
        }
        else
        {
          newState = REALIZATION;
          newStateStr = "REALIZATION";
        }
      }

    // ETAT REALIZATION
    // ----------------
    case REALIZATION: 
      {
        oldStateStr = "REALIZATION";
        if(  ( stock == 0 ) )
        {
          newState = REALIZATION_NOSTOCK;
          newStateStr = "REALIZATION_NOSTOCK";
        }
        else
        {
          newState = REALIZATION;
          newStateStr = "REALIZATION";
        }
      }

    // ETAT REALIZATION_NOSTOCK
    // ------------------------
    case REALIZATION_NOSTOCK: 
      {
        oldStateStr = "REALIZATION_NOSTOCK";
        if(  ( stock == 0 ) )
        {
          newState = REALIZATION_NOSTOCK;
          newStateStr = "REALIZATION_NOSTOCK";
        }
        else
        {
          newState = REALIZATION;
          newStateStr = "REALIZATION";
        }
      }

    // ETAT LIGULE
    // -----------
    case MATURITY: 
      {
        oldStateStr = "MATURITY";
        if(  ( stock == 0 ) )
        {
          newState = MATURITY_NOSTOCK;
          newStateStr = "MATURITY_NOSTOCK";
        }
        else
        {
          newState = MATURITY;
          newStateStr = "MATURITY";
        }
      }

    // ETAT LIGULE SANS STOCK
    // ----------------------
    case MATURITY_NOSTOCK: 
      {
        oldStateStr = "MATURITY_NOSTOCK";
        if(  ( stock == 0 ) )
        {
          newState = MATURITY_NOSTOCK;
          newStateStr = "MATURITY_NOSTOCK";
        }
        else
        {
          newState = MATURITY;
          newStateStr = "MATURITY";
        }
      }

    case DEAD: 
      {
        oldStateStr = "DEAD";
        newState = DEAD;
        newStateStr = "DEAD";
      }

    case ENDFILLING: 
      {
        oldStateStr = "ENDFILLING";
        newState = ENDFILLING;
        newStateStr = "ENDFILLING";
      }

      // ETAT NON DEFINIT :
      // ----------------
      default:
      {
        newState = UNKNOWN;
      }
    }

  // retourne l'état (pas de modification d'état)
  SRwriteln( "**** Internode --> Current State " + oldStateStr + " --> " + newStateStr + " ***" );
  result = newState;
return result;
}

void TillerManagerLeafCreation( TEntityInstance & instance,   TDateTime const& date,    bool const& IsInitiation = false,    bool const& LL_BL_MODIFIED = false)
{
  Tsample sample;
  std::string name;
  TEntityInstance refMeristem;
  double Lef1; double MGR; double plasto; double ligulo; double WLR; double LL_BL; double allo_area; double G_L; double Tb; double resp_LER; double coeffLifespan; double mu;
  double LL_BL_New; double phenoStageAtPI; double currentPhenoStage;
  double slope_LL_BL_at_PI;
  TEntityInstance newLeaf;
  TAttribute attributeTmp;
  int exeOrder;
//  pos : Integer;
  int rank;
  std::string previousLeafPredimName; std::string currentLeafPredimName;

  rank = 0;
//  pos := -1;
  if(  IsInitiation )
  {
    // creation d'une premiere feuille
    sample.date = date; sample.value = 1;
    instance.GetTAttribute( "leafNb" ).SetSample( sample );
    name = "L1_" + instance.GetName(  );
    exeOrder = 2000;
//    pos := 1;
  }
  else
  {
    // creation des autres feuilles
    rank = FindGreatestSuffixforASpecifiedCategory( instance, "Leaf" ) + 1;
    name = "L" + floattostr( rank ) + "_" + instance.GetName(  );
    exeOrder = FindFirstEmptyPlaceAfterASpecifiedCategory( instance, "Leaf" );
//    pos := 0;
  }
  refMeristem = instance.GetFather(  ) as TEntityInstance;
  Lef1 = refMeristem.GetTAttribute( "Lef1" ).GetSample( date ).value;
  MGR = refMeristem.GetTAttribute( "MGR" ).GetSample( date ).value;
  plasto = refMeristem.GetTAttribute( "plasto" ).GetSample( date ).value;
  ligulo = refMeristem.GetTAttribute( "ligulo" ).GetSample( date ).value;
  WLR = refMeristem.GetTAttribute( "WLR" ).GetSample( date ).value;
  LL_BL = refMeristem.GetTAttribute( "LL_BL" ).GetSample( date ).value;
  allo_area = refMeristem.GetTAttribute( "allo_area" ).GetSample( date ).value;
  G_L = refMeristem.GetTAttribute( "G_L" ).GetSample( date ).value;
  Tb = refMeristem.GetTAttribute( "Tb" ).GetSample( date ).value;
  resp_LER = refMeristem.GetTAttribute( "resp_LER" ).GetSample( date ).value;
  phenoStageAtPI = instance.GetTAttribute( "phenoStageAtPI" ).GetSample( date ).value;
  currentPhenoStage = instance.GetTAttribute( "phenoStage" ).GetSample( date ).value;
  coeffLifespan = refMeristem.GetTAttribute( "coeff_lifespan" ).GetSample( date ).value;
  mu = refMeristem.GetTAttribute( "mu" ).GetSample( date ).value;

  SRwriteln( "****  creation de la feuille : " + name + "  ****" );

  slope_LL_BL_at_PI = refMeristem.GetTAttribute( "slope_LL_BL_at_PI" ).GetSample( date ).value;
  if(  LL_BL_MODIFIED )
    LL_BL_New = LL_BL + slope_LL_BL_at_PI * ( currentPhenoStage - phenoStageAtPI );else
    LL_BL_New = LL_BL;

  newLeaf = ImportLeaf_LE( name , Lef1 , MGR , plasto , ligulo , WLR , LL_BL_New , allo_area , G_L , Tb , resp_LER , coeffLifespan , mu , 0 );

  // determination de l'ordre d'execution de newLeaf :
  // recherche de la premiere place vide apres la premiere entité de type 'Leaf'
  newLeaf.SetExeOrder( exeOrder );
  newLeaf.SetStartDate( date );
  newLeaf.InitCreationDate( date );
  newLeaf.InitNextDate(  );
  instance.AddTInstance( newLeaf );

  if(  IsInitiation )
  {
    // connection port <-> attribut pour newLeaf
    attributeTmp = TAttributeTmp.Create( "predim_L1" );
    instance.AddTAttribute( attributeTmp );

    newLeaf.ExternalConnect( [ "zero"
                             , "predimLeafOnMainstem"
                             , "predim_L1"
                             , "degreeDayForLeafInitiation"
                             , "degreeDayForLeafRealization"
                             , "testIc"
                             , "fcstr"
                             , "phenoStage"
                             , "SLA"
                             , "plantStock"
                             , "Tair"
                             , "plasto_delay"
                             , "thresLER"
                             , "slopeLER"
                             , "FTSW"
                             , "lig"
                             , "P" ] );
  }
  else
  {
    previousLeafPredimName = "predim_L" + floattostr( rank-1 );
    currentLeafPredimName = "predim_L" + floattostr( rank );

    attributeTmp = TAttributeTmp.Create( currentLeafPredimName );
    instance.AddTAttribute( attributeTmp );

    newLeaf.ExternalConnect( [ previousLeafPredimName
                             , "predimLeafOnMainstem"
                             , currentLeafPredimName
                             , "degreeDayForLeafInitiation"
                             , "degreeDayForLeafRealization"
                             , "testIc"
                             , "fcstr"
                             , "phenoStage"
                             , "SLA"
                             , "plantStock"
                             , "Tair"
                             , "plasto_delay"
                             , "thresLER"
                             , "slopeLER"
                             , "FTSW"
                             , "lig"
                             , "P" ] );

    sample = instance.GetTAttribute( "leafNb" ).GetCurrentSample(  );
    sample.value = sample.value+1; sample.date = date;
    instance.GetTAttribute( "leafNb" ).SetSample( sample );
  }
  instance.SortTInstance(  );
}

void TillerManagerPhytomerLeafActivation( TEntityInstance & instance,   TDateTime const& date)
{
  TEntityInstance refMeristem; TEntityInstance entityLeaf;
  double activeLeavesNb; double Lef1; double MGR; double plasto; double ligulo; double WLR; double LL_BL; double allo_area; double G_L; double resp_LER; double Tb; double coeffLifespan; double mu;
  Tsample sample;

  refMeristem = instance.GetFather(  ) as TEntityInstance;
  activeLeavesNb = instance.GetTAttribute( "activeLeavesNb" ).GetSample( date ).value;

  SRwriteln( "activeLeavesNb --> " + floattostr( activeLeavesNb ) );

  entityLeaf = FindLeafAtRank( instance , floattostr( activeLeavesNb + 1 ) );
  activeLeavesNb = activeLeavesNb + 1;
  sample.date = date;
  sample.value = activeLeavesNb;
  instance.GetTAttribute( "activeLeavesNb" ).SetSample( sample );

  SRwriteln( "La feuille : " + entityLeaf.GetName(  ) + " est activée" );

  Lef1 = refMeristem.GetTAttribute( "Lef1" ).GetSample( date ).value;
  MGR = refMeristem.GetTAttribute( "MGR" ).GetSample( date ).value;
  plasto = refMeristem.GetTAttribute( "plasto" ).GetSample( date ).value;
  ligulo = refMeristem.GetTAttribute( "ligulo" ).GetSample( date ).value;
  WLR = refMeristem.GetTAttribute( "WLR" ).GetSample( date ).value;
  LL_BL = refMeristem.GetTAttribute( "LL_BL" ).GetSample( date ).value;
  allo_area = refMeristem.GetTAttribute( "allo_area" ).GetSample( date ).value;
  G_L = refMeristem.GetTAttribute( "G_L" ).GetSample( date ).value;
  Tb = refMeristem.GetTAttribute( "Tb" ).GetSample( date ).value;
  resp_LER = refMeristem.GetTAttribute( "resp_LER" ).GetSample( date ).value;
  coeffLifespan = refMeristem.GetTAttribute( "coeff_lifespan" ).GetSample( date ).value;
  mu = refMeristem.GetTAttribute( "mu" ).GetSample( date ).value;

  sample = entityLeaf.GetTAttribute( "Lef1" ).GetSample( date );
  sample.value = Lef1;
  entityLeaf.GetTAttribute( "Lef1" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "rank" ).GetSample( date );
  sample.value = activeLeavesNb;
  entityLeaf.GetTAttribute( "rank" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "MGR" ).GetSample( date );
  sample.value = MGR;
  entityLeaf.GetTAttribute( "MGR" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "plasto" ).GetSample( date );
  sample.value = plasto;
  entityLeaf.GetTAttribute( "plasto" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "ligulo" ).GetSample( date );
  sample.value = ligulo;
  entityLeaf.GetTAttribute( "ligulo" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "WLR" ).GetSample( date );
  sample.value = WLR;
  entityLeaf.GetTAttribute( "WLR" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "allo_area" ).GetSample( date );
  sample.value = allo_area;
  entityLeaf.GetTAttribute( "allo_area" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "G_L" ).GetSample( date );
  sample.value = G_L;
  entityLeaf.GetTAttribute( "G_L" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "Tb" ).GetSample( date );
  sample.value = Tb;
  entityLeaf.GetTAttribute( "Tb" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "resp_LER" ).GetSample( date );
  sample.value = resp_LER;
  entityLeaf.GetTAttribute( "resp_LER" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "LL_BL" ).GetSample( date );
  sample.value = LL_BL;
  entityLeaf.GetTAttribute( "LL_BL" ).SetSample( sample );

  sample = entityLeaf.GetTAttribute( "coeffLifespan" ).GetSample( date );
  sample.value = coeffLifespan;
  entityLeaf.GetTAttribute( "coeffLifespan" ).SetSample( sample );

  sample = entityLeaf.GetTAttribute( "mu" ).GetSample( date );
  sample.value = mu;
  entityLeaf.GetTAttribute( "mu" ).SetSample( sample );


  entityLeaf.SetCurrentState( 1 );

  entityLeaf.SetStartDate( date );
  entityLeaf.InitCreationDate( date );
  entityLeaf.InitNextDate(  );


}

void TillerManagerPhytomerLeafCreation( TEntityInstance & instance,   TDateTime const& date,    bool const& IsInitiation = false,    bool const& LL_BL_MODIFIED = false)
{
  TEntityInstance refMeristem; TEntityInstance newLeaf;
  double nb_leaf_max_after_PI;
  double activeLeavesNb; double leafNb;
  Tsample sample;
  TAttribute attributeTmp;
  int exeOrder;
  std::string name;
  std::string previousLeafPredimName; std::string currentLeafPredimName;

  refMeristem = instance.GetFather(  ) as TEntityInstance;

  TillerManagerPhytomerLeafActivation( instance , date );

  activeLeavesNb = instance.GetTAttribute( "activeLeavesNb" ).GetSample( date ).value;
  nb_leaf_max_after_PI = refMeristem.GetTAttribute( "nb_leaf_max_after_PI" ).GetSample( date ).value;

  leafNb = instance.GetTAttribute( "leafNb" ).GetSample( date ).value;
  leafNb = leafNb + 1;
  sample.date = date;
  sample.value = leafNb;

  name = "L" + floattostr( activeLeavesNb + nb_leaf_max_after_PI ) + "_" + instance.GetName(  );

  SRwriteln( "****  creation de la feuille : " + name + "  ****" );
  newLeaf = ImportLeaf_LE( name , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );

  instance.GetTAttribute( "leafNb" ).SetSample( sample );

  // determination de l'ordre d'execution de newLeaf :
  exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfFirstPhytomer" ).GetSample( date ).value + ( 2 * ( activeLeavesNb + nb_leaf_max_after_PI ) ) - 1 );
  SRwriteln( "ExeOrder : " + IntToStr( exeOrder ) );


  newLeaf.SetExeOrder( exeOrder );
  newLeaf.SetStartDate( MAX_DATE );
  newLeaf.InitCreationDate( MAX_DATE );
  newLeaf.InitNextDate(  );
  newLeaf.SetCurrentState( 2000 );
  instance.AddTInstance( newLeaf );

  // connection port <-> attribut pour newLeaf
  previousLeafPredimName = "predim_L" + floattostr( ( activeLeavesNb + nb_leaf_max_after_PI ) - 1 );
  currentLeafPredimName = "predim_L" + floattostr( activeLeavesNb + nb_leaf_max_after_PI );

  attributeTmp = TAttributeTmp.Create( currentLeafPredimName );
  instance.AddTAttribute( attributeTmp );

  newLeaf.ExternalConnect( [ previousLeafPredimName
                           , "predimLeafOnMainstem"
                           , currentLeafPredimName
                           , "degreeDayForLeafInitiation"
                           , "degreeDayForLeafRealization"
                           , "testIc"
                           , "fcstr"
                           , "phenoStage"
                           , "SLA"
                           , "plantStock"
                           , "Tair"
                           , "plasto_delay"
                           , "thresLER"
                           , "slopeLER"
                           , "FTSW"
                           , "lig"
                           , "P" ] );
  instance.SortTInstance(  );
}

void TillerManagerInternodeCreation( TEntityInstance & instance,   TDateTime const& date,    bool const& IsFirstDayOfPI = false,    double const& plantStock = 0)
{
  double sumOfTillerLeafBiomass; double SumOfPlantBiomass;
  Tsample sample;
  int exeOrder; int pos;
  int internodeNumber;
//  rank : Double;
  std::string name;
  double TT;
  TAttribute TT_PI_Attribute;
//  attributeTmp : TAttribute;
  TEntityInstance refMeristem;
  TEntityInstance newInternode;
  double MGR; double plasto; double Tb; double resp_LER; double resp_INER; double LIN1; double IN_A; double IN_B; double density_IN;
  double MaximumReserveInInternode;
  double leaf_width_to_IN_diameter; double leaf_length_to_IN_length;
  double leafLength; double leafWidth;
//  previousInternodePredimName, currentInternodePredimName : String;
//  sampleTmp : TSample;

//  i : integer;


  refMeristem = instance.GetFather(  ) as TEntityInstance;
  if(  IsFirstDayOfPI )
  {
    sumOfTillerLeafBiomass = instance.GetTAttribute( "sumOfTillerLeafBiomass" ).GetSample( date ).value;
    SumOfPlantBiomass = refMeristem.GetTAttribute( "sumOfLeafBiomass" ).GetSample( date ).value;
    sample = instance.GetTAttribute( "stock_tiller" ).GetSample( date );
    sample.value = ( sumOfTillerLeafBiomass *1.0/ SumOfPlantBiomass ) * plantStock;
    instance.GetTAttribute( "stock_tiller" ).SetSample( sample );

    sample = instance.GetTAttribute( "isFirstDayOfPi" ).GetSample( date );
    sample.value = 0;
    instance.GetTAttribute( "isFirstDayOfPi" ).SetSample( sample );

    exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfFirstInternode" ).GetSample( date ).value );
    SRwriteln( "exe order: " + IntToStr( exeOrder ) );

    //
    // les entrenoeuds seront creees de la même manière que sur le brin maitre
    // -----------------------------------------------------------------


    ////////////////////////////////////////////////////////////////
    // On récupère la valeur de TT et on la stocke dans Tiller_TT_PI
    ////////////////////////////////////////////////////////////////

    TT = refMeristem.GetTAttribute( "TT" ).GetSample( date ).value;
    TT_PI_Attribute = refMeristem.GetTAttribute( "TT_PI" );
    sample = TT_PI_Attribute.GetSample( date );
    sample.value = TT;
    TT_PI_Attribute.SetSample( sample );
    SRwriteln( "TT_PI : " + floattostr( sample.value ) );

    pos = 1;
  }
  else
  {
    exeOrder = FindFirstEmptyPlaceAfterASpecifiedCategory( instance, "Internode" );
    pos = 0;
  }
//  rank := instance.GetTAttribute('leafNb').GetSample(date).value - 1;
  internodeNumber = StrToInt( FindLastLigulatedLeafNumberOnCulm( instance ) );
  name = "IN" + IntToStr( internodeNumber ) + "_" + instance.GetName(  );
  leafLength = FindLeafOnSameRankLength( instance , IntToStr( internodeNumber ) );
  leafWidth = FindLeafOnSameRankWidth( instance , IntToStr( internodeNumber ) );
  MGR = refMeristem.GetTAttribute( "MGR" ).GetSample( date ).value;
  plasto = refMeristem.GetTAttribute( "plasto" ).GetSample( date ).value;
  Tb = refMeristem.GetTAttribute( "Tb" ).GetSample( date ).value;
  resp_LER = refMeristem.GetTAttribute( "resp_LER" ).GetSample( date ).value;
  resp_INER = resp_LER;
  LIN1 = refMeristem.GetTAttribute( "LIN1" ).GetSample( date ).value;
  IN_A = refMeristem.GetTAttribute( "IN_A" ).GetSample( date ).value;
  IN_B = refMeristem.GetTAttribute( "IN_B" ).GetSample( date ).value;
  density_IN = refMeristem.GetTAttribute( "density_IN" ).GetSample( date ).value;
  MaximumReserveInInternode = refMeristem.GetTAttribute( "maximumReserveInInternode" ).GetSample( date ).value;
  leaf_width_to_IN_diameter = refMeristem.GetTAttribute( "leaf_width_to_IN_diameter" ).GetSample( date ).value;
  leaf_length_to_IN_length = refMeristem.GetTAttribute( "leaf_length_to_IN_length" ).GetSample( date ).value;


  // on crée le premier entre noeud

  SRwriteln( "****  creation de l\'entrenoeud : " + name + "  ****" );

  newInternode = ImportInternode_LE( name , MGR , plasto , Tb , resp_INER , LIN1 , IN_A , IN_B , density_IN , leaf_width_to_IN_diameter , leaf_length_to_IN_length , leafLength , leafWidth , MaximumReserveInInternode , pos , 1 );
  newInternode.SetExeOrder( exeOrder );
  newInternode.SetStartDate( date );
  newInternode.InitCreationDate( date );
  newInternode.InitNextDate(  );

  instance.AddTInstance( newInternode );

  newInternode.ExternalConnect( [ "degreeDayForInternodeInitiation"
                                , "degreeDayForInternodeRealization"
                                , "testIc"
                                , "fcstr"
                                , "phenoStage"
                                , "stock_tiller"
                                , "Tair"
                                , "plasto_delay"
                                , "thresINER"
                                , "slopeINER"
                                , "FTSW"
                                , "P" ] );

  instance.SortTInstance(  );

}

void TillerManagerPhytomerInternodeCreation( TEntityInstance & instance,   TDateTime const& date,    bool const& IsFirstDayOfPI = false,    double const& plantStock = 0)
{
  TEntityInstance refMeristem; TEntityInstance newInternode;
//  nb_leaf_max_after_PI,
  double internodeNb;
  int exeOrder;
  Tsample sample;
  std::string name;

  refMeristem = instance.GetFather(  ) as TEntityInstance;

//  nb_leaf_max_after_PI := refMeristem.GetTAttribute('nb_leaf_max_after_PI').GetSample(date).value;
  internodeNb = instance.GetTAttribute( "internodeNb" ).GetSample( date ).value;
  SRwriteln( "Nombre d\'entrenoeud : " + floattostr( internodeNb ) );

  exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfFirstPhytomer" ).GetSample( date ).value + ( 2 * internodeNb ) );

  name = "IN" + floattostr( internodeNb + 1 ) + "_" + instance.GetName(  );
  // on crée le premier entre noeud

  SRwriteln( "****  creation de l\'entrenoeud : " + name + "  ****" );
  SRwriteln( "ExeOrder : " + IntToStr( exeOrder ) );

  newInternode = ImportInternode_LE( name , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );

  sample.date = date;
  sample.value = internodeNb + 1;
  instance.GetTAttribute( "internodeNb" ).SetSample( sample );

  sample.date = date;
  sample.value = internodeNb + 1;
  newInternode.GetTAttribute( "rank" ).SetSample( sample );

  newInternode.SetExeOrder( exeOrder );
  newInternode.SetStartDate( date );
  newInternode.InitCreationDate( date );
  newInternode.SetCurrentState( 2000 );
  newInternode.InitNextDate(  );

  instance.AddTInstance( newInternode );

  newInternode.ExternalConnect( [ "degreeDayForInternodeInitiation"
                                , "degreeDayForInternodeRealization"
                                , "testIc"
                                , "fcstr"
                                , "phenoStage"
                                , "stock_tiller"
                                , "Tair"
                                , "plasto_delay"
                                , "thresINER"
                                , "slopeINER"
                                , "FTSW"
                                , "P" ] );

  instance.SortTInstance(  );
}

void TillerManagerPeduncleCreation( TEntityInstance & instance,   TDateTime const& date,    bool const& IsFirstDayOfPI = false,    double const& plantStock = 0)
{
  double sumOfTillerLeafBiomass; double SumOfPlantBiomass;
  Tsample sample;
  int exeOrder; int pos;
  int internodeNumber;
//  rank : Double;
  std::string name; std::string instanceName;
  double TT;
  TAttribute TT_PI_Attribute;
//  attributeTmp : TAttribute;
  TEntityInstance refMeristem;
  TEntityInstance newInternode;
  double MGR; double plasto; double Tb; double resp_LER; double resp_INER; double LIN1; double IN_A; double IN_B; double density_IN;
  double MaximumReserveInInternode;
  double leaf_width_to_IN_diameter; double leaf_length_to_IN_length;
  double leafLength;
//  previousInternodePredimName, currentInternodePredimName : String;
//  sampleTmp : TSample;

//  i : integer;


  internodeNumber = 0;
  refMeristem = instance.GetFather(  ) as TEntityInstance;
  sumOfTillerLeafBiomass = instance.GetTAttribute( "sumOfTillerLeafBiomass" ).GetSample( date ).value;
  SumOfPlantBiomass = refMeristem.GetTAttribute( "sumOfLeafBiomass" ).GetSample( date ).value;
  sample = instance.GetTAttribute( "stock_tiller" ).GetSample( date );
  sample.value = ( sumOfTillerLeafBiomass *1.0/ SumOfPlantBiomass ) * plantStock;
  instance.GetTAttribute( "stock_tiller" ).SetSample( sample );
  sample = instance.GetTAttribute( "isFirstDayOfPi" ).GetSample( date );
  sample.value = 0;
  instance.GetTAttribute( "isFirstDayOfPi" ).SetSample( sample );

  exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfPeduncle" ).GetSample( date ).value );
  SRwriteln( "exe order: " + IntToStr( exeOrder ) );

  TT = refMeristem.GetTAttribute( "TT" ).GetSample( date ).value;
  TT_PI_Attribute = refMeristem.GetTAttribute( "TT_PI" );
  sample = TT_PI_Attribute.GetSample( date );
  sample.value = TT;
  TT_PI_Attribute.SetSample( sample );
  SRwriteln( "TT_PI : " + floattostr( sample.value ) );

  pos = 1;

  instanceName = instance.GetName(  );
  Delete( instanceName , 1 , 2 );

  name = "Ped" + instanceName;
  leafLength = FindLeafOnSameRankLength( instance , IntToStr( internodeNumber ) );
//  leafWidth := FindLeafOnSameRankWidth(instance, intToStr(internodeNumber));
  MGR = refMeristem.GetTAttribute( "MGR" ).GetSample( date ).value;
  plasto = refMeristem.GetTAttribute( "plasto" ).GetSample( date ).value;
  Tb = refMeristem.GetTAttribute( "Tb" ).GetSample( date ).value;
  resp_LER = refMeristem.GetTAttribute( "resp_LER" ).GetSample( date ).value;
  resp_INER = resp_LER;
  LIN1 = refMeristem.GetTAttribute( "LIN1" ).GetSample( date ).value;
  IN_A = refMeristem.GetTAttribute( "IN_A" ).GetSample( date ).value;
  IN_B = refMeristem.GetTAttribute( "IN_B" ).GetSample( date ).value;
  density_IN = refMeristem.GetTAttribute( "density_IN" ).GetSample( date ).value;
  MaximumReserveInInternode = refMeristem.GetTAttribute( "maximumReserveInInternode" ).GetSample( date ).value;
//  stock_mainstem := refMeristem.GetTAttribute('stock_mainstem').GetSample(date).value;

  leaf_width_to_IN_diameter = refMeristem.GetTAttribute( "leaf_width_to_IN_diameter" ).GetSample( date ).value;
  leaf_length_to_IN_length = refMeristem.GetTAttribute( "leaf_length_to_IN_length" ).GetSample( date ).value;


  // on crée le premier entre noeud

  SRwriteln( "****  creation du pedoncule : " + name + "  ****" );

  newInternode = ImportPeduncle_LE( name , MGR , plasto , Tb , resp_INER , LIN1 , IN_A , IN_B , density_IN , leaf_width_to_IN_diameter , leaf_length_to_IN_length , leafLength , MaximumReserveInInternode , 0 , 0 , pos , 1 );
  newInternode.SetExeOrder( exeOrder );
  newInternode.SetStartDate( date );
  newInternode.InitCreationDate( date );
  newInternode.InitNextDate(  );

  instance.AddTInstance( newInternode );

  newInternode.ExternalConnect( [ "degreeDayForInternodeInitiation"
                                , "degreeDayForInternodeRealization"
                                , "testIc"
                                , "fcstr"
                                , "phenoStage"
                                , "stock_tiller"
                                , "Tair"
                                , "plasto_delay"
                                , "thresINER"
                                , "slopeINER"
                                , "FTSW"
                                , "P" ] );

  instance.SortTInstance(  );

}

void TillerManagerPhytomerPeduncleCreation( TEntityInstance & instance,   TDateTime const& date,    bool const& IsFirstDayOfPI = false,    double const& plantStock = 0)
{
//  sumOfTillerLeafBiomass, SumOfPlantBiomass : Double;
  Tsample sample;
  int exeOrder;
//  internodeNumber : Integer;
//  rank : Double;
  std::string name; std::string instanceName;
  double TT;
  TAttribute TT_PI_Attribute;
//  attributeTmp : TAttribute;
  TEntityInstance refMeristem;
  TEntityInstance newInternode;
//  previousInternodePredimName, currentInternodePredimName : String;
//  sampleTmp : TSample;

//  i : integer;


  refMeristem = instance.GetFather(  ) as TEntityInstance;

  exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfPeduncle" ).GetSample( date ).value );
  SRwriteln( "exe order: " + IntToStr( exeOrder ) );

  TT = refMeristem.GetTAttribute( "TT" ).GetSample( date ).value;
  TT_PI_Attribute = refMeristem.GetTAttribute( "TT_PI" );
  sample = TT_PI_Attribute.GetSample( date );
  sample.value = TT;
  TT_PI_Attribute.SetSample( sample );
  SRwriteln( "TT_PI : " + floattostr( sample.value ) );

//  pos := 1;

  instanceName = instance.GetName(  );
  Delete( instanceName , 1 , 2 );

  name = "Ped" + instanceName;

  // on crée le premier entre noeud

  SRwriteln( "****  creation du pedoncule : " + name + "  ****" );

  newInternode = ImportPeduncle_LE( name , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 );
  newInternode.SetExeOrder( exeOrder );
  newInternode.SetStartDate( date );
  newInternode.InitCreationDate( date );
  newInternode.InitNextDate(  );
  newInternode.SetCurrentState( 2000 );

  instance.AddTInstance( newInternode );

  newInternode.ExternalConnect( [ "degreeDayForInternodeInitiation"
                                , "degreeDayForInternodeRealization"
                                , "testIc"
                                , "fcstr"
                                , "phenoStage"
                                , "stock_tiller"
                                , "Tair"
                                , "plasto_delay"
                                , "thresINER"
                                , "slopeINER"
                                , "FTSW"
                                , "P" ] );

  instance.SortTInstance(  );

}


// ----------------------------------------------------------------------------
//  fonction TillerManager_LE
//  ------------------------
//
/// gere le comportement d'une talle
///
/// description : TODO
///
///
/// @param instance instance d'entité portant le manageur
/// @param date date actuelle de simulation  (par défaut, MIN_DATE)
/// @param state état actuel de l'instance portant le manageur (par défaut, 0)
/// @return nouvelle etat
// ---------------------------------------------------------------------------

int TillerManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0)
{   int result;
  static double/*?*/ const INITIATION = 1;
  static double/*?*/ const REALIZATION = 2;
  static double/*?*/ const FERTILECAPACITY = 3;
  static double/*?*/ const PI = 4;
  static double/*?*/ const PRE_PI = 5;
  static double/*?*/ const PRE_FLO = 6;
  static double/*?*/ const FLO = 7;
  static double/*?*/ const DEAD = 1000;

  int newState;
  double boolCrossedPlasto; double plantStock;
  int IsFirstDayOfPI;
  double phenoStageAtPI;
  double nb_leaf_max_after_PI;
  double currentPhenoStage;
  double TTAtPI;
  double TT;
  double TT_PI_To_Flo;
  TInstance refInstance;
  

   switch( state ) {
    // ETAT INITIATION :
    // -----------------
    case INITIATION: 
      {
        // creation d'une premiere feuille
        TillerManagerLeafCreation( instance , date , true , false );
        // nouvelle état
        newState = REALIZATION;
      }

    // ETAT REALISATION :
    // ----------------
    case REALIZATION: 
      {
        // initiation d'une nouvelle feuille a chaque nouveau plasto
        boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
        plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
        if(  ( ( boolCrossedPlasto>=0 ) && ( plantStock >=0 ) ) ) // New plastochron et stock disponible
        {
          TillerManagerLeafCreation( instance , date , false , false );
        }
        // pas de changement d etat
        newState = state;
      }

    // Etat en attente de la fertilité
    // -------------------------------
    case FERTILECAPACITY: 
      {
        // initiation d'une nouvelle feuille a chaque nouveau plasto
        boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
        plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
        if(  ( ( boolCrossedPlasto>=0 ) && ( plantStock >=0 ) ) ) // New plastochron et stock disponible
        {
          // creation d'une nouvelle feuille
          // -------------------------------
          TillerManagerLeafCreation( instance , date , false , false );
      }
      // pas de changement d etat
      newState = state;
    }
  case PRE_PI: 
    {
      // initiation d'une nouvelle feuille a chaque nouveau plasto
      boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
      plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
      if(  ( ( boolCrossedPlasto>=0 ) && ( plantStock >=0 ) ) ) // New plastochron et stock disponible
      {
        // creation d'une nouvelle feuille
        // -------------------------------
        TillerManagerLeafCreation( instance , date , false , false );
      }
      // pas de changement d etat
      newState = state;
    }
  case PI: 
    {

      refInstance = ( instance as TEntityInstance ).GetFather(  );
      nb_leaf_max_after_PI = ( refInstance as TEntityInstance ).GetTAttribute( "nb_leaf_max_after_PI" ).GetSample( date ).value;

      boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
      plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
      IsFirstDayOfPI = Trunc( instance.GetTAttribute( "isFirstDayOfPi" ).GetSample( date ).value );
      phenoStageAtPI = instance.GetTAttribute( "phenoStageAtPI" ).GetSample( date ).value;
      currentPhenoStage = instance.GetTAttribute( "phenoStage" ).GetSample( date ).value;

      if(  ( plantStock > 0 ) )
      {
        if(  ( boolCrossedPlasto >= 0 ) )
        {
          SRwriteln( "currentPhenoStage    --> " + floattostr( currentPhenoStage ) );
          SRwriteln( "phenoStageAtPI       --> " + floattostr( phenoStageAtPI ) );
          SRwriteln( "nb_leaf_max_after_PI --> " + floattostr( nb_leaf_max_after_PI ) );
          if(  ( currentPhenoStage <= phenoStageAtPI + nb_leaf_max_after_PI ) )
          {
            if(  ( IsFirstDayOfPI == 1 ) )
            {
              TillerManagerLeafCreation( instance , date , false , true );
              TillerManagerInternodeCreation( instance , date , true , plantStock );
              TillerManagerPanicleCreation( instance , date );
              newState = PI;
            }
            else
            {
              TillerManagerLeafCreation( instance , date , false , true );
              TillerManagerInternodeCreation( instance , date , false , plantStock );
              newState = PI;
            }
          }
          else
          {
            newState = PI;
          }
          if(  ( currentPhenoStage == ( phenoStageAtPI + nb_leaf_max_after_PI ) + 1 ) )
          {
            TillerManagerInternodeCreation( instance , date , false , plantStock );
            newState = PI;
          }
          else
          {
            newState = PI;}
          if(  ( currentPhenoStage == ( phenoStageAtPI + nb_leaf_max_after_PI ) + 2 ) )
          {
            TillerManagerPeduncleCreation( instance , date , false , plantStock );
            newState = PRE_FLO;
          }
          else
          {
            newState = PI;
          }
        }
        else
        {
          newState = PI;
        }
      }
      else
      {
        newState = PI;
      }


      // ------------------------
      // pas de changement d'etat
      // ------------------------
    }
  case PRE_FLO: 
    {
      TTAtPI = instance.GetTAttribute( "TTAtPI" ).GetSample( date ).value;
      refInstance = ( instance as TEntityInstance ).GetFather(  );
      TT = ( refInstance as TEntityInstance ).GetTAttribute( "TT" ).GetSample( date ).value;
      TT_PI_To_Flo = ( refInstance as TEntityInstance ).GetTAttribute( "TT_PI_To_Flo" ).GetSample( date ).value;
      if(  ( ( TT - TTAtPI ) >= TT_PI_To_Flo ) )
      {
        PanicleTransitionToFLO( instance );
        PeduncleTransitionToFLO( instance );
        newState = FLO;
      }
      else
      {
        newState = PRE_FLO;
      }
    }
  case FLO: 
    {
      newState = FLO;
    }
  case DEAD: 
    {
      newState = DEAD;
    }
    // ETAT NON DEFINI :
    // ----------------
    default:
    {
      result = -1;
      return result;
    }
  }


  SRwriteln( "Tiller Manager, transition de : " + IntToStr( state ) + " --> " + IntToStr( newState ) );

  // retourne le nouvel état
  result = newState;
return result;
}

void TillerManagerCreateFirstLeaf( TEntityInstance & instance,    TDateTime const& date = 0)
{
  std::string name;
  TEntityInstance refMeristem; TEntityInstance newLeaf;
  int exeOrder; int pos;
  double Lef1; double MGR; double plasto; double ligulo; double WLR; double LL_BL; double LL_BL_New; double allo_area; double G_L; double Tb; double resp_LER; double coeffLifespan; double mu;
  double phenoStageAtPI; double currentPhenoStage; double slope_LL_BL_at_PI;
  Tsample sample;
  TAttribute attributeTmp;

  refMeristem = instance.GetFather(  ) as TEntityInstance;
  // creation d'une premiere feuille
  sample.date = date;
  sample.value = 1;
  instance.GetTAttribute( "leafNb" ).SetSample( sample );
  instance.GetTAttribute( "activeLeavesNb" ).SetSample( sample );
  exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfFirstPhytomer" ).GetCurrentSample(  ).value ) + 1;
  name = "L1_" + instance.GetName(  );
  pos = 1;

  Lef1 = refMeristem.GetTAttribute( "Lef1" ).GetSample( date ).value;
  MGR = refMeristem.GetTAttribute( "MGR_init" ).GetSample( date ).value;
  plasto = refMeristem.GetTAttribute( "plasto_init" ).GetSample( date ).value;
  ligulo = refMeristem.GetTAttribute( "ligulo1" ).GetSample( date ).value;
  WLR = refMeristem.GetTAttribute( "WLR" ).GetSample( date ).value;
  LL_BL = refMeristem.GetTAttribute( "LL_BL_init" ).GetSample( date ).value;
  allo_area = refMeristem.GetTAttribute( "allo_area" ).GetSample( date ).value;
  G_L = refMeristem.GetTAttribute( "G_L" ).GetSample( date ).value;
  Tb = refMeristem.GetTAttribute( "Tb" ).GetSample( date ).value;
  resp_LER = refMeristem.GetTAttribute( "resp_LER" ).GetSample( date ).value;
  phenoStageAtPI = instance.GetTAttribute( "phenoStageAtPI" ).GetSample( date ).value;
  currentPhenoStage = instance.GetTAttribute( "phenoStage" ).GetSample( date ).value;
  coeffLifespan = refMeristem.GetTAttribute( "coeff_lifespan" ).GetSample( date ).value;
  mu = refMeristem.GetTAttribute( "mu" ).GetSample( date ).value;

  SRwriteln( "****  creation de la feuille : " + name + "  ****" );

  slope_LL_BL_at_PI = refMeristem.GetTAttribute( "slope_LL_BL_at_PI" ).GetSample( date ).value;
  if(  ( ( currentPhenoStage > phenoStageAtPI ) && ( phenoStageAtPI != 0 ) ) )
  {
    LL_BL_New = LL_BL + slope_LL_BL_at_PI * ( currentPhenoStage - phenoStageAtPI );
  }
  else
  {
    LL_BL_New = LL_BL;
  }

  newLeaf = ImportLeaf_LE( name , Lef1 , MGR , plasto , ligulo , WLR , LL_BL_New , allo_area , G_L , Tb , resp_LER , coeffLifespan , mu , pos , 0 );

  sample = newLeaf.GetTAttribute( "rank" ).GetSample( date );
  sample.value = 1;
  newLeaf.GetTAttribute( "rank" ).SetSample( sample );

  // determination de l'ordre d'execution de newLeaf :
  // recherche de la premiere place vide apres la premiere entité de type 'Leaf'
  newLeaf.SetExeOrder( exeOrder );
  newLeaf.SetStartDate( date );
  newLeaf.InitCreationDate( date );
  newLeaf.InitNextDate(  );
  instance.AddTInstance( newLeaf );

  // connection port <-> attribut pour newLeaf
  attributeTmp = TAttributeTmp.Create( "predim_L1" );
  instance.AddTAttribute( attributeTmp );

  newLeaf.ExternalConnect( [ "zero"
                           , "predimLeafOnMainstem"
                           , "predim_L1"
                           , "degreeDayForLeafInitiation"
                           , "degreeDayForLeafRealization"
                           , "testIc"
                           , "fcstr"
                           , "phenoStage"
                           , "SLA"
                           , "plantStock"
                           , "Tair"
                           , "plasto_delay"
                           , "thresLER"
                           , "slopeLER"
                           , "FTSW"
                           , "lig"
                           , "P" ] );
  instance.SortTInstance(  );
}

void TillerManagerCreateFirstIN( TEntityInstance & instance,    TDateTime const& date = 0)
{
  int exeOrder;
  std::string name;
  TEntityInstance newInternode; TEntityInstance refMeristem;
  double internodeNb;
  Tsample sample;

  refMeristem = instance.GetFather(  ) as TEntityInstance;
  exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfFirstPhytomer" ).GetSample( date ).value );
  name = "IN1_" + instance.GetName(  );

  // on crée le premier entre noeud

  SRwriteln( "****  creation de l\'entrenoeud : " + name + "  ****" );

  newInternode = ImportInternode_LE( name , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 );

  internodeNb = instance.GetTAttribute( "internodeNb" ).GetCurrentSample(  ).value + 1;
  sample.date = date;
  sample.value = internodeNb;
  instance.GetTAttribute( "internodeNb" ).SetSample( sample );

  sample.date = date;
  sample.value = 1;
  newInternode.GetTAttribute( "rank" ).SetSample( sample );

  newInternode.SetExeOrder( exeOrder );
  newInternode.SetStartDate( date );
  newInternode.InitCreationDate( date );
  newInternode.InitNextDate(  );
  newInternode.SetCurrentState( 2000 );

  instance.AddTInstance( newInternode );

  newInternode.ExternalConnect( [ "degreeDayForInternodeInitiation"
                                , "degreeDayForInternodeRealization"
                                , "testIc"
                                , "fcstr"
                                , "phenoStage"
                                , "stock_tiller"
                                , "Tair"
                                , "plasto_delay"
                                , "thresINER"
                                , "slopeINER"
                                , "FTSW"
                                , "P" ] );
  instance.SortTInstance(  );
}

void TillerManagerCreateFirstPhytomers( TEntityInstance & instance,    TDateTime const& date = 0)
{
  TillerManagerCreateFirstIN( instance , date );
  TillerManagerCreateFirstLeaf( instance , date );
}

void TillerManagerCreateOthersLeaf( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0)
{
  std::string name;
  int exeOrder;
  TEntityInstance newLeaf; TEntityInstance refMeristem;
  std::string previousLeafPredimName; std::string currentLeafPredimName;
  TAttribute attributeTmp;
  Tsample sample;

  // creation des autres feuilles
  name = "L" + floattostr( n + 1 ) + "_" + instance.GetName(  );
  refMeristem = instance.GetFather as TEntityInstance;
  exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfFirstPhytomer" ).GetSample( date ).value ) + ( 2 * n ) + 1;

  SRwriteln( "****  creation de la feuille : " + name + "  ****" );

  newLeaf = ImportLeaf_LE( name , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );

  // determination de l'ordre d'execution de newLeaf :
  // recherche de la premiere place vide apres la premiere entité de type 'Leaf'
  newLeaf.SetExeOrder( exeOrder );
  newLeaf.SetStartDate( MAX_DATE );
  newLeaf.InitCreationDate( MAX_DATE );
  newLeaf.InitNextDate(  );
  newLeaf.SetCurrentState( 2000 );
  instance.AddTInstance( newLeaf );

  previousLeafPredimName = "predim_L" + floattostr( n );
  currentLeafPredimName = "predim_L" + floattostr( n + 1 );

  attributeTmp = TAttributeTmp.Create( currentLeafPredimName );
  instance.AddTAttribute( attributeTmp );

  newLeaf.ExternalConnect( [ previousLeafPredimName
                           , "predimLeafOnMainstem"
                           , currentLeafPredimName
                           , "degreeDayForLeafInitiation"
                           , "degreeDayForLeafRealization"
                           , "testIc"
                           , "fcstr"
                           , "phenoStage"
                           , "SLA"
                           , "plantStock"
                           , "Tair"
                           , "plasto_delay"
                           , "thresLER"
                           , "slopeLER"
                           , "FTSW"
                           , "lig"
                           , "P" ] );

  sample = instance.GetTAttribute( "leafNb" ).GetCurrentSample(  );
  sample.value = sample.value + 1;
  sample.date = date;
  instance.GetTAttribute( "leafNb" ).SetSample( sample );
  instance.SortTInstance(  );
}

void TillerManagerCreateOthersIN( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0)
{
  TEntityInstance refMeristem; TEntityInstance newInternode;
  int exeOrder;
  std::string name;
  double internodeNb;
  Tsample sample;

  refMeristem = instance.GetFather as TEntityInstance;
  exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfFirstPhytomer" ).GetSample( date ).value ) + ( 2 * n );
  name = "IN" + IntToStr( n + 1 ) + "_" + instance.GetName(  );

  // on crée le premier entre noeud

  SRwriteln( "****  creation de l\'entrenoeud : " + name + "  ****" );

  newInternode = ImportInternode_LE( name , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 );

  internodeNb = instance.GetTAttribute( "internodeNb" ).GetCurrentSample(  ).value + 1;
  sample.date = date;
  sample.value = internodeNb;
  instance.GetTAttribute( "internodeNb" ).SetSample( sample );

  sample.date = date;
  sample.value = internodeNb;
  newInternode.GetTAttribute( "rank" ).SetSample( sample );

  //showmessage(inttostr(exeorder));

  newInternode.SetExeOrder( exeOrder );
  newInternode.SetStartDate( date );
  newInternode.InitCreationDate( date );
  newInternode.InitNextDate(  );
  newInternode.SetCurrentState( 2000 );

  instance.AddTInstance( newInternode );

  newInternode.ExternalConnect( [ "degreeDayForInternodeInitiation"
                                , "degreeDayForInternodeRealization"
                                , "testIc"
                                , "fcstr"
                                , "phenoStage"
                                , "stock_tiller"
                                , "Tair"
                                , "plasto_delay"
                                , "thresINER"
                                , "slopeINER"
                                , "FTSW"
                                , "P" ] );
  instance.SortTInstance(  );
}

void TillerManagerCreateOthersPhytomers( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0)
{
  TillerManagerCreateOthersIN( instance , date , n );
  TillerManagerCreateOthersLeaf( instance , date , n );
}

int TillerManagerPhytomer_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0)
{   int result;
  static double/*?*/ const INITIATION = 1;
  static double/*?*/ const REALIZATION = 2;
  static double/*?*/ const FERTILECAPACITY = 3;
  static double/*?*/ const PI = 4;
  static double/*?*/ const PRE_PI = 5;
  static double/*?*/ const PRE_FLO = 6;
  static double/*?*/ const FLO = 7;
  static double/*?*/ const ELONG = 9;
  static double/*?*/ const PRE_ELONG = 10;
  static double/*?*/ const ENDFILLING = 12;
  static double/*?*/ const MATURITY = 13;
  static double/*?*/ const DEAD = 1000;

  int newState;
  int plantState;
  std::string newStateStr; std::string oldStateStr;
  double boolCrossedPlasto; double plantStock;
  int IsFirstDayOfPI;
  double phenoStageAtPI;
  double nb_leaf_max_after_PI;
  double currentPhenoStage;
  double TTAtPI;
  double TT;
  double TT_PI_To_Flo;
  TInstance refInstance;
  int i;
  std::string nameOfLastLigulatedLeafOnTiller;
  int fatherState;
  Tsample sample;
  double previousState;
  double phenoStageAtPreFlo; double phenostage_PRE_FLO_to_FLO; double phenostage;

   switch( state ) {
    // ETAT INITIATION :
    // -----------------
    case INITIATION: 
      {
        oldStateStr = "INITIATION";
        refInstance = ( instance as TEntityInstance ).GetFather(  );
        nb_leaf_max_after_PI = ( refInstance as TEntityInstance ).GetTAttribute( "nb_leaf_max_after_PI" ).GetSample( date ).value;
        // creation du premier phytomere
        TillerManagerCreateFirstPhytomers( instance , date );

        { long i_end = Trunc( nb_leaf_max_after_PI )+1 ; for( i = 1 ; i < i_end ; ++i )
        {
          TillerManagerCreateOthersPhytomers ( instance , date , i );
        }}
        // nouvel état
        fatherState = ( refInstance as TEntityInstance ).GetCurrentState(  );
        if(  ( fatherState == 9 ) )
        {
          newState = PRE_ELONG;
          newStateStr = "PRE_ELONG";
        }
        else
        {
          newState = REALIZATION;
          newStateStr = "REALIZATION";
        }
      }

    // ETAT REALISATION :
    // ----------------
    case REALIZATION: 
      {
        oldStateStr = "REALIZATION";
        // initiation d'une nouvelle feuille a chaque nouveau plasto
        boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
        plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
        if(  ( ( boolCrossedPlasto >= 0 ) && ( plantStock > 0 ) ) ) // New plastochron et stock disponible
        {
          TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );
          TillerManagerPhytomerLeafCreation( instance , date , false , false );
        }
        // pas de changement d etat
        newState = state;
        newStateStr = "REALIZATION";
      }

    // Etat en attente de la fertilité
    // -------------------------------
    case FERTILECAPACITY: 
      {
        oldStateStr = "FERTILECAPACITY";
        // initiation d'une nouvelle feuille a chaque nouveau plasto
        boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
        plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
        if(  ( ( boolCrossedPlasto >= 0 ) && ( plantStock > 0 ) ) ) // New plastochron et stock disponible
        {
          // creation d'une nouvelle feuille
          // -------------------------------
          TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );
          TillerManagerPhytomerLeafCreation( instance , date , false , false );
        }
        // pas de changement d etat
        newState = state;
        newStateStr = "FERTILECAPACITY";
      }

    case PRE_PI: 
      {
        SRwriteln( "PRE_PI" );
        oldStateStr = "PRE_PI";
        // initiation d'une nouvelle feuille a chaque nouveau plasto
        boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
        plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
        previousState = instance.GetTAttribute( "previousState" ).GetSample( date ).value;
        SRwriteln( "boolCrossedPlasto --> " + floattostr( boolCrossedPlasto ) );
        SRwriteln( "plantStock        --> " + floattostr( plantStock ) );
        SRwriteln( "previousState     --> " + floattostr( previousState ) );
        if(  ( ( boolCrossedPlasto >= 0 ) && ( plantStock > 0 ) ) ) // New plastochron et stock disponible
        {
          // creation d'une nouvelle feuille
          // -------------------------------
          SRwriteln( "TillerManagerPhytomerInternodeCreation" );
          TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );
          SRwriteln( "TillerManagerPhytomerLeafCreation" );
          TillerManagerPhytomerLeafCreation( instance , date , false , false );
          if(  ( previousState == 9 ) )
          {
            SRwriteln( "previous state --> " + floattostr( previousState ) + " on allonge des IN meme a PRE_PI" );
            TillerManagerStartInternodeElongation( instance , date , Trunc( currentPhenoStage ) );
          }
        }
        // pas de changement d etat
        newState = state;
        newStateStr = "PRE_PI";
      }

    case PI: 
      {
        oldStateStr = "PI";
        SRwriteln( "PI" );
        refInstance = ( instance as TEntityInstance ).GetFather(  );
        nb_leaf_max_after_PI = ( refInstance as TEntityInstance ).GetTAttribute( "nb_leaf_max_after_PI" ).GetSample( date ).value;
        boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
        plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
        IsFirstDayOfPI = Trunc( instance.GetTAttribute( "isFirstDayOfPi" ).GetSample( date ).value );
        phenoStageAtPI = instance.GetTAttribute( "phenoStageAtPI" ).GetSample( date ).value;
        currentPhenoStage = instance.GetTAttribute( "phenoStage" ).GetSample( date ).value;
        plantState = ( refInstance as TEntityInstance ).GetCurrentState(  );
        SRwriteln( "boolCrossedPlasto --> " + floattostr( boolCrossedPlasto ) );
        SRwriteln( "plantstock --> " + floattostr( plantStock ) );
        SRwriteln( "plantState --> " + floattostr( plantState ) );
        SRwriteln( "currentPhenoStage    --> " + floattostr( currentPhenoStage ) );
        SRwriteln( "phenoStageAtPI       --> " + floattostr( phenoStageAtPI ) );
        SRwriteln( "nb_leaf_max_after_PI --> " + floattostr( nb_leaf_max_after_PI ) );
        //if (plantStock > 0) then
        if(  ( plantState == 4 ) || ( plantState == 6 ) || ( plantState == 5 ) )
        {
          if(  ( boolCrossedPlasto >= 0 ) )
          {
            SRwriteln( "currentPhenoStage    --> " + floattostr( currentPhenoStage ) );
            SRwriteln( "phenoStageAtPI       --> " + floattostr( phenoStageAtPI ) );
            SRwriteln( "nb_leaf_max_after_PI --> " + floattostr( nb_leaf_max_after_PI ) );
            if(  ( currentPhenoStage <= phenoStageAtPI + nb_leaf_max_after_PI ) )
            {
              if(  ( IsFirstDayOfPI == 1 ) )
              {
                SRwriteln( "First day of PI on tiller" );
                TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );
                TillerManagerPhytomerLeafCreation( instance , date , false , false );
                TillerManagerStartInternodeElongation( instance , date , Trunc( currentPhenoStage ) );
                SRwriteln( "TillerManagerPhytomerPanicleCreation" );
                TillerManagerPhytomerPanicleCreation( instance , date );
                SRwriteln( "TillerManagerPhytomerPeduncleCreation" );
                TillerManagerPhytomerPeduncleCreation( instance , date , false , plantStock );
                SRwriteln( "TillerManagerPhytomerPanicleActivation" );
                TillerManagerPhytomerPanicleActivation( instance );
                sample = instance.GetTAttribute( "isFirstDayOfPi" ).GetSample( date );
                sample.value = 0;
                instance.GetTAttribute( "isFirstDayOfPi" ).SetSample( sample );
                newState = PI;
                newStateStr = "PI";
              }
              else
              {
                SRwriteln( "pas first day of pi" );
                SRwriteln( "TillerManagerPhytomerLeafActivation" );
                TillerManagerPhytomerLeafActivation( instance , date );
                SRwriteln( "TillerManagerStartInternodeElongation" );
                TillerManagerStartInternodeElongation( instance , date , Trunc( currentPhenoStage ) );
                newState = PI;
                newStateStr = "PI";
              }
            }
            else
            {
              if(  ( currentPhenoStage == ( phenoStageAtPI + nb_leaf_max_after_PI + 1 ) ) )
              {
                SRwriteln( "currentPhenoStage = (phenoStageAtPI + nb_leaf_max_after_PI + 1)" );
                SRwriteln( "--- Appel de TillerManagerStartInternodeElongation ---" );
                //TillerManagerStartInternodeElongation(instance, date, Trunc(currentPhenoStage));
                SRwriteln( "--- Appel de TillerManagerPhytomerPeduncleActivation ---" );
                TillerManagerPhytomerPeduncleActivation( instance , date );
                TillerStorePhenostageAtPre_Flo( instance , date , currentPhenoStage );
                newState = PRE_FLO;
                newStateStr = "PRE_FLO";
              }
            }
          }
          else
          {
            newState = PI;
            newStateStr = "PI";
          }
        }
        else
        {
          newState = PI;
          newStateStr = "PI";
        }


        // ------------------------
        // pas de changement d'etat
        // ------------------------
      }

    case PRE_FLO: 
      {
        oldStateStr = "PRE_FLO";
        phenoStageAtPreFlo = instance.GetTAttribute( "phenoStageAtPreFlo" ).GetSample( date ).value;
        refInstance = ( instance as TEntityInstance ).GetFather(  );
        phenostage = ( refInstance as TEntityInstance ).GetTAttribute( "n" ).GetSample( date ).value;
        phenostage_PRE_FLO_to_FLO = ( refInstance as TEntityInstance ).GetTAttribute( "phenostage_PRE_FLO_to_FLO" ).GetSample( date ).value;
        SRwriteln( "phenostage                --> " + floattostr( phenostage ) );
        SRwriteln( "phenoStageAtPreFlo        --> " + floattostr( phenoStageAtPreFlo ) );
        SRwriteln( "phenostage_PRE_FLO_to_FLO --> " + floattostr( phenostage_PRE_FLO_to_FLO ) );
        if(  ( phenostage == ( phenoStageAtPreFlo + phenostage_PRE_FLO_to_FLO ) ) )
        {
          PanicleTransitionToFLO( instance );
          PeduncleTransitionToFLO( instance );
          newState = FLO;
          newStateStr = "FLO";
        }
        else
        {
          newState = PRE_FLO;
          newStateStr = "PRE_FLO";
        }
      }

    case FLO: 
      {
        oldStateStr = "FLO";
        newState = FLO;
        newStateStr = "FLO";
      }

    case DEAD: 
      {
        oldStateStr = "DEAD";
        newState = DEAD;
        newStateStr = "DEAD";
      }

    case ELONG: 
      {
        oldStateStr = "ELONG";
        nameOfLastLigulatedLeafOnTiller = FindLastLigulatedLeafNumberOnCulm( instance );
        if(  ( nameOfLastLigulatedLeafOnTiller == "" ) )
        {
          SRwriteln( "Pas de feuille ligulee sur la talle, on passe en PRE_ELONG" );
          newState = PRE_ELONG;
          newStateStr = "PRE_ELONG";
        }
        else
        {
          SRwriteln( "Elongation" );
          boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
          plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
          currentPhenoStage = instance.GetTAttribute( "phenoStage" ).GetSample( date ).value;
          SRwriteln( "currentPhenoStage --> " + floattostr( currentPhenoStage ) );
          if(  ( ( boolCrossedPlasto >= 0 ) && ( plantStock > 0 ) ) ) // New plastochron et stock disponible
          {
            TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );
            SRwriteln( "Internode creation" );
            TillerManagerPhytomerLeafCreation( instance , date , false , false );
            SRwriteln( "Leaf creation" );
            TillerManagerStartInternodeElongation( instance , date , Trunc( currentPhenoStage ) );
            SRwriteln( "Internode elongation" );
          }
          newState = ELONG;
          newStateStr = "ELONG";
        }
      }

      case PRE_ELONG: 
        {
          oldStateStr = "PRE_ELONG";
          nameOfLastLigulatedLeafOnTiller = FindLastLigulatedLeafNumberOnCulm( instance );
          if(  ( nameOfLastLigulatedLeafOnTiller == "-1" ) )
          {
            SRwriteln( "Pas de feuille ligulee sur la talle, on reste en PRE_ELONG" );

            boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
            plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
            currentPhenoStage = instance.GetTAttribute( "phenoStage" ).GetSample( date ).value;
            if(  ( ( boolCrossedPlasto >= 0 ) && ( plantStock > 0 ) ) )
            {
              TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );
              SRwriteln( "Internode creation" );
              TillerManagerPhytomerLeafCreation( instance , date , false , false );
              SRwriteln( "Leaf creation" );
            }


            newState = PRE_ELONG;
            newStateStr = "PRE_ELONG";
          }
          else
          {
            boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
            plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
            currentPhenoStage = instance.GetTAttribute( "phenoStage" ).GetSample( date ).value;
            if(  ( ( boolCrossedPlasto >= 0 ) && ( plantStock > 0 ) ) ) // New plastochron et stock disponible
            {
              SRwriteln( "Une feuille ligulee sur la talle et changement de plastochron et stock positif, on passe en ELONG" );
              TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );
              SRwriteln( "Internode creation" );
              TillerManagerPhytomerLeafCreation( instance , date , false , false );
              SRwriteln( "Leaf creation" );
              TillerManagerStartInternodeElongation( instance , date , Trunc( currentPhenoStage ) );
              SRwriteln( "Internode elongation" );
              newState = ELONG;
              newStateStr = "ELONG";
            }
            else
            {
              newState = PRE_ELONG;
              newStateStr = "PRE_ELONG";
            }
          }

        }

      case ENDFILLING: 
        {
          oldStateStr = "ENDFILLING";
          newState = ENDFILLING;
          newStateStr = "ENDFILLING";
        }

      case MATURITY: 
        {
          oldStateStr = "MATURITY";
          newState = MATURITY;
          newStateStr = "MATURITY";
        }

      // ETAT NON DEFINI :
      // ----------------
      default:
      {
        result = -1;
        return result;
      }
    }


  SRwriteln( "Tiller Manager Phytomer, transition de : " + oldStateStr + " --> " + newStateStr );

  // retourne le nouvel état
  result = newState;
return result;
}

void MeristemManagerLeafCreation( TEntityInstance & instance,    TDateTime const& date = 0,    double const& n = 0,    bool const& LL_BL_MODIFIED = false)
{
  std::string name;
  double Lef1; double MGR; double plasto; double ligulo; double WLR; double LL_BL; double allo_area;
  double G_L; double Tb; double resp_LER; double nbleaf_pi; double LL_BL_New; double slope_LL_BL_at_PI; double coeffLifespan; double mu;
  int exeOrder;
  TEntityInstance newLeaf;
  TAttribute attributeTmp;
  TAttribute refAttribute;
  TPort refPort;
  std::string previousLeafPredimName; std::string currentLeafPredimName;


  // creation d'une nouvelle feuille
  // -------------------------------
  //
  name = "L" + floattostr( n );
  Lef1 = instance.GetTAttribute( "Lef1" ).GetSample( date ).value;
  MGR = instance.GetTAttribute( "MGR" ).GetSample( date ).value;
  plasto = instance.GetTAttribute( "plasto" ).GetSample( date ).value;
  ligulo = instance.GetTAttribute( "ligulo" ).GetSample( date ).value;
  WLR = instance.GetTAttribute( "WLR" ).GetSample( date ).value;
  LL_BL = instance.GetTAttribute( "LL_BL" ).GetSample( date ).value;
  allo_area = instance.GetTAttribute( "allo_area" ).GetSample( date ).value;
  G_L = instance.GetTAttribute( "G_L" ).GetSample( date ).value;
  Tb = instance.GetTAttribute( "Tb" ).GetSample( date ).value;
  resp_LER = instance.GetTAttribute( "resp_LER" ).GetSample( date ).value;
  coeffLifespan = instance.GetTAttribute( "coeff_lifespan" ).GetSample( date ).value;
  mu = instance.GetTAttribute( "mu" ).GetSample( date ).value;


  slope_LL_BL_at_PI = instance.GetTAttribute( "slope_LL_BL_at_PI" ).GetSample( date ).value;
  nbleaf_pi = instance.GetTAttribute( "nbleaf_pi" ).GetSample( date ).value;
  if(  LL_BL_MODIFIED )
  {
    LL_BL_New = LL_BL + slope_LL_BL_at_PI * ( n - nbleaf_pi );
  }
  else
  {
    LL_BL_New = LL_BL;
  }

  SRwriteln( "LL_BL_New --> " + floattostr( LL_BL_New ) );
  SRwriteln( "****  creation de la feuille : " + name + "  ****" );
  newLeaf = ImportLeaf_LE( name , Lef1 , MGR , plasto , ligulo , WLR , LL_BL_New , allo_area , G_L , Tb , resp_LER , coeffLifespan , mu , 0 , 1 );

  // determination de l'ordre d'execution de newLeaf :
  // recherche de la premiere place vide apres la premiere entité de type 'Leaf'
  exeOrder = FindFirstEmptyPlaceAfterASpecifiedCategory( instance, "Leaf" );

  newLeaf.SetExeOrder( exeOrder );
  newLeaf.SetStartDate( date );
  newLeaf.InitCreationDate( date );
  newLeaf.InitNextDate(  );
  instance.AddTInstance( newLeaf );

  // connection port <-> attribut pour newLeaf
  previousLeafPredimName = "predim_L" + floattostr( n-1 );
  currentLeafPredimName = "predim_L" + floattostr( n );

  attributeTmp = TAttributeTmp.Create( currentLeafPredimName );
  instance.AddTAttribute( attributeTmp );

  newLeaf.ExternalConnect( [ previousLeafPredimName
                           , previousLeafPredimName
                           , currentLeafPredimName
                           , "DD"
                           , "EDD"
                           , "testIc"
                           , "fcstr"
                           , "n"
                           , "SLA"
                           , "stock"
                           , "Tair"
                           , "plasto_delay"
                           , "thresLER"
                           , "slopeLER"
                           , "FTSW"
                           , "lig"
                           , "P" ] );
  instance.SortTInstance(  );

  // deconnecte le port 'predimOfCurrentLeaf' de la feuille L(n-2) avec
  // l'attribut 'predimOfNewLeaf' et realise une connectique
  // port 'predimOfCurrentLeaf' de feuille L(n-1) <-> attribut 'predimOfNewLeaf'
  // TODO -u Model : ce qui a ci-dessous risque de crasher lorsque trop de destruction de feuille...
  if( ( n >= 3 ) )
  {
    if(  ( instance.GetTInstance( "L" + floattostr( n - 2 ) ) != 00 ) )
    {
      refPort = instance.GetTInstance( "L" + floattostr( n - 2 ) ).GetTPort( "predimOfCurrentLeaf" );
      refPort.ExternalUnconnect( 1 );
    }

    refAttribute = instance.GetTAttribute( "predimOfNewLeaf" );
    instance.GetTInstance( "L" + floattostr( n - 1 ) ).GetTPort( "predimOfCurrentLeaf" ).ExternalConnect( refAttribute );
  }
}

void MeristemManagerPhytomerPanicleActivation( TEntityInstance & instance)
{
  TEntityInstance entityPanicle;

  entityPanicle = FindPanicle( instance );
  entityPanicle.SetCurrentState( 1 );
  SRwriteln( "La panicule : " + entityPanicle.GetName(  ) + " est activee" );
}

void MeristemManagerPhytomerPeduncleActivation( TEntityInstance & instance,   TDateTime const& date)
{
  TEntityInstance entityPeduncle; TEntityInstance entityInternode;
  std::string internodeNumber; std::string name;
  double leafLength; double leafWidth; double MGR; double plasto; double ligulo; double Tb; double resp_LER; double resp_INER; double LIN1; double IN_A;
  double IN_B; double density_IN; double MaximumReserveInInternode; double stock_mainstem; double leaf_width_to_IN_diameter;
  double leaf_length_to_IN_length; double n; double ratioINPed; double peduncleDiam;
  Tsample sample;

  entityPeduncle = FindPeduncle( instance );

  internodeNumber = FindLastLigulatedLeafNumberOnCulm( instance );

  SRwriteln( "Last ligulated leaf : " + internodeNumber );

  name = "IN" + internodeNumber;

  entityInternode = FindInternodeAtRank( instance , internodeNumber );

  leafLength = FindLeafOnSameRankLength( instance , internodeNumber );
  leafWidth = FindLeafOnSameRankWidth( instance , internodeNumber );
  MGR = instance.GetTAttribute( "MGR" ).GetSample( date ).value;
  plasto = instance.GetTAttribute( "plasto" ).GetSample( date ).value;
  ligulo = instance.GetTAttribute( "ligulo" ).GetSample( date ).value;
  Tb = instance.GetTAttribute( "Tb" ).GetSample( date ).value;
  resp_LER = instance.GetTAttribute( "resp_LER" ).GetSample( date ).value;
  resp_INER = resp_LER;
  LIN1 = instance.GetTAttribute( "LIN1" ).GetSample( date ).value;
  IN_A = instance.GetTAttribute( "IN_A" ).GetSample( date ).value;
  IN_B = instance.GetTAttribute( "IN_B" ).GetSample( date ).value;
  density_IN = instance.GetTAttribute( "density_IN" ).GetSample( date ).value;
  MaximumReserveInInternode = instance.GetTAttribute( "maximumReserveInInternode" ).GetSample( date ).value;
  stock_mainstem = instance.GetTAttribute( "stock_mainstem" ).GetSample( date ).value;
  leaf_width_to_IN_diameter = instance.GetTAttribute( "leaf_width_to_IN_diameter" ).GetSample( date ).value;
  leaf_length_to_IN_length = instance.GetTAttribute( "leaf_length_to_IN_length" ).GetSample( date ).value;
  n = instance.GetTAttribute( "n" ).GetSample( date ).value;
  ratioINPed = instance.GetTAttribute( "ratio_INPed" ).GetSample( date ).value;
  peduncleDiam = instance.GetTAttribute( "peduncle_diam" ).GetSample( date ).value;

  sample.date = date;
  sample.value = leafLength;
  entityPeduncle.GetTAttribute( "leafLength" ).SetSample( sample );

  sample.date = date;
  sample.value = leafWidth;
  entityPeduncle.GetTAttribute( "leafWidth" ).SetSample( sample );

  sample.date = date;
  sample.value = MGR;
  entityPeduncle.GetTAttribute( "MGR" ).SetSample( sample );

  sample.date = date;
  sample.value = plasto;
  entityPeduncle.GetTAttribute( "plasto" ).SetSample( sample );

  sample.date = date;
  sample.value = ligulo;
  entityPeduncle.GetTAttribute( "ligulo" ).SetSample( sample );

  sample.date = date;
  sample.value = Tb;
  entityPeduncle.GetTAttribute( "Tb" ).SetSample( sample );

  sample.date = date;
  sample.value = resp_INER;
  entityPeduncle.GetTAttribute( "resp_INER" ).SetSample( sample );

  sample.date = date;
  sample.value = LIN1;
  entityPeduncle.GetTAttribute( "LIN1" ).SetSample( sample );

  sample.date = date;
  sample.value = IN_A;
  entityPeduncle.GetTAttribute( "IN_A" ).SetSample( sample );

  sample.date = date;
  sample.value = IN_B;
  entityPeduncle.GetTAttribute( "IN_B" ).SetSample( sample );

  sample.date = date;
  sample.value = density_IN;
  entityPeduncle.GetTAttribute( "density_IN" ).SetSample( sample );

  sample.date = date;
  sample.value = MaximumReserveInInternode;
  entityPeduncle.GetTAttribute( "MaximumReserveInInternode" ).SetSample( sample );

  sample.date = date;
  sample.value = stock_mainstem;
  entityPeduncle.GetTAttribute( "stock_culm" ).SetSample( sample );

  sample.date = date;
  sample.value = leaf_width_to_IN_diameter;
  entityPeduncle.GetTAttribute( "leaf_width_to_IN_diameter" ).SetSample( sample );

  sample.date = date;
  sample.value = leaf_length_to_IN_length;
  entityPeduncle.GetTAttribute( "leaf_length_to_IN_length" ).SetSample( sample );

  sample.date = date;
  sample.value = ratioINPed;
  entityPeduncle.GetTAttribute( "ratio_INPed" ).SetSample( sample );

  sample.date = date;
  sample.value = peduncleDiam;
  entityPeduncle.GetTAttribute( "peduncle_diam" ).SetSample( sample );

  sample.date = date;
  sample.value = n;
  entityPeduncle.GetTAttribute( "rank" ).SetSample( sample );

  entityPeduncle.SetCurrentState( 1 );
  SRwriteln( "Le pedoncule : " + entityPeduncle.GetName(  ) + " est active" );
}

void MeristemManagerPhytomerLeafActivation( TEntityInstance & instance,    TDateTime const& date = 0,    double const& n = 0)
{
  TEntityInstance entityLeaf;
  double Lef1; double MGR; double plasto; double ligulo; double WLR; double LL_BL; double allo_area; double G_L; double Tb; double resp_LER; double coeffLifespan; double mu;
  Tsample sample;

  entityLeaf = FindLeafAtRank( instance , floattostr( n ) );
  SRwriteln( "La feuille : " + entityLeaf.GetName(  ) + " est activée" );

  Lef1 = instance.GetTAttribute( "Lef1" ).GetSample( date ).value;
  MGR = instance.GetTAttribute( "MGR" ).GetSample( date ).value;
  plasto = instance.GetTAttribute( "plasto" ).GetSample( date ).value;
  ligulo = instance.GetTAttribute( "ligulo" ).GetSample( date ).value;
  WLR = instance.GetTAttribute( "WLR" ).GetSample( date ).value;
  LL_BL = instance.GetTAttribute( "LL_BL" ).GetSample( date ).value;
  allo_area = instance.GetTAttribute( "allo_area" ).GetSample( date ).value;
  G_L = instance.GetTAttribute( "G_L" ).GetSample( date ).value;
  Tb = instance.GetTAttribute( "Tb" ).GetSample( date ).value;
  resp_LER = instance.GetTAttribute( "resp_LER" ).GetSample( date ).value;
  coeffLifespan = instance.GetTAttribute( "coeff_lifespan" ).GetSample( date ).value;
  mu = instance.GetTAttribute( "mu" ).GetSample( date ).value;

  sample = entityLeaf.GetTAttribute( "Lef1" ).GetSample( date );
  sample.value = Lef1;
  entityLeaf.GetTAttribute( "Lef1" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "rank" ).GetSample( date );
  sample.value = n;
  entityLeaf.GetTAttribute( "rank" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "MGR" ).GetSample( date );
  sample.value = MGR;
  entityLeaf.GetTAttribute( "MGR" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "plasto" ).GetSample( date );
  sample.value = plasto;
  entityLeaf.GetTAttribute( "plasto" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "ligulo" ).GetSample( date );
  sample.value = ligulo;
  entityLeaf.GetTAttribute( "ligulo" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "WLR" ).GetSample( date );
  sample.value = WLR;
  entityLeaf.GetTAttribute( "WLR" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "allo_area" ).GetSample( date );
  sample.value = allo_area;
  entityLeaf.GetTAttribute( "allo_area" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "G_L" ).GetSample( date );
  sample.value = G_L;
  entityLeaf.GetTAttribute( "G_L" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "Tb" ).GetSample( date );
  sample.value = Tb;
  entityLeaf.GetTAttribute( "Tb" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "resp_LER" ).GetSample( date );
  sample.value = resp_LER;
  entityLeaf.GetTAttribute( "resp_LER" ).SetSample( sample );
  sample = entityLeaf.GetTAttribute( "LL_BL" ).GetSample( date );
  sample.value = LL_BL;
  entityLeaf.GetTAttribute( "LL_BL" ).SetSample( sample );

  sample = entityLeaf.GetTAttribute( "coeffLifespan" ).GetSample( date );
  sample.value = coeffLifespan;
  entityLeaf.GetTAttribute( "coeffLifespan" ).SetSample( sample );

  sample = entityLeaf.GetTAttribute( "mu" ).GetSample( date );
  sample.value = mu;
  entityLeaf.GetTAttribute( "mu" ).SetSample( sample );

  entityLeaf.SetCurrentState( 1 );

  entityLeaf.SetStartDate( date );
  entityLeaf.InitCreationDate( date );
  entityLeaf.InitNextDate(  );


}

void MeristemManagerPhytomerLeafCreation( TEntityInstance & instance,    TDateTime const& date = 0,    double const& n = 0,    bool const& LL_BL_MODIFIED = false)
{
  std::string name;
  double Lef1; double MGR; double plasto; double ligulo; double WLR; double LL_BL; double allo_area;
  double G_L; double Tb; double resp_LER; double nbleaf_pi; double LL_BL_New; double slope_LL_BL_at_PI;
  double nb_leaf_max_after_PI;
  int exeOrder;
  TEntityInstance newLeaf;
  TEntityInstance entityLeaf;
  TAttribute attributeTmp;
  TAttribute refAttribute;
  Tsample sample;
  TPort refPort;
  std::string previousLeafPredimName; std::string currentLeafPredimName;



  MeristemManagerPhytomerLeafActivation( instance , date , n );

  nb_leaf_max_after_PI = instance.GetTAttribute( "nb_leaf_max_after_PI" ).GetSample( date ).value;
  name = "L" + floattostr( n + nb_leaf_max_after_PI );
  SRwriteln( "nb_leaf_max_after_PI : " + floattostr( nb_leaf_max_after_PI ) );

  SRwriteln( "****  creation de la feuille : " + name + "  ****" );
  newLeaf = ImportLeaf_LE( name , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 );

  // determination de l'ordre d'execution de newLeaf :
  exeOrder = Trunc( instance.GetTAttribute( "exeOrderOfFirstPhytomer" ).GetSample( date ).value + ( 2 * ( n + nb_leaf_max_after_PI ) ) - 1 );
  SRwriteln( "exeOrder : " + floattostr( exeOrder ) );

  newLeaf.SetExeOrder( exeOrder );
  newLeaf.SetStartDate( MAX_DATE );
  newLeaf.InitCreationDate( MAX_DATE );
  newLeaf.InitNextDate(  );
  newLeaf.SetCurrentState( 2000 );
  instance.AddTInstance( newLeaf );

  // connection port <-> attribut pour newLeaf
  previousLeafPredimName = "predim_L" + floattostr( ( n + nb_leaf_max_after_PI ) - 1 );
  currentLeafPredimName = "predim_L" + floattostr( n + nb_leaf_max_after_PI );

  attributeTmp = TAttributeTmp.Create( currentLeafPredimName );
  instance.AddTAttribute( attributeTmp );

  newLeaf.ExternalConnect( [ previousLeafPredimName
                           , previousLeafPredimName
                           , currentLeafPredimName
                           , "DD"
                           , "EDD"
                           , "testIc"
                           , "fcstr"
                           , "n"
                           , "SLA"
                           , "stock"
                           , "Tair"
                           , "plasto_delay"
                           , "thresLER"
                           , "slopeLER"
                           , "FTSW"
                           , "lig"
                           , "P" ] );
  instance.SortTInstance(  );

  // deconnecte le port 'predimOfCurrentLeaf' de la feuille L(n-2) avec
  // l'attribut 'predimOfNewLeaf' et realise une connectique
  // port 'predimOfCurrentLeaf' de feuille L(n-1) <-> attribut 'predimOfNewLeaf'
  // TODO -u Model : ce qui a ci-dessous risque de crasher lorsque trop de destruction de feuille...
  if( ( n>=3 ) )
  {
    if(  ( instance.GetTInstance( "L" + floattostr( n-2 ) ) != 00 ) )
    {
      refPort = instance.GetTInstance( "L" + floattostr( n-2 ) ).GetTPort( "predimOfCurrentLeaf" );
      refPort.ExternalUnconnect( 1 );
    }

    refAttribute = instance.GetTAttribute( "predimOfNewLeaf" );
    instance.GetTInstance( "L" + floattostr( n-1 ) ).GetTPort( "predimOfCurrentLeaf" ).ExternalConnect( refAttribute );
  }
}

void MeristemManagerInternodeCreation( TEntityInstance & instance,    TDateTime const& date = 0,   bool isFirstInternode = true,    double const& n = 0,    double const& stock = 0)
{
  std::string name;
  std::string previousInternodePredimName;
  std::string currentInternodePredimName;
  int internodeNumber;
  double MGR; double plasto; double Tb; double resp_LER; double resp_INER; double LIN1; double IN_A; double IN_B; double density_IN;
  double MaximumReserveInInternode;
  double TT; double coef_plasto_PI; double SumOfBiomassLeafMainstem; double SumOfPlantBiomass;

  double leaf_width_to_IN_diameter; double leaf_length_to_IN_length;
  double leafLength; double leafWidth;

  TAttribute TT_PI_Attribute;
  TAttribute Plasto_Attribute;
  TAttribute StockMainstem_Attribute;
  TAttribute refAttribute;
  TEntityInstance newInternode;
  Tsample sample;
  int exeOrder;
  double stock_mainstem;
  double pos;

  double ligulo;

  TAttribute attributeTmp;
  Tsample sampleTmp;
  TPort refPort;


  if(  isFirstInternode )
  {
    /////////////////////////////////////
    // On crée le 1er entrenoeud
    /////////////////////////////////////

    /////////////////////////////////////////////////////////
    // On récupère la valeur de TT et on la stocke dans TT_PI
    /////////////////////////////////////////////////////////

    TT = instance.GetTAttribute( "TT" ).GetSample( date ).value;
    SRwriteln( "TT : " + floattostr( TT ) );

    TT_PI_Attribute = instance.GetTAttribute( "TT_PI" );
    sample = TT_PI_Attribute.GetSample( date );
    sample.value = TT;

    TT_PI_Attribute.SetSample( sample );
    SRwriteln( "TT_PI : " + floattostr( sample.value ) );

    SumOfBiomassLeafMainstem = instance.GetTAttribute( "sumOfMainstemLeafBiomass" ).GetSample( date ).value;
    SumOfPlantBiomass = instance.GetTAttribute( "sumOfLeafBiomass" ).GetSample( date ).value;
    StockMainstem_Attribute = instance.GetTAttribute( "stock_mainstem" );

    SRwriteln( "sumOfMainstemLeafBiomass --> " + floattostr( SumOfBiomassLeafMainstem ) );
    SRwriteln( "sumOfLeafBiomass         --> " + floattostr( SumOfPlantBiomass ) );
    SRwriteln( "stock                    --> " + floattostr( stock ) );
    SRwriteln( "stock_mainstem           --> " + floattostr( ( SumOfBiomassLeafMainstem *1.0/ SumOfPlantBiomass ) * stock ) );

    sample = StockMainstem_Attribute.GetSample( date );
    sample.value = ( SumOfBiomassLeafMainstem *1.0/ SumOfPlantBiomass ) * stock;

    StockMainstem_Attribute.SetSample( sample );
    // on récupère la valeur de la position de la première entité entrenoeud
    exeOrder = Trunc( instance.GetTAttribute( "exeOrderOfFirstInternode" ).GetSample( date ).value );
    pos = 1;

  }
  else
  {
    exeOrder = FindFirstEmptyPlaceAfterASpecifiedCategory( instance, "Internode" );
    pos = 0;
  }

  internodeNumber = StrToInt( FindLastLigulatedLeafNumberOnCulm( instance ) );
  name = "IN" + IntToStr( internodeNumber );
  leafLength = FindLeafOnSameRankLength( instance , IntToStr( internodeNumber ) );
  leafWidth = FindLeafOnSameRankWidth( instance , IntToStr( internodeNumber ) );
  MGR = instance.GetTAttribute( "MGR" ).GetSample( date ).value;
  plasto = instance.GetTAttribute( "plasto" ).GetSample( date ).value;
  ligulo = instance.GetTAttribute( "ligulo" ).GetSample( date ).value;
  Tb = instance.GetTAttribute( "Tb" ).GetSample( date ).value;
  resp_LER = instance.GetTAttribute( "resp_LER" ).GetSample( date ).value;
  resp_INER = resp_LER;
  LIN1 = instance.GetTAttribute( "LIN1" ).GetSample( date ).value;
  IN_A = instance.GetTAttribute( "IN_A" ).GetSample( date ).value;
  IN_B = instance.GetTAttribute( "IN_B" ).GetSample( date ).value;
  density_IN = instance.GetTAttribute( "density_IN" ).GetSample( date ).value;
  MaximumReserveInInternode = instance.GetTAttribute( "maximumReserveInInternode" ).GetSample( date ).value;
  stock_mainstem = instance.GetTAttribute( "stock_mainstem" ).GetSample( date ).value;
  leaf_width_to_IN_diameter = instance.GetTAttribute( "leaf_width_to_IN_diameter" ).GetSample( date ).value;
  leaf_length_to_IN_length = instance.GetTAttribute( "leaf_length_to_IN_length" ).GetSample( date ).value;


  // on crée le premier entre noeud

  SRwriteln( "****  creation de l\'entrenoeud : " + name + "  ****" );

  newInternode = ImportInternode_LE( name , MGR , plasto , Tb , resp_INER , LIN1 , IN_A , IN_B , density_IN , leaf_width_to_IN_diameter , leaf_length_to_IN_length , leafLength , leafWidth , MaximumReserveInInternode , pos , 1 );

  //showmessage(inttostr(exeorder));

  newInternode.SetExeOrder( exeOrder );
  newInternode.SetStartDate( date );
  newInternode.InitCreationDate( date );
  newInternode.InitNextDate(  );

  instance.AddTInstance( newInternode );

  newInternode.ExternalConnect( [ "DD"
                                , "EDD"
                                , "testIc"
                                , "fcstr"
                                , "n"
                                , "stock_mainstem"
                                , "Tair"
                                , "plasto_delay"
                                , "thresINER"
                                , "slopeINER"
                                , "FTSW"
                                , "P" ] );
  instance.SortTInstance(  );
}

void MeristemManagerPhytomerInternodeCreation( TEntityInstance & instance,    TDateTime const& date = 0,   bool isFirstInternode = true,    double const& n = 0,    double const& stock = 0)
{
  std::string name;
  TEntityInstance newInternode;
  double nb_leaf_max_after_PI;
  int exeOrder;
  Tsample sample;


  nb_leaf_max_after_PI = instance.GetTAttribute( "nb_leaf_max_after_PI" ).GetSample( date ).value;

  exeOrder = Trunc( instance.GetTAttribute( "exeOrderOfFirstPhytomer" ).GetSample( date ).value + ( 2 * ( n + nb_leaf_max_after_PI - 1 ) ) );

  name = "IN" + floattostr( n + nb_leaf_max_after_PI );

  SRwriteln( "****  creation de l\'entrenoeud : " + name + "  ****" );

  newInternode = ImportInternode_LE( name , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 );

  sample.date = date;
  sample.value = n + nb_leaf_max_after_PI;
  newInternode.GetTAttribute( "rank" ).SetSample( sample );

  //showmessage(inttostr(exeorder));

  newInternode.SetExeOrder( exeOrder );
  newInternode.SetStartDate( date );
  newInternode.InitCreationDate( date );
  newInternode.SetCurrentState( 2000 );
  newInternode.InitNextDate(  );

  instance.AddTInstance( newInternode );

  newInternode.ExternalConnect( [ "DD"
                                , "EDD"
                                , "testIc"
                                , "fcstr"
                                , "n"
                                , "stock_mainstem"
                                , "Tair"
                                , "plasto_delay"
                                , "thresINER"
                                , "slopeINER"
                                , "FTSW"
                                , "P" ] );
  instance.SortTInstance(  );
}

void MeristemManagerPhytomerPeduncleCreation( TEntityInstance & instance,    TDateTime const& date = 0,   bool isFirstInternode = true,    double const& n = 0,    double const& stock = 0)
{
  std::string name;
  std::string previousInternodePredimName;
  std::string currentInternodePredimName;
  int internodeNumber;
  double MGR; double plasto; double Tb; double resp_LER; double resp_INER; double LIN1; double IN_A; double IN_B; double density_IN;
  double MaximumReserveInInternode;
  double TT; double coef_plasto_PI; double SumOfBiomassLeafMainstem; double SumOfPlantBiomass;

  double leaf_width_to_IN_diameter; double leaf_length_to_IN_length;
  double leafLength; double leafWidth;

  TAttribute TT_PI_Attribute;
  TAttribute Plasto_Attribute;
  TAttribute StockMainstem_Attribute;
  TAttribute refAttribute;
  TEntityInstance newPeduncle;
  Tsample sample;
  int exeOrder;
  double stock_mainstem;
  double pos;

  double ligulo;

  TAttribute attributeTmp;
  Tsample sampleTmp;
  TPort refPort;



  // on récupère la valeur de la position de la première entité entrenoeud
  exeOrder = Trunc( instance.GetTAttribute( "exeOrderOfPeduncle" ).GetSample( date ).value );
  name = "Ped0";
  SRwriteln( "exeOrderOfPeduncle" + floattostr( exeOrder ) );


  // on crée le premier entre noeud

  SRwriteln( "****  creation du pedoncule : " + name + "  ****" );

  newPeduncle = ImportPeduncle_LE( name , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 );

  newPeduncle.SetExeOrder( exeOrder );
  newPeduncle.SetStartDate( date );
  newPeduncle.InitCreationDate( date );
  newPeduncle.InitNextDate(  );
  newPeduncle.SetCurrentState( 2000 );

  instance.AddTInstance( newPeduncle );

  newPeduncle.ExternalConnect( [ "DD"
                                , "EDD"
                                , "testIc"
                                , "fcstr"
                                , "n"
                                , "stock_mainstem"
                                , "Tair"
                                , "plasto_delay"
                                , "thresINER"
                                , "slopeINER"
                                , "FTSW"
                                , "P" ] );
  instance.SortTInstance(  );
}


void MeristemManagerPeduncleCreation( TEntityInstance & instance,    TDateTime const& date = 0,   bool isFirstInternode = true,    double const& n = 0,    double const& stock = 0)
{
  std::string name;
  std::string previousInternodePredimName;
  std::string currentInternodePredimName;
  int internodeNumber;
  double MGR; double plasto; double Tb; double resp_LER; double resp_INER; double LIN1; double IN_A; double IN_B; double density_IN;
  double MaximumReserveInInternode;
  double TT; double coef_plasto_PI; double SumOfBiomassLeafMainstem; double SumOfPlantBiomass;

  double leaf_width_to_IN_diameter; double leaf_length_to_IN_length;
  double leafLength; double leafWidth;

  TAttribute TT_PI_Attribute;
  TAttribute Plasto_Attribute;
  TAttribute StockMainstem_Attribute;
  TAttribute refAttribute;
  TEntityInstance newPeduncle;
  Tsample sample;
  int exeOrder;
  double stock_mainstem;
  double pos;

  double ligulo;

  TAttribute attributeTmp;
  Tsample sampleTmp;
  TPort refPort;



  // on récupère la valeur de la position de la première entité entrenoeud
  exeOrder = Trunc( instance.GetTAttribute( "exeOrderOfPeduncle" ).GetSample( date ).value );
  pos = 1;

  internodeNumber = StrToInt( FindLastLigulatedLeafNumberOnCulm( instance ) ) + 1;
  name = "Ped0";
  leafLength = FindLeafOnSameRankLength( instance , IntToStr( internodeNumber - 1 ) );
  leafWidth = FindLeafOnSameRankWidth( instance , IntToStr( internodeNumber - 1 ) );
  MGR = instance.GetTAttribute( "MGR" ).GetSample( date ).value;
  plasto = instance.GetTAttribute( "plasto" ).GetSample( date ).value;
  ligulo = instance.GetTAttribute( "ligulo" ).GetSample( date ).value;
  Tb = instance.GetTAttribute( "Tb" ).GetSample( date ).value;
  resp_LER = instance.GetTAttribute( "resp_LER" ).GetSample( date ).value;
  resp_INER = resp_LER;
  LIN1 = instance.GetTAttribute( "LIN1" ).GetSample( date ).value;
  IN_A = instance.GetTAttribute( "IN_A" ).GetSample( date ).value;
  IN_B = instance.GetTAttribute( "IN_B" ).GetSample( date ).value;
  density_IN = instance.GetTAttribute( "density_IN" ).GetSample( date ).value;
  MaximumReserveInInternode = instance.GetTAttribute( "maximumReserveInInternode" ).GetSample( date ).value;
  stock_mainstem = instance.GetTAttribute( "stock_mainstem" ).GetSample( date ).value;
  leaf_width_to_IN_diameter = instance.GetTAttribute( "leaf_width_to_IN_diameter" ).GetSample( date ).value;
  leaf_length_to_IN_length = instance.GetTAttribute( "leaf_length_to_IN_length" ).GetSample( date ).value;

  SRwriteln( "exeOrderOfPeduncle" + floattostr( exeOrder ) );


  // on crée le premier entre noeud

  SRwriteln( "****  creation du pedoncule : " + name + "  ****" );

  newPeduncle = ImportPeduncle_LE( name , MGR , plasto , Tb , resp_INER , LIN1 , IN_A , IN_B , density_IN , stock_mainstem , leaf_width_to_IN_diameter , leaf_length_to_IN_length , leafLength , MaximumReserveInInternode , 0 , 0 , pos , 1 );

  //showmessage(inttostr(exeorder));

  newPeduncle.SetExeOrder( exeOrder );
  newPeduncle.SetStartDate( date );
  newPeduncle.InitCreationDate( date );
  newPeduncle.InitNextDate(  );

  instance.AddTInstance( newPeduncle );

  newPeduncle.ExternalConnect( [ "DD"
                                , "EDD"
                                , "testIc"
                                , "fcstr"
                                , "n"
                                , "stock"
                                , "Tair"
                                , "plasto_delay"
                                , "thresINER"
                                , "slopeINER"
                                , "FTSW"
                                , "P" ] );
  instance.SortTInstance(  );
}

void MeristemManagerPhytomerPanicleCreation( TEntityInstance & instance,    TDateTime const& date = 0)
{
  std::string name;
  double spike_creation_rate; double grain_filling_rate; double Tb;
  double gdw_empty; double grain_per_cm_on_panicle; double gdw; double n;
  int exeOrder;
  TEntityInstance newPanicle;

  spike_creation_rate = instance.GetTAttribute( "spike_creation_rate" ).GetSample( date ).value;
  grain_filling_rate = instance.GetTAttribute( "grain_filling_rate" ).GetSample( date ).value;
  gdw_empty = instance.GetTAttribute( "gdw_empty" ).GetSample( date ).value;
  gdw = instance.GetTAttribute( "gdw" ).GetSample( date ).value;
  gdw_empty = gdw_empty * gdw;
  grain_per_cm_on_panicle = instance.GetTAttribute( "grain_per_cm_on_panicle" ).GetSample( date ).value;
  exeOrder = Trunc( instance.GetTAttribute( "exeOrderOfPanicle" ).GetSample( date ).value );
  n = Trunc( instance.GetTAttribute( "n" ).GetSample( date ).value );
  Tb = instance.GetTAttribute( "Tb" ).GetSample( date ).value;


  SRwriteln( "exeOrderOfPanicle : " + IntToStr( exeOrder ) );

  name = "Pan0";

  SRwriteln( "****  creation de la panicule : " + name + "  ****" );

  newPanicle = ImportPanicle_LE( name , spike_creation_rate , grain_filling_rate , grain_per_cm_on_panicle , gdw_empty , gdw , Tb , n , 1 );

  newPanicle.SetExeOrder( exeOrder );
  newPanicle.SetStartDate( date );
  newPanicle.InitCreationDate( date );
  newPanicle.InitNextDate(  );
  newPanicle.SetCurrentState( 2000 );

  instance.AddTInstance( newPanicle );

  newPanicle.ExternalConnect( [ "DD"
                              , "EDD"
                              , "testIc"
                              , "fcstr"
                              , "n"
                              , "Tair"
                              , "plasto_delay"
                              , "thresLER"
                              , "slopeLER"
                              , "FTSW"
                              , "P" ] );


  instance.SortTInstance(  );
}


void MeristemManagerPanicleCreation( TEntityInstance & instance,    TDateTime const& date = 0)
{
  std::string name;
  double spike_creation_rate; double grain_filling_rate; double Tb;
  double gdw_empty; double grain_per_cm_on_panicle; double gdw; double n;
  int exeOrder;
  TEntityInstance newPanicle;

  spike_creation_rate = instance.GetTAttribute( "spike_creation_rate" ).GetSample( date ).value;
  grain_filling_rate = instance.GetTAttribute( "grain_filling_rate" ).GetSample( date ).value;
  gdw_empty = instance.GetTAttribute( "gdw_empty" ).GetSample( date ).value;
  gdw = instance.GetTAttribute( "gdw" ).GetSample( date ).value;
  gdw_empty = gdw_empty * gdw;
  grain_per_cm_on_panicle = instance.GetTAttribute( "grain_per_cm_on_panicle" ).GetSample( date ).value;
  exeOrder = Trunc( instance.GetTAttribute( "exeOrderOfPanicle" ).GetSample( date ).value );
  n = Trunc( instance.GetTAttribute( "n" ).GetSample( date ).value );
  Tb = instance.GetTAttribute( "Tb" ).GetSample( date ).value;


  SRwriteln( "exeOrderOfPanicle : " + IntToStr( exeOrder ) );

  name = "Pan0";

  SRwriteln( "****  creation de la panicule : " + name + "  ****" );

  newPanicle = ImportPanicle_LE( name , spike_creation_rate , grain_filling_rate , grain_per_cm_on_panicle , gdw_empty , gdw , Tb , n , 1 );

  newPanicle.SetExeOrder( exeOrder );
  newPanicle.SetStartDate( date );
  newPanicle.InitCreationDate( date );
  newPanicle.InitNextDate(  );

  instance.AddTInstance( newPanicle );

  newPanicle.ExternalConnect( [ "DD"
                              , "EDD"
                              , "testIc"
                              , "fcstr"
                              , "n"
                              , "Tair"
                              , "plasto_delay"
                              , "thresLER"
                              , "slopeLER"
                              , "FTSW"
                              , "P" ] );


  instance.SortTInstance(  );
}



void TillerManagerPanicleCreation( TEntityInstance & instance,    TDateTime const& date = 0)
{
  std::string instanceName;
  std::string name;
  double spike_creation_rate; double grain_filling_rate;
  double gdw_empty; double gdw; double grain_per_cm_on_panicle; double Tb;
  double phenostage;
  int exeOrder;
  TEntityInstance newPanicle;
  TEntityInstance refMeristem;

  refMeristem = instance.GetFather(  ) as TEntityInstance;
  spike_creation_rate = refMeristem.GetTAttribute( "spike_creation_rate" ).GetSample( date ).value;
  grain_filling_rate = refMeristem.GetTAttribute( "grain_filling_rate" ).GetSample( date ).value;
  gdw_empty = refMeristem.GetTAttribute( "gdw_empty" ).GetSample( date ).value;
  gdw = refMeristem.GetTAttribute( "gdw" ).GetSample( date ).value;
  gdw_empty = gdw_empty * gdw;
  grain_per_cm_on_panicle = refMeristem.GetTAttribute( "grain_per_cm_on_panicle" ).GetSample( date ).value;
  exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfPanicle" ).GetSample( date ).value );
  phenostage = refMeristem.GetTAttribute( "n" ).GetSample( date ).value;
  Tb = refMeristem.GetTAttribute( "Tb" ).GetSample( date ).value;

  SRwriteln( "exeOrderOfPanicle : " + IntToStr( exeOrder ) );

  instanceName = instance.GetName(  );
  Delete( instanceName , 1 , 2 );

  name = "Pan" + instanceName;


  SRwriteln( "****  creation de la panicule : " + name + "  ****" );

  newPanicle = ImportPanicle_LE( name , spike_creation_rate , grain_filling_rate , grain_per_cm_on_panicle , gdw_empty , gdw , Tb , phenostage , 0 );

  newPanicle.SetExeOrder( exeOrder );
  newPanicle.SetStartDate( date );
  newPanicle.InitCreationDate( date );
  newPanicle.InitNextDate(  );

  instance.AddTInstance( newPanicle );

  newPanicle.ExternalConnect( [ "degreeDayForInternodeInitiation"
                              , "degreeDayForInternodeRealization"
                              , "testIc"
                              , "fcstr"
                              , "phenoStage"
                              , "Tair"
                              , "plasto_delay"
                              , "thresLER"
                              , "slopeLER"
                              , "FTSW"
                              , "P" ] );


  instance.SortTInstance(  );
}

void TillerManagerPhytomerPanicleActivation( TEntityInstance & instance)
{
  TEntityInstance entityPanicle;

  entityPanicle = FindPanicle( instance );
  entityPanicle.SetCurrentState( 1 );
  SRwriteln( "La panicule : " + entityPanicle.GetName(  ) + " est activee" );
}

void TillerManagerPhytomerPeduncleActivation( TEntityInstance & instance,   TDateTime const& date)
{
  TEntityInstance entityPeduncle; TEntityInstance entityInternode; TEntityInstance refMeristem;
  std::string internodeNumber; std::string name;
  double leafLength; double leafWidth; double MGR; double plasto; double Tb; double resp_LER; double resp_INER; double LIN1;
  double IN_A; double IN_B; double density_IN; double MaximumReserveInInternode; double stock_mainstem;
  double leaf_width_to_IN_diameter; double leaf_length_to_IN_length; double ligulo; double n;
  double ratioINPed; double peduncleDiam;
  Tsample sample;

  entityPeduncle = FindPeduncle( instance );

  refMeristem = instance.GetFather(  ) as TEntityInstance;

  internodeNumber = FindLastLigulatedLeafNumberOnCulm( instance );

  SRwriteln( "Last ligulated leaf : " + internodeNumber );

  name = "IN" + internodeNumber;

  entityInternode = FindInternodeAtRank( instance , internodeNumber );


  leafLength = FindLeafOnSameRankLength( instance , internodeNumber );
  leafWidth = FindLeafOnSameRankWidth( instance , internodeNumber );
  MGR = refMeristem.GetTAttribute( "MGR" ).GetSample( date ).value;
  plasto = refMeristem.GetTAttribute( "plasto" ).GetSample( date ).value;
  ligulo = refMeristem.GetTAttribute( "ligulo" ).GetSample( date ).value;
  Tb = refMeristem.GetTAttribute( "Tb" ).GetSample( date ).value;
  resp_LER = refMeristem.GetTAttribute( "resp_LER" ).GetSample( date ).value;
  resp_INER = resp_LER;
  LIN1 = refMeristem.GetTAttribute( "LIN1" ).GetSample( date ).value;
  IN_A = refMeristem.GetTAttribute( "IN_A" ).GetSample( date ).value;
  IN_B = refMeristem.GetTAttribute( "IN_B" ).GetSample( date ).value;
  density_IN = refMeristem.GetTAttribute( "density_IN" ).GetSample( date ).value;
  MaximumReserveInInternode = refMeristem.GetTAttribute( "maximumReserveInInternode" ).GetSample( date ).value;
  stock_mainstem = refMeristem.GetTAttribute( "stock_mainstem" ).GetSample( date ).value;
  leaf_width_to_IN_diameter = refMeristem.GetTAttribute( "leaf_width_to_IN_diameter" ).GetSample( date ).value;
  leaf_length_to_IN_length = refMeristem.GetTAttribute( "leaf_length_to_IN_length" ).GetSample( date ).value;
  n = refMeristem.GetTAttribute( "n" ).GetSample( date ).value;
  ratioINPed = refMeristem.GetTAttribute( "ratio_INPed" ).GetSample( date ).value;
  peduncleDiam = refMeristem.GetTAttribute( "peduncle_diam" ).GetSample( date ).value;

  sample.date = date;
  sample.value = leafLength;
  entityPeduncle.GetTAttribute( "leafLength" ).SetSample( sample );

  sample.date = date;
  sample.value = leafWidth;
  entityPeduncle.GetTAttribute( "leafWidth" ).SetSample( sample );

  sample.date = date;
  sample.value = MGR;
  entityPeduncle.GetTAttribute( "MGR" ).SetSample( sample );

  sample.date = date;
  sample.value = plasto;
  entityPeduncle.GetTAttribute( "plasto" ).SetSample( sample );

  sample.date = date;
  sample.value = ligulo;
  entityPeduncle.GetTAttribute( "ligulo" ).SetSample( sample );

  sample.date = date;
  sample.value = Tb;
  entityPeduncle.GetTAttribute( "Tb" ).SetSample( sample );

  sample.date = date;
  sample.value = resp_INER;
  entityPeduncle.GetTAttribute( "resp_INER" ).SetSample( sample );

  sample.date = date;
  sample.value = LIN1;
  entityPeduncle.GetTAttribute( "LIN1" ).SetSample( sample );

  sample.date = date;
  sample.value = IN_A;
  entityPeduncle.GetTAttribute( "IN_A" ).SetSample( sample );

  sample.date = date;
  sample.value = IN_B;
  entityPeduncle.GetTAttribute( "IN_B" ).SetSample( sample );

  sample.date = date;
  sample.value = density_IN;
  entityPeduncle.GetTAttribute( "density_IN" ).SetSample( sample );

  sample.date = date;
  sample.value = MaximumReserveInInternode;
  entityPeduncle.GetTAttribute( "MaximumReserveInInternode" ).SetSample( sample );

  sample.date = date;
  sample.value = stock_mainstem;
  entityPeduncle.GetTAttribute( "stock_culm" ).SetSample( sample );

  sample.date = date;
  sample.value = leaf_width_to_IN_diameter;
  entityPeduncle.GetTAttribute( "leaf_width_to_IN_diameter" ).SetSample( sample );

  sample.date = date;
  sample.value = leaf_length_to_IN_length;
  entityPeduncle.GetTAttribute( "leaf_length_to_IN_length" ).SetSample( sample );

  sample.date = date;
  sample.value = ratioINPed;
  entityPeduncle.GetTAttribute( "ratio_INPed" ).SetSample( sample );

  sample.date = date;
  sample.value = peduncleDiam;
  entityPeduncle.GetTAttribute( "peduncle_diam" ).SetSample( sample );

  sample.date = date;
  sample.value = n;
  entityPeduncle.GetTAttribute( "rank" ).SetSample( sample );

  entityPeduncle.SetCurrentState( 1 );
  SRwriteln( "Le pedoncule : " + entityPeduncle.GetName(  ) + " est active" );
}



void TillerManagerPhytomerPanicleCreation( TEntityInstance & instance,    TDateTime const& date = 0)
{
  std::string instanceName;
  std::string name;
  double spike_creation_rate; double grain_filling_rate; double Tb;
  double gdw_empty; double gdw; double grain_per_cm_on_panicle;
  double phenostage;
  int exeOrder;
  TEntityInstance newPanicle;
  TEntityInstance refMeristem;

  refMeristem = instance.GetFather(  ) as TEntityInstance;
  spike_creation_rate = refMeristem.GetTAttribute( "spike_creation_rate" ).GetSample( date ).value;
  grain_filling_rate = refMeristem.GetTAttribute( "grain_filling_rate" ).GetSample( date ).value;
  gdw_empty = refMeristem.GetTAttribute( "gdw_empty" ).GetSample( date ).value;
  gdw = refMeristem.GetTAttribute( "gdw" ).GetSample( date ).value;
  gdw_empty = gdw_empty * gdw;
  grain_per_cm_on_panicle = refMeristem.GetTAttribute( "grain_per_cm_on_panicle" ).GetSample( date ).value;
  exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfPanicle" ).GetSample( date ).value );
  phenostage = refMeristem.GetTAttribute( "n" ).GetSample( date ).value;
  Tb = refMeristem.GetTAttribute( "Tb" ).GetSample( date ).value;

  SRwriteln( "exeOrderOfPanicle : " + IntToStr( exeOrder ) );

  instanceName = instance.GetName(  );
  Delete( instanceName , 1 , 2 );

  name = "Pan" + instanceName;


  SRwriteln( "****  creation de la panicule : " + name + "  ****" );

  newPanicle = ImportPanicle_LE( name , spike_creation_rate , grain_filling_rate , grain_per_cm_on_panicle , gdw_empty , gdw , Tb , phenostage , 0 );

  newPanicle.SetExeOrder( exeOrder );
  newPanicle.SetStartDate( date );
  newPanicle.InitCreationDate( date );
  newPanicle.InitNextDate(  );
  newPanicle.SetCurrentState( 2000 );

  instance.AddTInstance( newPanicle );

  newPanicle.ExternalConnect( [ "degreeDayForInternodeInitiation"
                              , "degreeDayForInternodeRealization"
                              , "testIc"
                              , "fcstr"
                              , "phenoStage"
                              , "Tair"
                              , "plasto_delay"
                              , "thresLER"
                              , "slopeLER"
                              , "FTSW"
                              , "P" ] );


  instance.SortTInstance(  );
}


// -----------------------------------------------------------------------------
// procedure SetAllActiveStateToValue
// ----------------------------------
//
// permet de faire passer toutes les TProcInstance à une valeur donnée
// par exemple dans le cas où la plante meurt.
//
// -----------------------------------------------------------------------------

void SetAllActiveStateToValue(TEntityInstance instance,  int value)
{
	double tmp;
  TInstance currentInstance;
  TManager manager;
  int nbInstance;
  int i;

  instance.SetCurrentState( value );
	nbInstance = instance.LengthTInstanceList(  );
  { long i_end = nbInstance ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = instance.GetTInstance( i );
    if(  ( currentInstance is TProcInstance ) )
    {
      if(  ( currentInstance.GetName != "setAliveToDead" ) )
      {
        currentInstance.SetActiveState( value * 2 );
      }
    }
    else
    {
    	if(  ( currentInstance is TEntityInstance ) )
      {
      	SetAllActiveStateToValue( currentInstance as TEntityInstance , value );
      }
    }

  }}
}

// -----------------------------------------------------------------------------
//  fonction MeristemManagerUpdatePlasto
// -------------------------------------
//
// lorsque l'on passe à PI, on change plasto qui devient :
//    plasto = plasto * coef_plasto_PI
//
// cette valeur doit être changée pour toutes les entités, c'est pourquoi cette
// procedure est séparée de la création des feuilles et des entrenoeuds
//
// -----------------------------------------------------------------------------

void MeristemManagerUpdatePlasto( TEntityInstance & instance)
{
  double coef_plasto_PI;
  TAttribute Plasto_Attribute;
  Tsample sample;

  coef_plasto_PI = instance.GetTAttribute( "coef_plasto_PI" ).GetSample( date ).value;
  Plasto_Attribute = instance.GetTAttribute( "plasto" );
  sample = Plasto_Attribute.GetSample( date );
  sample.value = sample.value * coef_plasto_PI;
  Plasto_Attribute.SetSample( sample );
}

// ----------------------------------------------------------------------------
//  fonction MeristemManager_LE
//  ----------------------------
//
/// gere le comportement du méristem
///
/// description :
/// - Au premier pas de simulation (état initial) :
///     - initialisation de certaine feuille
///     - TODO
///     - passage a l'etat 3 si stock>=0
///     - passage a l etat 4 si stock<0
///
/// - etat 2 = comportement lorsque stock>=0 :
///     - TODO
///
/// - etat 3 = comportement lorsque stock<0 :
///     - TODO
///
///
/// @param instance instance d'entité portant le manageur
/// @param date date actuelle de simulation  (par défaut, MIN_DATE)
/// @param state état actuel de l'instance portant le manageur (par défaut, 0)
/// @return nouvel etat
// ---------------------------------------------------------------------------

int MeristemManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0)
{   int result;
  static double/*?*/ const INITIAL = 1;
  static double/*?*/ const LEAFMORPHOGENESIS = 2;
  static double/*?*/ const NOGROWTH = 3;
  static double/*?*/ const PI = 4;
  static double/*?*/ const FLO = 5;
  static double/*?*/ const PRE_FLO = 6;
  static double/*?*/ const NOGROWTH_PI = 7;
  static double/*?*/ const NOGROWTH_FLO = 8;
  static double/*?*/ const DEAD = 1000;

   int newState;
   double stock;
   double boolCrossedPlasto; double n;
   double nbleaf_pi;
   double nb_leaf_max_after_PI;
   double coeff_flo_lag;
   double TT_PI_To_Flo; double TT_PI; double TT;
   double FTSW;
   double Ic;
   double total;



  stock = instance.GetTAttribute( "stock" ).GetSample( date ).value;
  nbleaf_pi = instance.GetTAttribute( "nbleaf_pi" ).GetSample( date ).value;
  n = instance.GetTAttribute( "n" ).GetSample( date ).value;
  boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
  nb_leaf_max_after_PI = instance.GetTAttribute( "nb_leaf_max_after_PI" ).GetSample( date ).value;
  coeff_flo_lag = instance.GetTAttribute( "coeff_flo_lag" ).GetSample( date ).value;
  FTSW = instance.GetTAttribute( "FTSW" ).GetSample( date ).value;
  Ic = instance.GetTAttribute( "Ic" ).GetSample( date ).value;
  TT_PI_To_Flo = instance.GetTAttribute( "TT_PI_To_Flo" ).GetSample( date ).value;
  TT_PI = instance.GetTAttribute( "TT_PI" ).GetSample( date ).value;
  TT = instance.GetTAttribute( "TT" ).GetSample( date ).value;

   switch( state ) {
    case INITIAL: 
    {
      if(  ( stock > 0 ) && ( n < nbleaf_pi ) )
        newState = LEAFMORPHOGENESIS;else
        newState = NOGROWTH;
    }

    case LEAFMORPHOGENESIS: 
    {
    	if(  ( FTSW <= 0 ) || ( Ic == -1 ) )
      {
      	newState = DEAD;
        SetAllActiveStateToValue( instance , DEAD );
      }
      else
      {
	      if(  ( stock <= 0 ) )
	        newState = NOGROWTH;else
	      {
	        if(  ( boolCrossedPlasto < 0 ) )
	          newState = LEAFMORPHOGENESIS;else
	        {
            if(  ( n == nbleaf_pi ) )
            {
              MeristemManagerUpdatePlasto( instance );
              MeristemManagerLeafCreation( instance , date , n , false );
              MeristemManagerInternodeCreation( instance , date , true , n , stock );
              MeristemManagerPanicleCreation( instance , date );
              TillersTransitionToPre_PI_LE( instance , 0 );
              RootTransitionToPI( instance );
              newState = PI;
            }
            else
            {
              MeristemManagerLeafCreation( instance , date , n , false );
              newState = LEAFMORPHOGENESIS;
            }
	        }
	      }
	    }
    }

    case NOGROWTH: 
    {
    	if(  ( FTSW <= 0 ) || ( Ic == -1 ) )
      {
      	newState = DEAD;
        SetAllActiveStateToValue( instance , DEAD );
      }
      else
      {
	      if(  ( stock > 0 ) )
	      {
	        if(  ( n < nbleaf_pi ) )
	        {
	          if(  ( boolCrossedPlasto >= 0 ) )
	          {
	            MeristemManagerLeafCreation( instance , date , n , false );
	          }
	          newState = LEAFMORPHOGENESIS;
	        }
	        else
	        {
	          if(  ( boolCrossedPlasto >= 0 ) )
	          {
              MeristemManagerUpdatePlasto( instance );
              MeristemManagerLeafCreation( instance , date , n , false );
              MeristemManagerInternodeCreation( instance , date , true , n , stock );
              MeristemManagerPanicleCreation( instance , date );
              TillersTransitionToPre_PI_LE( instance , 0 );
              RootTransitionToPI( instance );
              newState = PI;              
	          }
	        }
	      }
	      else
	      {
	        newState = NOGROWTH;
	      }
	    }
    }

    case PI: 
    {
      if(  ( stock <= 0 ) )
      {
        newState = NOGROWTH_PI;
      }
      else
      {
        if(  ( boolCrossedPlasto >= 0 ) )
        {
          if(  ( n <= nbleaf_pi + nb_leaf_max_after_PI ) )
          {
            SRwriteln( "n <= nbleaf_pi + nb_leaf_max_after_PI" );
            MeristemManagerLeafCreation( instance , date , n , true );
            MeristemManagerInternodeCreation( instance , date , false , n , stock );
            newState = PI;
          }
          if(  ( n == ( nbleaf_pi + nb_leaf_max_after_PI + 1 ) ) )
          {
            SRwriteln( "n = (nbleaf_pi + nb_leaf_max_after_PI + 1)" );
            MeristemManagerInternodeCreation( instance , date , false , n , stock );
            newState = PI;
          }
          if(  ( n == ( nbleaf_pi + nb_leaf_max_after_PI + 2 ) ) )
          {
            SRwriteln( "n = (nbleaf_pi + nb_leaf_max_after_PI + 2)" );
            MeristemManagerPeduncleCreation( instance , date , false , n , stock );
            RootTransitionToFLO( instance );
            newState = PRE_FLO;
          }
        }
        else
        {
          newState = PI;
        }
      }
    }

    case NOGROWTH_PI: 
    {
      if(  ( stock <= 0 ) )
      {
        newState = NOGROWTH_PI;
      }
      else
      {
        if(  ( boolCrossedPlasto >= 0 ) )
        {
          if(  ( n <= nbleaf_pi + nb_leaf_max_after_PI ) )
          {
            MeristemManagerLeafCreation( instance , date , n , true );
            MeristemManagerInternodeCreation( instance , date , false , n , stock );
            newState = PI;
          }
          if(  ( n == ( nbleaf_pi + nb_leaf_max_after_PI + 1 ) ) )
          {
            MeristemManagerInternodeCreation( instance , date , false , n , stock );
            newState = PI;
          }
          if(  ( n == ( nbleaf_pi + nb_leaf_max_after_PI + 2 ) ) )
          {
            MeristemManagerPeduncleCreation( instance , date , false , n , stock );
            newState = PRE_FLO;
          }
        }
      }
    }

    case PRE_FLO: 
    {
      if(  ( ( TT - TT_PI ) >= TT_PI_To_Flo ) )
      {
        PanicleTransitionToFLO( instance );
        PeduncleTransitionToFLO( instance );
        newState = FLO;
      }
      else
      {
        newState = PRE_FLO;
      }
    }

    case NOGROWTH_FLO: 
    {
      newState = NOGROWTH_FLO;
    }

    case FLO: 
    {
      if(  ( stock <= 0 ) )
      {
        newState = NOGROWTH_FLO;
      }
    }
    case DEAD: 
    {
    	newState = DEAD;
    }

  }

  SRwriteln( "n        --> " + floattostr( n ) );
  SRwriteln( "nbleafpi --> " + floattostr( nbleaf_pi ) );
  SRwriteln( "nbleafpi + nbleafmaxafterpi --> " + floattostr( nbleaf_pi + nb_leaf_max_after_PI ) );
  SRwriteln( "Meristem Manager, transition de : " + IntToStr( state ) + " --> " + IntToStr( newState ) );

  result = newState;
return result;
}

void MeristemManagerStartInternodeElongation( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0,    bool const& isFirstInternode = false)
{
  TEntityInstance entityInternode;
  std::string internodeNumber;
  std::string name;
  double leafLength; double leafWidth; double MGR; double plasto; double ligulo; double Tb; double resp_LER; double resp_INER; double LIN1; double IN_A; double IN_B;
  double density_IN; double MaximumReserveInInternode; double stock_mainstem; double leaf_width_to_IN_diameter;
  double stock; double SumOfBiomassLeafMainstem; double SumOfPlantBiomass;
  double leaf_length_to_IN_length; double slope_length_IN; double IN_length_to_IN_diam; double coef_lin_IN_diam;
  Tsample sample;
  TAttribute StockMainstem_Attribute;

  if(  isFirstInternode )
  {
    stock = instance.GetTAttribute( "stock" ).GetSample( date ).value;
    SumOfBiomassLeafMainstem = instance.GetTAttribute( "sumOfMainstemLeafBiomass" ).GetSample( date ).value;
    SumOfPlantBiomass = instance.GetTAttribute( "sumOfLeafBiomass" ).GetSample( date ).value;
    StockMainstem_Attribute = instance.GetTAttribute( "stock_mainstem" );

    SRwriteln( "sumOfMainstemLeafBiomass --> " + floattostr( SumOfBiomassLeafMainstem ) );
    SRwriteln( "sumOfLeafBiomass         --> " + floattostr( SumOfPlantBiomass ) );
    SRwriteln( "stock                    --> " + floattostr( stock ) );
    SRwriteln( "stock_mainstem           --> " + floattostr( ( SumOfBiomassLeafMainstem *1.0/ SumOfPlantBiomass ) * stock ) );

    sample = StockMainstem_Attribute.GetSample( date );
    sample.value = ( SumOfBiomassLeafMainstem *1.0/ SumOfPlantBiomass ) * stock;

    StockMainstem_Attribute.SetSample( sample );
  }
  internodeNumber = IntToStr( StrToInt( FindLastLigulatedLeafNumberOnCulm( instance ) ) + 1 );
  name = "IN" + internodeNumber;

  entityInternode = FindInternodeAtRank( instance , internodeNumber );

  SRwriteln( "L\'entrenoeud : " + entityInternode.GetName(  ) + " commence de s\'allonger" );

  leafLength = FindLeafOnSameRankLength( instance , internodeNumber );
  leafWidth = FindLeafOnSameRankWidth( instance , internodeNumber );
  MGR = instance.GetTAttribute( "MGR" ).GetSample( date ).value;
  plasto = instance.GetTAttribute( "plasto" ).GetSample( date ).value;
  ligulo = instance.GetTAttribute( "ligulo" ).GetSample( date ).value;
  Tb = instance.GetTAttribute( "Tb" ).GetSample( date ).value;
  resp_LER = instance.GetTAttribute( "resp_LER" ).GetSample( date ).value;
  resp_INER = resp_LER;
  LIN1 = instance.GetTAttribute( "LIN1" ).GetSample( date ).value;
  IN_A = instance.GetTAttribute( "IN_A" ).GetSample( date ).value;
  IN_B = instance.GetTAttribute( "IN_B" ).GetSample( date ).value;
  density_IN = instance.GetTAttribute( "density_IN" ).GetSample( date ).value;
  MaximumReserveInInternode = instance.GetTAttribute( "maximumReserveInInternode" ).GetSample( date ).value;
  stock_mainstem = instance.GetTAttribute( "stock_mainstem" ).GetSample( date ).value;
  leaf_width_to_IN_diameter = instance.GetTAttribute( "leaf_width_to_IN_diameter" ).GetSample( date ).value;
  leaf_length_to_IN_length = instance.GetTAttribute( "leaf_length_to_IN_length" ).GetSample( date ).value;
  slope_length_IN = instance.GetTAttribute( "slope_length_IN" ).GetSample( date ).value;
  IN_length_to_IN_diam = instance.GetTAttribute( "IN_length_to_IN_diam" ).GetSample( date ).value;
  coef_lin_IN_diam = instance.GetTAttribute( "coef_lin_IN_diam" ).GetSample( date ).value;

  sample.date = date;
  sample.value = leafLength;
  entityInternode.GetTAttribute( "leafLength" ).SetSample( sample );

  sample.date = date;
  sample.value = leafWidth;
  entityInternode.GetTAttribute( "leafWidth" ).SetSample( sample );

  sample.date = date;
  sample.value = MGR;
  entityInternode.GetTAttribute( "MGR" ).SetSample( sample );

  sample.date = date;
  sample.value = plasto;
  entityInternode.GetTAttribute( "plasto" ).SetSample( sample );

  sample.date = date;
  sample.value = ligulo;
  entityInternode.GetTAttribute( "ligulo" ).SetSample( sample );

  sample.date = date;
  sample.value = Tb;
  entityInternode.GetTAttribute( "Tb" ).SetSample( sample );

  sample.date = date;
  sample.value = resp_INER;
  entityInternode.GetTAttribute( "resp_INER" ).SetSample( sample );

  sample.date = date;
  sample.value = LIN1;
  entityInternode.GetTAttribute( "LIN1" ).SetSample( sample );

  sample.date = date;
  sample.value = IN_A;
  entityInternode.GetTAttribute( "IN_A" ).SetSample( sample );

  sample.date = date;
  sample.value = IN_B;
  entityInternode.GetTAttribute( "IN_B" ).SetSample( sample );

  sample.date = date;
  sample.value = density_IN;
  entityInternode.GetTAttribute( "density_IN" ).SetSample( sample );

  sample.date = date;
  sample.value = MaximumReserveInInternode;
  entityInternode.GetTAttribute( "MaximumReserveInInternode" ).SetSample( sample );

  sample.date = date;
  sample.value = stock_mainstem;
  entityInternode.GetTAttribute( "stock_culm" ).SetSample( sample );

  sample.date = date;
  sample.value = leaf_width_to_IN_diameter;
  entityInternode.GetTAttribute( "leaf_width_to_IN_diameter" ).SetSample( sample );

  sample.date = date;
  sample.value = leaf_length_to_IN_length;
  entityInternode.GetTAttribute( "leaf_length_to_IN_length" ).SetSample( sample );

  sample.date = date;
  sample.value = slope_length_IN;
  entityInternode.GetTAttribute( "slope_length_IN" ).SetSample( sample );

  sample.date = date;
  sample.value = IN_length_to_IN_diam;
  entityInternode.GetTAttribute( "IN_length_to_IN_diam" ).SetSample( sample );

  sample.date = date;
  sample.value = coef_lin_IN_diam;
  entityInternode.GetTAttribute( "coef_lin_IN_diam" ).SetSample( sample );

  entityInternode.SetCurrentState( 1 );
  
}

void TillerManagerStartInternodeElongation( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0)
{
  TEntityInstance entityInternode; TEntityInstance refMeristem;
  std::string internodeNumber;
  std::string name;
  double leafLength; double leafWidth; double MGR; double plasto; double ligulo; double Tb; double resp_LER; double resp_INER; double LIN1; double IN_A; double IN_B;
  double density_IN; double MaximumReserveInInternode; double stock_tiller; double leaf_width_to_IN_diameter;
  double stock; double SumOfBiomassLeafMainstem; double SumOfPlantBiomass; double activeLeavesNb;
  double leaf_length_to_IN_length; double slope_length_IN;
  double IN_length_to_IN_diam; double coef_lin_IN_diam;
  int phenosStageAtCreation;
  Tsample sample;
  TAttribute StockMainstem_Attribute;

  refMeristem = ( instance.GetFather(  ) as TEntityInstance );
  activeLeavesNb = instance.GetTAttribute( "activeLeavesNb" ).GetSample( date ).value;
  phenosStageAtCreation = Trunc( ( instance as TEntityInstance ).GetTAttribute( "phenoStageAtCreation" ).GetCurrentSample(  ).value );
  internodeNumber = FindLastLigulatedLeafNumberOnCulm( instance );
  SRwriteln( "internodeNumber --> " + internodeNumber );
  if(  ( internodeNumber == "-1" ) )
  {
    SRwriteln( "Pas de feuille ligulee sur la talle, je passe mon tour" );
  }
  else
  {
    internodeNumber = IntToStr( n - phenosStageAtCreation );
    SRwriteln( "internode number : " + internodeNumber );
    name = "IN" + internodeNumber;

    entityInternode = FindInternodeAtRank( instance , internodeNumber );

    SRwriteln( "L\'entrenoeud : " + entityInternode.GetName(  ) + " commence de s\'allonger" );

    leafLength = FindLeafOnSameRankLength( instance , internodeNumber );
    leafWidth = FindLeafOnSameRankWidth( instance , internodeNumber );
    MGR = refMeristem.GetTAttribute( "MGR" ).GetSample( date ).value;
    plasto = refMeristem.GetTAttribute( "plasto" ).GetSample( date ).value;
    ligulo = refMeristem.GetTAttribute( "ligulo" ).GetSample( date ).value;
    Tb = refMeristem.GetTAttribute( "Tb" ).GetSample( date ).value;
    resp_LER = refMeristem.GetTAttribute( "resp_LER" ).GetSample( date ).value;
    resp_INER = resp_LER;
    LIN1 = refMeristem.GetTAttribute( "LIN1" ).GetSample( date ).value;
    IN_A = refMeristem.GetTAttribute( "IN_A" ).GetSample( date ).value;
    IN_B = refMeristem.GetTAttribute( "IN_B" ).GetSample( date ).value;
    density_IN = refMeristem.GetTAttribute( "density_IN" ).GetSample( date ).value;
    MaximumReserveInInternode = refMeristem.GetTAttribute( "maximumReserveInInternode" ).GetSample( date ).value;
    stock_tiller = instance.GetTAttribute( "stock_tiller" ).GetSample( date ).value;
    leaf_width_to_IN_diameter = refMeristem.GetTAttribute( "leaf_width_to_IN_diameter" ).GetSample( date ).value;
    leaf_length_to_IN_length = refMeristem.GetTAttribute( "leaf_length_to_IN_length" ).GetSample( date ).value;
    slope_length_IN = refMeristem.GetTAttribute( "slope_length_IN" ).GetSample( date ).value;
    IN_length_to_IN_diam = refMeristem.GetTAttribute( "IN_length_to_IN_diam" ).GetSample( date ).value;
    coef_lin_IN_diam = refMeristem.GetTAttribute( "coef_lin_IN_diam" ).GetSample( date ).value;

    sample.date = date;
    sample.value = leafLength;
    entityInternode.GetTAttribute( "leafLength" ).SetSample( sample );

    sample.date = date;
    sample.value = leafWidth;
    entityInternode.GetTAttribute( "leafWidth" ).SetSample( sample );

    sample.date = date;
    sample.value = MGR;
    entityInternode.GetTAttribute( "MGR" ).SetSample( sample );

    sample.date = date;
    sample.value = plasto;
    entityInternode.GetTAttribute( "plasto" ).SetSample( sample );

    sample.date = date;
    sample.value = ligulo;
    entityInternode.GetTAttribute( "ligulo" ).SetSample( sample );

    sample.date = date;
    sample.value = Tb;
    entityInternode.GetTAttribute( "Tb" ).SetSample( sample );

    sample.date = date;
    sample.value = resp_INER;
    entityInternode.GetTAttribute( "resp_INER" ).SetSample( sample );

    sample.date = date;
    sample.value = LIN1;
    entityInternode.GetTAttribute( "LIN1" ).SetSample( sample );

    sample.date = date;
    sample.value = IN_A;
    entityInternode.GetTAttribute( "IN_A" ).SetSample( sample );

    sample.date = date;
    sample.value = IN_B;
    entityInternode.GetTAttribute( "IN_B" ).SetSample( sample );

    sample.date = date;
    sample.value = density_IN;
    entityInternode.GetTAttribute( "density_IN" ).SetSample( sample );

    sample.date = date;
    sample.value = MaximumReserveInInternode;
    entityInternode.GetTAttribute( "MaximumReserveInInternode" ).SetSample( sample );

    sample.date = date;
    sample.value = stock_tiller;
    entityInternode.GetTAttribute( "stock_culm" ).SetSample( sample );

    sample.date = date;
    sample.value = leaf_width_to_IN_diameter;
    entityInternode.GetTAttribute( "leaf_width_to_IN_diameter" ).SetSample( sample );

    sample.date = date;
    sample.value = leaf_length_to_IN_length;
    entityInternode.GetTAttribute( "leaf_length_to_IN_length" ).SetSample( sample );

    sample.date = date;
    sample.value = slope_length_IN;
    entityInternode.GetTAttribute( "slope_length_IN" ).SetSample( sample );

    sample.date = date;
    sample.value = IN_length_to_IN_diam;
    entityInternode.GetTAttribute( "IN_length_to_IN_diam" ).SetSample( sample );

    sample.date = date;
    sample.value = coef_lin_IN_diam;
    entityInternode.GetTAttribute( "coef_lin_IN_diam" ).SetSample( sample );

    entityInternode.SetCurrentState( 1 );
  }
  
}

int MeristemManagerPhytomers_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0)
{   int result;
  //INITIAL = 1;
  static double/*?*/ const LEAFMORPHOGENESIS = 2;
  static double/*?*/ const NOGROWTH = 3;
  static double/*?*/ const PI = 4;
  static double/*?*/ const FLO = 5;
  static double/*?*/ const PRE_FLO = 6;
  static double/*?*/ const NOGROWTH_PI = 7;
  static double/*?*/ const NOGROWTH_FLO = 8;
  static double/*?*/ const ELONG = 9;
  static double/*?*/ const NOGROWTH_ELONG = 10;
  static double/*?*/ const NOGROWTH_PRE_FLO = 11;
  static double/*?*/ const ENDFILLING = 12;
  static double/*?*/ const NOGROWTH_ENDFILLING = 13;
  static double/*?*/ const MATURITY = 14;
  static double/*?*/ const NOGROWTH_MATURITY = 15;
  static double/*?*/ const DEAD = 1000;

   int newState;
   double stock;
   double boolCrossedPlasto; double n;
   double nbleaf_pi;
   double nb_leaf_max_after_PI;
   double coeff_flo_lag;
   double TT_PI_To_Flo; double TT_PI; double TT; double phenostageAtPre_Flo; double phenostage_PRE_FLO_to_FLO;
   double FTSW;
   double Ic;
   double total;
   double nb_leaf_stem_elong; double nb_leaf_param2; double phenostage_to_end_filling; double phenostage_to_maturity;
   std::string newStateStr; std::string oldStateStr;


  stock = instance.GetTAttribute( "stock" ).GetSample( date ).value;
  nbleaf_pi = instance.GetTAttribute( "nbleaf_pi" ).GetSample( date ).value;
  n = instance.GetTAttribute( "n" ).GetSample( date ).value;
  boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
  nb_leaf_max_after_PI = instance.GetTAttribute( "nb_leaf_max_after_PI" ).GetSample( date ).value;
  coeff_flo_lag = instance.GetTAttribute( "coeff_flo_lag" ).GetSample( date ).value;
  FTSW = instance.GetTAttribute( "FTSW" ).GetSample( date ).value;
  Ic = instance.GetTAttribute( "Ic" ).GetSample( date ).value;
  TT_PI_To_Flo = instance.GetTAttribute( "TT_PI_To_Flo" ).GetSample( date ).value;
  TT_PI = instance.GetTAttribute( "TT_PI" ).GetSample( date ).value;
  TT = instance.GetTAttribute( "TT" ).GetSample( date ).value;
  nb_leaf_stem_elong = instance.GetTAttribute( "nb_leaf_stem_elong" ).GetSample( date ).value;
  nb_leaf_param2 = instance.GetTAttribute( "nb_leaf_param2" ).GetSample( date ).value;
  phenostageAtPre_Flo = instance.GetTAttribute( "phenostage_at_PRE_FLO" ).GetSample( date ).value;
  phenostage_PRE_FLO_to_FLO = instance.GetTAttribute( "phenostage_PRE_FLO_to_FLO" ).GetSample( date ).value;
  phenostage_to_end_filling = instance.GetTAttribute( "phenostage_to_end_filling" ).GetSample( date ).value;
  phenostage_to_maturity = instance.GetTAttribute( "phenostage_to_maturity" ).GetSample( date ).value;
  
   switch( state ) {
    case INITIAL: 
    {
      oldStateStr = "INITIAL";
      if(  ( stock >= 0 ) && ( n < nbleaf_pi ) )
      {
        newState = LEAFMORPHOGENESIS;
        newStateStr = "LEAFMORPHOGENESIS";
      }
      else
      {
        newState = NOGROWTH;
        newStateStr = "NOGROWTH";
      }
    }

    case LEAFMORPHOGENESIS: 
    {
      oldStateStr = "LEAFMORPHOGENESIS";
    	if(  ( FTSW <= 0 ) || ( Ic == -1 ) )
      {
      	newState = DEAD;
        newStateStr = "DEAD";
        SetAllActiveStateToValue( instance , DEAD );
      }
      else
      {
	      if(  ( stock == 0 ) )
        {
	        newState = NOGROWTH;
          newStateStr = "NOGROWTH";
        }
	      else if(  ( stock > 0 ) )
	      {
	        if(  ( boolCrossedPlasto < 0 ) )
          {
	          newState = LEAFMORPHOGENESIS;
            newStateStr = "LEAFMORPHOGENESIS";
          }
	        else
	        {
            if(  ( ( n == Trunc( nb_leaf_stem_elong ) ) && ( n < nbleaf_pi ) && ( Trunc( nb_leaf_stem_elong ) < nbleaf_pi ) ) )
            {
              SRwriteln( "elong" );
              MeristemManagerPhytomerLeafCreation( instance , date , n , false );
              MeristemManagerPhytomerInternodeCreation( instance , date , true , n , stock );
              TillersStockIndividualization( instance , date );
              MeristemManagerStartInternodeElongation( instance , date , Trunc( n ) , true );
              TillersTransitionToElong( instance );
              RootTransitionToElong( instance );
              newState = ELONG;
              newStateStr = "ELONG";
            }
            else
            {
              if(  ( n == nbleaf_pi ) )
              {
                SRwriteln( "n = nbleaf_pi, on passe a PI" );
                MeristemManagerPhytomerLeafCreation( instance , date , n , false );
                MeristemManagerPhytomerInternodeCreation( instance , date , true , n , stock );
                TillersStockIndividualization( instance , date );
                MeristemManagerStartInternodeElongation( instance , date , Trunc( n ) , true );
                MeristemManagerPhytomerPanicleCreation( instance , date );
                MeristemManagerPhytomerPeduncleCreation( instance , date );
                MeristemManagerPhytomerPanicleActivation( instance );
                TillersTransitionToPre_PI_LE( instance , state );
                StoreThermalTimeAtPI( instance , date );
                RootTransitionToPI( instance );
                newState = PI;
                newStateStr = "PI";
              }
              else
              {
                MeristemManagerPhytomerLeafCreation( instance , date , n , false );
                MeristemManagerPhytomerInternodeCreation( instance , date , true , n , stock );
                newState = LEAFMORPHOGENESIS;
                newStateStr = "LEAFMORPHOGENESIS";
              }
            }
	        }
	      }
	    }
    }

    case NOGROWTH: 
    {
      oldStateStr = "NOGROWTH";
    	if(  ( FTSW <= 0 ) || ( Ic == -1 ) )
      {
      	newState = DEAD;
        newStateStr = "DEAD";
        SetAllActiveStateToValue( instance , DEAD );
      }
      else
      {
	      if(  ( stock > 0 ) )
	      {
	        if(  ( ( n < Trunc( nb_leaf_stem_elong ) ) && ( n < nbleaf_pi ) ) )
	        {
	          if(  ( boolCrossedPlasto >= 0 ) )
	          {
	            MeristemManagerPhytomerLeafCreation( instance , date , n , false );
              MeristemManagerPhytomerInternodeCreation( instance , date , true , n , stock );
	          }
	          newState = LEAFMORPHOGENESIS;
            newStateStr = "LEAFMORPHOGENESIS";
	        }
	        else
	        {
            if(  ( ( n == Trunc( nb_leaf_stem_elong ) ) && ( n < nbleaf_pi ) && ( Trunc( nb_leaf_stem_elong ) < nbleaf_pi ) ) )
            {
              if(  ( boolCrossedPlasto >= 0 ) )
              {
                MeristemManagerPhytomerLeafCreation( instance , date , n , false );
                MeristemManagerPhytomerInternodeCreation( instance , date , true , n , stock );
                TillersStockIndividualization( instance , date );
                MeristemManagerStartInternodeElongation( instance , date , Trunc( n ) , true );
                TillersTransitionToElong( instance );
                RootTransitionToElong( instance );
                newState = ELONG;
                newStateStr = "ELONG";
              }
            }
            else
            {
              if(  ( n == nbleaf_pi ) )
              {
                if(  ( boolCrossedPlasto >= 0 ) )
  	            {
                  SRwriteln( "On passe de NO_GROWTH à PI" );
                  MeristemManagerPhytomerLeafCreation( instance , date , n , false );
                  MeristemManagerPhytomerInternodeCreation( instance , date , true , n , stock );
                  TillersStockIndividualization( instance , date );
                  MeristemManagerStartInternodeElongation( instance , date , Trunc( n ) , true );
                  MeristemManagerPhytomerPanicleCreation( instance , date );
                  MeristemManagerPhytomerPeduncleCreation( instance , date );
                  MeristemManagerPhytomerPanicleActivation( instance );
                  TillersTransitionToPre_PI_LE( instance , state );
                  StoreThermalTimeAtPI( instance , date );
                  RootTransitionToPI( instance );
                  newState = PI;
                  newStateStr = "PI";
                }
              }
	          }
	        }
	      }
	      else
	      {
	        newState = NOGROWTH;
          newStateStr = "NOGROWTH";
	      }
	    }
    }

    case PI: 
    {
      oldStateStr = "PI";
      if(  ( stock == 0 ) )
      {
        newState = NOGROWTH_PI;
        newStateStr = "NOGROWTH_PI";
      }
      else if(  ( stock  >0 ) )
      {
        if(  ( boolCrossedPlasto >= 0 ) )
        {
          if(  ( n <= nbleaf_pi + nb_leaf_max_after_PI ) )
          {
            SRwriteln( "n <= nbleaf_pi + nb_leaf_max_after_PI" );
            MeristemManagerPhytomerLeafActivation( instance , date , n );
            MeristemManagerStartInternodeElongation( instance , date , Trunc( n ) , false );
            SRwriteln( "activation feuille et entrenoeud n° : " + floattostr( n - 1 ) );
            newState = PI;
            newStateStr = "PI";
          }
          if(  ( n == ( nbleaf_pi + nb_leaf_max_after_PI + 1 ) ) )
          {
            SRwriteln( "n = (nbleaf_pi + nb_leaf_max_after_PI + 1)" );
            MeristemManagerStartInternodeElongation( instance , date , Trunc( n ) , false );
            SRwriteln( "activation entrenoeud n° : " + floattostr( n - 1 ) );
            MeristemManagerPhytomerPeduncleActivation( instance , date );
            isFirstDayOfPRE_FLO = true;
            newState = PRE_FLO;
            newStateStr = "PRE_FLO";
            StorePhenostageAtPre_Flo( instance , date , n );
          }
        }
        else
        {
          newState = PI;
          newStateStr = "PI";
        }
      }
    }

    case NOGROWTH_PI: 
    {
      oldStateStr = "NOGROWTH_PI";
      if(  ( stock == 0 ) )
      {
        newState = NOGROWTH_PI;
        newStateStr = "NOGROWTH_PI";
      }
      else if(  ( stock > 0 ) )
      {
        if(  ( boolCrossedPlasto >= 0 ) )
        {
          if(  ( n <= nbleaf_pi + nb_leaf_max_after_PI ) )
          {
            MeristemManagerPhytomerLeafActivation( instance , date , n );
            MeristemManagerStartInternodeElongation( instance , date , Trunc( n ) , false );
            newState = PI;
            newStateStr = "PI";
          }
          if(  ( n == ( nbleaf_pi + nb_leaf_max_after_PI + 1 ) ) )
          {
            MeristemManagerStartInternodeElongation( instance , date , Trunc( n ) , false );
            SRwriteln( "activation entrenoeud n° : " + floattostr( n ) );
            MeristemManagerPhytomerPeduncleActivation( instance , date );
            isFirstDayOfPRE_FLO = true;
            newState = PRE_FLO;
            newStateStr = "PRE_FLO";
            StorePhenostageAtPre_Flo( instance , date , n );
          }
        }
        else
        {
          newState = PI;
          newStateStr = "PI";
        }
      }
    }

    case PRE_FLO: 
    {
      oldStateStr = "PRE_FLO";
      if(  ( stock == 0 ) )
      {
        newState = NOGROWTH_PRE_FLO;
        newStateStr = "NOGROWTH_PRE_FLO";
      }
      else if(  ( stock > 0 ) )
      {
        SRwriteln( "currentPhenostage         --> " + floattostr( n ) );
        SRwriteln( "phenostageAtPre_Flo       --> " + floattostr( phenostageAtPre_Flo ) );
        SRwriteln( "phenostage_PRE_FLO_to_FLO --> " + floattostr( phenostage_PRE_FLO_to_FLO ) );
        if(  ( n == ( phenostageAtPre_Flo + phenostage_PRE_FLO_to_FLO ) ) )
        {
          PanicleTransitionToFLO( instance );
          PeduncleTransitionToFLO( instance );
          newState = FLO;
          newStateStr = "FLO";
        }
        else
        {
          newState = PRE_FLO;
          newStateStr = "PRE_FLO";
        }
      }
    }

    case NOGROWTH_PRE_FLO: 
    {
      oldStateStr = "NOGROWTH_PRE_FLO";
      if(  ( stock > 0 ) )
      {
        SRwriteln( "currentPhenostage         --> " + floattostr( n ) );
        SRwriteln( "phenostageAtPre_Flo       --> " + floattostr( phenostageAtPre_Flo ) );
        SRwriteln( "phenostage_PRE_FLO_to_FLO --> " + floattostr( phenostage_PRE_FLO_to_FLO ) );
        if(  ( n == ( phenostageAtPre_Flo + phenostage_PRE_FLO_to_FLO ) ) )
        {
          PanicleTransitionToFLO( instance );
          PeduncleTransitionToFLO( instance );
          newState = FLO;
          newStateStr = "FLO";
        }
        else
        {
          newState = PRE_FLO;
          newStateStr = "PRE_FLO";
        }
      }
      else if(  ( stock == 0 ) )
      {
        newState = NOGROWTH_PRE_FLO;
        newStateStr = "NOGROWTH_PRE_FLO";
      }
    }

    case NOGROWTH_FLO: 
    {
      oldStateStr = "NOGROWTH_FLO";
      SRwriteln( "n                         --> " + floattostr( n ) );
      SRwriteln( "phenostage_to_end_filling --> " + floattostr( phenostage_to_end_filling ) );
      if(  ( stock > 0 ) )
      {
        if(  ( n == phenostage_to_end_filling ) )
        {
          TillersTransitionToEndfilling_LE( instance , FLO );
          PanicleTransitionToEndFilling( instance );
          PeduncleTransitionToEndfilling( instance );
          newState = ENDFILLING;
          newStateStr = "ENDFILLING";
        }
        else
        {
          newState = FLO;
          newStateStr = "FLO";
        }
      }
      else if(  ( stock == 0 ) )
      {
        newState = NOGROWTH_FLO;
        newStateStr = "NOGROWTH_FLO";
      }
    }

    case FLO: 
    {
      oldStateStr = "FLO";
      SRwriteln( "n                         --> " + floattostr( n ) );
      SRwriteln( "phenostage_to_end_filling --> " + floattostr( phenostage_to_end_filling ) );
      if(  ( stock > 0 ) )
      {
        if(  ( n == phenostage_to_end_filling ) )
        {
          newState = ENDFILLING;
          newStateStr = "ENDFILLING";
          TillersTransitionToEndfilling_LE( instance , FLO );
          PanicleTransitionToEndFilling( instance );
          PeduncleTransitionToEndfilling( instance );
        }
        else
        {
          newState = FLO;
          newStateStr = "FLO";
        }
      }
      else if(  ( stock == 0 ) )
      {
        newState = NOGROWTH_FLO;
        newStateStr = "NOGROWTH_FLO";
      }
    }

    case DEAD: 
    {
      oldStateStr = "DEAD";
    	newState = DEAD;
      newStateStr = "DEAD";
      SetAllActiveStateToValue( instance , DEAD );
    }

    case ELONG: 
    {
      oldStateStr = "ELONG";
     	if(  ( FTSW <= 0 ) || ( Ic == -1 ) )
      {
      	newState = DEAD;
        newStateStr = "DEAD";
        SetAllActiveStateToValue( instance , DEAD );
      }
      else
      {
        if(  ( stock == 0 ) )
        {
          newState = NOGROWTH_ELONG;
          newStateStr = "NOGROWTH_ELONG";
        }
        else if(  ( stock > 0 ) )
        {
          if(  ( boolCrossedPlasto >= 0 ) )
          {
            if(  ( ( n >= nb_leaf_stem_elong ) && ( n < nbleaf_pi ) ) )
            {
              MeristemManagerPhytomerLeafCreation( instance , date , n , false );
              MeristemManagerPhytomerInternodeCreation( instance , date , true , n , stock );
              MeristemManagerStartInternodeElongation( instance , date , Trunc( n ) , false );
              newState = ELONG;
              newStateStr = "ELONG";
            }
            else
            {
              if(  ( n == nbleaf_pi ) )
              {
                SRwriteln( "n = nbleaf_pi, on passe a PI" );
                MeristemManagerPhytomerLeafCreation( instance , date , n , false );
                MeristemManagerPhytomerInternodeCreation( instance , date , true , n , stock );
                MeristemManagerStartInternodeElongation( instance , date , Trunc( n ) , false );
                MeristemManagerPhytomerPanicleCreation( instance , date );
                MeristemManagerPhytomerPeduncleCreation( instance , date );
                MeristemManagerPhytomerPanicleActivation( instance );
                TillersTransitionToPI_LE( instance , state );
                StoreThermalTimeAtPI( instance , date );
                TillerStorePhenostageAtPI( instance , date , n );
                RootTransitionToPI( instance );
                newState = PI;
                newStateStr = "PI";
              }
            }
          }
          else
          {
            newState = ELONG;
            newStateStr = "ELONG";
          }
        }
      }
    }

    case NOGROWTH_ELONG: 
    {
      oldStateStr = "NOGROWTH_ELONG";
    	if(  ( FTSW <= 0 ) || ( Ic == -1 ) )
      {
      	newState = DEAD;
        newStateStr = "DEAD";
        SetAllActiveStateToValue( instance , DEAD );
      }
      else
      {
	      if(  ( stock > 0 ) )
	      {
          if(  ( ( n >= nb_leaf_stem_elong ) && ( n < nbleaf_pi ) ) )
          {
            if(  ( boolCrossedPlasto >= 0 ) )
            {
              MeristemManagerPhytomerLeafCreation( instance , date , n , false );
              MeristemManagerPhytomerInternodeCreation( instance , date , true , n , stock );
              MeristemManagerStartInternodeElongation( instance , date , Trunc( n ) , true );
              newState = ELONG;
              newStateStr = "ELONG";
            }
            else
            {
              newState = ELONG;
              newStateStr = "ELONG";
            }
          }
          else
          {
            if(  ( n == nbleaf_pi ) )
            {
              if(  ( boolCrossedPlasto >= 0 ) )
              {
                SRwriteln( "n = nbleaf_pi, on passe a PI" );
                MeristemManagerPhytomerLeafCreation( instance , date , n , false );
                MeristemManagerPhytomerInternodeCreation( instance , date , true , n , stock );
                MeristemManagerStartInternodeElongation( instance , date , Trunc( n ) , true );
                MeristemManagerPhytomerPanicleCreation( instance , date );
                MeristemManagerPhytomerPeduncleCreation( instance , date );
                MeristemManagerPhytomerPanicleActivation( instance );
                TillersTransitionToPI_LE( instance , state );
                StoreThermalTimeAtPI( instance , date );
                TillerStorePhenostageAtPI( instance , date , n );
                RootTransitionToPI( instance );
                newState = PI;
                newStateStr = "PI";
              }
            }
          }
	      }
	      else if(  ( stock == 0 ) )
	      {
	        newState = NOGROWTH_ELONG;
          newStateStr = "NOGROWTH_ELONG";
	      }
	    }
    }

    case ENDFILLING: 
    {
      oldStateStr = "ENDFILLING";
      SRwriteln( "n                      --> " + floattostr( n ) );
      SRwriteln( "phenostage_to_maturity --> " + floattostr( phenostage_to_maturity ) );
      if(  ( stock == 0 ) )
      {
        newState = NOGROWTH_ENDFILLING;
        newStateStr = "NOGROWTH_ENDFILLING";
      }
      else if(  ( stock > 0 ) )
      {
        if(  ( n == phenostage_to_maturity ) )
        {
          newState = MATURITY;
          newStateStr = "MATURITY";
          TillersTransitionToMaturity_LE( instance , MATURITY );
          PanicleTransitionToMaturity( instance );
          PeduncleTransitionToMaturity( instance );
        }
        else
        {
          newState = ENDFILLING;
          newStateStr = "ENDFILLING";
        }
      }
    }

    case NOGROWTH_ENDFILLING: 
    {
      oldStateStr = "NOGROWTH_ENDFILLING";
      SRwriteln( "n                      --> " + floattostr( n ) );
      SRwriteln( "phenostage_to_maturity --> " + floattostr( phenostage_to_maturity ) );
      if(  ( stock > 0 ) )
      {
        if(  ( n == phenostage_to_maturity ) )
        {
          newState = MATURITY;
          newStateStr = "MATURITY";
          TillersTransitionToMaturity_LE( instance , MATURITY );
          PanicleTransitionToMaturity( instance );
          PeduncleTransitionToMaturity( instance );
        }
        else
        {
          newState = ENDFILLING;
          newStateStr = "ENDFILLING";
        }
      }
      else
      {
        newState = NOGROWTH_ENDFILLING;
        newStateStr = "NOGROWTH_ENDFILLING";
      }
    }

    case MATURITY: 
    {
      oldStateStr = "MATURITY";
      if(  ( stock == 0 ) )
      {
        newState = NOGROWTH_MATURITY;
        newStateStr = "NOGROWTH_MATURITY";
      }
      else
      {
        newState = MATURITY;
        newStateStr = "MATURITY";
      }
    }

    case NOGROWTH_MATURITY: 
    {
      oldStateStr = "NOGROWTH_MATURITY";
      if(  ( stock > 0 ) )
      {
        newState = MATURITY;
        newStateStr = "MATURITY";
      }
      else
      {
        newState = NOGROWTH_MATURITY;
        newStateStr = "NOGROWTH_MATURITY";
      }
    }

  }

  SRwriteln( "n                           --> " + floattostr( n ) );
  SRwriteln( "nb_leaf_stem_elong          --> " + floattostr( nb_leaf_stem_elong ) );
  SRwriteln( "nbleafpi                    --> " + floattostr( nbleaf_pi ) );
  SRwriteln( "nbleafpi + nbleafmaxafterpi --> " + floattostr( nbleaf_pi + nb_leaf_max_after_PI ) );
  SRwriteln( "-------------------------------------------------------" );
  SRwriteln( "Meristem Manager Phytomers, transition de : " + oldStateStr + " --> " + newStateStr );

  result = newState;
return result;
}


// ----------------------------------------------------------------------------
//  fonction ThermalTimeManager_LE
//  ------------------------
//
/// gere le comportement de l entité thermal time
///
/// description : lorsqu'il y a du stock (etat 1), realise les calculs. Sinon
/// (etat 2) ne rien faire
///
///
/// @param instance instance d'entité portant le manageur
/// @param date date actuelle de simulation  (par défaut, MIN_DATE)
/// @param state état actuel de l'instance portant le manageur (par défaut, 0)
/// @return nouvelle etat
// ---------------------------------------------------------------------------

int ThermalTimeManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0)
{   int result;
  static double/*?*/ const STOCK_AVAILABLE = 2;
  static double/*?*/ const NO_STOCK = 3;
  static double/*?*/ const DEAD = 1000;

   int newState;
   double stock;


  if(  ( state != DEAD ) )
  {
    stock = instance.GetTAttribute( "stock" ).GetSample( date ).value;
    SRwriteln( "stock --> " + floattostr( stock ) );

    if(  ( stock == 0 ) && ( state == 1 ) )
    {
      // PREMIER JOUR
      // ------------
      newState = STOCK_AVAILABLE;}
    else if(  ( stock > 0 ) )
    {
      // ETAT STOCK DISPONIBLE
      // ---------------------
      newState = STOCK_AVAILABLE;
    }
    else
    {
      // ETAT STOCK INDISPONIBLE
      // -----------------------
      SRwriteln( "*** phyllochrone is increased ***" );
      newState = NO_STOCK;
    }
  }
  else
  {
    newState = DEAD;
  }

  // retourne le nouvel état (pas de modification d'état)
  SRwriteln( "ThermalTime manager new state : " + floattostr( newState ) );
  result = newState;
return result;
}

void TillersTransitionToState( TEntityInstance & instance,   int const& state,   int const& oldState)
{
  int le; int i;
  TInstance currentInstance;
  TEntityInstance currentTEntityInstance;
  TAttribute refTAttribute;
  Tsample sample;
  std::string category;

  le = instance.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = instance.GetTInstance( i );
    if(  ( currentInstance is TEntityInstance ) )
    {
      category = ( currentInstance as TEntityInstance ).GetCategory(  );
      if(  ( category == "Tiller" ) )
      {
        currentTEntityInstance = ( currentInstance as TEntityInstance );
        currentTEntityInstance.SetCurrentState( state );
        SRwriteln( "La talle : " + currentInstance.GetName(  ) + " passe a l\'etat " + IntToStr( state ) );
        refTAttribute = currentTEntityInstance.GetTAttribute( "previousState" );
        sample = refTAttribute.GetCurrentSample(  );
        sample.value = oldState;
        refTAttribute.SetSample( sample );
        SRwriteln( "previousState --> " + IntToStr( oldState ) );
      }
    }
  }}

}

void TillerStorePhenostageAtPI( TEntityInstance & instance,    TDateTime const& date = 0,    double const& phenostage = 0)
{
  int le; int i;
  TInstance currentInstance;
  std::string category;
  TAttribute refAttribute;
  Tsample sample;

  le = instance.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = instance.GetTInstance( i );
    if(  ( currentInstance is TEntityInstance ) )
    {
      category = ( currentInstance as TEntityInstance ).GetCategory(  );
      if(  ( category == "Tiller" ) )
      {
        refAttribute = ( currentInstance as TEntityInstance ).GetTAttribute( "phenoStageAtPI" );
        sample = refAttribute.GetCurrentSample(  );
        sample.value = phenostage;
        refAttribute.SetSample( sample );
      }
    }
  }}
}

void TillersTransitionToElong( TEntityInstance & instance)
{
  int le; int i;
  TInstance currentInstance;
  TEntityInstance currentTEntityInstance;
  std::string nameOfLastLigulatedLeafOnTiller;

  le = instance.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = instance.GetTInstance( i );
    if(  ( currentInstance is TEntityInstance ) )
    {
      currentTEntityInstance = currentInstance as TEntityInstance;
      if(  ( currentTEntityInstance.GetCategory(  ) == "Tiller" ) )
      {
        nameOfLastLigulatedLeafOnTiller = FindLastLigulatedLeafNumberOnCulm( currentTEntityInstance );
        if(  ( nameOfLastLigulatedLeafOnTiller == "-1" ) )
        {
          SRwriteln( "La talle : " + currentTEntityInstance.GetName(  ) + " n\'a pas de feuille ligulée, elle passe à l\'etat PRE_ELONG" );
          currentTEntityInstance.SetCurrentState( 10 );
        }
        else
        {
          SRwriteln( "La talle : " + currentTEntityInstance.GetName(  ) + " a une feuille ligulée, elle passe à l\'etat ELONG" );
          currentTEntityInstance.SetCurrentState( 9 );
        }
      }
    }
  }}
}

void TillersStockIndividualization( TEntityInstance & instance,   TDateTime const& date)
{
  int le; int i;
  TInstance currentInstance;
  TEntityInstance currentTEntityInstance;
  std::string category;
  double SumOfPlantBiomass; double sumOfTillerLeafBiomass; double plantStock;
  TAttribute stockTillerAttribute;
  Tsample sample;

  le = instance.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = instance.GetTInstance( i );
    if(  ( currentInstance is TEntityInstance ) )
    {
      category = ( currentInstance as TEntityInstance ).GetCategory(  );
      if(  ( category == "Tiller" ) )
      {
        currentTEntityInstance = ( currentInstance as TEntityInstance );
        SumOfPlantBiomass = instance.GetTAttribute( "sumOfLeafBiomass" ).GetSample( date ).value;
        sumOfTillerLeafBiomass = currentTEntityInstance.GetTAttribute( "sumOfTillerLeafBiomass" ).GetSample( date ).value;
        plantStock = instance.GetTAttribute( "stock" ).GetSample( date ).value;
        SRwriteln( "sumOfPlantBiomass      --> " + floattostr( SumOfPlantBiomass ) );
        SRwriteln( "sumOfTillerLeafBiomass --> " + floattostr( sumOfTillerLeafBiomass ) );
        SRwriteln( "plantStock             --> " + floattostr( plantStock ) );
        stockTillerAttribute = currentTEntityInstance.GetTAttribute( "stock_tiller" );
        sample = stockTillerAttribute.GetSample( date );
        sample.value = ( sumOfTillerLeafBiomass *1.0/ SumOfPlantBiomass ) * plantStock;
        stockTillerAttribute.SetSample( sample );
        SRwriteln( "La talle : " + currentInstance.GetName(  ) + " propre stock : " + floattostr( stockTillerAttribute.GetSample( date ).value ) );
      }
    }
  }}
}

void TillersTransitionToPre_PI_LE( TEntityInstance & instance,   int const& oldState)
{
  TillersTransitionToState( instance , 5 , oldState );
}

void TillersTransitionToPI_LE( TEntityInstance & instance,   int const& oldState)
{
  TillersTransitionToState( instance , 4 , oldState );
}

void TillersTransitionToEndfilling_LE( TEntityInstance & instance,   int const& oldState)
{
  TillersTransitionToState( instance , 12 , oldState );
}

void TillersTransitionToMaturity_LE( TEntityInstance & instance,   int const& oldState)
{
  TillersTransitionToState( instance , 13 , oldState );
}

void RootTransitionToState( TEntityInstance & instance,   int const& state)
{
  int le;
  int i;
  TInstance currentInstance;

  le = instance.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = instance.GetTInstance( i );
    if(  ( currentInstance is TEntityInstance ) && ( currentInstance.GetName(  ) == "Root" ) )
    {
      ( currentInstance as TEntityInstance ).SetCurrentState( state );
    }
  }}
}

void RootTransitionToElong( TEntityInstance & instance)
{
  RootTransitionToState( instance , 9 );
}

void RootTransitionToPI( TEntityInstance & instance)
{
  RootTransitionToState( instance , 4 );
}

void RootTransitionToFLO( TEntityInstance & instance)
{
  RootTransitionToState( instance , 6 );
}

void PanicleTransitionToFLO( TEntityInstance & instance)
{
  int i1; int i2; int le1; int le2;
  TInstance currentInstance1; TInstance currentInstance2;

  le1 = instance.LengthTInstanceList(  );
  { long i1_end = le1 ; for( i1 = 0 ; i1 < i1_end ; ++i1 )
  {
    currentInstance1 = instance.GetTInstance( i1 );
    if(  ( currentInstance1 is TEntityInstance ) )
    {
      if(  ( ( currentInstance1 as TEntityInstance ).GetCategory(  ) == "Panicle" ) )
      {
        ( currentInstance1 as TEntityInstance ).SetCurrentState( 2 );
      }
      else if(  ( ( currentInstance1 as TEntityInstance ).GetCategory(  ) == "Tiller" ) )
      {
        le2 = ( currentInstance1 as TEntityInstance ).LengthTInstanceList(  );
        { long i2_end = le2 ; for( i2 = 0 ; i2 < i2_end ; ++i2 )
        {
          currentInstance2 = ( currentInstance1 as TEntityInstance ).GetTInstance( i2 );
          if(  ( currentInstance2 is TEntityInstance ) )
          {
            if(  ( ( currentInstance2 as TEntityInstance ).GetCategory(  ) == "Panicle" ) )
            {
              ( currentInstance2 as TEntityInstance ).SetCurrentState( 2 );
            }
          }
        }}
      }
    }
  }}
}

void PeduncleTransitionToFLO( TEntityInstance & instance)
{
  int i1; int i2; int le1; int le2;
  TInstance currentInstance1; TInstance currentInstance2;

  le1 = instance.LengthTInstanceList(  );
  { long i1_end = le1 ; for( i1 = 0 ; i1 < i1_end ; ++i1 )
  {
    currentInstance1 = instance.GetTInstance( i1 );
    if(  ( currentInstance1 is TEntityInstance ) )
    {
      if(  ( ( currentInstance1 as TEntityInstance ).GetCategory(  ) == "Peduncle" ) )
      {
        ( currentInstance1 as TEntityInstance ).SetCurrentState( 500 );
      }
      else if(  ( ( currentInstance1 as TEntityInstance ).GetCategory(  ) == "Tiller" ) )
      {
        le2 = ( currentInstance1 as TEntityInstance ).LengthTInstanceList(  );
        { long i2_end = le2 ; for( i2 = 0 ; i2 < i2_end ; ++i2 )
        {
          currentInstance2 = ( currentInstance1 as TEntityInstance ).GetTInstance( i2 );
          if(  ( currentInstance2 is TEntityInstance ) )
          {
            if(  ( ( currentInstance2 as TEntityInstance ).GetCategory(  ) == "Peduncle" ) )
            {
              ( currentInstance2 as TEntityInstance ).SetCurrentState( 500 );
            }
          }
        }}
      }
    }
  }}
}

void PanicleTransitionToEndFilling( TEntityInstance & instance)
{
  int i1; int i2; int le1; int le2;
  TInstance currentInstance1; TInstance currentInstance2;

  le1 = instance.LengthTInstanceList(  );
  { long i1_end = le1 ; for( i1 = 0 ; i1 < i1_end ; ++i1 )
  {
    currentInstance1 = instance.GetTInstance( i1 );
    if(  ( currentInstance1 is TEntityInstance ) )
    {
      if(  ( ( currentInstance1 as TEntityInstance ).GetCategory(  ) == "Panicle" ) )
      {
        ( currentInstance1 as TEntityInstance ).SetCurrentState( 12 );
      }
      else if(  ( ( currentInstance1 as TEntityInstance ).GetCategory(  ) == "Tiller" ) )
      {
        le2 = ( currentInstance1 as TEntityInstance ).LengthTInstanceList(  );
        { long i2_end = le2 ; for( i2 = 0 ; i2 < i2_end ; ++i2 )
        {
          currentInstance2 = ( currentInstance1 as TEntityInstance ).GetTInstance( i2 );
          if(  ( currentInstance2 is TEntityInstance ) )
          {
            if(  ( ( currentInstance2 as TEntityInstance ).GetCategory(  ) == "Panicle" ) )
            {
              ( currentInstance2 as TEntityInstance ).SetCurrentState( 12 );
            }
          }
        }}
      }
    }
  }}
}

void PeduncleTransitionToEndfilling( TEntityInstance & instance)
{
  int i1; int i2; int le1; int le2;
  TInstance currentInstance1; TInstance currentInstance2;

  le1 = instance.LengthTInstanceList(  );
  { long i1_end = le1 ; for( i1 = 0 ; i1 < i1_end ; ++i1 )
  {
    currentInstance1 = instance.GetTInstance( i1 );
    if(  ( currentInstance1 is TEntityInstance ) )
    {
      if(  ( ( currentInstance1 as TEntityInstance ).GetCategory(  ) == "Peduncle" ) )
      {
        ( currentInstance1 as TEntityInstance ).SetCurrentState( 12 );
      }
      else if(  ( ( currentInstance1 as TEntityInstance ).GetCategory(  ) == "Tiller" ) )
      {
        le2 = ( currentInstance1 as TEntityInstance ).LengthTInstanceList(  );
        { long i2_end = le2 ; for( i2 = 0 ; i2 < i2_end ; ++i2 )
        {
          currentInstance2 = ( currentInstance1 as TEntityInstance ).GetTInstance( i2 );
          if(  ( currentInstance2 is TEntityInstance ) )
          {
            if(  ( ( currentInstance2 as TEntityInstance ).GetCategory(  ) == "Peduncle" ) )
            {
              ( currentInstance2 as TEntityInstance ).SetCurrentState( 12 );
            }
          }
        }}
      }
    }
  }}
}

void PanicleTransitionToMaturity( TEntityInstance & instance)
{
  int i1; int i2; int le1; int le2;
  TInstance currentInstance1; TInstance currentInstance2;

  le1 = instance.LengthTInstanceList(  );
  { long i1_end = le1 ; for( i1 = 0 ; i1 < i1_end ; ++i1 )
  {
    currentInstance1 = instance.GetTInstance( i1 );
    if(  ( currentInstance1 is TEntityInstance ) )
    {
      if(  ( ( currentInstance1 as TEntityInstance ).GetCategory(  ) == "Panicle" ) )
      {
        ( currentInstance1 as TEntityInstance ).SetCurrentState( 15 );
      }
      else if(  ( ( currentInstance1 as TEntityInstance ).GetCategory(  ) == "Tiller" ) )
      {
        le2 = ( currentInstance1 as TEntityInstance ).LengthTInstanceList(  );
        { long i2_end = le2 ; for( i2 = 0 ; i2 < i2_end ; ++i2 )
        {
          currentInstance2 = ( currentInstance1 as TEntityInstance ).GetTInstance( i2 );
          if(  ( currentInstance2 is TEntityInstance ) )
          {
            if(  ( ( currentInstance2 as TEntityInstance ).GetCategory(  ) == "Panicle" ) )
            {
              ( currentInstance2 as TEntityInstance ).SetCurrentState( 15 );
            }
          }
        }}
      }
    }
  }}
}

void PeduncleTransitionToMaturity( TEntityInstance & instance)
{
  int i1; int i2; int le1; int le2;
  TInstance currentInstance1; TInstance currentInstance2;

  le1 = instance.LengthTInstanceList(  );
  { long i1_end = le1 ; for( i1 = 0 ; i1 < i1_end ; ++i1 )
  {
    currentInstance1 = instance.GetTInstance( i1 );
    if(  ( currentInstance1 is TEntityInstance ) )
    {
      if(  ( ( currentInstance1 as TEntityInstance ).GetCategory(  ) == "Peduncle" ) )
      {
        ( currentInstance1 as TEntityInstance ).SetCurrentState( 4 );
      }
      else if(  ( ( currentInstance1 as TEntityInstance ).GetCategory(  ) == "Tiller" ) )
      {
        le2 = ( currentInstance1 as TEntityInstance ).LengthTInstanceList(  );
        { long i2_end = le2 ; for( i2 = 0 ; i2 < i2_end ; ++i2 )
        {
          currentInstance2 = ( currentInstance1 as TEntityInstance ).GetTInstance( i2 );
          if(  ( currentInstance2 is TEntityInstance ) )
          {
            if(  ( ( currentInstance2 as TEntityInstance ).GetCategory(  ) == "Peduncle" ) )
            {
              ( currentInstance2 as TEntityInstance ).SetCurrentState( 4 );
            }
          }
        }}
      }
    }
  }}
}

std::string FindLastLigulatedLeafNumberOnCulm( TEntityInstance & instance)
{   std::string result;
  int le;
  int i;
  TInstance currentInstance;
  TEntityInstance currentEntityInstance;
  int state;
  TDateTime minDate; TDateTime currentDate;
  std::string minName;

  minDate = MIN_DATE;
  minName = "";
  le = instance.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = ( instance as TEntityInstance ).GetTInstance( i );
    if(  ( currentInstance is TEntityInstance ) )
    {
      currentEntityInstance = ( currentInstance as TEntityInstance );
      if(  ( currentEntityInstance.GetCategory(  ) == "Leaf" ) )
      {
        state = currentEntityInstance.GetCurrentState(  );
        if(  ( ( state == 4 ) || ( state == 5 ) ) )
        {
          currentDate = currentEntityInstance.GetCreationDate(  );
          if(  ( currentDate > minDate ) )
          {
            minDate = currentDate;
            minName = currentEntityInstance.GetName(  );
          }
        }
      }
    }
  }}
  if(  ( minName != "" ) )
  {
    SRwriteln( "last ligulated leaf name : " + minName );
    if(  ( instance.GetCategory(  ) != "Tiller" ) )
    {
      //SRwriteln('Sur brin maitre');
      Delete( minName , AnsiPos( "L" , minName ) , 1 );
    }
    else
    {
      //SRwriteln('Sur talle');
      Delete( minName , AnsiPos( "_" , minName ) , Length( minName ) );
      Delete( minName , AnsiPos( "L" , minName ) , 1 );
    }
    //SRwriteln('last ligulated leaf number : ' + minName);
  }
  else
  {
    SRwriteln( "Pas de feuille ligulee sur l\'axe : " + instance.GetName(  ) );
    minName = "-1";
  }
  result = minName;
return result;
}

double FindLeafOnSameRankWidth( TEntityInstance & instance,  std::string rank)
{   double result;
  int le; int i;
  TInstance currentInstance;
  TEntityInstance currentEntityInstance;
  double leafPredim; double LL_BL; double WLR; double leafWidthPredim;
  std::string leafName;
  std::string leafRank;

  leafWidthPredim = 0;
  le = instance.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = ( instance as TEntityInstance ).GetTInstance( i );
    if(  ( currentInstance is TEntityInstance ) )
    {
      currentEntityInstance = ( currentInstance as TEntityInstance );
      if(  ( currentEntityInstance.GetCategory(  ) == "Leaf" ) )
      {
        leafName = currentEntityInstance.GetName(  );
        if(  ( instance.GetCategory(  ) != "Tiller" ) )
        {
          Delete( leafName , AnsiPos( "L" , leafName ) , 1 );
        }
        else
        {
          Delete( leafName , AnsiPos( "_" , leafName ) , Length( leafName ) );
          Delete( leafName , AnsiPos( "L" , leafName ) , 1 );
        }
        if(  ( leafName == rank ) )
        {
          SRwriteln( "Feuille considérée : " + currentEntityInstance.GetName(  ) );
          leafPredim = currentEntityInstance.GetTAttribute( "predim" ).GetCurrentSample(  ).value;
          LL_BL = currentEntityInstance.GetTAttribute( "LL_BL" ).GetCurrentSample(  ).value;
          WLR = currentEntityInstance.GetTAttribute( "WLR" ).GetCurrentSample(  ).value;
          leafWidthPredim = ( leafPredim *1.0/ LL_BL ) * WLR;
          SRwriteln( "leafPredim      --> " + floattostr( leafPredim ) );
          SRwriteln( "LL_BL           --> " + floattostr( LL_BL ) );
          SRwriteln( "WLR             --> " + floattostr( WLR ) );
          SRwriteln( "leafWidthPredim --> " + floattostr( leafWidthPredim ) );
        }
      }
    }
  }}
  SRwriteln( "Feuille de rang : " + rank + " --> widthPredim : " + floattostr( leafWidthPredim ) );
  result = leafWidthPredim;
return result;
}

double FindLeafOnSameRankLength( TEntityInstance & instance,  std::string rank)
{   double result;
  int le; int i;
  TInstance currentInstance;
  TEntityInstance currentEntityInstance;
  double leafLength;
  std::string leafName;
  std::string leafRank;

  le = instance.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = ( instance as TEntityInstance ).GetTInstance( i );
    if(  ( currentInstance is TEntityInstance ) )
    {
      currentEntityInstance = ( currentInstance as TEntityInstance );
      if(  ( currentEntityInstance.GetCategory(  ) == "Leaf" ) )
      {
        leafName = currentEntityInstance.GetName(  );
        if(  ( instance.GetCategory(  ) != "Tiller" ) )
        {
          Delete( leafName , AnsiPos( "L" , leafName ) , 1 );
        }
        else
        {
          Delete( leafName , AnsiPos( "_" , leafName ) , Length( leafName ) );
          Delete( leafName , AnsiPos( "L" , leafName ) , 1 );
        }
        if(  ( leafName == rank ) )
        {
          SRwriteln( "Feuille considérée : " + currentEntityInstance.GetName(  ) );
          leafLength = currentEntityInstance.GetTAttribute( "len" ).GetCurrentSample(  ).value;
        }
      }
    }
  }}
  SRwriteln( "Feuille de rang : " + rank + " --> leafLength : " + floattostr( leafLength ) );
  result = leafLength;
return result;
}

TEntityInstance FindPeduncle( TEntityInstance & instance)
{   TEntityInstance result;
  int le; int i;
  TInstance currentInstance;
  TEntityInstance currentEntityInstance;
  TEntityInstance entityPeduncle;

  le = instance.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = ( instance as TEntityInstance ).GetTInstance( i );
    if(  ( currentInstance is TEntityInstance ) )
    {
      currentEntityInstance = ( currentInstance as TEntityInstance );
      if(  ( currentEntityInstance.GetCategory(  ) == "Peduncle" ) )
      {
        entityPeduncle = currentEntityInstance;
      }
    }
  }}
  result = entityPeduncle;
return result;
}

TEntityInstance FindPanicle( TEntityInstance & instance)
{   TEntityInstance result;
  int le; int i;
  TInstance currentInstance;
  TEntityInstance currentEntityInstance;
  TEntityInstance entityPanicle;

  le = instance.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = ( instance as TEntityInstance ).GetTInstance( i );
    if(  ( currentInstance is TEntityInstance ) )
    {
      currentEntityInstance = ( currentInstance as TEntityInstance );
      if(  ( currentEntityInstance.GetCategory(  ) == "Panicle" ) )
      {
        entityPanicle = currentEntityInstance;
      }
    }
  }}
  result = entityPanicle;
return result;
}
TEntityInstance FindLeafAtRank( TEntityInstance & instance,  std::string rank)
{   TEntityInstance result;
  int le; int i;
  TInstance currentInstance;
  TEntityInstance currentEntityInstance;
  TEntityInstance entityLeaf;
  std::string leafName;
  std::string leafRank;

  le = instance.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = ( instance as TEntityInstance ).GetTInstance( i );
    if(  ( currentInstance is TEntityInstance ) )
    {
      currentEntityInstance = ( currentInstance as TEntityInstance );
      if(  ( currentEntityInstance.GetCategory(  ) == "Leaf" ) )
      {
        leafName = currentEntityInstance.GetName(  );
        if(  ( instance.GetCategory(  ) != "Tiller" ) )
        {
          Delete( leafName , AnsiPos( "L" , leafName ) , 1 );
        }
        else
        {
          Delete( leafName , AnsiPos( "_" , leafName ) , Length( leafName ) );
          Delete( leafName , AnsiPos( "L" , leafName ) , 1 );
        }
        if(  ( leafName == rank ) )
        {
          entityLeaf = currentEntityInstance;
        }
      }
    }
  }}
  SRwriteln( "Feuille de rang : " + rank );
  result = entityLeaf;
return result;
}

TEntityInstance FindInternodeAtRank( TEntityInstance & instance,  std::string rank)
{   TEntityInstance result;
  int le; int i;
  TInstance currentInstance;
  TEntityInstance currentEntityInstance;
  TEntityInstance entityInternode;
  std::string internodeName;

  le = instance.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    currentInstance = ( instance as TEntityInstance ).GetTInstance( i );
    if(  ( currentInstance is TEntityInstance ) )
    {
      currentEntityInstance = ( currentInstance as TEntityInstance );
      if(  ( currentEntityInstance.GetCategory(  ) == "Internode" ) )
      {
        internodeName = currentEntityInstance.GetName(  );
        if(  ( instance.GetCategory(  ) != "Tiller" ) )
        {
          Delete( internodeName , AnsiPos( "IN" , internodeName ) , 2 );
        }
        else
        {
          Delete( internodeName , AnsiPos( "_" , internodeName ) , Length( internodeName ) );
          Delete( internodeName , AnsiPos( "IN" , internodeName ) , 2 );
        }
        if(  ( internodeName == rank ) )
        {
          entityInternode = currentEntityInstance;
        }
      }
    }
  }}
  SRwriteln( "Entrenoeud de rang : " + rank );
  result = entityInternode;
return result;
}

int PanicleManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0)
{   int result;
  static double/*?*/ const PI = 1;
  static double/*?*/ const TRANSITION_TO_FLO = 2;
  static double/*?*/ const FLO = 3;
  static double/*?*/ const ENDFILLING = 12;
  static double/*?*/ const PI_NOSTOCK = 11;
  static double/*?*/ const FLO_NOSTOCK = 13;
  static double/*?*/ const DEAD = 1000;
  static double/*?*/ const MATURITY = 15;

  static double/*?*/ const VEGETATIVE = 2000;

  int newState;
  std::string oldStateStr; std::string newStateStr;
  std::string name;
  double stock;

  name = instance.GetFather(  ).GetName(  );
  if(  ( AnsiContainsStr( name , "T" ) ) )
  {
    SRwriteln( "Sur talle" );
    stock = ( instance.GetFather(  ).GetFather(  ) as TEntityInstance ).GetTAttribute( "stock" ).GetSample( date ).value;
  }
  else
  {
    SRwriteln( "Mainstem" );
    stock = ( instance.GetFather(  ) as TEntityInstance ).GetTAttribute( "stock" ).GetSample( date ).value;
  }
  SRwriteln( "stock --> " + floattostr( stock ) );
   switch( state ) {
    case PI: 
      {
        oldStateStr = "PI";
        if(  ( stock > 0 ) )
        {
          newState = PI;
          newStateStr = "PI";
        }
        else
        {
          newState = PI_NOSTOCK;
          newStateStr = "PI_NOSTOCK";
        }
      }

    case PI_NOSTOCK: 
      {
        oldStateStr = "PI_NOSTOCK";
        if(  ( stock > 0 ) )
        {
          newState = PI;
          newStateStr = "PI";
        }
        else
        {
          newState = PI_NOSTOCK;
          newStateStr = "PI_NOSTOCK";
        }
      }

    case TRANSITION_TO_FLO: 
      {
        oldStateStr = "TRANSITION_TO_FLO";
        newState = FLO;
        newStateStr = "FLO";
      }

    case FLO: 
      {
        oldStateStr = "FLO";
        if(  ( stock > 0 ) )
        {
          newState = FLO;
          newStateStr = "FLO";
        }
        else
        {
          newState = FLO_NOSTOCK;
          newStateStr = "FLO_NOSTOCK";
        }
      }

    case FLO_NOSTOCK: 
      {
        oldStateStr = "FLO_NOSTOCK";
        if(  ( stock > 0 ) )
        {
          newState = FLO;
          newStateStr = "FLO";
        }
        else
        {
          newState = FLO_NOSTOCK;
          newStateStr = "FLO_NOSTOCK";
        }
      }

    case VEGETATIVE: 
      {
        oldStateStr = "VEGETATIVE";
        newState = VEGETATIVE;
        newStateStr = "VEGETATIVE";
      }

    case ENDFILLING: 
     {
        oldStateStr = "ENDFILLING";
        newState = ENDFILLING;
        newStateStr = "ENDFILLING";
     }

     case MATURITY: 
     {
        oldStateStr = "MATURITY";
        newState = MATURITY;
        newStateStr = "MATURITY";
     }

    case DEAD: 
      {
        oldStateStr = "DEAD";
        newState = DEAD;
        newStateStr = "DEAD";
      }


  }
  SRwriteln( "Panicle Manager, transition de : " + oldStateStr + " --> " + newStateStr );
  result = newState;
return result;
}

int PeduncleManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0)
{   int result;
  static double/*?*/ const PREDIM = 1;
  static double/*?*/ const TRANSITION = 10;
  static double/*?*/ const REALIZATION = 2;
  static double/*?*/ const REALIZATION_NOSTOCK = 3;
  static double/*?*/ const MATURITY = 4;
  static double/*?*/ const MATURITY_NOSTOCK = 5;
  static double/*?*/ const ENDFILLING = 12;
  static double/*?*/ const UNKNOWN = -1;
  static double/*?*/ const DEAD = 1000;
  static double/*?*/ const FLO = 500;
  static double/*?*/ const FLO_NOSTOCK = 501;
  static double/*?*/ const VEGETATIVE = 2000;

   int newState;
   double stockPeduncle;
   std::string newStateStr; std::string oldStateStr;
   std::string name;
   double stock;

  name = instance.GetFather(  ).GetName(  );
  if(  ( AnsiContainsStr( name , "T" ) ) )
  {
    SRwriteln( "Sur talle" );
    stock = ( instance.GetFather(  ).GetFather(  ) as TEntityInstance ).GetTAttribute( "stock" ).GetSample( date ).value;}
  else
  {
    SRwriteln( "Mainstem" );
    stock = ( instance.GetFather(  ) as TEntityInstance ).GetTAttribute( "stock" ).GetSample( date ).value;}
  SRwriteln( "stock --> " + floattostr( stock ) );
   switch( state ) {
    // ETAT PREDIMENSIONNEMENT :
    // -----------------------
    case PREDIM: 
      {
        oldStateStr = "PREDIM";
        newState = TRANSITION;
        newStateStr = "TRANSITION";
      }

    // ETAT TRANSITION : (evite que etat 1 et 2 soit fait pendant le meme pas de temps)
    // ---------------
    case TRANSITION: 
      {
        oldStateStr = "TRANSITION";
        newState = REALIZATION;
        newStateStr = "REALIZATION";
      }

    // ETAT REALIZATION
    // ----------------
    case REALIZATION: 
      {
        oldStateStr = "REALIZATION";
        if(  ( stock > 0 ) )
        {
          newState = REALIZATION;
          newStateStr = "REALIZATION";
        }
        else
        {
          newState = REALIZATION_NOSTOCK;
          newStateStr = "REALIZATION_NOSTOCK";
        }
      }

    case REALIZATION_NOSTOCK: 
      {
        oldStateStr = "REALIZATION_NOSTOCK";
        if(  ( stock > 0 ) )
        {
          newState = REALIZATION;
          newStateStr = "REALIZATION";
        }
        else
        {
          newState = REALIZATION_NOSTOCK;
          newStateStr = "REALIZATION_NOSTOCK";
        }
      }

    // ETAT LIGULE
    // -----------
    case MATURITY: 
      {
        oldStateStr = "MATURITY";
        if(  ( stock > 0 ) )
        {
          newState = MATURITY;
          newStateStr = "MATURITY";
        }
        else
        {
          newState = MATURITY_NOSTOCK;
          newStateStr = "MATURITY_NOSTOCK";
        }
      }

    case MATURITY_NOSTOCK: 
      {
        oldStateStr = "MATURITY_NOSTOCK";
        if(  ( stock > 0 ) )
        {
          newState = MATURITY;
          newStateStr = "MATURITY";
        }
        else
        {
          newState = MATURITY_NOSTOCK;
          newStateStr = "MATURITY_NOSTOCK";
        }
      }


    case DEAD: 
      {
        oldStateStr = "DEAD";
        newState = DEAD;
        newStateStr = "DEAD";
      }

    case FLO: 
      {
        oldStateStr = "FLO";
        if(  ( stock > 0 ) )
        {
          newState = FLO;
          newStateStr = "FLO";
        }
        else
        {
          newState = FLO_NOSTOCK;
          newStateStr = "FLO_NOSTOCK";
        }
      }

    case FLO_NOSTOCK: 
      {
        oldStateStr = "FLO_NOSTOCK";
        if(  ( stock > 0 ) )
        {
          newState = FLO;
          newStateStr = "FLO";
        }
        else
        {
          newState = FLO_NOSTOCK;
          newStateStr = "FLO_NOSTOCK";
        }
      }

    case VEGETATIVE: 
      {
        oldStateStr = "VEGETATIVE";
        newState = VEGETATIVE;
        newStateStr = "VEGETATIVE";
      }

    case ENDFILLING: 
      {
        oldStateStr = "ENDFILLING";
        newState = ENDFILLING;
        newStateStr = "ENDFILLING";
      }

      // ETAT NON DEFINIT :
      // ----------------
      default:
      {
        result = UNKNOWN;
        return result;
      }
    }

  // retourne l'état (pas de modification d'état)
  SRwriteln( "**** Peduncle transition de " + oldStateStr + " --> " + newStateStr );
  result = newState;

return result;
}

int EcoMeristemManager_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& state = 0)
{   int result;
  static double/*?*/ const INIT = 1;
  static double/*?*/ const INACTIVE = 2;

  int newState;

  SRwriteln( "**** EcoMeristemManager_LE --> Initial State = " + floattostr( state ) + " ***" );
   switch( state ) {
    case INIT: 
    {
      CreateInitialPhytomers_LE( instance , date );
      newState = INACTIVE;
    }
    case INACTIVE: 
    {
      newState = INACTIVE;
    }
  }
  SRwriteln( "**** EcoMeristemManager_LE --> New State = " + floattostr( newState ) + " ***" );
  result = newState;
return result;
}

void EcoMeristemFirstLeafCreation( TEntityInstance & instance,    TDateTime const& date = 0)
{
  std::string name;
  double Lef1; double MGR; double plasto; double ligulo; double WLR; double LL_BL; double allo_area; double coef_ligulo1; double coeffLifespan; double mu;
  double G_L; double Tb; double resp_LER;
  int exeOrder;
  TEntityInstance newLeaf;
  TModel refFather;
  TInstance refInstance;
  TEntityInstance refMeristem;
  TAttribute attributeTmp;
  TAttribute refAttribute;
  TPort refPort;
  Tsample sample;
  std::string previousLeafPredimName; std::string currentLeafPredimName;
  int le; int i;


  // creation d'une nouvelle feuille
  // -------------------------------
  //

  refFather = ( instance.GetFather(  ) as TModel );
  refMeristem = 00;

  le = refFather.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    refInstance = refFather.GetTInstance( i );
    if(  ( refInstance.GetName(  ) == "EntityMeristem" ) )
    {
      refMeristem = ( refInstance as TEntityInstance );
    }
  }}

  if(  ( refMeristem != 00 ) )
  {
    name = "L1";
    Lef1 = refMeristem.GetTAttribute( "Lef1" ).GetSample( date ).value;
    MGR = refMeristem.GetTAttribute( "MGR_init" ).GetSample( date ).value;
    plasto = refMeristem.GetTAttribute( "plasto_init" ).GetSample( date ).value;
    //ligulo := refMeristem.GetTAttribute('ligulo1').GetSample(date).value;
    coef_ligulo1 = refMeristem.GetTAttribute( "coef_ligulo1" ).GetSample( date ).value;
    ligulo = coef_ligulo1 * plasto;
    refAttribute = refMeristem.GetTAttribute( "ligulo1" );
    sample = refAttribute.GetSample( date );
    sample.date = date;
    sample.value = ligulo;
    refAttribute.SetSample( sample );
    WLR = refMeristem.GetTAttribute( "WLR" ).GetSample( date ).value;
    LL_BL = refMeristem.GetTAttribute( "LL_BL_init" ).GetSample( date ).value;
    allo_area = refMeristem.GetTAttribute( "allo_area" ).GetSample( date ).value;
    G_L = refMeristem.GetTAttribute( "G_L" ).GetSample( date ).value;
    Tb = refMeristem.GetTAttribute( "Tb" ).GetSample( date ).value;
    resp_LER = refMeristem.GetTAttribute( "resp_LER" ).GetSample( date ).value;
    coeffLifespan = refMeristem.GetTAttribute( "coeff_lifespan" ).GetSample( date ).value;
    mu = refMeristem.GetTAttribute( "mu" ).GetSample( date ).value;

    SRwriteln( "****  creation de la feuille : " + name + "  ****" );
    newLeaf = ImportLeaf_LE( name , Lef1 , MGR , plasto , ligulo , WLR , LL_BL , allo_area , G_L , Tb , resp_LER , coeffLifespan , mu , 1 , 1 );

    sample = newLeaf.GetTAttribute( "rank" ).GetSample( date );
    sample.value = 1;
    newLeaf.GetTAttribute( "rank" ).SetSample( sample );

    // determination de l'ordre d'execution de newLeaf :
    // recherche de la premiere place vide apres la premiere entité de type 'Leaf'
    exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfFirstPhytomer" ).GetSample( date ).value ) + 1;
    SRwriteln( "exeOrder : " + IntToStr( exeOrder ) );

    newLeaf.SetExeOrder( exeOrder );
    newLeaf.SetStartDate( date );
    newLeaf.InitCreationDate( date );
    newLeaf.InitNextDate(  );
    refMeristem.AddTInstance( newLeaf );

    // connection port <-> attribut pour newLeaf
    previousLeafPredimName = "zero";
    currentLeafPredimName = "predim_L1";

    attributeTmp = TAttributeTmp.Create( currentLeafPredimName );
    refMeristem.AddTAttribute( attributeTmp );

    newLeaf.ExternalConnect( [ previousLeafPredimName
                             , previousLeafPredimName
                             , currentLeafPredimName
                             , "DD"
                             , "EDD"
                             , "testIc"
                             , "fcstr"
                             , "n"
                             , "SLA"
                             , "stock"
                             , "Tair"
                             , "plasto_delay"
                             , "thresLER"
                             , "slopeLER"
                             , "FTSW"
                             , "lig"
                             , "P" ] );
    refMeristem.SortTInstance(  );
  }
}

void EcoMeristemFirstINCreation( TEntityInstance & instance,    TDateTime const& date = 0)
{
  std::string name;
  TModel refFather;
  TInstance refInstance;
  TEntityInstance refMeristem; TEntityInstance newInternode;
  int le; int i; int exeOrder;
  Tsample sample;


  refFather = ( instance.GetFather(  ) as TModel );
  refMeristem = 00;

  le = refFather.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    refInstance = refFather.GetTInstance( i );
    if(  ( refInstance.GetName(  ) == "EntityMeristem" ) )
    {
      refMeristem = ( refInstance as TEntityInstance );
    }
  }}

  if(  ( refMeristem != 00 ) )
  {
    exeOrder = Trunc( refMeristem.GetTAttribute( "exeOrderOfFirstPhytomer" ).GetSample( date ).value );
    name = "IN1";

    // on crée le premier entre noeud

    SRwriteln( "****  creation de l\'entrenoeud : " + name + "  ****" );

    newInternode = ImportInternode_LE( name , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 );

    sample.date = date;
    sample.value = 1;
    newInternode.GetTAttribute( "rank" ).SetSample( sample );

    newInternode.SetExeOrder( exeOrder );
    newInternode.SetStartDate( date );
    newInternode.InitCreationDate( date );
    newInternode.InitNextDate(  );
    newInternode.SetCurrentState( 2000 );

    refMeristem.AddTInstance( newInternode );

    newInternode.ExternalConnect( [ "DD"
                                  , "EDD"
                                  , "testIc"
                                  , "fcstr"
                                  , "n"
                                  , "stock_mainstem"
                                  , "Tair"
                                  , "plasto_delay"
                                  , "thresINER"
                                  , "slopeINER"
                                  , "FTSW"
                                  , "P" ] );
    refMeristem.SortTInstance(  );
  }
}

void EcoMeristemInitialOtherINCreation( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0)
{
  std::string name;
  int exeOrder; int pos;
  TEntityInstance newInternode;
  Tsample sample;


  exeOrder = Trunc( instance.GetTAttribute( "exeOrderOfFirstPhytomer" ).GetSample( date ).value ) + ( 2 * n );
  pos = 1;
  name = "IN" + IntToStr( n + 1 );

  // on crée le premier entre noeud

  SRwriteln( "****  creation de l\'entrenoeud : " + name + "  ****" );

  newInternode = ImportInternode_LE( name , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 );

  sample.date = date;
  sample.value = n + 1;
  newInternode.GetTAttribute( "rank" ).SetSample( sample );

  //showmessage(inttostr(exeorder));

  newInternode.SetExeOrder( exeOrder );
  newInternode.SetStartDate( date );
  newInternode.InitCreationDate( date );
  newInternode.InitNextDate(  );
  newInternode.SetCurrentState( 2000 );

  instance.AddTInstance( newInternode );

  newInternode.ExternalConnect( [ "DD"
                                , "EDD"
                                , "testIc"
                                , "fcstr"
                                , "n"
                                , "stock_mainstem"
                                , "Tair"
                                , "plasto_delay"
                                , "thresINER"
                                , "slopeINER"
                                , "FTSW"
                                , "P" ] );
  instance.SortTInstance(  );
}

void EcoMeristemInitialOtherLeafCreation( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0)
{
  std::string name;
  int exeOrder;
  TEntityInstance newLeaf;
  TAttribute attributeTmp;
  TAttribute refAttribute;
  TPort refPort;
  std::string previousLeafPredimName; std::string currentLeafPredimName;


  // creation d'une nouvelle feuille
  // -------------------------------
  //
  name = "L" + floattostr( n + 1 );
  SRwriteln( "****  creation de la feuille : " + name + "  ****" );
  newLeaf = ImportLeaf_LE( name , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 );

  // determination de l'ordre d'execution de newLeaf :
  // recherche de la premiere place vide apres la premiere entité de type 'Leaf'
  exeOrder = Trunc( instance.GetTAttribute( "exeOrderOfFirstPhytomer" ).GetSample( date ).value ) + ( 2 * n ) + 1;
  SRwriteln( "exeOrder : " + IntToStr( exeOrder ) );

  newLeaf.SetExeOrder( exeOrder );
  /*newLeaf.SetStartDate(date);
  newLeaf.InitCreationDate(date);*/
  newLeaf.SetStartDate( MAX_DATE );
  newLeaf.InitCreationDate( MAX_DATE );
  newLeaf.InitNextDate(  );
  newLeaf.SetCurrentState( 2000 );

  instance.AddTInstance( newLeaf );


  // connection port <-> attribut pour newLeaf
  previousLeafPredimName = "predim_L" + floattostr( n );
  currentLeafPredimName = "predim_L" + floattostr( n + 1 );

  attributeTmp = TAttributeTmp.Create( currentLeafPredimName );
  instance.AddTAttribute( attributeTmp );

  newLeaf.ExternalConnect( [ previousLeafPredimName
                           , previousLeafPredimName
                           , currentLeafPredimName
                           , "DD"
                           , "EDD"
                           , "testIc"
                           , "fcstr"
                           , "n"
                           , "SLA"
                           , "stock"
                           , "Tair"
                           , "plasto_delay"
                           , "thresLER"
                           , "slopeLER"
                           , "FTSW"
                           , "lig"
                           , "P" ] );
  instance.SortTInstance(  );

  // deconnecte le port 'predimOfCurrentLeaf' de la feuille L(n-2) avec
  // l'attribut 'predimOfNewLeaf' et realise une connectique
  // port 'predimOfCurrentLeaf' de feuille L(n-1) <-> attribut 'predimOfNewLeaf'
  // TODO -u Model : ce qui a ci-dessous risque de crasher lorsque trop de destruction de feuille...
  /*if(n>=3) then
  begin
    if (instance.GetTInstance('L' + FloatToStr(n-2)) <> nil) then
    begin
      refPort := instance.GetTInstance('L' + FloatToStr(n-2)).GetTPort('predimOfCurrentLeaf');
      refPort.ExternalUnconnect(1);
    end;

    refAttribute := instance.GetTAttribute('predimOfNewLeaf');
    instance.GetTInstance('L' + FloatToStr(n-1)).GetTPort('predimOfCurrentLeaf').ExternalConnect(refAttribute);
  end;       */
}

void CreateFirstPhytomer_LE( TEntityInstance & instance,    TDateTime const& date = 0)
{
  EcoMeristemFirstINCreation( instance , date );
  EcoMeristemFirstLeafCreation( instance , date );
}

void CreateOtherPhytomers_LE( TEntityInstance & instance,    TDateTime const& date = 0,    int const& n = 0)
{
  EcoMeristemInitialOtherINCreation( instance , date , n );
  EcoMeristemInitialOtherLeafCreation( instance , date , n );
}

void CreateInitialPhytomers_LE( TEntityInstance & instance,    TDateTime const& date = 0)
{
  double nb_leaf_max_after_PI;
  int i; int le;
  TModel refFather;
  TEntityInstance refMeristem;
  TInstance refInstance;

  CreateFirstPhytomer_LE( instance , date );
  refFather = ( instance.GetFather(  ) as TModel );
  refMeristem = 00;

  le = refFather.LengthTInstanceList(  );
  { long i_end = le ; for( i = 0 ; i < i_end ; ++i )
  {
    refInstance = refFather.GetTInstance( i );
    if(  ( refInstance.GetName(  ) == "EntityMeristem" ) )
    {
      refMeristem = ( refInstance as TEntityInstance );
    }
  }}

  if(  ( refMeristem != 00 ) )
  {

    nb_leaf_max_after_PI = refMeristem.GetTAttribute( "nb_leaf_max_after_PI" ).GetSample( date ).value;
    { long i_end = Trunc( nb_leaf_max_after_PI )+1 ; for( i = 1 ; i < i_end ; ++i )
    {
      CreateOtherPhytomers_LE( refMeristem , date , i );
    }}
  }
}

void StoreThermalTimeAtPI( TEntityInstance & instance,    TDateTime const& date = 0)
{
  double TT;
  TAttribute refAttributeTTPI;
  Tsample sample;

  TT = instance.GetTAttribute( "TT" ).GetSample( date ).value;
  refAttributeTTPI = instance.GetTAttribute( "TT_PI" );
  sample.date = date;
  sample.value = TT;
  refAttributeTTPI.SetSample( sample );
}

void StorePhenostageAtPre_Flo( TEntityInstance & instance,    TDateTime const& date = 0,    double const& phenostage = 0)
{
  TAttribute refPhenostagePre_Flo_To_Flo;
  Tsample sample;

  refPhenostagePre_Flo_To_Flo = instance.GetTAttribute( "phenostage_at_PRE_FLO" );
  sample.date = date;
  sample.value = phenostage;
  refPhenostagePre_Flo_To_Flo.SetSample( sample );
}

void TillerStorePhenostageAtPre_Flo( TEntityInstance & instance,    TDateTime const& date = 0,    double const& phenostage = 0)
{
  TAttribute refPhenostageAtPre_Flo;
  Tsample sample;

  refPhenostageAtPre_Flo = instance.GetTAttribute( "phenoStageAtPreFlo" );
  sample.date = date;
  sample.value = phenostage;
  refPhenostageAtPre_Flo.SetSample( sample );
}

void SetAllInstanceToEndFilling( TEntityInstance & instance)
{
  int i1; int i2; int le1; int le2;
  TInstance currentInstance1; TInstance currentInstance2;
  std::string category1; std::string category2;

  le1 = ( instance as TEntityInstance ).LengthTAttributeList(  );
  { long i1_end = le1 ; for( i1 = 0 ; i1 < i1_end ; ++i1 )
  {
    currentInstance1 = ( instance as TEntityInstance ).GetTInstance( i1 );
    if(  ( currentInstance1 is TEntityInstance ) )
    {
      category1 = ( currentInstance1 as TEntityInstance ).GetCategory(  );
      if(  ( category1 == "Internode" ) || ( category1 == "Leaf" ) || ( category1 == "Panicle" ) || ( category1 == "Peduncle" ) || ( category1 == "Root" ) )
      {
        ( currentInstance1 as TEntityInstance ).SetCurrentState( 12 );
        SRwriteln( currentInstance1.GetName(  ) + " passe a END FILLING" );
      }
      else if(  ( category1 == "Tiller" ) )
      {
        ( currentInstance1 as TEntityInstance ).SetCurrentState( 12 );
        le2 = ( currentInstance1 as TEntityInstance ).LengthTInstanceList(  );
        { long i2_end = le2 ; for( i2 = 0 ; i2 < i2_end ; ++i2 )
        {
          currentInstance2 = ( currentInstance1 as TEntityInstance ).GetTInstance( i2 );
          if(  ( currentInstance2 is TEntityInstance ) )
          {
            category2 = ( currentInstance2 as TEntityInstance ).GetCategory(  );
            if(  ( category2 == "Internode" ) || ( category2 == "Leaf" ) || ( category2 == "Panicle" ) || ( category2 == "Peduncle" ) || ( category2 == "Root" ) )
            {
              ( currentInstance2 as TEntityInstance ).SetCurrentState( 12 );
              SRwriteln( currentInstance2.GetName(  ) + " passe a END FILLING" );
            }
          }
        }}
      }
    }
  }}
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
//
// INITIALIZATION
//
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************


#if 0 //INITIALIZATION


MANAGER_DECLARATION = TManagerInternal.Create( "", LeafManager_LE );
MANAGER_DECLARATION.SetFunctName( "LeafManager_LE" );
MANAGER_LIBRARY.AddTManager( MANAGER_DECLARATION );

MANAGER_DECLARATION = TManagerInternal.Create( "", RootManager_LE );
MANAGER_DECLARATION.SetFunctName( "RootManager_LE" );
MANAGER_LIBRARY.AddTManager( MANAGER_DECLARATION );

MANAGER_DECLARATION = TManagerInternal.Create( "", InternodeManager_LE );
MANAGER_DECLARATION.SetFunctName( "InternodeManager_LE" );
MANAGER_LIBRARY.AddTManager( MANAGER_DECLARATION );

MANAGER_DECLARATION = TManagerInternal.Create( "", TillerManager_LE );
MANAGER_DECLARATION.SetFunctName( "TillerManager_LE" );
MANAGER_LIBRARY.AddTManager( MANAGER_DECLARATION );

MANAGER_DECLARATION = TManagerInternal.Create( "", TillerManagerPhytomer_LE );
MANAGER_DECLARATION.SetFunctName( "TillerManagerPhytomer_LE" );
MANAGER_LIBRARY.AddTManager( MANAGER_DECLARATION );

MANAGER_DECLARATION = TManagerInternal.Create( "", MeristemManager_LE );
MANAGER_DECLARATION.SetFunctName( "MeristemManager_LE" );
MANAGER_LIBRARY.AddTManager( MANAGER_DECLARATION );

MANAGER_DECLARATION = TManagerInternal.Create( "", MeristemManagerPhytomers_LE );
MANAGER_DECLARATION.SetFunctName( "MeristemManagerPhytomers_LE" );
MANAGER_LIBRARY.AddTManager( MANAGER_DECLARATION );

MANAGER_DECLARATION = TManagerInternal.Create( "", ThermalTimeManager_LE );
MANAGER_DECLARATION.SetFunctName( "ThermalTime_LE" );
MANAGER_LIBRARY.AddTManager( MANAGER_DECLARATION );

MANAGER_DECLARATION = TManagerInternal.Create( "", PanicleManager_LE );
MANAGER_DECLARATION.SetFunctName( "PanicleManager_LE" );
MANAGER_LIBRARY.AddTManager( MANAGER_DECLARATION );

MANAGER_DECLARATION = TManagerInternal.Create( "", PanicleManager_LE );
MANAGER_DECLARATION.SetFunctName( "PeduncleManager_LE" );
MANAGER_LIBRARY.AddTManager( MANAGER_DECLARATION );

MANAGER_DECLARATION = TManagerInternal.Create( "", EcoMeristemManager_LE );
MANAGER_DECLARATION.SetFunctName( "EcoMeristemManager_LE" );
MANAGER_LIBRARY.AddTManager( MANAGER_DECLARATION );


#endif//INITIALIZATION


//END
