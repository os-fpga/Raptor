#include "License_manager.hpp"

using namespace std;

map<License_Manager::LicensedProductName, string> License_Manager::licensedProductNameMap = {
    {License_Manager::LicensedProductName::READ_VERILOG, "read_verilog"},
    {License_Manager::LicensedProductName::OPENFPGA_RS, "openfpga_rs"},
    {License_Manager::LicensedProductName::YOSYS_RS, "yosys_rs"},
    {License_Manager::LicensedProductName::YOSYS_RS_PLUGIN, "yosys_rs_plugin"},
    {License_Manager::LicensedProductName::DE, "de"}
};

map<License_Manager::LicensedProductName, int> License_Manager::licenseDebitsPerProduct = {
    {License_Manager::LicensedProductName::READ_VERILOG, 1},
    {License_Manager::LicensedProductName::OPENFPGA_RS, 1},
    {License_Manager::LicensedProductName::YOSYS_RS, 1},
    {License_Manager::LicensedProductName::YOSYS_RS_PLUGIN, 1},
    {License_Manager::LicensedProductName::DE, 1}
};

License_Manager::License_Manager() {
    if (lc_new_job(0, lc_new_job_arg2, &code, &lm_job))
    {
        lc_perror(lm_job, "lc_new_job failed");
        exit(lc_get_errno(lm_job));
    }
}

License_Manager::License_Manager(LicensedProductName licensedProductName): License_Manager() {
    mylicensedProductName = licensedProductName;
    this->licenseCheckout(  licensedProductNameMap.find(mylicensedProductName)->second,
                            licenseDebitsPerProduct.find(mylicensedProductName)->second,
                            "1.0");
}

License_Manager::~License_Manager()
{
    this->licenseCheckin(licensedProductNameMap.find(mylicensedProductName)->second);
    lc_free_job(lm_job);
}

bool License_Manager::licenseCheckout(string& productName, int licenseCount, 
        const string& version)
{
    int errorno = lc_checkout(lm_job, productName.c_str(), version.c_str(), licenseCount, LM_CO_WAIT, &code, LM_DUP_NONE);
    if(errorno) {
        lc_perror(lm_job, "License Checkout Failed");
        switch (errorno){
            case LM_BUSYNEWSERV:
            case LM_CANTCONNECT:
            case LM_SERVBUSY:
            case LM_NOT_THIS_HOST:
            case LM_HOSTDOWN:
                throw(LicenseCorrectableException());
            default:
                throw(LicenseFatalException());
        }
    }
    return true;
}

bool License_Manager::licenseCheckin(string& productName) 
{
    lc_checkin(lm_job, productName.c_str(), 0);
    return true;
}

// Figure out why this is needed
//void License_Manager::licenattribute() 
//{
//    (void)lc_set_attr(lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)LICPATH);
//}
