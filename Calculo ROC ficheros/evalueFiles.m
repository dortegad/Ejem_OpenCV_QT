

createFiles('scoresFiles_0.txt', 'correlations_0.txt',1000);
scores_0 = readFiles('scoresFiles_0.txt', 'correlations_0.txt');

createFiles('scoresFiles_1.txt', 'correlations_1.txt',1000);
scores_1 = readFiles('scoresFiles_1.txt', 'correlations_1.txt');

close all

[rocData_0, auc_0, eer_0] = createROC(scores_0(:,1)==scores_0(:,2), scores_0(:,3), 'result', false );

[rocData_1, auc_1, eer_1] = createROC(scores_1(:,1)==scores_1(:,2), scores_1(:,3), 'result', false );

loglog(rocData_0(:,1),rocData_0(:,2),'r', ...
      rocData_1(:,1),rocData_1(:,2),'b');
legend( ['scores-0 - ERR=',num2str(eer_0)],...
        ['scores-1 - ERR=',num2str(eer_1)] );
ylabel('VPR - true positive rate'); 
xlabel('FPR - false positive rate');
title('ROC')

VPR_0 = 0.1;
FPR_0 = FPRwithVPR(rocData_0, VPR_0);
fprintf('In scores_0 FPR is %6.2f when VPR is %6.2f \n',FPR_0,VPR_0)

FPR_0 = 0.1;
VPR_0 = VPRwithFPR(rocData_0, FPR_0);
fprintf('In scores_0 VPR is %6.2f when FPR is %6.2f \n',VPR_0,FPR_0)


VPR_1 = 0.1;
FPR_1 = FPRwithVPR(rocData_1, VPR_1);
fprintf('In scores_1 FPR is %6.2f when VPR is %6.2f \n',FPR_1,VPR_1)

FPR_1 = 0.1;
VPR_1 = VPRwithFPR(rocData_1, FPR_1);
fprintf('In scores_1 VPR is %6.2f when FPR is %6.2f \n',VPR_1,FPR_1)


