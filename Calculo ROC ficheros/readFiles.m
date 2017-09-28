function [ scores ] = readFiles( scoresFileName, correlationsFileName )
    %read scores files
    scores = csvread(scoresFileName);

    %delete scores on error
    scores = scores(scores(:,3)~=-1,:);

    %read correspondations
    correlations = csvread(correlationsFileName);

    %chage correspondations in scores
    scoresCorrelations = correlations(scores(:,2),2);
    scores(:,2) = scoresCorrelations;
    
    %normalice scores
    scores(:,3) = scores(:,3)/100;
end

