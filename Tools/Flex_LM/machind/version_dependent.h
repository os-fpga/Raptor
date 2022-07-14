#ifndef _VERSION_DEPENDENT_H_
#define _VERSION_DEPENDENT_H_

/* for (lm_code2.h */
#define LM_VER_BEHAVIOR LM_BEHAVIOR_V11

/* for lmclient.h */

#include "version_info.h"
#include "version_hotfix.h"

#define FLEXLM_VERSION              FNP_VER_MAJOR
#define FLEXLM_REVISION             FNP_VER_MINOR
#define FLEXLM_SUB_MINOR_REVISION   FNP_VER_MAINT
#define FLEXLM_PATCH_NUMBER         FNP_VER_HOTFIX

#define FLEXLM_VERSION_REVISION_STRING FNP_VER_SMAJOR "." FNP_VER_SMINOR "." FNP_VER_SMAINT "." FNP_VER_SHOTFIX

#define LM_BEHAVIOR_V11		"11.0"
#define LM_BEHAVIOR_CURRENT	LM_BEHAVIOR_V11

#define LM_CODE_GLOBAL(name, x, y, k1, k2, k3, k4, k5)  \
					VENDORCODE name = \
						{ VENDORCODE_7, \
						  { (x)^(k5), (y)^(k5) }, \
						  { (k1), (k2), (k3), (k4) }, \
						  FLEXLM_VERSION, \
						  FLEXLM_REVISION, \
						  FLEXLM_PATCH, \
						  LM_VER_BEHAVIOR, \
						  { TRL_KEY1, \
						  TRL_KEY2 }, \
						  0, \
						  LM_STRENGTH, \
						  0 \
						  }

#define LM_CODE(name, x, y, k1, k2, k3, k4, k5)  \
					static VENDORCODE name = \
						{ VENDORCODE_7, \
						  { (x)^(k5), (y)^(k5) }, \
						  { (k1), (k2), (k3), (k4) }, \
						  FLEXLM_VERSION, \
						  FLEXLM_REVISION, \
						  FLEXLM_PATCH, \
						  LM_VER_BEHAVIOR, \
						  { TRL_KEY1, \
						  TRL_KEY2 }, \
						  0, \
						  LM_STRENGTH, \
						  0 \
						  }

#if 0
lm_extern int API_ENTRY lc_activation lm_args((LM_HANDLE_PTR job, const LM_CHAR_PTR xmlPayload, LM_CHAR_PTR *xmlResponse));

lm_extern int API_ENTRY la_createStatusSpecifier lm_args((LM_HANDLE_PTR job,
                            void ** statusSpec));
lm_extern int API_ENTRY la_statSpecSetDetail lm_args((LM_HANDLE_PTR job,
                            void * statusSpec, int detailFlag));
lm_extern int API_ENTRY la_statSpecSetFulfillmentId lm_args((LM_HANDLE_PTR job,
                            void * statusSpec, const LM_CHAR_PTR fulfillId));
lm_extern int API_ENTRY la_statSpecSetProductId lm_args((LM_HANDLE_PTR job,
                            void * statusSpec, const LM_CHAR_PTR productId));
lm_extern int API_ENTRY la_statSpecSetEntitlementId lm_args((LM_HANDLE_PTR job,
                            void * statusSpec, const LM_CHAR_PTR entitlementId));
lm_extern int API_ENTRY la_statSpecSetFeatureName lm_args((LM_HANDLE_PTR job,
                            void * statusSpec, const LM_CHAR_PTR featureName));
lm_extern int API_ENTRY la_getFulfillmentStatus lm_args((LM_HANDLE_PTR job,
                            void * statusSpec, void ** statusHandle));
lm_extern int API_ENTRY la_statGetNumFulfillments lm_args((LM_HANDLE_PTR job,
                            void * statusHandle));
lm_extern int API_ENTRY la_statSelectFirstFulfillment lm_args((LM_HANDLE_PTR job,
                            void * statusHandle));
lm_extern int API_ENTRY la_statSelectNextFulfillment lm_args((LM_HANDLE_PTR job,
                            void * statusHandle));
lm_extern int API_ENTRY la_statSelectPrevFulfillment lm_args((LM_HANDLE_PTR job,
                            void * statusHandle));
lm_extern int API_ENTRY la_statSelectLastFulfillment lm_args((LM_HANDLE_PTR job,
                            void * statusHandle));
lm_extern int API_ENTRY la_statSelectNthFulfillment lm_args((LM_HANDLE_PTR job,
                            void * statusHandle, int n));
lm_extern int API_ENTRY la_statGetFulfillmentVersion lm_args((LM_HANDLE_PTR job,
                            void * statusHandle));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetFulfillmentId lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetEntitlementId lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetProductId lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetSuiteId lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetExpirationDate lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetOriginalExpirationDate lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetFulfillmentChain lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern int API_ENTRY la_statGetHops lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern int * API_ENTRY la_statGetAvailableCounts lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern int * API_ENTRY la_statGetMaximumCounts lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetMaximumOverdraftDuration lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetFeatureLines lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern int API_ENTRY la_statGetNumDeductions lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern int API_ENTRY la_statSelectFirstDeduction lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern int API_ENTRY la_statSelectNextDeduction lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern int API_ENTRY la_statSelectPrevDeduction lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern int API_ENTRY la_statSelectLastDeduction lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern int API_ENTRY la_statSelectNthDeduction lm_args((
                            LM_HANDLE_PTR job, void * statusHandle, int n));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetDeductionDestinationFulfillId lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetDeductionSystemName lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern int * API_ENTRY la_statGetDeductionCounts lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetDeductionExpirationDate lm_args((
                            LM_HANDLE_PTR job, void * statusHandle));
lm_extern int  API_ENTRY la_freeStatusSpecifier lm_args((
                            LM_HANDLE * job, void * statusSpec));
lm_extern int  API_ENTRY la_freeFulfillmentStatus lm_args((
                            LM_HANDLE * job, void * statusHandle));
lm_extern LM_CHAR_PTR API_ENTRY la_statGetServerChain lm_args((
							LM_HANDLE * job, void * statusHandle));
lm_extern int API_ENTRY la_statGetAvailableRepairs lm_args((
							LM_HANDLE * job, void * statusHandle));
lm_extern int API_ENTRY la_statGetDeductionRepairCount lm_args((
							LM_HANDLE * job, void * statusHandle));
lm_extern int API_ENTRY la_statGetDeductionType lm_args((
							LM_HANDLE * job, void * statusHandle));
#endif
#endif
