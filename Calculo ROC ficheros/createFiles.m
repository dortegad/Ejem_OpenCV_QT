function [] = createFiles( scoresFileName, correlationsFileName, numFiles)
    %file Scores
    files = 1:numFiles;

    colum1 = repmat(files,[numFiles,1]);
    colum1 = reshape(colum1,numFiles*numFiles,1);

    colum2 = repmat(files,[numFiles,1])';
    colum2 = reshape(colum2,numFiles*numFiles,1);

    colum3 = randi(100,1,numFiles*numFiles)';

    scores = [colum1, colum2, colum3];

    %add localication error
    numErrors = 2;
    errors = randi(numErrors,1,numErrors);
    for error = 1:numErrors;
        scores(scores(:,2)==errors(error),3)=-1;
    end

    csvwrite(scoresFileName,scores);

    %file correspondations
    corresColum2 = randperm(numFiles,numFiles)';
    correlations = [files',corresColum2];

    csvwrite(correlationsFileName,correlations);
end



