function [ rocData, auc, eer] = createROC(success, scores, curveName, plotCurve )
%CREATEROC Summary of this function goes here
%   Detailed explanation goes here

%umbrales = unique(scores);
umbrales = [min(scores):(max(scores)-min(scores))/100:max(scores)];
rocData = zeros(length(umbrales),2);
eer = -1;
%for (i=1:length(umbrales))
%     th = umbrales(i);
i = 0;
for (th=0:0.001:1)
    i = i + 1;
    predic = scores(:) > th;
    
    VP = nnz((predic==1)&(success==1));
    VPR = VP / nnz(success==1);
    
    FP = nnz((predic==1)&(success==0));
    FPR = FP / nnz(success==0);
    
    FN = nnz((predic==0)&(success==1));
    FNR = FN/(VP+FN);
    
    %ROC
    rocData(i,2) = VPR;
    rocData(i,1) = FPR;
    
    %ROC_FRAV
    rocData(i,1) = FPR;
    rocData(i,2) = FNR;
    
    if ((FNR >= FPR) && (eer ==-1))
        eer = FNR+FPR/2
    end
end
%rocData = sort(rocData);
auc = 1-sum(rocData(:,1))/length(scores);

if (plotCurve)
    figure;
    plot(rocData(:,1),rocData(:,2));
    title({'roc ',curveName});
    legend( [curveName,' - AUC = ',num2str(auc),' - EER = ',num2str(eer)]);
    ylabel('FNR - false negative rate');
    xlabel('TPR - true positive rate'); 
    %descr = {'517 users';curveName};
    %text(.025,0.15,descr);   
end
end

