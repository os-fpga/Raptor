#if !defined( SDT_DEFS_H_INCLUDED )
#define SDT_DEFS_H_INCLUDED

#include "fnpsdtapi.h"

SDT_FEATURE_DEFINE( "ADD", 0, FeatureAdd );
SDT_FEATURE_DEFINE( "SUBTRACT", 1, FeatureSubtract );
SDT_FEATURE_DEFINE( "MULTIPLY", 2, FeatureMultiply );
SDT_FEATURE_DEFINE( "DIVIDE", 3, FeatureDivide );
SDT_FEATURE_DEFINE( "POWER", 4, FeaturePower );
SDT_FEATURE_DEFINE( "AVG", 5, FeatureAvg );

#endif /* !defined( SDT_DEFS_H_INCLUDED ) */
