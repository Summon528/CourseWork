SELECT matchType, COUNT(*) as `count`
FROM `match`
WHERE matchType LIKE '%fpp%'
GROUP BY matchType
ORDER BY `count` ASC;
