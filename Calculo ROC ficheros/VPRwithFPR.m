function [ VPR ] = VPRwithFPR( rocData, FPR )
    %know FPR calculate FNR
    m =rocData(rocData(:,1)==FPR,:);
    if ~isempty(m);
       VPR = m(:,2); 
    else
        mSup =rocData(rocData(:,1)>FPR,:);
        mInf =rocData(rocData(:,1)<FPR,:);
        VPR = mInf(1,2) + ((mSup(size(mSup,1),2) - mInf(1,2))/2);
    end
end

