//#include <defines.hpp>
//enum culm_phase {
//    INITIATION = 1,
//    REALIZATION = 2,
//    FERTILECAPACITY = 3,
//    PI = 4,
//    PRE_PI = 5,
//    PRE_FLO = 6,
//    FLO = 7,
//    ELONG = 9,
//    PRE_ELONG = 10,
//    ENDFILLING = 12,
//    MATURITY = 13,
//    DEAD = 1000
//};

//void step_state( double t)
//{
//    ecomeristem::ModelParameters _parameters;
//    culm_phase state;

//    int result;
//    int newState;
//    int plantState;
//    std::string newStateStr; std::string oldStateStr;
//    double boolCrossedPlasto; double plantStock;
//    int IsFirstDayOfPI;
//    double phenoStageAtPI;
//    double nb_leaf_max_after_PI;
//    double currentPhenoStage;
//    double TTAtPI;
//    double TT;
//    double TT_PI_To_Flo;
//    int i;
//    std::string nameOfLastLigulatedLeafOnTiller;
//    int fatherState;
//    double previousState;
//    double phenoStageAtPreFlo; double phenostage_PRE_FLO_to_FLO; double phenostage;

//    switch( state ) {
//    // ETAT INITIATION :
//    // -----------------
//    case INITIATION:
//    {
//        oldStateStr = "INITIATION";
//        nb_leaf_max_after_PI = _parameters.get <double>( "nb_leaf_max_after_PI" );
//        // creation du premier phytomere
//        TillerManagerCreateFirstPhytomers( instance , date );

//        { long i_end = Trunc( nb_leaf_max_after_PI )+1 ; for( i = 1 ; i < i_end ; ++i )
//            {
//                TillerManagerCreateOthersPhytomers ( instance , date , i );
//            }}
//        // nouvel état
//        fatherState = ( refInstance as TEntityInstance ).GetCurrentState(  );
//        if(  ( fatherState == 9 ) )
//        {
//            newState = PRE_ELONG;
//            newStateStr = "PRE_ELONG";
//        }
//        else
//        {
//            newState = REALIZATION;
//            newStateStr = "REALIZATION";
//        }
//    }

//        // ETAT REALISATION :
//        // ----------------
//    case REALIZATION:
//    {
//        oldStateStr = "REALIZATION";
//        // initiation d'une nouvelle feuille a chaque nouveau plasto
//        boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
//        plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
//        if(  ( ( boolCrossedPlasto >= 0 ) && ( plantStock > 0 ) ) ) // New plastochron et stock disponible
//        {
//            TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );
//            TillerManagerPhytomerLeafCreation( instance , date , false , false );
//        }
//        // pas de changement d etat
//        newState = state;
//        newStateStr = "REALIZATION";
//    }

//        // Etat en attente de la fertilité
//        // -------------------------------
//    case FERTILECAPACITY:
//    {
//        oldStateStr = "FERTILECAPACITY";
//        // initiation d'une nouvelle feuille a chaque nouveau plasto
//        boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
//        plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
//        if(  ( ( boolCrossedPlasto >= 0 ) && ( plantStock > 0 ) ) ) // New plastochron et stock disponible
//        {
//            // creation d'une nouvelle feuille
//            // -------------------------------
//            TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );
//            TillerManagerPhytomerLeafCreation( instance , date , false , false );
//        }
//        // pas de changement d etat
//        newState = state;
//        newStateStr = "FERTILECAPACITY";
//    }

//    case PRE_PI:
//    {

//        oldStateStr = "PRE_PI";
//        // initiation d'une nouvelle feuille a chaque nouveau plasto
//        boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
//        plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
//        previousState = instance.GetTAttribute( "previousState" ).GetSample( date ).value;



//        if(  ( ( boolCrossedPlasto >= 0 ) && ( plantStock > 0 ) ) ) // New plastochron et stock disponible
//        {
//            // creation d'une nouvelle feuille
//            // -------------------------------

//            TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );

//            TillerManagerPhytomerLeafCreation( instance , date , false , false );
//            if(  ( previousState == 9 ) )
//            {

//                TillerManagerStartInternodeElongation( instance , date , Trunc( currentPhenoStage ) );
//            }
//        }
//        // pas de changement d etat
//        newState = state;
//        newStateStr = "PRE_PI";
//    }

//    case PI:
//    {
//        oldStateStr = "PI";

//        refInstance = ( instance as TEntityInstance ).GetFather(  );
//        nb_leaf_max_after_PI = ( refInstance as TEntityInstance ).GetTAttribute( "nb_leaf_max_after_PI" ).GetSample( date ).value;
//        boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
//        plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
//        IsFirstDayOfPI = Trunc( instance.GetTAttribute( "isFirstDayOfPi" ).GetSample( date ).value );
//        phenoStageAtPI = instance.GetTAttribute( "phenoStageAtPI" ).GetSample( date ).value;
//        currentPhenoStage = instance.GetTAttribute( "phenoStage" ).GetSample( date ).value;
//        plantState = ( refInstance as TEntityInstance ).GetCurrentState(  );






