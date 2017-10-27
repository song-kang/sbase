/* Do not modify this file. Changes will be overwritten.                      */
/* Generated automatically by the ASN.1 to Wireshark dissector compiler       */
/* packet-mms.c                                                               */
/* E:/wireshark-1.12.1/tools/asn2wrs.py -b -p mms -c ./mms.cnf -s ./packet-mms-template -D . mms.asn */

#include "SDecodeMms.h"
/* Input file: packet-mms-template.c */



#define PNAME  "MMS"
#define PSNAME "MMS"
#define PFNAME "mms"

/* TypeSpecification -> TypeSpecification/array -> TypeSpecification */
/* TypeSpecification -> TypeSpecification/structure -> TypeSpecification/structure/components -> TypeSpecification/structure/components/_item -> TypeSpecification */
static int dissect_mms_TypeSpecification(void* cbParam, stuAsn1Pdu *pPdu);

/* VariableSpecification -> ScatteredAccessDescription -> ScatteredAccessDescription/_item -> VariableSpecification */
static int dissect_mms_VariableSpecification(void* cbParam, stuAsn1Pdu *pPdu);

/* AlternateAccess -> AlternateAccess/_item -> AlternateAccessSelection -> AlternateAccessSelection/selectAlternateAccess -> AlternateAccess */
static int dissect_mms_AlternateAccess(void* cbParam, stuAsn1Pdu *pPdu);

/* Data -> Data/array -> Data */
static int dissect_mms_Data(void* cbParam, stuAsn1Pdu *pPdu);

int dissect_acse_AP_title(void* cbParam, stuAsn1Pdu *pPdu);
int dissect_acse_AE_qualifier(void* cbParam, stuAsn1Pdu *pPdu);
int dissect_acse_AP_invocation_identifier(void* cbParam, stuAsn1Pdu *pPdu);
int dissect_acse_AE_invocation_identifier(void* cbParam, stuAsn1Pdu *pPdu);
int dissect_acse_EXTERNALt(void* cbParam, stuAsn1Pdu *pPdu);








/*--- Included file: packet-mms-fn.c ---*/
/*--- Cyclic dependencies ---*/

/* TypeSpecification -> TypeSpecification/array -> TypeSpecification */
/* TypeSpecification -> TypeSpecification/structure -> TypeSpecification/structure/components -> TypeSpecification/structure/components/_item -> TypeSpecification */

/* VariableSpecification -> ScatteredAccessDescription -> ScatteredAccessDescription/_item -> VariableSpecification */

/* AlternateAccess -> AlternateAccess/_item -> AlternateAccessSelection -> AlternateAccessSelection/selectAlternateAccess -> AlternateAccess */

/* Data -> Data/array -> Data */




