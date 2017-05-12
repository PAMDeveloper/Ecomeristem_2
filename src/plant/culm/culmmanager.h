enum culm_phase {
	INITIATION = 1,
	REALIZATION = 2,
	FERTILECAPACITY = 3,
	PI = 4,
	PRE_PI = 5,
	PRE_FLO = 6,
	FLO = 7,
	DEAD = 1000
}

/* VARIABLES NECESSAIRES
_bool_crossed_plasto, _pheno_stage_at_pi, _nb_leaf_max_after_pi,
_phenostage, _tt_at_pi, _tt, _tt_pi_to_flo, _plant_stock
*/

switch (_culm_phase) {
	case CulmModel::INITIATION:
		//TillerManagerLeafCreation
		_culm_phase = CulmModel::REALIZATION;
	
	case CulmModel::REALIZATION:
		if (_bool_crossed_plasto >= 0 and _plant_stock >= 0) {
			//TillerManagerLeafCreation
		}
		
	case CulmModel::FERTILECAPACITY:
		if (_bool_crossed_plasto >= 0 and _plant_stock >= 0) {
			//TillerManagerLeafCreation
		}
		
	case CulmModel::PRE_PI:
		if (_bool_crossed_plasto >= 0 and _plant_stock >= 0) {
			//TillerManagerLeafCreation
		}
		
	case CulmModel::PI:
		if (_plant_stock > 0) {
			if (_bool_crossed_plasto >= 0) {
				if (_phenostage <= _pheno_stage_at_pi + _nb_leaf_max_after_pi) {
					if (t == _pi_t) {
						//TillerManagerLeafCreation
						//TillerManagerInternodeCreation
						//TillerManagerPanicleCreation
						_culm_phase = CulmModel::PI;
					} else {
						//TillerManagerLeafCreation
						//TillerManagerInternodeCreation
						_culm_phase = CulmModel::PI;
					}
				}
			} else {
				_culm_phase = CulmModel::PI;
			}
			if (_phenostage == _pheno_stage_at_pi + _nb_leaf_max_after_pi + 1) {
				//TillerManagerInternodeCreation
				_culm_phase = CulmModel::PI;
			} else {
				_culm_phase = CulmModel::PI;
			}
			if (_phenostage == _pheno_stage_at_pi + _nb_leaf_max_after_pi + 2) {
				//TillerManagerPeduncleCreation
				_culm_phase = CulmModel::PRE_FLO;
			} else {
				_culm_phase = CulmModel::PI;
			}
		} else {
				_culm_phase = CulmModel::PI;
		}
		
	case CulmModel::PRE_FLO:
		if (_tt - _tt_at_pi >= _tt_pi_to_flo) {
			//PanicleTransitionToFlo;
			//PeduncleTransitionToFlo;
			_culm_phase = CulmModel::FLO;
		} else {
			_culm_phase = CulmModel::PRE_FLO;
		}
		
	case CulmModel::FLO:
		_culm_phase = CulmModel::FLO;
		
	case CulmModel::DEAD:
		_culm_phase = CulmModel::DEAD;
}




private:
	culm_phase _culm_phase