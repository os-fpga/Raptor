#pragma once
using namespace std;
#include <string>
#include <cstdlib>
#include "lmclient.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ctype.h>
#include <sstream>
#ifdef PC
#include <time.h>
#else
#include <sys/time.h>
#endif
#include "lm_attr.h"
#include "lm_redir_std.h"
#ifdef PC
#define LICPATH "@localhost"
#else
#define LICPATH "@localhost:license.dat:."
#include <sys/time.h>
#endif /* PC */
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>

class License_Manager{
public:
    // List of licensed features. Please update this, the map of
    // feature enum -> name (licensedProductNameMap), and the
    // map of feature enum -> num debted licenses (licenseDebitsPerProduct)
    // in License_manager.cpp every time a new licensed feature is added
    enum class LicensedProductName {
        READ_VERILOG,
        OPENFPGA_RS,
	YOSYS_RS,
	YOSYS_RS_PLUGIN,
	DE
    };

    struct LicenseFatalException : public exception {
        const char * what () const throw () {
            return "License was not acquired due to a fatal error";
        }
    };

    struct LicenseCorrectableException : public exception {
        const char * what () const throw () {
            return "License was not acquired due to a correctable error";
        }
    };

    License_Manager(LicensedProductName licensedProductName);
    ~License_Manager();

private:
    License_Manager();
    License_Manager(const License_Manager &) = delete;
    License_Manager & operator=(const License_Manager &) = delete;
    // Checks out the license
    bool licenseCheckout(string& productName, int licenseCount, const string& version);
    // Checks in the license which was checked out earlier
    bool licenseCheckin( string& productName);
    // // Initializes the license - Figure out why we need this
    // void licenattribute();

    VENDORCODE code;
    LM_HANDLE *lm_job;
    LicensedProductName mylicensedProductName;
    static map<LicensedProductName, string> licensedProductNameMap;
    static map<LicensedProductName, int> licenseDebitsPerProduct;
};