static int dissect_mms_Unsigned32(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Identifier(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_restricted_string(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_domain_specific_sequence[] = {
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_NOOWNTAG, dissect_mms_Identifier, "domainId" },
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_NOOWNTAG, dissect_mms_Identifier, "itemId" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_domain_specific(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_domain_specific_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_ObjectName_vals[] = {
  {   0, "vmd-specific" },
  {   1, "domain-specific" },
  {   2, "aa-specific" },
  { 0, NULL }
};

static const ber_choice_t ObjectName_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "vmd-specific" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_domain_specific, "domain-specific" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "aa-specific" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ObjectName(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, ObjectName_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const asn_namedbit Transitions_bits[] = {
  {  0, -1, -1, "idle-to-disabled", NULL },
  {  1, -1, -1, "active-to-disabled", NULL },
  {  2, -1, -1, "disabled-to-idle", NULL },
  {  3, -1, -1, "active-to-idle", NULL },
  {  4, -1, -1, "disabled-to-active", NULL },
  {  5, -1, -1, "idle-to-active", NULL },
  {  6, -1, -1, "any-to-deleted", NULL },
  { 0, 0, 0, NULL, NULL }
};

static int dissect_mms_Transitions(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_bitstring(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t AttachToEventCondition_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventEnrollmentName" },
  { BER_CLASS_CON, 1, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventConditionName" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Transitions, "causingTransitions" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "acceptableDelay" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_AttachToEventCondition(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, AttachToEventCondition_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Unsigned8(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Priority(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Unsigned8(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_BOOLEAN(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_boolean(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t AttachToSemaphore_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "semaphoreName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "namedToken" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Priority, "priority" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "acceptableDelay" },
  { BER_CLASS_CON, 4, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "controlTimeOut" },
  { BER_CLASS_CON, 5, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "abortOnTimeOut" },
  { BER_CLASS_CON, 6, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "relinquishIfConnectionLost" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_AttachToSemaphore(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, AttachToSemaphore_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_Modifier_vals[] = {
  {   0, "attach-To-Event-Condition" },
  {   1, "attach-To-Semaphore" },
  { 0, NULL }
};

static const ber_choice_t Modifier_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_AttachToEventCondition, "attach-To-Event-Condition" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_AttachToSemaphore, "attach-To-Semaphore" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Modifier(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, Modifier_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SEQUENCE_OF_Modifier_sequence_of[1] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_Modifier, "_item" },
};

static int dissect_mms_SEQUENCE_OF_Modifier(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SEQUENCE_OF_Modifier_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Status_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_boolean(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_objectClass_vals[] = {
  {   0, "nammedVariable" },
  {   1, "scatteredAccess" },
  {   2, "namedVariableList" },
  {   3, "namedType" },
  {   4, "semaphore" },
  {   5, "eventCondition" },
  {   6, "eventAction" },
  {   7, "eventEnrollment" },
  {   8, "journal" },
  {   9, "domain" },
  {  10, "programInvocation" },
  {  11, "operatorStation" },
  { 0, NULL }
};


static int dissect_mms_T_objectClass(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_extendedObjectClass_vals[] = {
  {   0, "objectClass" },
  { 0, NULL }
};

static const ber_choice_t T_extendedObjectClass_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_T_objectClass, "objectClass" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_extendedObjectClass(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_extendedObjectClass_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_NULL(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_objectScope_vals[] = {
  {   0, "vmdSpecific" },
  {   1, "domainSpecific" },
  {   2, "aaSpecific" },
  { 0, NULL }
};

static const ber_choice_t T_objectScope_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "vmdSpecific" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "domainSpecific" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "aaSpecific" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_objectScope(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_objectScope_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetNameList_Request_sequence[] = {
  { BER_CLASS_CON, 0, 0, dissect_mms_T_extendedObjectClass, "extendedObjectClass" },
  { BER_CLASS_CON, 1, 0, dissect_mms_T_objectScope, "objectScope" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "continueAfter" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetNameList_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetNameList_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Identify_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_objectClass_01_vals[] = {
  {   0, "namedVariable" },
  {   1, "scatteredAccess" },
  {   2, "namedVariableList" },
  {   3, "namedType" },
  {   4, "semaphore" },
  {   5, "eventCondition" },
  {   6, "eventAction" },
  {   7, "eventEnrollment" },
  {   8, "journal" },
  {   9, "domain" },
  {  10, "programInvocation" },
  {  11, "operatorStation" },
  { 0, NULL }
};


static int dissect_mms_T_objectClass_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_extendedObjectClass_01_vals[] = {
  {   0, "objectClass" },
  { 0, NULL }
};

static const ber_choice_t T_extendedObjectClass_01_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_T_objectClass_01, "objectClass" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_extendedObjectClass_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_extendedObjectClass_01_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Rename_Request_sequence[] = {
  { BER_CLASS_CON, 0, 0, dissect_mms_T_extendedObjectClass_01, "extendedObjectClass" },
  { BER_CLASS_CON, 1, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "currentName" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "newIdentifier" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Rename_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Rename_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_VisibleString(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_restricted_string(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_OCTET_STRING(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_octet_string(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_Address_vals[] = {
  {   0, "numericAddress" },
  {   1, "symbolicAddress" },
  {   2, "unconstrainedAddress" },
  { 0, NULL }
};

static const ber_choice_t Address_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "numericAddress" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_VisibleString, "symbolicAddress" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_OCTET_STRING, "unconstrainedAddress" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Address(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, Address_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_array_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "packed" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "numberOfElements" },
  { BER_CLASS_CON, 2, BER_FLAGS_NOTCHKTAG, dissect_mms_TypeSpecification, "elementType" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_array(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_array_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_components_item_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "componentName" },
  { BER_CLASS_CON, 1, BER_FLAGS_NOTCHKTAG, dissect_mms_TypeSpecification, "componentType" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_components_item(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_components_item_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_components_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_T_components_item, "_item" },
};

static int dissect_mms_T_components(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_components_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_structure_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "packed" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_components, "components" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_structure(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_structure_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Integer32(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_TypeSpecification_vals[] = {
  {   0, "typeName" },
  {   1, "array" },
  {   2, "structure" },
  {   3, "boolean" },
  {   4, "bit-string" },
  {   5, "integer" },
  {   6, "unsigned" },
  {   9, "octet-string" },
  {  10, "visible-string" },
  {  11, "generalized-time" },
  {  12, "binary-time" },
  {  13, "bcd" },
  {  15, "objId" },
  { 0, NULL }
};

static const ber_choice_t TypeSpecification_choice[] = {
  {   0, BER_CLASS_CON, 0, 0, dissect_mms_ObjectName, "typeName" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_array, "array" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_T_structure, "structure" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "boolean" },
  {   4, BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_Integer32, "bit-string" },
  {   5, BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned8, "integer" },
  {   6, BER_CLASS_CON, 6, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned8, "unsigned" },
  {   9, BER_CLASS_CON, 9, BER_FLAGS_IMPLTAG, dissect_mms_Integer32, "octet-string" },
  {  10, BER_CLASS_CON, 10, BER_FLAGS_IMPLTAG, dissect_mms_Integer32, "visible-string" },
  {  11, BER_CLASS_CON, 11, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "generalized-time" },
  {  12, BER_CLASS_CON, 12, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "binary-time" },
  {  13, BER_CLASS_CON, 13, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned8, "bcd" },
  {  15, BER_CLASS_CON, 15, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "objId" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_TypeSpecification(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, TypeSpecification_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_variableDescription_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_Address, "address" },
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_TypeSpecification, "typeSpecification" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_variableDescription(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_variableDescription_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_indexRange_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "lowIndex" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "numberOfElements" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_indexRange(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_indexRange_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_accessSelection_vals[] = {
  {   0, "component" },
  {   1, "index" },
  {   2, "indexRange" },
  {   3, "allElements" },
  { 0, NULL }
};

static const ber_choice_t T_accessSelection_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "component" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "index" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_T_indexRange, "indexRange" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "allElements" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_accessSelection(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_accessSelection_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_selectAlternateAccess_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_T_accessSelection, "accessSelection" },
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_AlternateAccess, "alternateAccess" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_selectAlternateAccess(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_selectAlternateAccess_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_indexRange_01_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "lowIndex" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "nmberOfElements" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_indexRange_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_indexRange_01_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_selectAccess_vals[] = {
  {   1, "component" },
  {   2, "index" },
  {   3, "indexRange" },
  {   4, "allElements" },
  { 0, NULL }
};

static const ber_choice_t T_selectAccess_choice[] = {
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "component" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "index" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_T_indexRange_01, "indexRange" },
  {   4, BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "allElements" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_selectAccess(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_selectAccess_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_AlternateAccessSelection_vals[] = {
  {   0, "selectAlternateAccess" },
  {   1, "selectAccess" },
  { 0, NULL }
};

static const ber_choice_t AlternateAccessSelection_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_T_selectAlternateAccess, "selectAlternateAccess" },
  {   1, BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG, dissect_mms_T_selectAccess, "selectAccess" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_AlternateAccessSelection(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, AlternateAccessSelection_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_named_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "componentName" },
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_AlternateAccessSelection, "accesst" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_named(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_named_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_AlternateAccess_item_vals[] = {
  {   0, "unnamed" },
  {   1, "named" },
  { 0, NULL }
};

static const ber_choice_t AlternateAccess_item_choice[] = {
  {   0, BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG, dissect_mms_AlternateAccessSelection, "unnamed" },
  {   1, BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_T_named, "named" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_AlternateAccess_item(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, AlternateAccess_item_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t AlternateAccess_sequence_of[1] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_AlternateAccess_item, "_item" },
};

static int dissect_mms_AlternateAccess(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, AlternateAccess_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ScatteredAccessDescription_item_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "componentName" },
  { BER_CLASS_CON, 1, BER_FLAGS_NOTCHKTAG, dissect_mms_VariableSpecification, "variableSpecification" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_AlternateAccess, "alternateAccess" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ScatteredAccessDescription_item(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ScatteredAccessDescription_item_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ScatteredAccessDescription_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_ScatteredAccessDescription_item, "_item" },
};

static int dissect_mms_ScatteredAccessDescription(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ScatteredAccessDescription_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_VariableSpecification_vals[] = {
  {   0, "name" },
  {   1, "address" },
  {   2, "variableDescription" },
  {   3, "scatteredAccessDescription" },
  {   4, "invalidated" },
  { 0, NULL }
};

static const ber_choice_t VariableSpecification_choice[] = {
  {   0, BER_CLASS_CON, 0, 0, dissect_mms_ObjectName, "name" },
  {   1, BER_CLASS_CON, 1, 0, dissect_mms_Address, "address" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_T_variableDescription, "variableDescription" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_ScatteredAccessDescription, "scatteredAccessDescription" },
  {   4, BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "invalidated" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_VariableSpecification(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, VariableSpecification_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfVariable_item_02_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_VariableSpecification, "variableSpecification" },
  { BER_CLASS_CON, 5, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_AlternateAccess, "alternateAccess" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_listOfVariable_item_02(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfVariable_item_02_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfVariable_02_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_T_listOfVariable_item_02, "_item" },
};

static int dissect_mms_T_listOfVariable_02(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfVariable_02_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_VariableAccessSpecification_vals[] = {
  {   0, "listOfVariable" },
  {   1, "variableListName" },
  { 0, NULL }
};

static const ber_choice_t VariableAccessSpecification_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_T_listOfVariable_02, "listOfVariable" },
  {   1, BER_CLASS_CON, 1, 0, dissect_mms_ObjectName, "variableListName" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_VariableAccessSpecification(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, VariableAccessSpecification_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Read_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "specificationWithResult" },
  { BER_CLASS_CON, 1, BER_FLAGS_NOTCHKTAG, dissect_mms_VariableAccessSpecification, "variableAccessSpecificatn" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Read_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Read_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SEQUENCE_OF_Data_sequence_of[1] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_Data, "_item" },
};

static int dissect_mms_SEQUENCE_OF_Data(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SEQUENCE_OF_Data_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_BIT_STRING(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_bitstring(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_INTEGER(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_FloatingPoint(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_octet_string(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_TimeOfDay(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_octet_string(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_OBJECT_IDENTIFIER(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_object_identifier(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_MMSString(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_restricted_string(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_UtcTime(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_octet_string(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_Data_vals[] = {
  {   1, "array" },
  {   2, "structure" },
  {   3, "boolean" },
  {   4, "bit-string" },
  {   5, "integer" },
  {   6, "unsigned" },
  {   7, "floating-point" },
  {   9, "octet-string" },
  {  10, "visible-string" },
  {  12, "binary-time" },
  {  13, "bcd" },
  {  14, "booleanArray" },
  {  15, "objId" },
  {  16, "mMSString" },
  {  17, "utc-time" },
  { 0, NULL }
};

static const ber_choice_t Data_choice[] = {
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_Data, "array" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_Data, "structure" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "boolean" },
  {   4, BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_BIT_STRING, "bit-string" },
  {   5, BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_INTEGER, "integer" },
  {   6, BER_CLASS_CON, 6, BER_FLAGS_IMPLTAG, dissect_mms_INTEGER, "unsigned" },
  {   7, BER_CLASS_CON, 7, BER_FLAGS_IMPLTAG, dissect_mms_FloatingPoint, "floating-point" },
  {   9, BER_CLASS_CON, 9, BER_FLAGS_IMPLTAG, dissect_mms_OCTET_STRING, "octet-string" },
  {  10, BER_CLASS_CON, 10, BER_FLAGS_IMPLTAG, dissect_mms_VisibleString, "visible-string" },
  {  12, BER_CLASS_CON, 12, BER_FLAGS_IMPLTAG, dissect_mms_TimeOfDay, "binary-time" },
  {  13, BER_CLASS_CON, 13, BER_FLAGS_IMPLTAG, dissect_mms_INTEGER, "bcd" },
  {  14, BER_CLASS_CON, 14, BER_FLAGS_IMPLTAG, dissect_mms_BIT_STRING, "booleanArray" },
  {  15, BER_CLASS_CON, 15, BER_FLAGS_IMPLTAG, dissect_mms_OBJECT_IDENTIFIER, "objId" },
  {  16, BER_CLASS_CON, 16, BER_FLAGS_IMPLTAG, dissect_mms_MMSString, "mMSString" },
  {  17, BER_CLASS_CON, 17, BER_FLAGS_IMPLTAG, dissect_mms_UtcTime, "utc-time" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Data(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, Data_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Write_Request_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_VariableAccessSpecification, "variableAccessSpecificatn" },
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_Data, "listOfData" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Write_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Write_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_GetVariableAccessAttributes_Request_vals[] = {
  {   0, "name" },
  {   1, "address" },
  { 0, NULL }
};

static const ber_choice_t GetVariableAccessAttributes_Request_choice[] = {
  {   0, BER_CLASS_CON, 0, 0, dissect_mms_ObjectName, "name" },
  {   1, BER_CLASS_CON, 1, 0, dissect_mms_Address, "address" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetVariableAccessAttributes_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, GetVariableAccessAttributes_Request_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DefineNamedVariable_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "variableName" },
  { BER_CLASS_CON, 1, BER_FLAGS_NOTCHKTAG, dissect_mms_Address, "address" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_TypeSpecification, "typeSpecification" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DefineNamedVariable_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DefineNamedVariable_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DefineScatteredAccess_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "scatteredAccessName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_ScatteredAccessDescription, "scatteredAccessDescription" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DefineScatteredAccess_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DefineScatteredAccess_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_GetScatteredAccessAttributes_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ObjectName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_scopeOfDelete_vals[] = {
  {   0, "specific" },
  {   1, "aa-specific" },
  {   2, "domain" },
  {   3, "vmd" },
  { 0, NULL }
};


static int dissect_mms_T_scopeOfDelete(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SEQUENCE_OF_ObjectName_sequence_of[1] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "_item" },
};

static int dissect_mms_SEQUENCE_OF_ObjectName(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SEQUENCE_OF_ObjectName_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DeleteVariableAccess_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_scopeOfDelete, "scopeOfDelete" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_ObjectName, "listOfName" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "domainName" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DeleteVariableAccess_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DeleteVariableAccess_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfVariable_item_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_VariableSpecification, "variableSpecification" },
  { BER_CLASS_CON, 5, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_AlternateAccess, "alternateAccess" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_listOfVariable_item(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfVariable_item_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfVariable_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_T_listOfVariable_item, "_item" },
};

static int dissect_mms_T_listOfVariable(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfVariable_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DefineNamedVariableList_Request_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "variableListName" },
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_T_listOfVariable, "listOfVariable" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DefineNamedVariableList_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DefineNamedVariableList_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_GetNamedVariableListAttributes_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ObjectName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_scopeOfDelete_01_vals[] = {
  {   0, "specific" },
  {   1, "aa-specific" },
  {   2, "domain" },
  {   3, "vmd" },
  { 0, NULL }
};


static int dissect_mms_T_scopeOfDelete_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DeleteNamedVariableList_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_scopeOfDelete_01, "scopeOfDelete" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_ObjectName, "listOfVariableListName" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "domainName" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DeleteNamedVariableList_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DeleteNamedVariableList_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DefineNamedType_Request_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "typeName" },
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_TypeSpecification, "typeSpecification" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DefineNamedType_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DefineNamedType_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_GetNamedTypeAttributes_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ObjectName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_scopeOfDelete_02_vals[] = {
  {   0, "specific" },
  {   1, "aa-specific" },
  {   2, "domain" },
  {   3, "vmd" },
  { 0, NULL }
};


static int dissect_mms_T_scopeOfDelete_02(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DeleteNamedType_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_scopeOfDelete_02, "scopeOfDelete" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_ObjectName, "listOfTypeName" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "domainName" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DeleteNamedType_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DeleteNamedType_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfPromptData_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_NOOWNTAG, dissect_mms_VisibleString, "_item" },
};

static int dissect_mms_T_listOfPromptData(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfPromptData_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Input_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "operatorStationName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "echo" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_listOfPromptData, "listOfPromptData" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "inputTimeOut" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Input_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Input_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfOutputData_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_NOOWNTAG, dissect_mms_VisibleString, "_item" },
};

static int dissect_mms_T_listOfOutputData(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfOutputData_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Output_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "operatorStationName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_listOfOutputData, "listOfOutputData" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Output_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Output_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_T_ap_title(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_acse_AP_title(cbParam,pPdu);

	DECODE_CALLBACK_RET;
}



static int dissect_mms_T_ap_invocation_id(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_acse_AP_invocation_identifier(cbParam,pPdu);

	DECODE_CALLBACK_RET;
}



static int dissect_mms_T_ae_qualifier(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_acse_AE_qualifier(cbParam,pPdu);

	DECODE_CALLBACK_RET;
}



static int dissect_mms_T_ae_invocation_id(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_acse_AE_invocation_identifier(cbParam,pPdu);

	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ApplicationReference_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL, dissect_mms_T_ap_title, "ap-title" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL, dissect_mms_T_ap_invocation_id, "ap-invocation-id" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL, dissect_mms_T_ae_qualifier, "ae-qualifier" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL, dissect_mms_T_ae_invocation_id, "ae-invocation-id" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ApplicationReference(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ApplicationReference_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t TakeControl_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "semaphoreName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "namedToken" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Priority, "priority" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "acceptableDelay" },
  { BER_CLASS_CON, 4, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "controlTimeOut" },
  { BER_CLASS_CON, 5, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "abortOnTimeOut" },
  { BER_CLASS_CON, 6, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "relinquishIfConnectionLost" },
  { BER_CLASS_CON, 7, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_ApplicationReference, "applicationToPreempt" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_TakeControl_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, TakeControl_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t RelinquishControl_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "semaphoreName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "namedToken" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_RelinquishControl_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, RelinquishControl_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Unsigned16(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DefineSemaphore_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "semaphoreName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned16, "numbersOfTokens" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DefineSemaphore_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DefineSemaphore_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteSemaphore_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ObjectName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_ReportSemaphoreStatus_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ObjectName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ReportPoolSemaphoreStatus_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "semaphoreName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "nameToStartAfter" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ReportPoolSemaphoreStatus_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ReportPoolSemaphoreStatus_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_state_vals[] = {
  {   0, "queued" },
  {   1, "owner" },
  {   2, "hung" },
  { 0, NULL }
};


static int dissect_mms_T_state(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ReportSemaphoreEntryStatus_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "semaphoreName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_state, "state" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_OCTET_STRING, "entryIdToStartAfter" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ReportSemaphoreEntryStatus_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ReportSemaphoreEntryStatus_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfCapabilities_01_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_NOOWNTAG, dissect_mms_VisibleString, "_item" },
};

static int dissect_mms_T_listOfCapabilities_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfCapabilities_01_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t InitiateDownloadSequence_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "domainName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_listOfCapabilities_01, "listOfCapabilities" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "sharable" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_InitiateDownloadSequence_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, InitiateDownloadSequence_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DownloadSegment_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Identifier(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_vmd_state_vals[] = {
  {   0, "other" },
  {   1, "vmd-state-conflict" },
  {   2, "vmd-operational-problem" },
  {   3, "domain-transfer-problem" },
  {   4, "state-machine-id-invalid" },
  { 0, NULL }
};


static int dissect_mms_T_vmd_state(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_application_reference_vals[] = {
  {   0, "other" },
  {   1, "aplication-unreachable" },
  {   2, "connection-lost" },
  {   3, "application-reference-invalid" },
  {   4, "context-unsupported" },
  { 0, NULL }
};


static int dissect_mms_T_application_reference(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_definition_vals[] = {
  {   0, "other" },
  {   1, "object-undefined" },
  {   2, "invalid-address" },
  {   3, "type-unsupported" },
  {   4, "type-inconsistent" },
  {   5, "object-exists" },
  {   6, "object-attribute-inconsistent" },
  { 0, NULL }
};


static int dissect_mms_T_definition(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_resource_vals[] = {
  {   0, "other" },
  {   1, "memory-unavailable" },
  {   2, "processor-resource-unavailable" },
  {   3, "mass-storage-unavailable" },
  {   4, "capability-unavailable" },
  {   5, "capability-unknown" },
  { 0, NULL }
};


static int dissect_mms_T_resource(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_service_vals[] = {
  {   0, "other" },
  {   1, "primitives-out-of-sequence" },
  {   2, "object-sate-conflict" },
  {   3, "pdu-size" },
  {   4, "continuation-invalid" },
  {   5, "object-constraint-conflict" },
  { 0, NULL }
};


static int dissect_mms_T_service(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_service_preempt_vals[] = {
  {   0, "other" },
  {   1, "timeout" },
  {   2, "deadlock" },
  {   3, "cancel" },
  { 0, NULL }
};


static int dissect_mms_T_service_preempt(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_time_resolution_vals[] = {
  {   0, "other" },
  {   1, "unsupportable-time-resolution" },
  { 0, NULL }
};


static int dissect_mms_T_time_resolution(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_access_vals[] = {
  {   0, "other" },
  {   1, "object-access-unsupported" },
  {   2, "object-non-existent" },
  {   3, "object-access-denied" },
  {   4, "object-invalidated" },
  { 0, NULL }
};


static int dissect_mms_T_access(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_initiate_vals[] = {
  {   0, "other" },
  {   1, "version-incompatible" },
  {   2, "max-segment-insufficient" },
  {   3, "max-services-outstanding-calling-insufficient" },
  {   4, "max-services-outstanding-called-insufficient" },
  {   5, "service-CBB-insufficient" },
  {   6, "parameter-CBB-insufficient" },
  {   7, "nesting-level-insufficient" },
  { 0, NULL }
};


static int dissect_mms_T_initiate(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_conclude_vals[] = {
  {   0, "other" },
  {   1, "further-communication-required" },
  { 0, NULL }
};


static int dissect_mms_T_conclude(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_cancel_vals[] = {
  {   0, "other" },
  {   1, "invoke-id-unknown" },
  {   2, "cancel-not-possible" },
  { 0, NULL }
};


static int dissect_mms_T_cancel(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_file_vals[] = {
  {   0, "other" },
  {   1, "filename-ambiguous" },
  {   2, "file-busy" },
  {   3, "filename-syntax-error" },
  {   4, "content-type-invalid" },
  {   5, "position-invalid" },
  {   6, "file-acces-denied" },
  {   7, "file-non-existent" },
  {   8, "duplicate-filename" },
  {   9, "insufficient-space-in-filestore" },
  { 0, NULL }
};


static int dissect_mms_T_file(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_errorClass_vals[] = {
  {   0, "vmd-state" },
  {   1, "application-reference" },
  {   2, "definition" },
  {   3, "resource" },
  {   4, "service" },
  {   5, "service-preempt" },
  {   6, "time-resolution" },
  {   7, "access" },
  {   8, "initiate" },
  {   9, "conclude" },
  {  10, "cancel" },
  {  11, "file" },
  {  12, "others" },
  { 0, NULL }
};

static const ber_choice_t T_errorClass_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_T_vmd_state, "vmd-state" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_application_reference, "application-reference" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_T_definition, "definition" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_T_resource, "resource" },
  {   4, BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_T_service, "service" },
  {   5, BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_T_service_preempt, "service-preempt" },
  {   6, BER_CLASS_CON, 6, BER_FLAGS_IMPLTAG, dissect_mms_T_time_resolution, "time-resolution" },
  {   7, BER_CLASS_CON, 7, BER_FLAGS_IMPLTAG, dissect_mms_T_access, "access" },
  {   8, BER_CLASS_CON, 8, BER_FLAGS_IMPLTAG, dissect_mms_T_initiate, "initiate" },
  {   9, BER_CLASS_CON, 9, BER_FLAGS_IMPLTAG, dissect_mms_T_conclude, "conclude" },
  {  10, BER_CLASS_CON, 10, BER_FLAGS_IMPLTAG, dissect_mms_T_cancel, "cancel" },
  {  11, BER_CLASS_CON, 11, BER_FLAGS_IMPLTAG, dissect_mms_T_file, "file" },
  {  12, BER_CLASS_CON, 12, BER_FLAGS_IMPLTAG, dissect_mms_INTEGER, "others" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_errorClass(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_errorClass_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_ObtainFile_Error_vals[] = {
  {   0, "source-file" },
  {   1, "destination-file" },
  { 0, NULL }
};


static int dissect_mms_ObtainFile_Error(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_ProgramInvocationState_vals[] = {
  {   0, "non-existent" },
  {   1, "unrunable" },
  {   2, "idle" },
  {   3, "running" },
  {   4, "stopped" },
  {   5, "starting" },
  {   6, "stopping" },
  {   7, "resuming" },
  {   8, "resetting" },
  { 0, NULL }
};


static int dissect_mms_ProgramInvocationState(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Start_Error(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ProgramInvocationState(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Stop_Error(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ProgramInvocationState(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Resume_Error(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ProgramInvocationState(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Reset_Error(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ProgramInvocationState(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteVariableAccess_Error(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Unsigned32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteNamedVariableList_Error(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Unsigned32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteNamedType_Error(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Unsigned32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DefineEventEnrollment_Error(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ObjectName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const value_string mms_FileRename_Error_vals[] = {
  {   0, "source-file" },
  {   1, "destination-file" },
  { 0, NULL }
};


static int dissect_mms_FileRename_Error(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_serviceSpecificInformation_vals[] = {
  {   0, "obtainFile" },
  {   1, "start" },
  {   2, "stop" },
  {   3, "resume" },
  {   4, "reset" },
  {   5, "deleteVariableAccess" },
  {   6, "deleteNamedVariableList" },
  {   7, "deleteNamedType" },
  {   8, "defineEventEnrollment-Error" },
  {   9, "fileRename" },
  { 0, NULL }
};

static const ber_choice_t T_serviceSpecificInformation_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_ObtainFile_Error, "obtainFile" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Start_Error, "start" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Stop_Error, "stop" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_Resume_Error, "resume" },
  {   4, BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_Reset_Error, "reset" },
  {   5, BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_DeleteVariableAccess_Error, "deleteVariableAccess" },
  {   6, BER_CLASS_CON, 6, BER_FLAGS_IMPLTAG, dissect_mms_DeleteNamedVariableList_Error, "deleteNamedVariableList" },
  {   7, BER_CLASS_CON, 7, BER_FLAGS_IMPLTAG, dissect_mms_DeleteNamedType_Error, "deleteNamedType" },
  {   8, BER_CLASS_CON, 8, 0, dissect_mms_DefineEventEnrollment_Error, "defineEventEnrollment-Error" },
  {   9, BER_CLASS_CON, 9, BER_FLAGS_IMPLTAG, dissect_mms_FileRename_Error, "fileRename" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_serviceSpecificInformation(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_serviceSpecificInformation_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ServiceError_sequence[] = {
  { BER_CLASS_CON, 0, 0, dissect_mms_T_errorClass, "errorClass" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_INTEGER, "additionalCode" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_VisibleString, "additionalDescription" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL, dissect_mms_T_serviceSpecificInformation, "serviceSpecificInformation" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ServiceError(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ServiceError_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t TerminateDownloadSequence_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "domainName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_ServiceError, "discard" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_TerminateDownloadSequence_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, TerminateDownloadSequence_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_InitiateUploadSequence_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Identifier(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_UploadSegment_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Integer32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_TerminateUploadSequence_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Integer32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfCapabilities_03_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_NOOWNTAG, dissect_mms_VisibleString, "_item" },
};

static int dissect_mms_T_listOfCapabilities_03(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfCapabilities_03_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_GraphicString(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_restricted_string(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t FileName_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_GraphicString, BER_FLAGS_NOOWNTAG, dissect_mms_GraphicString, "_item" },
};

static int dissect_mms_FileName(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, FileName_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t RequestDomainDownload_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "domainName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_listOfCapabilities_03, "listOfCapabilities" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "sharable" },
  { BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_FileName, "fileName" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_RequestDomainDownload_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, RequestDomainDownload_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t RequestDomainUpload_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "domainName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_FileName, "fileName" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_RequestDomainUpload_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, RequestDomainUpload_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfCapabilities_04_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_NOOWNTAG, dissect_mms_VisibleString, "_item" },
};

static int dissect_mms_T_listOfCapabilities_04(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfCapabilities_04_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t LoadDomainContent_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "domainName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_listOfCapabilities_04, "listOfCapabilities" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "sharable" },
  { BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_FileName, "fileName" },
  { BER_CLASS_CON, 5, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_ApplicationReference, "thirdParty" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_LoadDomainContent_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, LoadDomainContent_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t StoreDomainContent_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "domainName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_FileName, "filenName" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_ApplicationReference, "thirdParty" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_StoreDomainContent_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, StoreDomainContent_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteDomain_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Identifier(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_GetDomainAttributes_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Identifier(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SEQUENCE_OF_Identifier_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_NOOWNTAG, dissect_mms_Identifier, "_item" },
};

static int dissect_mms_SEQUENCE_OF_Identifier(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SEQUENCE_OF_Identifier_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t CreateProgramInvocation_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "programInvocationName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_Identifier, "listOfDomainName" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "reusable" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "monitorType" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_CreateProgramInvocation_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, CreateProgramInvocation_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteProgramInvocation_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Identifier(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_executionArgument_vals[] = {
  {   0, "simpleString" },
  {   1, "encodedString" },
  { 0, NULL }
};

static const ber_choice_t T_executionArgument_choice[] = {
  {   0, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_VisibleString, "simpleString" },
  {   1, -1/*imported*/, -1/*imported*/, BER_FLAGS_NOOWNTAG, dissect_acse_EXTERNALt, "encodedString" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_executionArgument(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_executionArgument_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Start_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "programInvocationName" },
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_OPTIONAL|BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_T_executionArgument, "executionArgument" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Start_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Start_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Stop_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "programInvocationName" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Stop_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Stop_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_executionArgument_01_vals[] = {
  {   0, "simpleString" },
  {   1, "encodedString" },
  { 0, NULL }
};

static const ber_choice_t T_executionArgument_01_choice[] = {
  {   0, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_VisibleString, "simpleString" },
  {   1, -1/*imported*/, -1/*imported*/, BER_FLAGS_NOOWNTAG, dissect_acse_EXTERNALt, "encodedString" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_executionArgument_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_executionArgument_01_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Resume_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "programInvocationName" },
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_OPTIONAL|BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_T_executionArgument_01, "executionArgument" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Resume_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Resume_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Reset_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "programInvocationName" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Reset_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Reset_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Kill_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "programInvocationName" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Kill_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Kill_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_GetProgramInvocationAttributes_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Identifier(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ObtainFile_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_ApplicationReference, "sourceFileServer" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_FileName, "sourceFile" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_FileName, "destinationFile" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ObtainFile_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ObtainFile_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_EC_Class_vals[] = {
  {   0, "network-triggered" },
  {   1, "monitored" },
  { 0, NULL }
};


static int dissect_mms_EC_Class(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DefineEventCondition_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventConditionName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_EC_Class, "class" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Priority, "prio-rity" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned8, "severity" },
  { BER_CLASS_CON, 4, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "alarmSummaryReports" },
  { BER_CLASS_CON, 6, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_VariableSpecification, "monitoredVariable" },
  { BER_CLASS_CON, 7, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "evaluationInterval" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DefineEventCondition_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DefineEventCondition_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_DeleteEventCondition_Request_vals[] = {
  {   0, "specific" },
  {   1, "aa-specific" },
  {   2, "domain" },
  {   3, "vmd" },
  { 0, NULL }
};

static const ber_choice_t DeleteEventCondition_Request_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_ObjectName, "specific" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "aa-specific" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "domain" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "vmd" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DeleteEventCondition_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, DeleteEventCondition_Request_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_GetEventConditionAttributes_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ObjectName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_ReportEventConditionStatus_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ObjectName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t AlterEventConditionMonitoring_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventConditionName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "enabled" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Priority, "priority" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "alarmSummaryReports" },
  { BER_CLASS_CON, 4, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "evaluationInterval" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_AlterEventConditionMonitoring_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, AlterEventConditionMonitoring_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t TriggerEvent_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventConditionName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Priority, "priority" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_TriggerEvent_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, TriggerEvent_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DefineEventAction_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventActionName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_Modifier, "listOfModifier" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DefineEventAction_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DefineEventAction_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_DeleteEventAction_Request_vals[] = {
  {   0, "specific" },
  {   1, "aa-specific" },
  {   3, "domain" },
  {   4, "vmd" },
  { 0, NULL }
};

static const ber_choice_t DeleteEventAction_Request_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_ObjectName, "specific" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "aa-specific" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "domain" },
  {   4, BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "vmd" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DeleteEventAction_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, DeleteEventAction_Request_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_GetEventActionAttributes_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ObjectName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_ReportEventActionStatus_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ObjectName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const value_string mms_AlarmAckRule_vals[] = {
  {   0, "none" },
  {   1, "simple" },
  {   2, "ack-active" },
  {   3, "ack-all" },
  { 0, NULL }
};


static int dissect_mms_AlarmAckRule(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DefineEventEnrollment_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventEnrollmentName" },
  { BER_CLASS_CON, 1, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventConditionName" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Transitions, "eventConditionTransition" },
  { BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_AlarmAckRule, "alarmAcknowledgementRule" },
  { BER_CLASS_CON, 4, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventActionName" },
  { BER_CLASS_CON, 5, BER_FLAGS_OPTIONAL, dissect_mms_ApplicationReference, "clientApplication" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DefineEventEnrollment_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DefineEventEnrollment_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_DeleteEventEnrollment_Request_vals[] = {
  {   0, "specific" },
  {   1, "ec" },
  {   2, "ea" },
  { 0, NULL }
};

static const ber_choice_t DeleteEventEnrollment_Request_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_ObjectName, "specific" },
  {   1, BER_CLASS_CON, 1, 0, dissect_mms_ObjectName, "ec" },
  {   2, BER_CLASS_CON, 2, 0, dissect_mms_ObjectName, "ea" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DeleteEventEnrollment_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, DeleteEventEnrollment_Request_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t AlterEventEnrollment_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventEnrollmentName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Transitions, "eventConditionTransitions" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_AlarmAckRule, "alarmAcknowledgmentRule" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_AlterEventEnrollment_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, AlterEventEnrollment_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_ReportEventEnrollmentStatus_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ObjectName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_scopeOfRequest_vals[] = {
  {   0, "specific" },
  {   1, "client" },
  {   2, "ec" },
  {   3, "ea" },
  { 0, NULL }
};


static int dissect_mms_T_scopeOfRequest(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetEventEnrollmentAttributes_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_scopeOfRequest, "scopeOfRequest" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_ObjectName, "eventEnrollmentNames" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL, dissect_mms_ApplicationReference, "clientApplication" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventConditionName" },
  { BER_CLASS_CON, 4, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventActionName" },
  { BER_CLASS_CON, 5, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "continueAfter" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetEventEnrollmentAttributes_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetEventEnrollmentAttributes_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_EC_State_vals[] = {
  {   0, "disabled" },
  {   1, "idle" },
  {   2, "active" },
  { 0, NULL }
};


static int dissect_mms_EC_State(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_EventTime_vals[] = {
  {   0, "timeOfDayT" },
  {   1, "timeSequenceIdentifier" },
  { 0, NULL }
};

static const ber_choice_t EventTime_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_TimeOfDay, "timeOfDayT" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "timeSequenceIdentifier" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_EventTime(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, EventTime_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t AcknowledgeEventNotification_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventEnrollmentName" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_EC_State, "acknowledgedState" },
  { BER_CLASS_CON, 3, BER_FLAGS_NOTCHKTAG, dissect_mms_EventTime, "timeOfAcknowledgedTransition" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_AcknowledgeEventNotification_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, AcknowledgeEventNotification_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_acknowledgmentFilter_vals[] = {
  {   0, "not-acked" },
  {   1, "acked" },
  {   2, "all" },
  { 0, NULL }
};


static int dissect_mms_T_acknowledgmentFilter(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_severityFilter_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned8, "mostSevere" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned8, "leastSevere" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_severityFilter(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_severityFilter_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetAlarmSummary_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "enrollmentsOnly" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "activeAlarmsOnly" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_acknowledgmentFilter, "acknowledgmentFilter" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_severityFilter, "severityFilter" },
  { BER_CLASS_CON, 5, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "continueAfter" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetAlarmSummary_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetAlarmSummary_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_acknowledgmentFilter_01_vals[] = {
  {   0, "not-acked" },
  {   1, "acked" },
  {   2, "all" },
  { 0, NULL }
};


static int dissect_mms_T_acknowledgmentFilter_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_severityFilter_01_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned8, "mostSevere" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned8, "leastSevere" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_severityFilter_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_severityFilter_01_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetAlarmEnrollmentSummary_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "enrollmentsOnly" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "activeAlarmsOnly" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_acknowledgmentFilter_01, "acknowledgmentFilter" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_severityFilter_01, "severityFilter" },
  { BER_CLASS_CON, 5, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "continueAfter" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetAlarmEnrollmentSummary_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetAlarmEnrollmentSummary_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_rangeStartSpecification_vals[] = {
  {   0, "startingTime" },
  {   1, "startingEntry" },
  { 0, NULL }
};

static const ber_choice_t T_rangeStartSpecification_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_TimeOfDay, "startingTime" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_OCTET_STRING, "startingEntry" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_rangeStartSpecification(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_rangeStartSpecification_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_rangeStopSpecification_vals[] = {
  {   0, "endingTime" },
  {   1, "numberOfEntries" },
  { 0, NULL }
};

static const ber_choice_t T_rangeStopSpecification_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_TimeOfDay, "endingTime" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Integer32, "numberOfEntries" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_rangeStopSpecification(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_rangeStopSpecification_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfVariables_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_NOOWNTAG, dissect_mms_VisibleString, "_item" },
};

static int dissect_mms_T_listOfVariables(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfVariables_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_entryToStartAfter_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_TimeOfDay, "timeSpecification" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_OCTET_STRING, "entrySpecification" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_entryToStartAfter(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_entryToStartAfter_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ReadJournal_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "journalName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL, dissect_mms_T_rangeStartSpecification, "rangeStartSpecification" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL, dissect_mms_T_rangeStopSpecification, "rangeStopSpecification" },
  { BER_CLASS_CON, 4, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_listOfVariables, "listOfVariables" },
  { BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_T_entryToStartAfter, "entryToStartAfter" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ReadJournal_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ReadJournal_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_JOU_Additional_Detail(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_event_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventConditionName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_EC_State, "currentState" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_event(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_event_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfVariables_item_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_VisibleString, "variableTag" },
  { BER_CLASS_CON, 1, BER_FLAGS_NOTCHKTAG, dissect_mms_Data, "valueSpecification" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_listOfVariables_item(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfVariables_item_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfVariables_01_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_T_listOfVariables_item, "_item" },
};

static int dissect_mms_T_listOfVariables_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfVariables_01_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_data_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_event, "event" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_listOfVariables_01, "listOfVariables" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_data(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_data_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_entryForm_vals[] = {
  {   2, "data" },
  {   3, "annotation" },
  { 0, NULL }
};

static const ber_choice_t T_entryForm_choice[] = {
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_T_data, "data" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_VisibleString, "annotation" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_entryForm(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_entryForm_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t EntryContent_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_TimeOfDay, "occurenceTime" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL, dissect_mms_JOU_Additional_Detail, "additionalDetail" },
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_T_entryForm, "entryForm" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_EntryContent(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, EntryContent_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SEQUENCE_OF_EntryContent_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_EntryContent, "_item" },
};

static int dissect_mms_SEQUENCE_OF_EntryContent(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SEQUENCE_OF_EntryContent_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t WriteJournal_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "journalName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_EntryContent, "listOfJournalEntry" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_WriteJournal_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, WriteJournal_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_limitSpecification_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_TimeOfDay, "limitingTime" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_OCTET_STRING, "limitingEntry" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_limitSpecification(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_limitSpecification_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t InitializeJournal_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "journalName" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_limitSpecification, "limitSpecification" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_InitializeJournal_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, InitializeJournal_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_ReportJournalStatus_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ObjectName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t CreateJournal_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "journalName" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_CreateJournal_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, CreateJournal_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DeleteJournal_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "journalName" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DeleteJournal_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DeleteJournal_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetCapabilityList_Request_sequence[] = {
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_OPTIONAL|BER_FLAGS_NOOWNTAG, dissect_mms_VisibleString, "continueAfter" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetCapabilityList_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetCapabilityList_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t FileOpen_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_FileName, "fileName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "initialPosition" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_FileOpen_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, FileOpen_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_FileRead_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Integer32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_FileClose_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Integer32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t FileRename_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_FileName, "currentFileName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_FileName, "newFileName" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_FileRename_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, FileRename_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_FileDelete_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_FileName(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t FileDirectory_Request_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_FileName, "fileSpecification" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_FileName, "continueAfter" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_FileDirectory_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, FileDirectory_Request_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_ConfirmedServiceRequest_vals[] = {
  {   0, "status" },
  {   1, "getNameList" },
  {   2, "identify" },
  {   3, "rename" },
  {   4, "read" },
  {   5, "write" },
  {   6, "getVariableAccessAttributes" },
  {   7, "defineNamedVariable" },
  {   8, "defineScatteredAccess" },
  {   9, "getScatteredAccessAttributes" },
  {  10, "deleteVariableAccess" },
  {  11, "defineNamedVariableList" },
  {  12, "getNamedVariableListAttributes" },
  {  13, "deleteNamedVariableList" },
  {  14, "defineNamedType" },
  {  15, "getNamedTypeAttributes" },
  {  16, "deleteNamedType" },
  {  17, "input" },
  {  18, "output" },
  {  19, "takeControl" },
  {  20, "relinquishControl" },
  {  21, "defineSemaphore" },
  {  22, "deleteSemaphore" },
  {  23, "reportSemaphoreStatus" },
  {  24, "reportPoolSemaphoreStatus" },
  {  25, "reportSemaphoreEntryStatus" },
  {  26, "initiateDownloadSequence" },
  {  27, "downloadSegment" },
  {  28, "terminateDownloadSequence" },
  {  29, "initiateUploadSequence" },
  {  30, "uploadSegment" },
  {  31, "terminateUploadSequence" },
  {  32, "requestDomainDownload" },
  {  33, "requestDomainUpload" },
  {  34, "loadDomainContent" },
  {  35, "storeDomainContent" },
  {  36, "deleteDomain" },
  {  37, "getDomainAttributes" },
  {  38, "createProgramInvocation" },
  {  39, "deleteProgramInvocation" },
  {  40, "start" },
  {  41, "stop" },
  {  42, "resume" },
  {  43, "reset" },
  {  44, "kill" },
  {  45, "getProgramInvocationAttributes" },
  {  46, "obtainFile" },
  {  47, "defineEventCondition" },
  {  48, "deleteEventCondition" },
  {  49, "getEventConditionAttributes" },
  {  50, "reportEventConditionStatus" },
  {  51, "alterEventConditionMonitoring" },
  {  52, "triggerEvent" },
  {  53, "defineEventAction" },
  {  54, "deleteEventAction" },
  {  55, "getEventActionAttributes" },
  {  56, "reportEventActionStatus" },
  {  57, "defineEventEnrollment" },
  {  58, "deleteEventEnrollment" },
  {  59, "alterEventEnrollment" },
  {  60, "reportEventEnrollmentStatus" },
  {  61, "getEventEnrollmentAttributes" },
  {  62, "acknowledgeEventNotification" },
  {  63, "getAlarmSummary" },
  {  64, "getAlarmEnrollmentSummary" },
  {  65, "readJournal" },
  {  66, "writeJournal" },
  {  67, "initializeJournal" },
  {  68, "reportJournalStatus" },
  {  69, "createJournal" },
  {  70, "deleteJournal" },
  {  71, "getCapabilityList" },
  {  72, "fileOpen" },
  {  73, "fileRead" },
  {  74, "fileClose" },
  {  75, "fileRename" },
  {  76, "fileDelete" },
  {  77, "fileDirectory" },
  { 0, NULL }
};

static const ber_choice_t ConfirmedServiceRequest_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Status_Request, "status" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_GetNameList_Request, "getNameList" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Identify_Request, "identify" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_Rename_Request, "rename" },
  {   4, BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_Read_Request, "read" },
  {   5, BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_Write_Request, "write" },
  {   6, BER_CLASS_CON, 6, 0, dissect_mms_GetVariableAccessAttributes_Request, "getVariableAccessAttributes" },
  {   7, BER_CLASS_CON, 7, BER_FLAGS_IMPLTAG, dissect_mms_DefineNamedVariable_Request, "defineNamedVariable" },
  {   8, BER_CLASS_CON, 8, BER_FLAGS_IMPLTAG, dissect_mms_DefineScatteredAccess_Request, "defineScatteredAccess" },
  {   9, BER_CLASS_CON, 9, BER_FLAGS_IMPLTAG, dissect_mms_GetScatteredAccessAttributes_Request, "getScatteredAccessAttributes" },
  {  10, BER_CLASS_CON, 10, BER_FLAGS_IMPLTAG, dissect_mms_DeleteVariableAccess_Request, "deleteVariableAccess" },
  {  11, BER_CLASS_CON, 11, BER_FLAGS_IMPLTAG, dissect_mms_DefineNamedVariableList_Request, "defineNamedVariableList" },
  {  12, BER_CLASS_CON, 12, BER_FLAGS_IMPLTAG, dissect_mms_GetNamedVariableListAttributes_Request, "getNamedVariableListAttributes" },
  {  13, BER_CLASS_CON, 13, BER_FLAGS_IMPLTAG, dissect_mms_DeleteNamedVariableList_Request, "deleteNamedVariableList" },
  {  14, BER_CLASS_CON, 14, BER_FLAGS_IMPLTAG, dissect_mms_DefineNamedType_Request, "defineNamedType" },
  {  15, BER_CLASS_CON, 15, BER_FLAGS_IMPLTAG, dissect_mms_GetNamedTypeAttributes_Request, "getNamedTypeAttributes" },
  {  16, BER_CLASS_CON, 16, BER_FLAGS_IMPLTAG, dissect_mms_DeleteNamedType_Request, "deleteNamedType" },
  {  17, BER_CLASS_CON, 17, BER_FLAGS_IMPLTAG, dissect_mms_Input_Request, "input" },
  {  18, BER_CLASS_CON, 18, BER_FLAGS_IMPLTAG, dissect_mms_Output_Request, "output" },
  {  19, BER_CLASS_CON, 19, BER_FLAGS_IMPLTAG, dissect_mms_TakeControl_Request, "takeControl" },
  {  20, BER_CLASS_CON, 20, BER_FLAGS_IMPLTAG, dissect_mms_RelinquishControl_Request, "relinquishControl" },
  {  21, BER_CLASS_CON, 21, BER_FLAGS_IMPLTAG, dissect_mms_DefineSemaphore_Request, "defineSemaphore" },
  {  22, BER_CLASS_CON, 22, BER_FLAGS_IMPLTAG, dissect_mms_DeleteSemaphore_Request, "deleteSemaphore" },
  {  23, BER_CLASS_CON, 23, BER_FLAGS_IMPLTAG, dissect_mms_ReportSemaphoreStatus_Request, "reportSemaphoreStatus" },
  {  24, BER_CLASS_CON, 24, BER_FLAGS_IMPLTAG, dissect_mms_ReportPoolSemaphoreStatus_Request, "reportPoolSemaphoreStatus" },
  {  25, BER_CLASS_CON, 25, BER_FLAGS_IMPLTAG, dissect_mms_ReportSemaphoreEntryStatus_Request, "reportSemaphoreEntryStatus" },
  {  26, BER_CLASS_CON, 26, BER_FLAGS_IMPLTAG, dissect_mms_InitiateDownloadSequence_Request, "initiateDownloadSequence" },
  {  27, BER_CLASS_CON, 27, BER_FLAGS_IMPLTAG, dissect_mms_DownloadSegment_Request, "downloadSegment" },
  {  28, BER_CLASS_CON, 28, BER_FLAGS_IMPLTAG, dissect_mms_TerminateDownloadSequence_Request, "terminateDownloadSequence" },
  {  29, BER_CLASS_CON, 29, BER_FLAGS_IMPLTAG, dissect_mms_InitiateUploadSequence_Request, "initiateUploadSequence" },
  {  30, BER_CLASS_CON, 30, BER_FLAGS_IMPLTAG, dissect_mms_UploadSegment_Request, "uploadSegment" },
  {  31, BER_CLASS_CON, 31, BER_FLAGS_IMPLTAG, dissect_mms_TerminateUploadSequence_Request, "terminateUploadSequence" },
  {  32, BER_CLASS_CON, 32, BER_FLAGS_IMPLTAG, dissect_mms_RequestDomainDownload_Request, "requestDomainDownload" },
  {  33, BER_CLASS_CON, 33, BER_FLAGS_IMPLTAG, dissect_mms_RequestDomainUpload_Request, "requestDomainUpload" },
  {  34, BER_CLASS_CON, 34, BER_FLAGS_IMPLTAG, dissect_mms_LoadDomainContent_Request, "loadDomainContent" },
  {  35, BER_CLASS_CON, 35, BER_FLAGS_IMPLTAG, dissect_mms_StoreDomainContent_Request, "storeDomainContent" },
  {  36, BER_CLASS_CON, 36, BER_FLAGS_IMPLTAG, dissect_mms_DeleteDomain_Request, "deleteDomain" },
  {  37, BER_CLASS_CON, 37, BER_FLAGS_IMPLTAG, dissect_mms_GetDomainAttributes_Request, "getDomainAttributes" },
  {  38, BER_CLASS_CON, 38, BER_FLAGS_IMPLTAG, dissect_mms_CreateProgramInvocation_Request, "createProgramInvocation" },
  {  39, BER_CLASS_CON, 39, BER_FLAGS_IMPLTAG, dissect_mms_DeleteProgramInvocation_Request, "deleteProgramInvocation" },
  {  40, BER_CLASS_CON, 40, BER_FLAGS_IMPLTAG, dissect_mms_Start_Request, "start" },
  {  41, BER_CLASS_CON, 41, BER_FLAGS_IMPLTAG, dissect_mms_Stop_Request, "stop" },
  {  42, BER_CLASS_CON, 42, BER_FLAGS_IMPLTAG, dissect_mms_Resume_Request, "resume" },
  {  43, BER_CLASS_CON, 43, BER_FLAGS_IMPLTAG, dissect_mms_Reset_Request, "reset" },
  {  44, BER_CLASS_CON, 44, BER_FLAGS_IMPLTAG, dissect_mms_Kill_Request, "kill" },
  {  45, BER_CLASS_CON, 45, BER_FLAGS_IMPLTAG, dissect_mms_GetProgramInvocationAttributes_Request, "getProgramInvocationAttributes" },
  {  46, BER_CLASS_CON, 46, BER_FLAGS_IMPLTAG, dissect_mms_ObtainFile_Request, "obtainFile" },
  {  47, BER_CLASS_CON, 47, BER_FLAGS_IMPLTAG, dissect_mms_DefineEventCondition_Request, "defineEventCondition" },
  {  48, BER_CLASS_CON, 48, 0, dissect_mms_DeleteEventCondition_Request, "deleteEventCondition" },
  {  49, BER_CLASS_CON, 49, 0, dissect_mms_GetEventConditionAttributes_Request, "getEventConditionAttributes" },
  {  50, BER_CLASS_CON, 50, 0, dissect_mms_ReportEventConditionStatus_Request, "reportEventConditionStatus" },
  {  51, BER_CLASS_CON, 51, BER_FLAGS_IMPLTAG, dissect_mms_AlterEventConditionMonitoring_Request, "alterEventConditionMonitoring" },
  {  52, BER_CLASS_CON, 52, BER_FLAGS_IMPLTAG, dissect_mms_TriggerEvent_Request, "triggerEvent" },
  {  53, BER_CLASS_CON, 53, BER_FLAGS_IMPLTAG, dissect_mms_DefineEventAction_Request, "defineEventAction" },
  {  54, BER_CLASS_CON, 54, 0, dissect_mms_DeleteEventAction_Request, "deleteEventAction" },
  {  55, BER_CLASS_CON, 55, 0, dissect_mms_GetEventActionAttributes_Request, "getEventActionAttributes" },
  {  56, BER_CLASS_CON, 56, 0, dissect_mms_ReportEventActionStatus_Request, "reportEventActionStatus" },
  {  57, BER_CLASS_CON, 57, BER_FLAGS_IMPLTAG, dissect_mms_DefineEventEnrollment_Request, "defineEventEnrollment" },
  {  58, BER_CLASS_CON, 58, 0, dissect_mms_DeleteEventEnrollment_Request, "deleteEventEnrollment" },
  {  59, BER_CLASS_CON, 59, BER_FLAGS_IMPLTAG, dissect_mms_AlterEventEnrollment_Request, "alterEventEnrollment" },
  {  60, BER_CLASS_CON, 60, 0, dissect_mms_ReportEventEnrollmentStatus_Request, "reportEventEnrollmentStatus" },
  {  61, BER_CLASS_CON, 61, BER_FLAGS_IMPLTAG, dissect_mms_GetEventEnrollmentAttributes_Request, "getEventEnrollmentAttributes" },
  {  62, BER_CLASS_CON, 62, BER_FLAGS_IMPLTAG, dissect_mms_AcknowledgeEventNotification_Request, "acknowledgeEventNotification" },
  {  63, BER_CLASS_CON, 63, BER_FLAGS_IMPLTAG, dissect_mms_GetAlarmSummary_Request, "getAlarmSummary" },
  {  64, BER_CLASS_CON, 64, BER_FLAGS_IMPLTAG, dissect_mms_GetAlarmEnrollmentSummary_Request, "getAlarmEnrollmentSummary" },
  {  65, BER_CLASS_CON, 65, BER_FLAGS_IMPLTAG, dissect_mms_ReadJournal_Request, "readJournal" },
  {  66, BER_CLASS_CON, 66, BER_FLAGS_IMPLTAG, dissect_mms_WriteJournal_Request, "writeJournal" },
  {  67, BER_CLASS_CON, 67, BER_FLAGS_IMPLTAG, dissect_mms_InitializeJournal_Request, "initializeJournal" },
  {  68, BER_CLASS_CON, 68, BER_FLAGS_IMPLTAG, dissect_mms_ReportJournalStatus_Request, "reportJournalStatus" },
  {  69, BER_CLASS_CON, 69, BER_FLAGS_IMPLTAG, dissect_mms_CreateJournal_Request, "createJournal" },
  {  70, BER_CLASS_CON, 70, BER_FLAGS_IMPLTAG, dissect_mms_DeleteJournal_Request, "deleteJournal" },
  {  71, BER_CLASS_CON, 71, BER_FLAGS_IMPLTAG, dissect_mms_GetCapabilityList_Request, "getCapabilityList" },
  {  72, BER_CLASS_CON, 72, BER_FLAGS_IMPLTAG, dissect_mms_FileOpen_Request, "fileOpen" },
  {  73, BER_CLASS_CON, 73, BER_FLAGS_IMPLTAG, dissect_mms_FileRead_Request, "fileRead" },
  {  74, BER_CLASS_CON, 74, BER_FLAGS_IMPLTAG, dissect_mms_FileClose_Request, "fileClose" },
  {  75, BER_CLASS_CON, 75, BER_FLAGS_IMPLTAG, dissect_mms_FileRename_Request, "fileRename" },
  {  76, BER_CLASS_CON, 76, BER_FLAGS_IMPLTAG, dissect_mms_FileDelete_Request, "fileDelete" },
  {  77, BER_CLASS_CON, 77, BER_FLAGS_IMPLTAG, dissect_mms_FileDirectory_Request, "fileDirectory" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ConfirmedServiceRequest(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, ConfirmedServiceRequest_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_CS_Request_Detail_vals[] = {
  {   0, "foo" },
  { 0, NULL }
};

static const ber_choice_t CS_Request_Detail_choice[] = {
  {   0, BER_CLASS_UNI, BER_UNI_TAG_INTEGER, BER_FLAGS_NOOWNTAG, dissect_mms_INTEGER, "foo" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_CS_Request_Detail(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, CS_Request_Detail_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Confirmed_RequestPDU_sequence[] = {
  { BER_CLASS_UNI, BER_UNI_TAG_INTEGER, BER_FLAGS_NOOWNTAG, dissect_mms_Unsigned32, "invokeID" },
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_OPTIONAL|BER_FLAGS_NOOWNTAG, dissect_mms_SEQUENCE_OF_Modifier, "listOfModifier" },
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_ConfirmedServiceRequest, "confirmedServiceRequest" },
  { BER_CLASS_CON, 79, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_CS_Request_Detail, "cs-request-detail" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Confirmed_RequestPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Confirmed_RequestPDU_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_vmdLogicalStatus_vals[] = {
  {   0, "state-changes-allowed" },
  {   1, "no-state-changes-allowed" },
  {   2, "limited-services-allowed" },
  {   3, "support-services-allowed" },
  { 0, NULL }
};


static int dissect_mms_T_vmdLogicalStatus(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_vmdPhysicalStatus_vals[] = {
  {   0, "operational" },
  {   1, "partially-operational" },
  {   2, "inoperable" },
  {   3, "needs-commissioning" },
  { 0, NULL }
};


static int dissect_mms_T_vmdPhysicalStatus(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_BIT_STRING_SIZE_0_128(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_bitstring(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Status_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_T_vmdLogicalStatus, "vmdLogicalStatus" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_vmdPhysicalStatus, "vmdPhysicalStatus" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BIT_STRING_SIZE_0_128, "localDetail" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Status_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Status_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetNameList_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_Identifier, "listOfIdentifier" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "moreFollows" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetNameList_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetNameList_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfAbstractSyntaxes_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_OID, BER_FLAGS_NOOWNTAG, dissect_mms_OBJECT_IDENTIFIER, "_item" },
};

static int dissect_mms_T_listOfAbstractSyntaxes(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfAbstractSyntaxes_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Identify_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_VisibleString, "vendorName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_VisibleString, "modelName" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_VisibleString, "revision" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_listOfAbstractSyntaxes, "listOfAbstractSyntaxes" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Identify_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Identify_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Rename_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_DataAccessError_vals[] = {
  {   0, "object-invalidated" },
  {   1, "hardware-fault" },
  {   2, "temporarily-unavailable" },
  {   3, "object-access-denied" },
  {   4, "object-undefined" },
  {   5, "invalid-address" },
  {   6, "type-unsupported" },
  {   7, "type-inconsistent" },
  {   8, "object-attribute-inconsistent" },
  {   9, "object-access-unsupported" },
  {  10, "object-non-existent" },
  { 0, NULL }
};


static int dissect_mms_DataAccessError(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_AccessResult_vals[] = {
  {   0, "failure" },
  {   1, "success" },
  { 0, NULL }
};

static const ber_choice_t AccessResult_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_DataAccessError, "failure" },
  {   1, BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG, dissect_mms_Data, "success" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_AccessResult(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, AccessResult_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SEQUENCE_OF_AccessResult_sequence_of[1] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_AccessResult, "_item" },
};

static int dissect_mms_SEQUENCE_OF_AccessResult(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SEQUENCE_OF_AccessResult_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Read_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_VariableAccessSpecification, "variableAccessSpecificatn" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_AccessResult, "listOfAccessResult" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Read_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Read_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_Write_Response_item_vals[] = {
  {   0, "failure" },
  {   1, "success" },
  { 0, NULL }
};

static const ber_choice_t Write_Response_item_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_DataAccessError, "failure" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "success" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Write_Response_item(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, Write_Response_item_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Write_Response_sequence_of[1] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_Write_Response_item, "_item" },
};

static int dissect_mms_Write_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Write_Response_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetVariableAccessAttributes_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "mmsDeletable" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_Address, "address" },
  { BER_CLASS_CON, 2, BER_FLAGS_NOTCHKTAG, dissect_mms_TypeSpecification, "typeSpecification" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetVariableAccessAttributes_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetVariableAccessAttributes_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DefineNamedVariable_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DefineScatteredAccess_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetScatteredAccessAttributes_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "mmsDeletable" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_ScatteredAccessDescription, "scatteredAccessDescription" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetScatteredAccessAttributes_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetScatteredAccessAttributes_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DeleteVariableAccess_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "numberMatched" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "numberDeleted" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DeleteVariableAccess_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DeleteVariableAccess_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DefineNamedVariableList_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfVariable_item_01_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_VariableSpecification, "variableSpecification" },
  { BER_CLASS_CON, 5, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_AlternateAccess, "alternateAccess" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_listOfVariable_item_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfVariable_item_01_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfVariable_01_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_T_listOfVariable_item_01, "_item" },
};

static int dissect_mms_T_listOfVariable_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfVariable_01_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetNamedVariableListAttributes_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "mmsDeletable" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_listOfVariable_01, "listOfVariable" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetNamedVariableListAttributes_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetNamedVariableListAttributes_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DeleteNamedVariableList_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "numberMatched" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "numberDeleted" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DeleteNamedVariableList_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DeleteNamedVariableList_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DefineNamedType_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetNamedTypeAttributes_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "mmsDeletable" },
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_TypeSpecification, "typeSpecification" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetNamedTypeAttributes_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetNamedTypeAttributes_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DeleteNamedType_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "numberMatched" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "numberDeleted" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DeleteNamedType_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DeleteNamedType_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Input_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_restricted_string(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Output_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_TakeControl_Response_vals[] = {
  {   0, "noResult" },
  {   1, "namedToken" },
  { 0, NULL }
};

static const ber_choice_t TakeControl_Response_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "noResult" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "namedToken" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_TakeControl_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, TakeControl_Response_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_RelinquishControl_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DefineSemaphore_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteSemaphore_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_class_vals[] = {
  {   0, "token" },
  {   1, "pool" },
  { 0, NULL }
};


static int dissect_mms_T_class(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ReportSemaphoreStatus_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "mmsDeletable" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_class, "class" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned16, "numberOfTokens" },
  { BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned16, "numberOfOwnedTokens" },
  { BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned16, "numberOfHungTokens" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ReportSemaphoreStatus_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ReportSemaphoreStatus_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_listOfNamedTokens_item_vals[] = {
  {   0, "freeNamedToken" },
  {   1, "ownedNamedToken" },
  {   2, "hungNamedToken" },
  { 0, NULL }
};

static const ber_choice_t T_listOfNamedTokens_item_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "freeNamedToken" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "ownedNamedToken" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "hungNamedToken" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_listOfNamedTokens_item(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_listOfNamedTokens_item_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfNamedTokens_sequence_of[1] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_T_listOfNamedTokens_item, "_item" },
};

static int dissect_mms_T_listOfNamedTokens(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfNamedTokens_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ReportPoolSemaphoreStatus_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_T_listOfNamedTokens, "listOfNamedTokens" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "moreFollows" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ReportPoolSemaphoreStatus_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ReportPoolSemaphoreStatus_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_entryClass_vals[] = {
  {   0, "simple" },
  {   1, "modifier" },
  { 0, NULL }
};


static int dissect_mms_T_entryClass(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SemaphoreEntry_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_OCTET_STRING, "entryId" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_entryClass, "entryClass" },
  { BER_CLASS_CON, 2, 0, dissect_mms_ApplicationReference, "applicationReference" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Identifier, "namedToken" },
  { BER_CLASS_CON, 4, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Priority, "priority" },
  { BER_CLASS_CON, 5, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "remainingTimeOut" },
  { BER_CLASS_CON, 6, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "abortOnTimeOut" },
  { BER_CLASS_CON, 7, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "relinquishIfConnectionLost" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_SemaphoreEntry(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SemaphoreEntry_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SEQUENCE_OF_SemaphoreEntry_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_SemaphoreEntry, "_item" },
};

static int dissect_mms_SEQUENCE_OF_SemaphoreEntry(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SEQUENCE_OF_SemaphoreEntry_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ReportSemaphoreEntryStatus_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_SemaphoreEntry, "listOfSemaphoreEntry" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "moreFollows" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ReportSemaphoreEntryStatus_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ReportSemaphoreEntryStatus_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_InitiateDownloadSequence_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_loadData_vals[] = {
  {   0, "non-coded" },
  {   1, "coded" },
  { 0, NULL }
};

static const ber_choice_t T_loadData_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_OCTET_STRING, "non-coded" },
  {   1, -1/*imported*/, -1/*imported*/, BER_FLAGS_NOOWNTAG, dissect_acse_EXTERNALt, "coded" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_loadData(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_loadData_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DownloadSegment_Response_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_T_loadData, "loadData" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "moreFollows" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DownloadSegment_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DownloadSegment_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_TerminateDownloadSequence_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfCapabilities_02_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_NOOWNTAG, dissect_mms_VisibleString, "_item" },
};

static int dissect_mms_T_listOfCapabilities_02(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfCapabilities_02_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t InitiateUploadSequence_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Integer32, "ulsmID" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_listOfCapabilities_02, "listOfCapabilities" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_InitiateUploadSequence_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, InitiateUploadSequence_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_loadData_01_vals[] = {
  {   0, "non-coded" },
  {   1, "coded" },
  { 0, NULL }
};

static const ber_choice_t T_loadData_01_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_OCTET_STRING, "non-coded" },
  {   1, -1/*imported*/, -1/*imported*/, BER_FLAGS_NOOWNTAG, dissect_acse_EXTERNALt, "coded" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_loadData_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_loadData_01_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t UploadSegment_Response_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_T_loadData_01, "loadData" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "moreFollows" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_UploadSegment_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, UploadSegment_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_TerminateUploadSequence_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_RequestDomainDownload_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_RequestDomainUpload_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_LoadDomainContent_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_StoreDomainContent_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteDomain_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfCapabilities_05_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_NOOWNTAG, dissect_mms_VisibleString, "_item" },
};

static int dissect_mms_T_listOfCapabilities_05(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfCapabilities_05_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_DomainState_vals[] = {
  {   0, "non-existent" },
  {   1, "loading" },
  {   2, "ready" },
  {   3, "in-use" },
  {   4, "complete" },
  {   5, "incomplete" },
  {   7, "d1" },
  {   8, "d2" },
  {   9, "d3" },
  {  10, "d4" },
  {  11, "d5" },
  {  12, "d6" },
  {  13, "d7" },
  {  14, "d8" },
  {  15, "d9" },
  { 0, NULL }
};


static int dissect_mms_DomainState(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Integer8(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetDomainAttributes_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_T_listOfCapabilities_05, "listOfCapabilities" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_DomainState, "state" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "mmsDeletable" },
  { BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "sharable" },
  { BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_Identifier, "listOfProgramInvocations" },
  { BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_Integer8, "uploadInProgress" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetDomainAttributes_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetDomainAttributes_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_CreateProgramInvocation_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteProgramInvocation_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Start_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Stop_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Resume_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Reset_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Kill_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_executionArgument_02_vals[] = {
  {   0, "simpleString" },
  {   1, "encodedString" },
  { 0, NULL }
};

static const ber_choice_t T_executionArgument_02_choice[] = {
  {   0, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_VisibleString, "simpleString" },
  {   1, -1/*imported*/, -1/*imported*/, BER_FLAGS_NOOWNTAG, dissect_acse_EXTERNALt, "encodedString" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_executionArgument_02(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_executionArgument_02_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetProgramInvocationAttributes_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_ProgramInvocationState, "state" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_Identifier, "listOfDomainNames" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "mmsDeletable" },
  { BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "reusable" },
  { BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "monitor" },
  { BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_VisibleString, "startArgument" },
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_OPTIONAL|BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_T_executionArgument_02, "executionArgument" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetProgramInvocationAttributes_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetProgramInvocationAttributes_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_ObtainFile_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_GeneralizedTime(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_GeneralizedTime(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t FileAttributes_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "sizeOfFile" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_GeneralizedTime, "lastModified" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_FileAttributes(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, FileAttributes_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t FileOpen_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Integer32, "frsmID" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_FileAttributes, "fileAttributes" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_FileOpen_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, FileOpen_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DefineEventCondition_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteEventCondition_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Unsigned32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_monitoredVariable_vals[] = {
  {   0, "variableReference" },
  {   1, "undefined" },
  { 0, NULL }
};

static const ber_choice_t T_monitoredVariable_choice[] = {
  {   0, BER_CLASS_CON, 0, 0, dissect_mms_VariableSpecification, "variableReference" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "undefined" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_monitoredVariable(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_monitoredVariable_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetEventConditionAttributes_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "mmsDeletable" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_EC_Class, "class" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Priority, "prio-rity" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned8, "severity" },
  { BER_CLASS_CON, 4, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "alarmSummaryReports" },
  { BER_CLASS_CON, 6, BER_FLAGS_OPTIONAL, dissect_mms_T_monitoredVariable, "monitoredVariable" },
  { BER_CLASS_CON, 7, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "evaluationInterval" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetEventConditionAttributes_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetEventConditionAttributes_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ReportEventConditionStatus_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_EC_State, "currentState" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "numberOfEventEnrollments" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "enabled" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_EventTime, "timeOfLastTransitionToActive" },
  { BER_CLASS_CON, 4, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_EventTime, "timeOfLastTransitionToIdle" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ReportEventConditionStatus_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ReportEventConditionStatus_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_AlterEventConditionMonitoring_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_TriggerEvent_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DefineEventAction_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteEventAction_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Unsigned32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetEventActionAttributes_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "mmsDeletable" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_Modifier, "listOfModifier" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetEventActionAttributes_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetEventActionAttributes_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_ReportEventActionStatus_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Unsigned32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DefineEventEnrollment_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteEventEnrollment_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Unsigned32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const value_string mms_EE_State_vals[] = {
  {   0, "disabled" },
  {   1, "idle" },
  {   2, "active" },
  {   3, "activeNoAckA" },
  {   4, "idleNoAckI" },
  {   5, "idleNoAckA" },
  {   6, "idleAcked" },
  {   7, "activeAcked" },
  { 0, NULL }
};


static int dissect_mms_EE_State(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_currentState_vals[] = {
  {   0, "state" },
  {   1, "undefined" },
  { 0, NULL }
};

static const ber_choice_t T_currentState_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_EE_State, "state" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "undefined" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_currentState(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_currentState_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t AlterEventEnrollment_Response_sequence[] = {
  { BER_CLASS_CON, 0, 0, dissect_mms_T_currentState, "currentState" },
  { BER_CLASS_CON, 1, BER_FLAGS_NOTCHKTAG, dissect_mms_EventTime, "transitionTime" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_AlterEventEnrollment_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, AlterEventEnrollment_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_EE_Duration_vals[] = {
  {   0, "current" },
  {   1, "permanent" },
  { 0, NULL }
};


static int dissect_mms_EE_Duration(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ReportEventEnrollmentStatus_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Transitions, "eventConditionTransitions" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "notificationLost" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_EE_Duration, "duration" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_AlarmAckRule, "alarmAcknowledgmentRule" },
  { BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_EE_State, "currentState" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ReportEventEnrollmentStatus_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ReportEventEnrollmentStatus_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_eventConditionName_vals[] = {
  {   0, "eventCondition" },
  {   1, "undefined" },
  { 0, NULL }
};

static const ber_choice_t T_eventConditionName_choice[] = {
  {   0, BER_CLASS_CON, 0, 0, dissect_mms_ObjectName, "eventCondition" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "undefined" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_eventConditionName(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_eventConditionName_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_eventActionName_vals[] = {
  {   0, "eventAction" },
  {   1, "undefined" },
  { 0, NULL }
};

static const ber_choice_t T_eventActionName_choice[] = {
  {   0, BER_CLASS_CON, 0, 0, dissect_mms_ObjectName, "eventAction" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "undefined" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_eventActionName(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_eventActionName_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_EE_Class_vals[] = {
  {   0, "modifier" },
  {   1, "notification" },
  { 0, NULL }
};


static int dissect_mms_EE_Class(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t EventEnrollment_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventEnrollmentName" },
  { BER_CLASS_CON, 1, 0, dissect_mms_T_eventConditionName, "eventConditionName" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL, dissect_mms_T_eventActionName, "eventActionName" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL, dissect_mms_ApplicationReference, "clientApplication" },
  { BER_CLASS_CON, 4, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "mmsDeletable" },
  { BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_EE_Class, "enrollmentClass" },
  { BER_CLASS_CON, 6, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_EE_Duration, "duration" },
  { BER_CLASS_CON, 7, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "invokeID" },
  { BER_CLASS_CON, 8, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "remainingAcceptableDelay" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_EventEnrollment(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, EventEnrollment_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SEQUENCE_OF_EventEnrollment_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_EventEnrollment, "_item" },
};

static int dissect_mms_SEQUENCE_OF_EventEnrollment(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SEQUENCE_OF_EventEnrollment_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetEventEnrollmentAttributes_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_EventEnrollment, "listOfEventEnrollment" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "moreFollows" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetEventEnrollmentAttributes_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetEventEnrollmentAttributes_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_AcknowledgeEventNotification_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_unacknowledgedState_vals[] = {
  {   0, "none" },
  {   1, "active" },
  {   2, "idle" },
  {   3, "both" },
  { 0, NULL }
};


static int dissect_mms_T_unacknowledgedState(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t AlarmSummary_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventConditionName" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned8, "severity" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_EC_State, "currentState" },
  { BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_T_unacknowledgedState, "unacknowledgedState" },
  { BER_CLASS_CON, 5, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_EventTime, "timeOfLastTransitionToActive" },
  { BER_CLASS_CON, 6, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_EventTime, "timeOfLastTransitionToIdle" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_AlarmSummary(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, AlarmSummary_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SEQUENCE_OF_AlarmSummary_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_AlarmSummary, "_item" },
};

static int dissect_mms_SEQUENCE_OF_AlarmSummary(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SEQUENCE_OF_AlarmSummary_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetAlarmSummary_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_AlarmSummary, "listOfAlarmSummary" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "moreFollows" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetAlarmSummary_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetAlarmSummary_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t AlarmEnrollmentSummary_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventEnrollmentName" },
  { BER_CLASS_CON, 2, BER_FLAGS_OPTIONAL, dissect_mms_ApplicationReference, "clientApplication" },
  { BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned8, "severity" },
  { BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_EC_State, "currentState" },
  { BER_CLASS_CON, 6, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "notificationLost" },
  { BER_CLASS_CON, 7, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_AlarmAckRule, "alarmAcknowledgmentRule" },
  { BER_CLASS_CON, 8, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_EE_State, "enrollementState" },
  { BER_CLASS_CON, 9, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_EventTime, "timeOfLastTransitionToActive" },
  { BER_CLASS_CON, 10, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_EventTime, "timeActiveAcknowledged" },
  { BER_CLASS_CON, 11, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_EventTime, "timeOfLastTransitionToIdle" },
  { BER_CLASS_CON, 12, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_EventTime, "timeIdleAcknowledged" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_AlarmEnrollmentSummary(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, AlarmEnrollmentSummary_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SEQUENCE_OF_AlarmEnrollmentSummary_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_AlarmEnrollmentSummary, "_item" },
};

static int dissect_mms_SEQUENCE_OF_AlarmEnrollmentSummary(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SEQUENCE_OF_AlarmEnrollmentSummary_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetAlarmEnrollmentSummary_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_AlarmEnrollmentSummary, "listOfAlarmEnrollmentSummary" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "moreFollows" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetAlarmEnrollmentSummary_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetAlarmEnrollmentSummary_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t JournalEntry_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_OCTET_STRING, "entryIdentifier" },
  { BER_CLASS_CON, 1, 0, dissect_mms_ApplicationReference, "originatingApplication" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_EntryContent, "entryContent" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_JournalEntry(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, JournalEntry_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SEQUENCE_OF_JournalEntry_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_JournalEntry, "_item" },
};

static int dissect_mms_SEQUENCE_OF_JournalEntry(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SEQUENCE_OF_JournalEntry_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ReadJournal_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_JournalEntry, "listOfJournalEntry" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "moreFollows" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ReadJournal_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ReadJournal_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_WriteJournal_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_InitializeJournal_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Unsigned32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t ReportJournalStatus_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "currentEntries" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "mmsDeletable" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ReportJournalStatus_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ReportJournalStatus_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_CreateJournal_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_DeleteJournal_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_listOfCapabilities_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_VisibleString, BER_FLAGS_NOOWNTAG, dissect_mms_VisibleString, "_item" },
};

static int dissect_mms_T_listOfCapabilities(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_listOfCapabilities_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t GetCapabilityList_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_T_listOfCapabilities, "listOfCapabilities" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "moreFollows" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_GetCapabilityList_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, GetCapabilityList_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t FileRead_Response_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_OCTET_STRING, "fileData" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "moreFollows" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_FileRead_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, FileRead_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_FileClose_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_FileRename_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_FileDelete_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t DirectoryEntry_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_FileName, "filename" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_FileAttributes, "fileAttributes" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_DirectoryEntry(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, DirectoryEntry_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t SEQUENCE_OF_DirectoryEntry_sequence_of[1] = {
  { BER_CLASS_UNI, BER_UNI_TAG_SEQUENCE, BER_FLAGS_NOOWNTAG, dissect_mms_DirectoryEntry, "_item" },
};

static int dissect_mms_SEQUENCE_OF_DirectoryEntry(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence_of(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, SEQUENCE_OF_DirectoryEntry_sequence_of, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t FileDirectory_Response_sequence[] = {
  { BER_CLASS_CON, 0, 0, dissect_mms_SEQUENCE_OF_DirectoryEntry, "listOfDirectoryEntry" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "moreFollows" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_FileDirectory_Response(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, FileDirectory_Response_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_ConfirmedServiceResponse_vals[] = {
  {   0, "status" },
  {   1, "getNameList" },
  {   2, "identify" },
  {   3, "rename" },
  {   4, "read" },
  {   5, "write" },
  {   6, "getVariableAccessAttributes" },
  {   7, "defineNamedVariable" },
  {   8, "defineScatteredAccess" },
  {   9, "getScatteredAccessAttributes" },
  {  10, "deleteVariableAccess" },
  {  11, "defineNamedVariableList" },
  {  12, "getNamedVariableListAttributes" },
  {  13, "deleteNamedVariableList" },
  {  14, "defineNamedType" },
  {  15, "getNamedTypeAttributes" },
  {  16, "deleteNamedType" },
  {  17, "input" },
  {  18, "output" },
  {  19, "takeControl" },
  {  20, "relinquishControl" },
  {  21, "defineSemaphore" },
  {  22, "deleteSemaphore" },
  {  23, "reportSemaphoreStatus" },
  {  24, "reportPoolSemaphoreStatus" },
  {  25, "reportSemaphoreEntryStatus" },
  {  26, "initiateDownloadSequence" },
  {  27, "downloadSegment" },
  {  28, "terminateDownloadSequence" },
  {  29, "initiateUploadSequence" },
  {  30, "uploadSegment" },
  {  31, "terminateUploadSequence" },
  {  32, "requestDomainDownLoad" },
  {  33, "requestDomainUpload" },
  {  34, "loadDomainContent" },
  {  35, "storeDomainContent" },
  {  36, "deleteDomain" },
  {  37, "getDomainAttributes" },
  {  38, "createProgramInvocation" },
  {  39, "deleteProgramInvocation" },
  {  40, "start" },
  {  41, "stop" },
  {  42, "resume" },
  {  43, "reset" },
  {  44, "kill" },
  {  45, "getProgramInvocationAttributes" },
  {  46, "obtainFile" },
  {  72, "fileOpen" },
  {  47, "defineEventCondition" },
  {  48, "deleteEventCondition" },
  {  49, "getEventConditionAttributes" },
  {  50, "reportEventConditionStatus" },
  {  51, "alterEventConditionMonitoring" },
  {  52, "triggerEvent" },
  {  53, "defineEventAction" },
  {  54, "deleteEventAction" },
  {  55, "getEventActionAttributes" },
  {  56, "reportActionStatus" },
  {  57, "defineEventEnrollment" },
  {  58, "deleteEventEnrollment" },
  {  59, "alterEventEnrollment" },
  {  60, "reportEventEnrollmentStatus" },
  {  61, "getEventEnrollmentAttributes" },
  {  62, "acknowledgeEventNotification" },
  {  63, "getAlarmSummary" },
  {  64, "getAlarmEnrollmentSummary" },
  {  65, "readJournal" },
  {  66, "writeJournal" },
  {  67, "initializeJournal" },
  {  68, "reportJournalStatus" },
  {  69, "createJournal" },
  {  70, "deleteJournal" },
  {  71, "getCapabilityList" },
  {  73, "fileRead" },
  {  74, "fileClose" },
  {  75, "fileRename" },
  {  76, "fileDelete" },
  {  77, "fileDirectory" },
  { 0, NULL }
};

static const ber_choice_t ConfirmedServiceResponse_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Status_Response, "status" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_GetNameList_Response, "getNameList" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Identify_Response, "identify" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_Rename_Response, "rename" },
  {   4, BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_Read_Response, "read" },
  {   5, BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_Write_Response, "write" },
  {   6, BER_CLASS_CON, 6, BER_FLAGS_IMPLTAG, dissect_mms_GetVariableAccessAttributes_Response, "getVariableAccessAttributes" },
  {   7, BER_CLASS_CON, 7, BER_FLAGS_IMPLTAG, dissect_mms_DefineNamedVariable_Response, "defineNamedVariable" },
  {   8, BER_CLASS_CON, 8, BER_FLAGS_IMPLTAG, dissect_mms_DefineScatteredAccess_Response, "defineScatteredAccess" },
  {   9, BER_CLASS_CON, 9, BER_FLAGS_IMPLTAG, dissect_mms_GetScatteredAccessAttributes_Response, "getScatteredAccessAttributes" },
  {  10, BER_CLASS_CON, 10, BER_FLAGS_IMPLTAG, dissect_mms_DeleteVariableAccess_Response, "deleteVariableAccess" },
  {  11, BER_CLASS_CON, 11, BER_FLAGS_IMPLTAG, dissect_mms_DefineNamedVariableList_Response, "defineNamedVariableList" },
  {  12, BER_CLASS_CON, 12, BER_FLAGS_IMPLTAG, dissect_mms_GetNamedVariableListAttributes_Response, "getNamedVariableListAttributes" },
  {  13, BER_CLASS_CON, 13, BER_FLAGS_IMPLTAG, dissect_mms_DeleteNamedVariableList_Response, "deleteNamedVariableList" },
  {  14, BER_CLASS_CON, 14, BER_FLAGS_IMPLTAG, dissect_mms_DefineNamedType_Response, "defineNamedType" },
  {  15, BER_CLASS_CON, 15, BER_FLAGS_IMPLTAG, dissect_mms_GetNamedTypeAttributes_Response, "getNamedTypeAttributes" },
  {  16, BER_CLASS_CON, 16, BER_FLAGS_IMPLTAG, dissect_mms_DeleteNamedType_Response, "deleteNamedType" },
  {  17, BER_CLASS_CON, 17, BER_FLAGS_IMPLTAG, dissect_mms_Input_Response, "input" },
  {  18, BER_CLASS_CON, 18, BER_FLAGS_IMPLTAG, dissect_mms_Output_Response, "output" },
  {  19, BER_CLASS_CON, 19, 0, dissect_mms_TakeControl_Response, "takeControl" },
  {  20, BER_CLASS_CON, 20, BER_FLAGS_IMPLTAG, dissect_mms_RelinquishControl_Response, "relinquishControl" },
  {  21, BER_CLASS_CON, 21, BER_FLAGS_IMPLTAG, dissect_mms_DefineSemaphore_Response, "defineSemaphore" },
  {  22, BER_CLASS_CON, 22, BER_FLAGS_IMPLTAG, dissect_mms_DeleteSemaphore_Response, "deleteSemaphore" },
  {  23, BER_CLASS_CON, 23, BER_FLAGS_IMPLTAG, dissect_mms_ReportSemaphoreStatus_Response, "reportSemaphoreStatus" },
  {  24, BER_CLASS_CON, 24, BER_FLAGS_IMPLTAG, dissect_mms_ReportPoolSemaphoreStatus_Response, "reportPoolSemaphoreStatus" },
  {  25, BER_CLASS_CON, 25, BER_FLAGS_IMPLTAG, dissect_mms_ReportSemaphoreEntryStatus_Response, "reportSemaphoreEntryStatus" },
  {  26, BER_CLASS_CON, 26, BER_FLAGS_IMPLTAG, dissect_mms_InitiateDownloadSequence_Response, "initiateDownloadSequence" },
  {  27, BER_CLASS_CON, 27, BER_FLAGS_IMPLTAG, dissect_mms_DownloadSegment_Response, "downloadSegment" },
  {  28, BER_CLASS_CON, 28, BER_FLAGS_IMPLTAG, dissect_mms_TerminateDownloadSequence_Response, "terminateDownloadSequence" },
  {  29, BER_CLASS_CON, 29, BER_FLAGS_IMPLTAG, dissect_mms_InitiateUploadSequence_Response, "initiateUploadSequence" },
  {  30, BER_CLASS_CON, 30, BER_FLAGS_IMPLTAG, dissect_mms_UploadSegment_Response, "uploadSegment" },
  {  31, BER_CLASS_CON, 31, BER_FLAGS_IMPLTAG, dissect_mms_TerminateUploadSequence_Response, "terminateUploadSequence" },
  {  32, BER_CLASS_CON, 32, BER_FLAGS_IMPLTAG, dissect_mms_RequestDomainDownload_Response, "requestDomainDownLoad" },
  {  33, BER_CLASS_CON, 33, BER_FLAGS_IMPLTAG, dissect_mms_RequestDomainUpload_Response, "requestDomainUpload" },
  {  34, BER_CLASS_CON, 34, BER_FLAGS_IMPLTAG, dissect_mms_LoadDomainContent_Response, "loadDomainContent" },
  {  35, BER_CLASS_CON, 35, BER_FLAGS_IMPLTAG, dissect_mms_StoreDomainContent_Response, "storeDomainContent" },
  {  36, BER_CLASS_CON, 36, BER_FLAGS_IMPLTAG, dissect_mms_DeleteDomain_Response, "deleteDomain" },
  {  37, BER_CLASS_CON, 37, BER_FLAGS_IMPLTAG, dissect_mms_GetDomainAttributes_Response, "getDomainAttributes" },
  {  38, BER_CLASS_CON, 38, BER_FLAGS_IMPLTAG, dissect_mms_CreateProgramInvocation_Response, "createProgramInvocation" },
  {  39, BER_CLASS_CON, 39, BER_FLAGS_IMPLTAG, dissect_mms_DeleteProgramInvocation_Response, "deleteProgramInvocation" },
  {  40, BER_CLASS_CON, 40, BER_FLAGS_IMPLTAG, dissect_mms_Start_Response, "start" },
  {  41, BER_CLASS_CON, 41, BER_FLAGS_IMPLTAG, dissect_mms_Stop_Response, "stop" },
  {  42, BER_CLASS_CON, 42, BER_FLAGS_IMPLTAG, dissect_mms_Resume_Response, "resume" },
  {  43, BER_CLASS_CON, 43, BER_FLAGS_IMPLTAG, dissect_mms_Reset_Response, "reset" },
  {  44, BER_CLASS_CON, 44, BER_FLAGS_IMPLTAG, dissect_mms_Kill_Response, "kill" },
  {  45, BER_CLASS_CON, 45, BER_FLAGS_IMPLTAG, dissect_mms_GetProgramInvocationAttributes_Response, "getProgramInvocationAttributes" },
  {  46, BER_CLASS_CON, 46, BER_FLAGS_IMPLTAG, dissect_mms_ObtainFile_Response, "obtainFile" },
  {  72, BER_CLASS_CON, 72, BER_FLAGS_IMPLTAG, dissect_mms_FileOpen_Response, "fileOpen" },
  {  47, BER_CLASS_CON, 47, BER_FLAGS_IMPLTAG, dissect_mms_DefineEventCondition_Response, "defineEventCondition" },
  {  48, BER_CLASS_CON, 48, BER_FLAGS_IMPLTAG, dissect_mms_DeleteEventCondition_Response, "deleteEventCondition" },
  {  49, BER_CLASS_CON, 49, BER_FLAGS_IMPLTAG, dissect_mms_GetEventConditionAttributes_Response, "getEventConditionAttributes" },
  {  50, BER_CLASS_CON, 50, BER_FLAGS_IMPLTAG, dissect_mms_ReportEventConditionStatus_Response, "reportEventConditionStatus" },
  {  51, BER_CLASS_CON, 51, BER_FLAGS_IMPLTAG, dissect_mms_AlterEventConditionMonitoring_Response, "alterEventConditionMonitoring" },
  {  52, BER_CLASS_CON, 52, BER_FLAGS_IMPLTAG, dissect_mms_TriggerEvent_Response, "triggerEvent" },
  {  53, BER_CLASS_CON, 53, BER_FLAGS_IMPLTAG, dissect_mms_DefineEventAction_Response, "defineEventAction" },
  {  54, BER_CLASS_CON, 54, BER_FLAGS_IMPLTAG, dissect_mms_DeleteEventAction_Response, "deleteEventAction" },
  {  55, BER_CLASS_CON, 55, BER_FLAGS_IMPLTAG, dissect_mms_GetEventActionAttributes_Response, "getEventActionAttributes" },
  {  56, BER_CLASS_CON, 56, BER_FLAGS_IMPLTAG, dissect_mms_ReportEventActionStatus_Response, "reportActionStatus" },
  {  57, BER_CLASS_CON, 57, BER_FLAGS_IMPLTAG, dissect_mms_DefineEventEnrollment_Response, "defineEventEnrollment" },
  {  58, BER_CLASS_CON, 58, BER_FLAGS_IMPLTAG, dissect_mms_DeleteEventEnrollment_Response, "deleteEventEnrollment" },
  {  59, BER_CLASS_CON, 59, BER_FLAGS_IMPLTAG, dissect_mms_AlterEventEnrollment_Response, "alterEventEnrollment" },
  {  60, BER_CLASS_CON, 60, BER_FLAGS_IMPLTAG, dissect_mms_ReportEventEnrollmentStatus_Response, "reportEventEnrollmentStatus" },
  {  61, BER_CLASS_CON, 61, BER_FLAGS_IMPLTAG, dissect_mms_GetEventEnrollmentAttributes_Response, "getEventEnrollmentAttributes" },
  {  62, BER_CLASS_CON, 62, BER_FLAGS_IMPLTAG, dissect_mms_AcknowledgeEventNotification_Response, "acknowledgeEventNotification" },
  {  63, BER_CLASS_CON, 63, BER_FLAGS_IMPLTAG, dissect_mms_GetAlarmSummary_Response, "getAlarmSummary" },
  {  64, BER_CLASS_CON, 64, BER_FLAGS_IMPLTAG, dissect_mms_GetAlarmEnrollmentSummary_Response, "getAlarmEnrollmentSummary" },
  {  65, BER_CLASS_CON, 65, BER_FLAGS_IMPLTAG, dissect_mms_ReadJournal_Response, "readJournal" },
  {  66, BER_CLASS_CON, 66, BER_FLAGS_IMPLTAG, dissect_mms_WriteJournal_Response, "writeJournal" },
  {  67, BER_CLASS_CON, 67, BER_FLAGS_IMPLTAG, dissect_mms_InitializeJournal_Response, "initializeJournal" },
  {  68, BER_CLASS_CON, 68, BER_FLAGS_IMPLTAG, dissect_mms_ReportJournalStatus_Response, "reportJournalStatus" },
  {  69, BER_CLASS_CON, 69, BER_FLAGS_IMPLTAG, dissect_mms_CreateJournal_Response, "createJournal" },
  {  70, BER_CLASS_CON, 70, BER_FLAGS_IMPLTAG, dissect_mms_DeleteJournal_Response, "deleteJournal" },
  {  71, BER_CLASS_CON, 71, BER_FLAGS_IMPLTAG, dissect_mms_GetCapabilityList_Response, "getCapabilityList" },
  {  73, BER_CLASS_CON, 73, BER_FLAGS_IMPLTAG, dissect_mms_FileRead_Response, "fileRead" },
  {  74, BER_CLASS_CON, 74, BER_FLAGS_IMPLTAG, dissect_mms_FileClose_Response, "fileClose" },
  {  75, BER_CLASS_CON, 75, BER_FLAGS_IMPLTAG, dissect_mms_FileRename_Response, "fileRename" },
  {  76, BER_CLASS_CON, 76, BER_FLAGS_IMPLTAG, dissect_mms_FileDelete_Response, "fileDelete" },
  {  77, BER_CLASS_CON, 77, BER_FLAGS_IMPLTAG, dissect_mms_FileDirectory_Response, "fileDirectory" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_ConfirmedServiceResponse(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, ConfirmedServiceResponse_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Confirmed_ResponsePDU_sequence[] = {
  { BER_CLASS_UNI, BER_UNI_TAG_INTEGER, BER_FLAGS_NOOWNTAG, dissect_mms_Unsigned32, "invokeID" },
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_ConfirmedServiceResponse, "confirmedServiceResponse" },
  { BER_CLASS_CON, 79, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_CS_Request_Detail, "cs-request-detail" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Confirmed_ResponsePDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Confirmed_ResponsePDU_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Confirmed_ErrorPDU_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "invokeID" },
  { BER_CLASS_CON, 1, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "modifierPosition" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_ServiceError, "serviceError" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Confirmed_ErrorPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Confirmed_ErrorPDU_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t InformationReport_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_VariableAccessSpecification, "variableAccessSpecification" },
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_SEQUENCE_OF_AccessResult, "listOfAccessResult" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_InformationReport(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, InformationReport_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_UnsolicitedStatus(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Status_Response(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_eventConditionName_01_vals[] = {
  {   0, "eventCondition" },
  {   1, "undefined" },
  { 0, NULL }
};

static const ber_choice_t T_eventConditionName_01_choice[] = {
  {   0, BER_CLASS_CON, 0, 0, dissect_mms_ObjectName, "eventCondition" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_NULL, "undefined" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_eventConditionName_01(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_eventConditionName_01_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_eventActionResult_vals[] = {
  {   0, "success" },
  {   1, "failure" },
  { 0, NULL }
};

static const ber_choice_t T_eventActionResult_choice[] = {
  {   0, BER_CLASS_CON, 0, 0, dissect_mms_ConfirmedServiceResponse, "success" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_ServiceError, "failure" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_eventActionResult(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_eventActionResult_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t T_actionResult_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventActioName" },
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_T_eventActionResult, "eventActionResult" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_actionResult(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, T_actionResult_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t EventNotification_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_NOTCHKTAG, dissect_mms_ObjectName, "eventEnrollmentName" },
  { BER_CLASS_CON, 1, 0, dissect_mms_T_eventConditionName_01, "eventConditionName" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned8, "severity" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_EC_State, "currentState" },
  { BER_CLASS_CON, 4, BER_FLAGS_NOTCHKTAG, dissect_mms_EventTime, "transitionTime" },
  { BER_CLASS_CON, 6, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_BOOLEAN, "notificationLost" },
  { BER_CLASS_CON, 7, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_AlarmAckRule, "alarmAcknowledgmentRule" },
  { BER_CLASS_CON, 8, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_T_actionResult, "actionResult" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_EventNotification(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, EventNotification_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_UnconfirmedService_vals[] = {
  {   0, "informationReport" },
  {   1, "unsolicitedStatus" },
  {   2, "eventNotification" },
  { 0, NULL }
};

static const ber_choice_t UnconfirmedService_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_InformationReport, "informationReport" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_UnsolicitedStatus, "unsolicitedStatus" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_EventNotification, "eventNotification" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_UnconfirmedService(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, UnconfirmedService_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Unconfirmed_PDU_sequence[] = {
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_UnconfirmedService, "unconfirmedService" },
  { BER_CLASS_CON, 79, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_mms_CS_Request_Detail, "cs-request-detail" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Unconfirmed_PDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Unconfirmed_PDU_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_confirmed_requestPDU_vals[] = {
  {   0, "other" },
  {   1, "unrecognized-service" },
  {   2, "unrecognized-modifier" },
  {   3, "invalid-invokeID" },
  {   4, "invalid-argument" },
  {   5, "invalid-modifier" },
  {   6, "max-serv-outstanding-exceeded" },
  {   8, "max-recursion-exceeded" },
  {   9, "value-out-of-range" },
  { 0, NULL }
};


static int dissect_mms_T_confirmed_requestPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_confirmed_responsePDU_vals[] = {
  {   0, "other" },
  {   1, "unrecognized-service" },
  {   2, "invalid-invokeID" },
  {   3, "invalid-result" },
  {   5, "max-recursion-exceeded" },
  {   6, "value-out-of-range" },
  { 0, NULL }
};


static int dissect_mms_T_confirmed_responsePDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_confirmed_errorPDU_vals[] = {
  {   0, "other" },
  {   1, "unrecognized-service" },
  {   2, "invalid-invokeID" },
  {   3, "invalid-serviceError" },
  {   4, "value-out-of-range" },
  { 0, NULL }
};


static int dissect_mms_T_confirmed_errorPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_unconfirmedPDU_vals[] = {
  {   0, "other" },
  {   1, "unrecognized-service" },
  {   2, "invalid-argument" },
  {   3, "max-recursion-exceeded" },
  {   4, "value-out-of-range" },
  { 0, NULL }
};


static int dissect_mms_T_unconfirmedPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_pdu_error_vals[] = {
  {   0, "unknown-pdu-type" },
  {   1, "invalid-pdu" },
  {   2, "illegal-acse-mapping" },
  { 0, NULL }
};


static int dissect_mms_T_pdu_error(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_cancel_requestPDU_vals[] = {
  {   0, "other" },
  {   1, "invalid-invokeID" },
  { 0, NULL }
};


static int dissect_mms_T_cancel_requestPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_cancel_responsePDU_vals[] = {
  {   0, "other" },
  {   1, "invalid-invokeID" },
  { 0, NULL }
};


static int dissect_mms_T_cancel_responsePDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_cancel_errorPDU_vals[] = {
  {   0, "other" },
  {   1, "invalid-invokeID" },
  {   2, "invalid-serviceError" },
  {   3, "value-out-of-range" },
  { 0, NULL }
};


static int dissect_mms_T_cancel_errorPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_conclude_requestPDU_vals[] = {
  {   0, "other" },
  {   1, "invalid-argument" },
  { 0, NULL }
};


static int dissect_mms_T_conclude_requestPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_conclude_responsePDU_vals[] = {
  {   0, "other" },
  {   1, "invalid-result" },
  { 0, NULL }
};


static int dissect_mms_T_conclude_responsePDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_conclude_errorPDU_vals[] = {
  {   0, "other" },
  {   1, "invalid-serviceError" },
  {   2, "value-out-of-range" },
  { 0, NULL }
};


static int dissect_mms_T_conclude_errorPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const value_string mms_T_rejectReason_vals[] = {
  {   1, "confirmed-requestPDU" },
  {   2, "confirmed-responsePDU" },
  {   3, "confirmed-errorPDU" },
  {   4, "unconfirmedPDU" },
  {   5, "pdu-error" },
  {   6, "cancel-requestPDU" },
  {   7, "cancel-responsePDU" },
  {   8, "cancel-errorPDU" },
  {   9, "conclude-requestPDU" },
  {  10, "conclude-responsePDU" },
  {  11, "conclude-errorPDU" },
  { 0, NULL }
};

static const ber_choice_t T_rejectReason_choice[] = {
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_T_confirmed_requestPDU, "confirmed-requestPDU" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_T_confirmed_responsePDU, "confirmed-responsePDU" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_T_confirmed_errorPDU, "confirmed-errorPDU" },
  {   4, BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_T_unconfirmedPDU, "unconfirmedPDU" },
  {   5, BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_T_pdu_error, "pdu-error" },
  {   6, BER_CLASS_CON, 6, BER_FLAGS_IMPLTAG, dissect_mms_T_cancel_requestPDU, "cancel-requestPDU" },
  {   7, BER_CLASS_CON, 7, BER_FLAGS_IMPLTAG, dissect_mms_T_cancel_responsePDU, "cancel-responsePDU" },
  {   8, BER_CLASS_CON, 8, BER_FLAGS_IMPLTAG, dissect_mms_T_cancel_errorPDU, "cancel-errorPDU" },
  {   9, BER_CLASS_CON, 9, BER_FLAGS_IMPLTAG, dissect_mms_T_conclude_requestPDU, "conclude-requestPDU" },
  {  10, BER_CLASS_CON, 10, BER_FLAGS_IMPLTAG, dissect_mms_T_conclude_responsePDU, "conclude-responsePDU" },
  {  11, BER_CLASS_CON, 11, BER_FLAGS_IMPLTAG, dissect_mms_T_conclude_errorPDU, "conclude-errorPDU" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_T_rejectReason(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, T_rejectReason_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t RejectPDU_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "originalInvokeID" },
  { BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG, dissect_mms_T_rejectReason, "rejectReason" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_RejectPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, RejectPDU_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Cancel_RequestPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Unsigned32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Cancel_ResponsePDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_Unsigned32(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Cancel_ErrorPDU_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Unsigned32, "originalInvokeID" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_ServiceError, "serviceError" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Cancel_ErrorPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Cancel_ErrorPDU_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Integer16(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const asn_namedbit ParameterSupportOptions_bits[] = {
  {  0, -1, -1, "str1", NULL },
  {  1, -1, -1, "str2", NULL },
  {  2, -1, -1, "vnam", NULL },
  {  3, -1, -1, "valt", NULL },
  {  4, -1, -1, "vadr", NULL },
  {  5, -1, -1, "vsca", NULL },
  {  6, -1, -1, "tpy", NULL },
  {  7, -1, -1, "vlis", NULL },
  {  8, -1, -1, "real", NULL },
  { 10, -1, -1, "cei", NULL },
  { 0, 0, 0, NULL, NULL }
};

static int dissect_mms_ParameterSupportOptions(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_bitstring(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const asn_namedbit ServiceSupportOptions_bits[] = {
  {  0, -1, -1, "status", NULL },
  {  1, -1, -1, "getNameList", NULL },
  {  2, -1, -1, "identify", NULL },
  {  3, -1, -1, "rename", NULL },
  {  4, -1, -1, "read", NULL },
  {  5, -1, -1, "write", NULL },
  {  6, -1, -1, "getVariableAccessAttributes", NULL },
  {  7, -1, -1, "defineNamedVariable", NULL },
  {  8, -1, -1, "defineScatteredAccess", NULL },
  {  9, -1, -1, "getScatteredAccessAttributes", NULL },
  { 10, -1, -1, "deleteVariableAccess", NULL },
  { 11, -1, -1, "defineNamedVariableList", NULL },
  { 12, -1, -1, "getNamedVariableListAttributes", NULL },
  { 13, -1, -1, "deleteNamedVariableList", NULL },
  { 14, -1, -1, "defineNamedType", NULL },
  { 15, -1, -1, "getNamedTypeAttributes", NULL },
  { 16, -1, -1, "deleteNamedType", NULL },
  { 17, -1, -1, "input", NULL },
  { 18, -1, -1, "output", NULL },
  { 19, -1, -1, "takeControl", NULL },
  { 20, -1, -1, "relinquishControl", NULL },
  { 21, -1, -1, "defineSemaphore", NULL },
  { 22, -1, -1, "deleteSemaphore", NULL },
  { 23, -1, -1, "reportSemaphoreStatus", NULL },
  { 24, -1, -1, "reportPoolSemaphoreStatus", NULL },
  { 25, -1, -1, "reportSemaphoreEntryStatus", NULL },
  { 26, -1, -1, "initiateDownloadSequence", NULL },
  { 27, -1, -1, "downloadSegment", NULL },
  { 28, -1, -1, "terminateDownloadSequence", NULL },
  { 29, -1, -1, "initiateUploadSequence", NULL },
  { 30, -1, -1, "uploadSegment", NULL },
  { 31, -1, -1, "terminateUploadSequence", NULL },
  { 32, -1, -1, "requestDomainDownload", NULL },
  { 33, -1, -1, "requestDomainUpload", NULL },
  { 34, -1, -1, "loadDomainContent", NULL },
  { 35, -1, -1, "storeDomainContent", NULL },
  { 36, -1, -1, "deleteDomain", NULL },
  { 37, -1, -1, "getDomainAttributes", NULL },
  { 38, -1, -1, "createProgramInvocation", NULL },
  { 39, -1, -1, "deleteProgramInvocation", NULL },
  { 40, -1, -1, "start", NULL },
  { 41, -1, -1, "stop", NULL },
  { 42, -1, -1, "resume", NULL },
  { 43, -1, -1, "reset", NULL },
  { 44, -1, -1, "kill", NULL },
  { 45, -1, -1, "getProgramInvocationAttributes", NULL },
  { 46, -1, -1, "obtainFile", NULL },
  { 47, -1, -1, "defineEventCondition", NULL },
  { 48, -1, -1, "deleteEventCondition", NULL },
  { 49, -1, -1, "getEventConditionAttributes", NULL },
  { 50, -1, -1, "reportEventConditionStatus", NULL },
  { 51, -1, -1, "alterEventConditionMonitoring", NULL },
  { 52, -1, -1, "triggerEvent", NULL },
  { 53, -1, -1, "defineEventAction", NULL },
  { 54, -1, -1, "deleteEventAction", NULL },
  { 55, -1, -1, "getEventActionAttributes", NULL },
  { 56, -1, -1, "reportActionStatus", NULL },
  { 57, -1, -1, "defineEventEnrollment", NULL },
  { 58, -1, -1, "deleteEventEnrollment", NULL },
  { 59, -1, -1, "alterEventEnrollment", NULL },
  { 60, -1, -1, "reportEventEnrollmentStatus", NULL },
  { 61, -1, -1, "getEventEnrollmentAttributes", NULL },
  { 62, -1, -1, "acknowledgeEventNotification", NULL },
  { 63, -1, -1, "getAlarmSummary", NULL },
  { 64, -1, -1, "getAlarmEnrollmentSummary", NULL },
  { 65, -1, -1, "readJournal", NULL },
  { 66, -1, -1, "writeJournal", NULL },
  { 67, -1, -1, "initializeJournal", NULL },
  { 68, -1, -1, "reportJournalStatus", NULL },
  { 69, -1, -1, "createJournal", NULL },
  { 70, -1, -1, "deleteJournal", NULL },
  { 71, -1, -1, "getCapabilityList", NULL },
  { 72, -1, -1, "fileOpen", NULL },
  { 73, -1, -1, "fileRead", NULL },
  { 74, -1, -1, "fileClose", NULL },
  { 75, -1, -1, "fileRename", NULL },
  { 76, -1, -1, "fileDelete", NULL },
  { 77, -1, -1, "fileDirectory", NULL },
  { 78, -1, -1, "unsolicitedStatus", NULL },
  { 79, -1, -1, "informationReport", NULL },
  { 80, -1, -1, "eventNotification", NULL },
  { 81, -1, -1, "attachToEventCondition", NULL },
  { 82, -1, -1, "attachToSemaphore", NULL },
  { 83, -1, -1, "conclude", NULL },
  { 84, -1, -1, "cancel", NULL },
  { 0, 0, 0, NULL, NULL }
};

static int dissect_mms_ServiceSupportOptions(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_bitstring(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t InitRequestDetail_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Integer16, "proposedVersionNumber" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_ParameterSupportOptions, "proposedParameterCBB" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_ServiceSupportOptions, "servicesSupportedCalling" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_InitRequestDetail(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, InitRequestDetail_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Initiate_RequestPDU_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Integer32, "localDetailCalling" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Integer16, "proposedMaxServOutstandingCalling" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Integer16, "proposedMaxServOutstandingCalled" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Integer8, "proposedDataStructureNestingLevel" },
  { BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_InitRequestDetail, "mmsInitRequestDetail" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Initiate_RequestPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Initiate_RequestPDU_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t InitResponseDetail_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Integer16, "negociatedVersionNumber" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_ParameterSupportOptions, "negociatedParameterCBB" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_ServiceSupportOptions, "servicesSupportedCalled" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_InitResponseDetail(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, InitResponseDetail_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}


static const ber_sequence_t Initiate_ResponsePDU_sequence[] = {
  { BER_CLASS_CON, 0, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Integer32, "localDetailCalled" },
  { BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Integer16, "negociatedMaxServOutstandingCalling" },
  { BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Integer16, "negociatedMaxServOutstandingCalled" },
  { BER_CLASS_CON, 3, BER_FLAGS_OPTIONAL|BER_FLAGS_IMPLTAG, dissect_mms_Integer8, "negociatedDataStructureNestingLevel" },
  { BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_InitResponseDetail, "mmsInitResponseDetail" },
  { 0, 0, 0, NULL,NULL }
};

static int dissect_mms_Initiate_ResponsePDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_sequence(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, Initiate_ResponsePDU_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Initiate_ErrorPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ServiceError(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Conclude_RequestPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Conclude_ResponsePDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_null(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}



static int dissect_mms_Conclude_ErrorPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_mms_ServiceError(cbParam, pPdu);
	DECODE_CALLBACK_RET;
}


const value_string mms_MMSpdu_vals[] = {
  {   0, "confirmed-RequestPDU" },
  {   1, "confirmed-ResponsePDU" },
  {   2, "confirmed-ErrorPDU" },
  {   3, "unconfirmed-PDU" },
  {   4, "rejectPDU" },
  {   5, "cancel-RequestPDU" },
  {   6, "cancel-ResponsePDU" },
  {   7, "cancel-ErrorPDU" },
  {   8, "initiate-RequestPDU" },
  {   9, "initiate-ResponsePDU" },
  {  10, "initiate-ErrorPDU" },
  {  11, "conclude-RequestPDU" },
  {  12, "conclude-ResponsePDU" },
  {  13, "conclude-ErrorPDU" },
  { 0, NULL }
};

static const ber_choice_t MMSpdu_choice[] = {
  {   0, BER_CLASS_CON, 0, BER_FLAGS_IMPLTAG, dissect_mms_Confirmed_RequestPDU, "confirmed-RequestPDU" },
  {   1, BER_CLASS_CON, 1, BER_FLAGS_IMPLTAG, dissect_mms_Confirmed_ResponsePDU, "confirmed-ResponsePDU" },
  {   2, BER_CLASS_CON, 2, BER_FLAGS_IMPLTAG, dissect_mms_Confirmed_ErrorPDU, "confirmed-ErrorPDU" },
  {   3, BER_CLASS_CON, 3, BER_FLAGS_IMPLTAG, dissect_mms_Unconfirmed_PDU, "unconfirmed-PDU" },
  {   4, BER_CLASS_CON, 4, BER_FLAGS_IMPLTAG, dissect_mms_RejectPDU, "rejectPDU" },
  {   5, BER_CLASS_CON, 5, BER_FLAGS_IMPLTAG, dissect_mms_Cancel_RequestPDU, "cancel-RequestPDU" },
  {   6, BER_CLASS_CON, 6, BER_FLAGS_IMPLTAG, dissect_mms_Cancel_ResponsePDU, "cancel-ResponsePDU" },
  {   7, BER_CLASS_CON, 7, BER_FLAGS_IMPLTAG, dissect_mms_Cancel_ErrorPDU, "cancel-ErrorPDU" },
  {   8, BER_CLASS_CON, 8, BER_FLAGS_IMPLTAG, dissect_mms_Initiate_RequestPDU, "initiate-RequestPDU" },
  {   9, BER_CLASS_CON, 9, BER_FLAGS_IMPLTAG, dissect_mms_Initiate_ResponsePDU, "initiate-ResponsePDU" },
  {  10, BER_CLASS_CON, 10, BER_FLAGS_IMPLTAG, dissect_mms_Initiate_ErrorPDU, "initiate-ErrorPDU" },
  {  11, BER_CLASS_CON, 11, BER_FLAGS_IMPLTAG, dissect_mms_Conclude_RequestPDU, "conclude-RequestPDU" },
  {  12, BER_CLASS_CON, 12, BER_FLAGS_IMPLTAG, dissect_mms_Conclude_ResponsePDU, "conclude-ResponsePDU" },
  {  13, BER_CLASS_CON, 13, BER_FLAGS_IMPLTAG, dissect_mms_Conclude_ErrorPDU, "conclude-ErrorPDU" },
  { 0, 0, 0, 0, NULL,NULL }
};

static int dissect_mms_MMSpdu(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	gint branch_taken;

	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,0, MMSpdu_choice, pPdu->hf_index);

//  if( (branch_taken!=-1) && mms_MMSpdu_vals[branch_taken].strptr ){
//    col_append_fstr(actx->pinfo->cinfo, COL_INFO, "%s ", mms_MMSpdu_vals[branch_taken].strptr);
//  }




	DECODE_CALLBACK_RET;
}


/*--- End of included file: packet-mms-fn.c ---*/

//by skt acse add
static const ber_choice_t ASO_qualifier_choice[] = {
	{   0,  BER_CLASS_UNI, BER_UNI_TAG_SET, BER_FLAGS_NOOWNTAG, NULL/*dissect_acse_ASO_qualifier_form1*/ ,NULL},
	{   1,  BER_CLASS_UNI, BER_UNI_TAG_INTEGER, BER_FLAGS_NOOWNTAG, NULL/*dissect_acse_ASO_qualifier_form2*/ ,NULL},
	{   2,  BER_CLASS_UNI, BER_UNI_TAG_PrintableString, BER_FLAGS_NOOWNTAG, NULL/*dissect_acse_ASO_qualifier_form3*/ ,NULL},
	{   3,  BER_CLASS_UNI, BER_UNI_TAG_OCTETSTRING, BER_FLAGS_NOOWNTAG, NULL/*dissect_acse_ASO_qualifier_form_octets*/ ,NULL},
	{ 0,  0, 0, 0, NULL ,NULL}
};

int	dissect_acse_ASO_qualifier(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, ASO_qualifier_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}

static const ber_choice_t AP_title_choice[] = {
	{   0, BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG,NULL/* dissect_acse_AP_title_form1*/ ,NULL},
	{   1, BER_CLASS_UNI, BER_UNI_TAG_OID, BER_FLAGS_NOOWNTAG, NULL/*dissect_acse_AP_title_form2*/ ,NULL},
	{   2, BER_CLASS_UNI, BER_UNI_TAG_PrintableString, BER_FLAGS_NOOWNTAG, NULL/*dissect_acse_AP_title_form3*/ ,NULL},
	{	  0, 0, 0, 0, NULL ,NULL}
};

int dissect_acse_AP_title(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_choice(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, AP_title_choice, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}

int dissect_acse_AE_qualifier(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_acse_ASO_qualifier(cbParam,pPdu);
	DECODE_CALLBACK_RET;
}

int dissect_acse_AP_invocation_identifier(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}

int dissect_acse_AE_invocation_identifier(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_integer(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, NULL, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}

static const ber_sequence_t EXTERNALt_U_sequence[] = {
	{ BER_CLASS_UNI, BER_UNI_TAG_OID, BER_FLAGS_OPTIONAL|BER_FLAGS_NOOWNTAG,NULL /*dissect_acse_T_direct_reference*/ ,NULL},
	{ BER_CLASS_UNI, BER_UNI_TAG_INTEGER, BER_FLAGS_OPTIONAL|BER_FLAGS_NOOWNTAG,NULL /*dissect_acse_T_indirect_reference*/ ,NULL},
	{ BER_CLASS_UNI, BER_UNI_TAG_ObjectDescriptor, BER_FLAGS_OPTIONAL|BER_FLAGS_NOOWNTAG,NULL /*dissect_acse_ObjectDescriptor*/ ,NULL},
	{ BER_CLASS_ANY/*choice*/, -1/*choice*/, BER_FLAGS_NOOWNTAG|BER_FLAGS_NOTCHKTAG,NULL /*dissect_acse_T_encoding*/ ,NULL},
	{ 0, 0, 0, NULL ,NULL}
};

int dissect_acse_EXTERNALt(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	ret = dissect_ber_tagged_type(pThis,pPdu->val,pPdu->len,pPdu->implicit_tag, EXTERNALt_U_sequence, pPdu->hf_index);
	DECODE_CALLBACK_RET;
}
