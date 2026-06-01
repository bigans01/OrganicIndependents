#pragma once

#ifndef NGSCSCANTARGETGROUPING_H
#define NGSCSCANTARGETGROUPING_H

class NGSCScanTargetGrouping
{
	public:
		NGSCScanTargetGrouping() {};
		NGSCScanTargetGrouping(EnclaveKeyDef::Enclave2DKey in_targetGroupingSectorKey, int in_targetGroupingId) :
			targetGroupingSectorKey(in_targetGroupingSectorKey),
			targetGroupingId(in_targetGroupingId)
		{};

		EnclaveKeyDef::Enclave2DKey targetGroupingSectorKey;
		int targetGroupingId = -1;
};

#endif