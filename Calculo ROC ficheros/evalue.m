%file Scores
scoresFiles = fopen( 'scoresFiles.txt', 'wt' );
numFiles = 100;
scores = randi(100,1,numFiles*numFiles);
for fileA = 1:numFiles;
    for fileB = 1:numFiles;
        fprintf( scoresFiles, '%d, %d, %d\n', fileA, fileB, scores(((fileA-1)*numFiles)+fileB));
    end  
end
fclose(scoresFiles);

%file correspondations
correlationsFiles = fopen( 'correlations.txt', 'wt' );
corresB = randperm(numFiles,numFiles);
for fileA = 1:numFiles;
    fprintf( correlationsFiles, '%d, %d\n', fileA, corresB(fileA));
end
fclose(correlationsFiles);

%read scores files
scores = csvread('scoresFiles.txt')
 
%read correspondations
correlations = csvread('correlations.txt')
 
%chage correspondations in scores
scoresCorrelations = correlations(scores(:,2),2);
scores(:,2) = scoresCorrelations

%normalice scores
scores(:,3) = scores(:,3)/100;
[rocData, auc, eer] = createROC(scores(:,1)==scores(:,2), scores(:,3), 'result', true );