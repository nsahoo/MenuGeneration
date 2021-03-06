#include "../implementation/RegisterTriggerMacro.h"
#include "l1menu/L1TriggerDPGEvent.h"
#include <TMath.h>

#include <stdexcept>
#include "UserCode/L1TriggerUpgrade/interface/L1AnalysisDataFormat.h"

#include "l1menu/ITrigger.h"

#include <string>
#include <vector>

namespace l1menu
{
	namespace triggers
	{
		/** @brief Base class for all versions of the TkTau_TkMu trigger.
		 *
		 * Note that this class is abstract because it doesn't implement the "version"
		 * and "apply" methods. That's left up to the implementations of the different
		 * versions.
		 *
		 * It would have been nicer to implement this as a derived class of CrossTrigger,
		 * but there is a check on whether the electron and jet have the same value for
		 * eta and phi, and so the two triggers are not independent.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 02/Jun/2013
		 */
		class TkTau_TkMu : public l1menu::ITrigger
		{
		public:
			TkTau_TkMu();

			virtual const std::string name() const;
			virtual const std::vector<std::string> parameterNames() const;
			virtual float& parameter( const std::string& parameterName );
			virtual const float& parameter( const std::string& parameterName ) const;
		protected:
			float leg1threshold1_;
			float leg1regionCut_;
			float leg2threshold1_;
			float etaCut_;
			float muonQuality_;
			float zVtxCut_;
		}; // end of the TkTau_TkMu base class


		/** @brief First version of the TkTau_TkMu trigger.
		 *
		 * @author probably Brian Winer
		 * @date sometime
		 */
		class TkTau_TkMu_v1 : public TkTau_TkMu
		{
		public:
			virtual unsigned int version() const;
			virtual bool apply( const l1menu::L1TriggerDPGEvent& event ) const;
			virtual bool thresholdsAreCorrelated() const;
		}; // end of version 0 class

		/** @brief First version of the TkTau_TkMu trigger.
		 *
		 * @author probably Brian Winer
		 * @date sometime
		 */
		class TkTau_TkMu_v0 : public TkTau_TkMu
		{
		public:
			virtual unsigned int version() const;
			virtual bool apply( const l1menu::L1TriggerDPGEvent& event ) const;
			virtual bool thresholdsAreCorrelated() const;
		}; // end of version 0 class



		/* The REGISTER_TRIGGER macro will make sure that the given trigger is registered in the
		 * l1menu::TriggerTable when the program starts. I also want to provide some suggested binning
		 * however. The REGISTER_TRIGGER_AND_CUSTOMISE macro does exactly the same but lets me pass
		 * a pointer to a function that will be called directly after the trigger has been registered
		 * at program startup. The function takes no parameters and returns void. In this case I'm
		 * giving it a lambda function.
		 */
		REGISTER_TRIGGER_AND_CUSTOMISE( TkTau_TkMu_v1,
			[]() // Use a lambda function to customise rather than creating a named function that never gets used again.
			{
				l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();
				TkTau_TkMu_v1 tempTriggerInstance;
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "leg1threshold1", 100, 0, 100 );
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "leg2threshold1", 100, 0, 100 );
			} // End of customisation lambda function
		) // End of REGISTER_TRIGGER_AND_CUSTOMISE macro call
		REGISTER_TRIGGER( TkTau_TkMu_v0 )


	} // end of namespace triggers

} // end of namespace l1menu


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//---------------  Definitions below         ---------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

bool l1menu::triggers::TkTau_TkMu_v0::apply( const l1menu::L1TriggerDPGEvent& event ) const
{
	const L1Analysis::L1AnalysisDataFormat& analysisDataFormat=event.rawEvent();
	const bool* PhysicsBits=event.physicsBits();

	bool raw=PhysicsBits[0]; // ZeroBias
	if( !raw ) return false;

	bool ok = false;

	int NTktau=analysisDataFormat.NTktau;
	int NTkmu=analysisDataFormat.NTkmu;
	
	for( int itau=0; itau<NTktau; itau++ )
	{
		int bx=analysisDataFormat.BxTktau.at( itau );
		if( bx!=0 ) continue;
		float pt=analysisDataFormat.EtTktau.at( itau );
		float eta = analysisDataFormat.EtaTktau[itau];
		if (eta < leg1regionCut_ || eta > 21.-leg1regionCut_) continue;  // eta = 5 - 16  // eta = 5 - 16

		if( pt>=leg1threshold1_ ) {
		  

		     for( int imu=0; imu<NTkmu; imu++ )
		     {
			     int bx2=analysisDataFormat.BxTkmu.at( imu );
			     if( bx2!=0 ) continue;
			     float pt2=analysisDataFormat.PtTkmu.at( imu );
			     float eta2=analysisDataFormat.Etamu.at( imu ); // Commented out to stop unused variable compile warning
			     if (std::fabs(eta2) > etaCut_) continue;
			     int qual=analysisDataFormat.QualTkmu.at( imu );
			     if( qual<muonQuality_ ) continue;
			     

			     if (pt2 >= leg2threshold1_) ok = true;
		     }
                } 
		
	}

	
	return ok;
}