//        //if (plantStock > 0) then
//        if(  ( plantState == 4 ) || ( plantState == 6 ) || ( plantState == 5 ) )
//        {
//            if(  ( boolCrossedPlasto >= 0 ) )
//            {



//                if(  ( currentPhenoStage <= phenoStageAtPI + nb_leaf_max_after_PI ) )
//                {
//                    if(  ( IsFirstDayOfPI == 1 ) )
//                    {

//                        TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );
//                        TillerManagerPhytomerLeafCreation( instance , date , false , false );
//                        TillerManagerStartInternodeElongation( instance , date , Trunc( currentPhenoStage ) );

//                        TillerManagerPhytomerPanicleCreation( instance , date );

//                        TillerManagerPhytomerPeduncleCreation( instance , date , false , plantStock );

//                        TillerManagerPhytomerPanicleActivation( instance );
//                        sample = instance.GetTAttribute( "isFirstDayOfPi" ).GetSample( date );
//                        sample.value = 0;
//                        instance.GetTAttribute( "isFirstDayOfPi" ).SetSample( sample );
//                        newState = PI;
//                        newStateStr = "PI";
//                    }
//                    else
//                    {


//                        TillerManagerPhytomerLeafActivation( instance , date );

//                        TillerManagerStartInternodeElongation( instance , date , Trunc( currentPhenoStage ) );
//                        newState = PI;
//                        newStateStr = "PI";
//                    }
//                }
//                else
//                {
//                    if(  ( currentPhenoStage == ( phenoStageAtPI + nb_leaf_max_after_PI + 1 ) ) )
//                    {


//                        //TillerManagerStartInternodeElongation(instance, date, Trunc(currentPhenoStage));

//                        TillerManagerPhytomerPeduncleActivation( instance , date );
//                        TillerStorePhenostageAtPre_Flo( instance , date , currentPhenoStage );
//                        newState = PRE_FLO;
//                        newStateStr = "PRE_FLO";
//                    }
//                }
//            }
//            else
//            {
//                newState = PI;
//                newStateStr = "PI";
//            }
//        }
//        else
//        {
//            newState = PI;
//            newStateStr = "PI";
//        }


//        // ------------------------
//        // pas de changement d'etat
//        // ------------------------
//    }

//    case PRE_FLO:
//    {
//        oldStateStr = "PRE_FLO";
//        phenoStageAtPreFlo = instance.GetTAttribute( "phenoStageAtPreFlo" ).GetSample( date ).value;
//        refInstance = ( instance as TEntityInstance ).GetFather(  );
//        phenostage = ( refInstance as TEntityInstance ).GetTAttribute( "n" ).GetSample( date ).value;
//        phenostage_PRE_FLO_to_FLO = ( refInstance as TEntityInstance ).GetTAttribute( "phenostage_PRE_FLO_to_FLO" ).GetSample( date ).value;



//        if(  ( phenostage == ( phenoStageAtPreFlo + phenostage_PRE_FLO_to_FLO ) ) )
//        {
//            PanicleTransitionToFLO( instance );
//            PeduncleTransitionToFLO( instance );
//            newState = FLO;
//            newStateStr = "FLO";
//        }
//        else
//        {
//            newState = PRE_FLO;
//            newStateStr = "PRE_FLO";
//        }
//    }

//    case FLO:
//    {
//        oldStateStr = "FLO";
//        newState = FLO;
//        newStateStr = "FLO";
//    }

//    case DEAD:
//    {
//        oldStateStr = "DEAD";
//        newState = DEAD;
//        newStateStr = "DEAD";
//    }

//    case ELONG:
//    {
//        oldStateStr = "ELONG";
//        nameOfLastLigulatedLeafOnTiller = FindLastLigulatedLeafNumberOnCulm( instance );
//        if(  ( nameOfLastLigulatedLeafOnTiller == "" ) )
//        {

//            newState = PRE_ELONG;
//            newStateStr = "PRE_ELONG";
//        }
//        else
//        {

//            boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
//            plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
//            currentPhenoStage = instance.GetTAttribute( "phenoStage" ).GetSample( date ).value;

//            if(  ( ( boolCrossedPlasto >= 0 ) && ( plantStock > 0 ) ) ) // New plastochron et stock disponible
//            {
//                TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );

//                TillerManagerPhytomerLeafCreation( instance , date , false , false );

//                TillerManagerStartInternodeElongation( instance , date , Trunc( currentPhenoStage ) );

//            }
//            newState = ELONG;
//            newStateStr = "ELONG";
//        }
//    }

//    case PRE_ELONG:
//    {
//        oldStateStr = "PRE_ELONG";
//        nameOfLastLigulatedLeafOnTiller = FindLastLigulatedLeafNumberOnCulm( instance );
//        if(  ( nameOfLastLigulatedLeafOnTiller == "-1" ) )
//        {


