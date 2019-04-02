SELECT matchType, AVG(matchDuration) as averageDuration
FROM `match`
GROUP BY matchType
ORDER BY averageDuration ASC;

