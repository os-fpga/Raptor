/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*

*
*	Description:	This is a demo application program, to illustrate
*			the use of the Flexible License Manager.
*/

#if defined(_DONT_USE_CTYPE_INLINE_)
#undef _DONT_USE_CTYPE_INLINE_
#endif

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#ifdef WINDOWS
#include "wininstaller.h"
#endif /* WINDOWS */

#include "lmclient.h"
#include "lm_attr.h"

extern "C"{
#include "lm_redir_std.h"
} 

#ifdef PC
#define LICPATH "@localhost;license.dat;."
#else
#define LICPATH "@localhost:license.dat:."
#endif
#include "FlxActApp.h"
#include "sdtdefs.h"

VENDORCODE code;

const char *	pszAppName = "ezcalcsdtplus";
const char *	pszPublisherName = "demo";

static
void
sInstallService()
{
#ifdef WINDOWS
	fnpActSvcInstallWin("demo", "ezcalcsdtplus");
#endif /* WINDOWS */
}

class CheckoutGuard
{
public:
    CheckoutGuard( LM_HANDLE *job, const char * feature_name )
        : m_feature_name( feature_name )
        , m_job( job )
    {
        if( lc_checkout( m_job, const_cast<char*>( m_feature_name.c_str() ), "1.0", 1, LM_CO_NOWAIT, &code, LM_DUP_NONE ) )
        {
            lc_perror( m_job, "checkout failed" );

            std::ostringstream desc;
            desc
                << "Checkout failed for " << m_feature_name << " with error code=" << lc_get_errno(m_job);
            throw std::runtime_error( desc.str() );
        }
        std::cout << "Checked out: " << m_feature_name << std::endl;
    }

    ~CheckoutGuard()
    {
        lc_checkin( m_job, const_cast<char*>( m_feature_name.c_str() ), 0 );
        std::cout << "Checked in : " << m_feature_name << std::endl;
    }

private:
    // Non-copyable
    CheckoutGuard( const CheckoutGuard& );
    CheckoutGuard& operator = ( const CheckoutGuard& );

    const std::string m_feature_name;
    LM_HANDLE * m_job;
};


class Calculator
{
public:
    Calculator( LM_HANDLE *job )
        : m_value( 0 )
        , m_job( job )
    {
    }

    void set( int value )
    {
        m_value = value;
    }

    void add( int value )
    {
        CheckoutGuard holder( m_job, "ADD" );
		std::cout << "Operation  : " << m_value << " + " << value;
        FeatureAdd_int32 current( m_value );
        FeatureAdd_int32 entered( value );
        m_value = current + entered;
		std::cout << " = " << m_value << std::endl;
    }
	
	void add_no_checkout( int value )
	{
		std::cout << "Checkout   : skipped for ADD" << std::endl;
		std::cout << "Operation  : " << m_value << " + " << value;
        FeatureAdd_int32 current( m_value );
        FeatureAdd_int32 entered( value );
        m_value = current + entered;
		std::cout << " = " << m_value << std::endl;
	}

    void subtract( int value )
    {
        CheckoutGuard holder( m_job, "SUBTRACT" );
		std::cout << "Operation  : " << m_value << " - " << value;
        FeatureSubtract_int32 current( m_value );
        FeatureSubtract_int32 entered( value );
        m_value = current - entered;
		std::cout << " = " << m_value << std::endl;
    }

    void multiply( int value )
    {
        CheckoutGuard holder( m_job, "MULTIPLY" );
		std::cout << "Operation  : " << m_value << " x " << value;
        FeatureMultiply_int32 current( m_value );
        FeatureMultiply_int32 entered( value );
        m_value = current * entered;
		std::cout << " = " << m_value << std::endl;
    }

    void divide( int value )
    {
        CheckoutGuard holder( m_job, "DIVIDE" );
		std::cout << "Operation  : " << m_value << " / " << value;
        FeatureDivide_int32 current( m_value );
        FeatureDivide_int32 entered( value );
        m_value = current / entered;
		std::cout << " = " << m_value << std::endl;
    }

    void power( int value )
    {
        CheckoutGuard holder( m_job, "POWER" );

        FeaturePower_int32 current( m_value );
        FeaturePower_int32 entered( value );

		std::cout << "Operation  : " << m_value << " ^ " << value;

        if( entered == SDT_FEATURE_DEFINE_CONST( FeaturePower_int32, 0 ) )
        {
            current = SDT_FEATURE_DEFINE_CONST( FeaturePower_int32, 1 );
        }
        else
        {
            FeaturePower_int32 i = SDT_FEATURE_DEFINE_CONST( FeaturePower_int32, 0 );
            FeaturePower_int32 curValue = current;

            for( i = SDT_FEATURE_DEFINE_CONST( FeaturePower_int32, 1 ); i < entered; ++i )
            {
                current = current * curValue;
            }
        }
        m_value = current;
		std::cout << " = " << m_value << std::endl;
    }

    void average( int value )
    {
        CheckoutGuard holder( m_job, "AVG" );
		std::cout << "Operation  : Average of (" << m_value << " + " << value << ")";
        FeatureAvg_int32 current( m_value );
        FeatureAvg_int32 entered( value );
        m_value = (current + entered)/SDT_FEATURE_DEFINE_CONST( FeatureAvg_int32, 2 );
		std::cout << " = " << m_value << std::endl;
    }