bool l1menu::triggers::TkTau_TkMu_v0::thresholdsAreCorrelated() const
{
	return true;
}

unsigned int l1menu::triggers::TkTau_TkMu_v0::version() const
{
	return 0;
}


bool l1menu::triggers::TkTau_TkMu_v1::apply( const l1menu::L1TriggerDPGEvent& event ) const
{
	const L1Analysis::L1AnalysisDataFormat& analysisDataFormat=event.rawEvent();
	const bool* PhysicsBits=event.physicsBits();

	bool raw=PhysicsBits[0]; // ZeroBias
	if( !raw ) return false;

	bool ok = false;

	int NTktau=analysisDataFormat.NTktau;
	int NTkmu=analysisDataFormat.NTkmu;
	
	for( int itau=0; itau<NTktau; itau++ )
	{
		int bx=analysisDataFormat.BxTktau.at( itau );
		if( bx!=0 ) continue;
		float pt=analysisDataFormat.EtTktau.at( itau );
		float eta = analysisDataFormat.EtaTktau[itau];
		if (eta < leg1regionCut_ || eta > 21.-leg1regionCut_) continue;  // eta = 5 - 16  // eta = 5 - 16

		if( pt>=leg1threshold1_ ) {
		   
		     float tauZvtx = analysisDataFormat.zVtxTktau[itau];

		     for( int imu=0; imu<NTkmu; imu++ )
		     {
		        if(fabs(tauZvtx - analysisDataFormat.zVtxTkmu[imu]) < zVtxCut_)  {
			     int bx2=analysisDataFormat.BxTkmu.at( imu );
			     if( bx2!=0 ) continue;
			     float pt2=analysisDataFormat.PtTkmu.at( imu );
			     float eta2=analysisDataFormat.Etamu.at( imu ); // Commented out to stop unused variable compile warning
			     if (std::fabs(eta2) > etaCut_) continue;
			     int qual=analysisDataFormat.QualTkmu.at( imu );
			     if( qual<muonQuality_ ) continue;
			     

			     if (pt2 >= leg2threshold1_) ok = true;
			 }    
		     }
                } 
		
	}

	
	return ok;
}



bool l1menu::triggers::TkTau_TkMu_v1::thresholdsAreCorrelated() const
{
	return true;
}

unsigned int l1menu::triggers::TkTau_TkMu_v1::version() const
{
	return 1;
}


l1menu::triggers::TkTau_TkMu::TkTau_TkMu()
	: leg1threshold1_(20), leg1regionCut_(4.5), leg2threshold1_(20), etaCut_(5.0), muonQuality_(4), zVtxCut_(1.0)   
{
	// No operation other than the initialiser list
}

const std::string l1menu::triggers::TkTau_TkMu::name() const
{
	return "L1_TkTau_TkMu";
}

const std::vector<std::string> l1menu::triggers::TkTau_TkMu::parameterNames() const
{
	std::vector<std::string> returnValue;
	returnValue.push_back("leg1threshold1");
	returnValue.push_back("leg1regionCut");
	returnValue.push_back("leg2threshold1");	
	returnValue.push_back("etaCut");
	returnValue.push_back("muonQuality");
        returnValue.push_back("zVtxCut");
	 
	return returnValue;
}

float& l1menu::triggers::TkTau_TkMu::parameter( const std::string& parameterName )
{
	if( parameterName=="leg1threshold1" ) return leg1threshold1_;
	else if( parameterName=="leg2threshold1" ) return leg2threshold1_;
	else if( parameterName=="leg1regionCut" ) return leg1regionCut_;
	else if( parameterName=="etaCut" ) return etaCut_;
	else if( parameterName=="muonQuality" ) return muonQuality_;
	else if( parameterName=="zVtxCut" ) return zVtxCut_;		
	else throw std::logic_error( "Not a valid parameter name" );
}

const float& l1menu::triggers::TkTau_TkMu::parameter( const std::string& parameterName ) const
{
	if( parameterName=="leg1threshold1" ) return leg1threshold1_;
	else if( parameterName=="leg2threshold1" ) return leg2threshold1_;
	else if( parameterName=="leg1regionCut" ) return leg1regionCut_;
	else if( parameterName=="etaCut" ) return etaCut_;
	else if( parameterName=="muonQuality" ) return muonQuality_;
	else if( parameterName=="zVtxCut" ) return zVtxCut_;		
	else throw std::logic_error( "Not a valid parameter name" );
}
