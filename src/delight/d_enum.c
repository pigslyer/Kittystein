#include "d_internal.h"


bool delight_enum_usage_req_eval(UsageReq req, bool value)
{
	return req == UsageReqOptional || (value == (req == UsageReqRequired));
}
