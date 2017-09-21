%file Scores
scoresFiles = fopen( 'scoresFiles.txt', 'wt' );
numFiles = 10000;
files = 1:numFiles;

colum1 = repmat(files,[numFiles,1]);
colum1 = reshape(colum1,numFiles*numFiles,1);

colum2 = repmat(files,[numFiles,1])';
colum2 = reshape(colum2,numFiles*numFiles,1);

colum3 = randi(100,1,numFiles*numFiles)';

scores = [colum1, colum2, colum3];

%add localication error
numErrors = 5;
errors = randi(numErrors,1,numErrors);
for error = 1:numErrors;
    scores(scores(:,2)==errors(error),3)=-1;
end

csvwrite('scoresFiles.txt',scores);


%file correspondations
corresColum2 = randperm(numFiles,numFiles)';
correlations = [files',corresColum2];

csvwrite('correlations.txt',scores);


%read scores files
scores = csvread('scoresFiles.txt')

%delete scores on error
scores = scores(scores(:,3)~=-1,:);

%read correspondations
correlations = csvread('correlations.txt')
 
%chage correspondations in scores
scoresCorrelations = correlations(scores(:,2),2);
scores(:,2) = scoresCorrelations

%normalice scores
scores(:,3) = scores(:,3)/100;
[rocData, auc, eer] = createROC(scores(:,1)==scores(:,2), scores(:,3), 'result', true );