//            boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
//            plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
//            currentPhenoStage = instance.GetTAttribute( "phenoStage" ).GetSample( date ).value;
//            if(  ( ( boolCrossedPlasto >= 0 ) && ( plantStock > 0 ) ) )
//            {
//                TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );

//                TillerManagerPhytomerLeafCreation( instance , date , false , false );

//            }


//            newState = PRE_ELONG;
//            newStateStr = "PRE_ELONG";
//        }
//        else
//        {
//            boolCrossedPlasto = instance.GetTAttribute( "boolCrossedPlasto" ).GetSample( date ).value;
//            plantStock = instance.GetTAttribute( "plantStock" ).GetSample( date ).value;
//            currentPhenoStage = instance.GetTAttribute( "phenoStage" ).GetSample( date ).value;
//            if(  ( ( boolCrossedPlasto >= 0 ) && ( plantStock > 0 ) ) ) // New plastochron et stock disponible
//            {

//                TillerManagerPhytomerInternodeCreation( instance , date , false , plantStock );

//                TillerManagerPhytomerLeafCreation( instance , date , false , false );

//                TillerManagerStartInternodeElongation( instance , date , Trunc( currentPhenoStage ) );

//                newState = ELONG;
//                newStateStr = "ELONG";
//            }
//            else
//            {
//                newState = PRE_ELONG;
//                newStateStr = "PRE_ELONG";
//            }
//        }

//    }

//    case ENDFILLING:
//    {
//        oldStateStr = "ENDFILLING";
//        newState = ENDFILLING;
//        newStateStr = "ENDFILLING";
//    }

//    case MATURITY:
//    {
//        oldStateStr = "MATURITY";
//        newState = MATURITY;
//        newStateStr = "MATURITY";
//    }

//        // ETAT NON DEFINI :
//        // ----------------
//    default:
//    {
//        result = -1;
//        return result;
//    }
//    }




//    // retourne le nouvel état
//    result = newState;
//    return result;
//}


////enum culm_phase {
////	INITIATION = 1,
////	REALIZATION = 2,
////	FERTILECAPACITY = 3,
////	PI = 4,
////	PRE_PI = 5,
////	PRE_FLO = 6,
////	FLO = 7,
////	DEAD = 1000
////}

/////* VARIABLES NECESSAIRES
////_bool_crossed_plasto, _pheno_stage_at_pi, _nb_leaf_max_after_pi,
////_phenostage, _tt_at_pi, _tt, _tt_pi_to_flo, _plant_stock
////*/

////switch (_culm_phase) {
////	case CulmModel::INITIATION:
////		//TillerManagerLeafCreation
////		_culm_phase = CulmModel::REALIZATION;

////	case CulmModel::REALIZATION:
////		if (_bool_crossed_plasto >= 0 and _plant_stock >= 0) {
////			//TillerManagerLeafCreation
////		}

////	case CulmModel::FERTILECAPACITY:
////		if (_bool_crossed_plasto >= 0 and _plant_stock >= 0) {
////			//TillerManagerLeafCreation
////		}

////	case CulmModel::PRE_PI:
////		if (_bool_crossed_plasto >= 0 and _plant_stock >= 0) {
////			//TillerManagerLeafCreation
////		}

////	case CulmModel::PI:
////		if (_plant_stock > 0) {
////			if (_bool_crossed_plasto >= 0) {
////				if (_phenostage <= _pheno_stage_at_pi + _nb_leaf_max_after_pi) {
////					if (t == _pi_t) {
////						//TillerManagerLeafCreation
////						//TillerManagerInternodeCreation
////						//TillerManagerPanicleCreation
////						_culm_phase = CulmModel::PI;
////					} else {
////						//TillerManagerLeafCreation
////						//TillerManagerInternodeCreation
////						_culm_phase = CulmModel::PI;
////					}
////				}
////			} else {
////				_culm_phase = CulmModel::PI;
////			}
////			if (_phenostage == _pheno_stage_at_pi + _nb_leaf_max_after_pi + 1) {
////				//TillerManagerInternodeCreation
////				_culm_phase = CulmModel::PI;
////			} else {
////				_culm_phase = CulmModel::PI;
////			}
////			if (_phenostage == _pheno_stage_at_pi + _nb_leaf_max_after_pi + 2) {
////				//TillerManagerPeduncleCreation
////				_culm_phase = CulmModel::PRE_FLO;
////			} else {
////				_culm_phase = CulmModel::PI;
////			}
////		} else {
////				_culm_phase = CulmModel::PI;
////		}

////	case CulmModel::PRE_FLO:
////		if (_tt - _tt_at_pi >= _tt_pi_to_flo) {
////			//PanicleTransitionToFlo;
////			//PeduncleTransitionToFlo;
////			_culm_phase = CulmModel::FLO;
////		} else {
////			_culm_phase = CulmModel::PRE_FLO;
////		}

////	case CulmModel::FLO:
////		_culm_phase = CulmModel::FLO;

////	case CulmModel::DEAD:
////		_culm_phase = CulmModel::DEAD;
////}




////private:
////	culm_phase _culm_phase