    void reset()
    {
        m_value = 0;
    }

    int value() const
    {
        return m_value;
    }

private:
    // Non-copyable
    Calculator( const Calculator & );
    Calculator& operator = ( const Calculator & );

    // Member variables
    int m_value;
    LM_HANDLE *m_job;
};

int inputToInt( const std::string& input, bool bSkipFirstChar = true )
{
    std::istringstream istr( input );
    if( bSkipFirstChar )
    {
        char c;
        istr >> c;
    }
    int res;
    istr >> res;
    if( istr.bad() || !istr.eof() )
    {
        throw std::runtime_error( "Invalid input value!" );
    }
    return res;
}

void printScreen( int curValue )
{
    std::cout
        << "EZ Calculator" << std::endl
        << "Current value:      " << curValue << std::endl
        << std::endl
		<< "Enter operator and operand (right hand number) e.g. + 5" 
		<< std::endl
        << "  + to add" << std::endl
        << "  - to subtract" << std::endl
        << "  x to multiply" << std::endl
        << "  / to divide" << std::endl
        << "  ^ to raise to power" << std::endl
        << "  a to average" << std::endl
		<< "  t to add (behavior of SDTs when valid license checkout circumvented) e.g. t 5" << std::endl
        << "  c to clear" << std::endl
        << "  q to clear" << std::endl << std::endl
        << "Or, just enter a number  >:"
        ;
}

void process_user_input( Calculator& calc, const std::string& input )
{
    if( input.empty() )
    {
        return;
    }

    if( isdigit( input[0] ) )
    {
        calc.set( inputToInt( input, false ) );
        return;
    }

    switch( input[0] )
    {
    case '+': calc.add( inputToInt( input ) );       break;
    case '-': calc.subtract( inputToInt( input ) );  break;
    case 'x': calc.multiply( inputToInt( input ) );  break;
    case '/': calc.divide( inputToInt( input ) );  break;
    case '^': calc.power( inputToInt( input ) );     break;
    case 'a': calc.average( inputToInt( input ) );   break;
	case 't': calc.add_no_checkout( inputToInt( input ) ); break;
    case 'c': calc.reset();                     break;
    case 'q':                                   break;
    default:
        std::cout << "Invalid input!" << std::endl;
        break;
    }
}

static flexinit_property_handle *
init()
{
	flexinit_property_handle *ourHandle;
	int stat = lc_flexinit_property_handle_create(&ourHandle);
	if ( stat )
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_property_handle_create() failed: %d\n", stat);
		exit(1);
	}
    stat = lc_flexinit_property_handle_set( ourHandle,
			(FLEXINIT_PROPERTY_TYPE)FLEXINIT_PROPERTY_USE_TRUSTED_STORAGE,
			(FLEXINIT_VALUE_TYPE)1 );
	if ( stat )
	{
		fprintf( lm_flex_stderr(), "lc_flexinit_property_handle_set failed: %d\n", stat );
	    exit(1);
	}
    stat = lc_flexinit(ourHandle);
	if ( stat )
	{
		fprintf(lm_flex_stderr(), "lc_flexinit failed: %d\n", stat);
	    exit(1);
	}
	return ourHandle;
}

static void
cleanup( flexinit_property_handle **initHandle)
{
	int stat = lc_flexinit_cleanup( *initHandle );
	if ( stat )
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_cleanup failed: %d\n", stat);
	}
    stat = lc_flexinit_property_handle_free( *initHandle );
	if ( stat )
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_property_handle_free failed: %d\n", stat);
	}
    *initHandle = NULL;
}

int main()
{
	/* install service if it's not already installed */
	sInstallService();

	/* initialize */
	flexinit_property_handle *initHandle = init();

    /* initialize the job handle for license checking */
    LM_HANDLE *lm_job = NULL;
    if( lc_new_job( 0, lc_new_job_arg2, &code, &lm_job ) )
    {
        int errorInfoNum = lc_get_errno(lm_job);

        lc_perror( lm_job, "Failed in license handle creation" );
        cleanup( &initHandle );
        exit(errorInfoNum);
    }

    (void)lc_set_attr(lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)LICPATH);
    (void)lc_set_attr(lm_job, LM_A_CHECK_BADDATE, (LM_A_VAL_TYPE)1);

    Calculator calc( lm_job );
    std::string input;
    do
    {
        printScreen( calc.value() );
        {
            // Normally, this should be std::getline( std::cin, input );
            // On VC6 that is broken, so we revert to the following:
            // There is no check for buffer overflow here.
            char tmp[512];
            std::cin.getline( &tmp[0], 512 );
            input = tmp;
        }
        std::cout << std::endl;
        try
        {
            process_user_input( calc, input );
        }
        catch( const std::exception& e )
        {
            std::cout << e.what() << std::endl << std::endl;
        }
    }
    while( input != "q" );

    lc_free_job( lm_job );
    cleanup( &initHandle );

    std::cout << "Thank you for using EZCalcPlusPlus. Bye!" << std::endl;

    return 0;
}
