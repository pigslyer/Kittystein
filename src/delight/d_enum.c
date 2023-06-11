#include "d_internal.h"


bool delight_enum_usage_req_eval(UsageReq req, bool value)
{
	return req == USAGE_REQ_OPTIONAL || (value == (req == USAGE_REQ_REQUIRED));
}
