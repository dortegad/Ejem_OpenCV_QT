function [ FPR ] = FPRwithVPR( rocData, VPR )
    %know VPR calculate FPR
    m =rocData(rocData(:,2)==VPR,:);
    if ~isempty(m);
       FPR = m(1); 
    else
        mSup =rocData(rocData(:,2)>VPR,:);
        mInf =rocData(rocData(:,2)<VPR,:);
        FPR = mInf(1) + ((mSup(size(mSup,1)) - mInf(1))/2);
    end
end

